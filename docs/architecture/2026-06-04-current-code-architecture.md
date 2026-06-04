# 2026-06-04 현재 코드 아키텍처 기록

## 목적

본격적인 리팩터링과 멀티플레이 동기화 수정 전에 현재 코드 구조를 기준선으로 기록한다.

이 문서는 "현재 구조가 이상적이다"라는 의미가 아니다. 이후 수정 작업에서 어떤 책임을 어디로 옮겼는지 비교하기 위한 현재 상태 지도다.

## 전체 모듈 구조

프로젝트 C++ 모듈은 `Source/SoulLike` 하나를 중심으로 구성되어 있다.

- `Actor/`: 캐릭터, 아이템, 컨트롤러, 게임모드, 투사체, 플레이어 상태
- `Component/`: 캐릭터 상태, 스탯, 장비, 몽타주, AI 행동 상태 보조 컴포넌트
- `Behavior/`: Behavior Tree Task/Service
- `Notifies/`: Anim Notify와 Anim Notify State
- `Data/`: 액션 데이터, 아이템 데이터, 입력 데이터
- `UI/`, `HUD/`: 세션 UI, 인게임 UI, 메뉴 UI
- `SubSystem/`: 데이터 서브시스템
- `Interface/`: 메뉴, 상호작용, 전투 인터페이스
- `MISC/`: 이름 테이블, 액터 풀, 레벨 전환 유틸

## 핵심 런타임 구조

### 캐릭터 계층

현재 캐릭터 구조는 다음 계층을 가진다.

```text
ACharacter
  -> ABaseCharacter
      -> ABasePlayer
          -> ASpearman
      -> AAIBaseCharacter
          -> AGrux
```

`ABaseCharacter`는 플레이어와 AI가 공유하는 기본 기능을 가진다.

- `UStateComponent`: Idle/Action/Hitted/Evade/Dead 같은 상태
- `UEquipComponent`: 현재 장착 아이템
- `UStatusComponent`: HP, MP, SP, 이동 속도, 데미지
- `UMontageComponent`: 공통 피격/회피/사망 몽타주 재생
- `DefaultItemClass`: 시작 시 기본 아이템 스폰

현재 `ABaseCharacter`는 캐릭터 공통 베이스이면서 장비 생성, 데미지 처리, 사망 처리, 피격 몽타주 재생까지 담당한다.

### 플레이어 흐름

`ABasePlayer`는 입력 이벤트를 액션 실행으로 연결한다.

대표 흐름:

```text
Input
  -> ABasePlayer::OnMouseL / OnMouseR / OnQ / OnEvade
  -> Server RPC
  -> NetMulticast
  -> UEquipComponent::GetCurrentItem()
  -> AItem::OnDefaultAction / OnSkillAction
  -> ACharacter::PlayAnimMontage
  -> Anim Notify
  -> AItem / ABaseCharacter / ABaseWeapon
```

특징:

- 입력 함수가 네트워크 호출, 상태 체크, 액션 실행을 함께 처리한다.
- 액션 실행 결과는 주로 `NetMulticast`로 몽타주 재생을 뿌리는 방식이다.
- 액션의 현재 상태와 데이터는 별도의 복제 가능한 액션 상태로 저장되어 있지 않다.

### AI 흐름

AI는 `ABaseAIController`, `BehaviorComponent`, Behavior Tree Task/Service, `AAIBaseCharacter`가 함께 동작한다.

대표 흐름:

```text
Behavior Tree Task
  -> ABaseAIController::OnMeleeAttack / OnSkill / OnUltimate
  -> AAIBaseCharacter::OnMelee / OnSkill
  -> NetMulticast
  -> UEquipComponent::GetCurrentItem()
  -> AItem / AGruxItem 액션 실행
  -> Montage / Anim Notify
```

`ABaseAIController`는 다음 책임을 가진다.

- Perception 구성
- 가장 가까운 플레이어 탐색
- Blackboard Target/Location/Behavior 값 갱신
- Behavior Tree 실행
- AI 액션 실행 요청
- 일부 타이밍 공격 랜덤 처리

`BehaviorComponent`는 AIController 내부 컴포넌트로 Blackboard 값을 읽고 쓰는 보조 역할이다.

현재 AIController는 서버에만 존재하는 특성 때문에, 여기에서 결정한 타이밍/랜덤/몽타주 속도 변경은 클라이언트와 어긋날 위험이 있다.

## 액션 데이터 구조

액션 데이터는 `UCombatActionDataAsset`과 `FActionDataTableRow`를 중심으로 구성된다.

```text
UCombatActionDataAsset
  -> FItemInfoData
      -> UDataTable
          -> FActionDataTableRow
              -> TArray<FActionData>
```

`FActionData`는 다음 값을 가진다.

- GameplayTag 기반 AbilityTag
- AnimMontage
- PlayRate
- StartSection
- Power
- HitStop
- Effect
- CameraShake
- DamageType
- SpecificCollisionName
- bCanCombo
- bCanMove

현재 `AItem`은 DataTable에서 `FActionData`를 읽어 `ActionTagMap`에 저장하고, 액션 실행 시 `CurrentData`를 갱신한다.

중요한 현재 상태:

- `AItem`은 `bReplicates = false`
- `AItem::CurrentData` 복제는 주석 처리되어 있다.
- `UEquipComponent::SelectItem` 복제도 주석 처리되어 있다.
- 따라서 Anim Notify가 참조하는 현재 아이템과 현재 액션 데이터가 서버/클라이언트에서 항상 같은 상태라고 보장하기 어렵다.

## Anim Notify 구조

현재 Anim Notify는 단순 시각 이벤트보다 더 많은 게임플레이 책임을 가진다.

주요 Notify:

- `AN_ItemAction`: 콤보 다음 액션 실행
- `AN_EndAction`: 캐릭터 상태를 Idle로 되돌림
- `ANS_Collision`: 무기 충돌 활성/비활성
- `ANS_MontagePlayRate`: AI 몽타주 속도 변경
- `ANS_MoveToTarget`: Notify Tick 동안 캐릭터 위치 보간
- `ANS_Rotate`, `ANS_Roll`, `ANS_Parrying`, `ANS_ItemSkill`, `ANS_Combo`

현재 특징:

- Notify가 `AItem`, `ABaseCharacter`, `ABaseWeapon`, `ABaseAIController`를 직접 호출한다.
- 충돌 판정 시작/종료도 Notify 타이밍에 강하게 의존한다.
- `ANS_MoveToTarget`는 `SetActorLocation`을 직접 호출한다.
- `ANS_MontagePlayRate`는 AIController를 통해 랜덤하게 속도 변경을 요청한다.

향후 방향:

- Notify는 "확정된 액션의 타이밍 이벤트" 역할로 줄인다.
- 액션 선택, 랜덤 결정, 서버 판정, 상태 복제는 캐릭터/액션 컴포넌트 쪽으로 이전한다.

## 네트워크 구조

현재 네트워크 구조는 부분적으로 서버 권위 구조를 사용한다.

현재 복제되는 주요 요소:

- `UStatusComponent::HP`
- `ABasePlayer::bAirBone`
- `ASpearman` 일부 스킬 상태
- `AGrux::bFly`, `AGrux::bTravel`
- `UStateComponent::Type`은 2026-06-04에 복제 적용됨

현재 불안정한 요소:

- `AItem`
- `UEquipComponent::SelectItem`
- `AItem::CurrentData`
- `UStatusComponent::MP`, `SP`
- `UStatusComponent::bCanMove`
- 액션 시작 시간, 몽타주 섹션, 재생 속도
- AI 타이밍 공격의 랜덤 결과

현재 액션 네트워크 흐름은 다음에 많이 의존한다.

```text
Server RPC
  -> NetMulticast
  -> 각 클라이언트에서 로컬 아이템 상태를 기준으로 몽타주 실행
```

향후 목표 흐름은 다음과 같다.

```text
Client Request
  -> Server Validation
  -> Server Action State Update
  -> Replicated Action State / OnRep
  -> Client Visual Playback
  -> Server-authoritative Hit / Cost / Result
```

## 세션/Online Subsystem 구조

세션 관리는 `UASGameInstance`가 담당한다.

현재 상태:

- `OnlineSubsystem`, `OnlineSubsystemSteam` 모듈 의존성이 있다.
- `DefaultEngine.ini`에 SteamNetDriver 설정이 있다.
- 현재 `DefaultPlatformService=NULL`이다.
- `CreateSession()`에서 `bIsLANMatch = true`로 고정되어 있다.
- Steam 전환 준비 흔적은 있지만 실제 동작 기준은 NULL/LAN 테스트에 가깝다.

판단:

- Steam OSS 전환은 가능하지만, 그 전에 액션/AI 동기화를 NULL OSS PIE에서 재현 가능하게 만드는 것이 우선이다.
- 세션 문제가 생겼을 때 OSS 설정 문제와 게임플레이 복제 문제를 분리하기 위해서다.

## UI/HUD 구조

UI는 세션 UI와 인게임 UI로 나뉜다.

- `UUI_ServerMenu`, `UUI_ServerRow`, `UUI_SessionWidget`: 세션 생성/검색/참가 UI
- `UMenuWidget`, `UPauseMenuWidget`, `UConfigWidget`: 메뉴와 설정
- `UUI_MainInGame`, `UUI_PlayerStatus`, `UUI_BossStatus`: 인게임 HUD
- `AInGameHUD`: 메인 UI를 붙이는 HUD

현재 AI 체력 UI는 `AAIBaseCharacter`에서 직접 HUD/UI를 찾아 조작하는 흐름이 있다. 이 부분은 캐릭터와 UI 책임이 섞여 있으므로 장기적으로 분리 대상이다.

## 현재 구조의 장점

- 플레이어와 AI가 `ABaseCharacter` 기반으로 공통 컴포넌트를 공유한다.
- 액션 데이터가 DataTable/DataAsset 기반이라 데이터 중심 구조로 확장할 여지가 있다.
- 공격 판정은 서버 권위 체크가 일부 적용되어 있다.
- Behavior Tree와 Blackboard 기반 AI 구조가 이미 있다.
- GameplayTag 기반 액션 식별 구조가 들어가 있다.

## 현재 구조의 주요 문제

### 책임 혼합

- Player가 입력, RPC, 액션 실행, UI/카메라 일부를 함께 담당한다.
- AIController가 타겟 탐색, BT 실행, 액션 결정, 타이밍 공격 랜덤 처리까지 담당한다.
- Anim Notify가 판정, 이동, 콤보 진행, 액션 종료를 직접 호출한다.
- Character가 장비 생성, 데미지, 상태, UI 연결까지 일부 담당한다.

### 복제 기준 부족

- 액션 상태의 원본이 서버에 명확히 저장되고 복제되는 구조가 부족하다.
- 아이템/장비/현재 액션 데이터가 네트워크 상태로 보장되지 않는다.
- 몽타주 실행 호출은 전파되지만, 현재 액션 의미 자체는 복제되지 않는다.

### 테스트 기준 부족

- NULL OSS PIE 2인 환경에서 어떤 동기화 결과를 성공으로 볼지 아직 체크리스트가 부족하다.
- AI 타이밍 공격, 콤보, 피격, 회피, 투사체, 사망/부활을 각각 분리해 재현해야 한다.

## 리팩터링 전 기준 규칙

이 문서를 기준으로 다음 원칙을 둔다.

1. 바로 대규모 구조 변경을 하지 않는다.
2. 먼저 현재 동작을 문서화하고, 작은 단위로 서버 권위 상태를 추가한다.
3. `StateComponent`처럼 공통 기반 상태부터 복제 안정성을 확보한다.
4. AIController에 있는 클라이언트 표시용 결정을 캐릭터/액션 상태로 옮긴다.
5. Anim Notify는 최종적으로 게임플레이 결정자가 아니라 타이밍 신호 역할로 축소한다.
6. Steam OSS 전환은 NULL OSS에서 주요 액션 동기화가 검증된 뒤 진행한다.

## 다음 작업 후보

우선순위는 다음과 같다.

1. AI 타이밍 공격 몽타주 속도 변경 구조 정리
2. 액션 상태 구조 초안 작성
3. `AItem`/`EquipComponent` 복제 방향 결정
4. NULL OSS PIE 2인 동기화 체크리스트 작성
5. 플레이어 기본 공격/콤보 동기화 검증
6. AI 이동 Notify와 캐릭터 무브먼트 충돌 여부 확인
7. Steam OSS 전환

## 현재 기준 결론

현재 프로젝트는 완전히 객체지향적으로 분리된 구조라기보다, 캐릭터/아이템/노티파이/컨트롤러가 서로 직접 호출하며 액션을 완성하는 구조다.

따라서 앞으로의 작업은 단순한 "리팩터링"보다 "서버 권위 액션 구조로 현대화"라고 보는 것이 적절하다.
