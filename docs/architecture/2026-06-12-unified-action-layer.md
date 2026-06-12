# 2026-06-12 통일 액션 레이어 설계

## 작업 목표

플레이어·AI의 모든 전투 액션(평타, 콤보, 회피, 스텝백, 스킬, 궁극기, 피격)을 **하나의 네트워크 패턴**으로 수렴시킨다. 액션마다 RPC 방식이 달라서 "한 곳을 고치면 다른 액션이 어긋나는" 구조적 피로를 제거하고, Steam 지연 환경에서도 본인 입력이 즉시 반응하도록 소유 클라이언트 예측을 추가한다.

이 문서는 목표 아키텍처와 마이그레이션 기준선이다. 구현은 단계별로 진행하며 각 단계는 작업 로그에 남긴다.

## 기존 문제 (진단 결과)

### 1. 액션마다 네트워크 패턴이 제각각

| 액션 | 현재 패턴 | 위치 |
| --- | --- | --- |
| 평타 | `HasAuthority면 Multicast, 아니면 Server→Multicast` | `BasePlayer::OnMouseL` |
| 회피/스텝백 | `Server_Impl에서 로컬 재생 + Multicast에서 HasAuthority 리턴` | `BasePlayer::OnEvade / OnStepBack` |
| 스킬(R/Q) | `Server→Multicast 직행, 상태/코스트 검증 없음` | `BasePlayer::OnMouseR / OnQ` |
| AI 평타/스킬 | `Server→Multicast로 Item 액션 전체 재실행` | `AIBaseCharacter::OnMelee / OnSkill` |
| AI 몽타주 속도 | 복제 변수 + OnRep + Multicast (정답 패턴) | `AIBaseCharacter::TimedMontagePlayRate` |
| 피격 리액션 | Multicast (06-08 정리됨) | `BaseCharacter::MulticastPlayHitReaction` |

새 액션을 추가하거나 하나를 수정할 때마다 "이건 어느 패턴이었지"를 매번 재판단해야 한다. 이것이 변경 churn의 근본 원인이다.

### 2. `StateComponent::Type` 복제 변수를 클라에서 로컬로 덮어씀

- `Type`은 `Replicated`(OnRep 없음)인데 `MontageComponent::PlayAnimMontage`가 멀티캐스트를 타고 **클라에서도** `state->ChangeType()`를 직접 호출한다.
- 클라가 복제 변수를 로컬로 쓰면 다음 서버 복제가 덮어쓰며 상태가 한 박자 튄다.

### 3. 클라이언트 예측 없음 → Steam에서 입력렉으로 드러남

- 평타는 `Server→Multicast` 왕복 후에야 본인 화면에서도 재생된다.
- NULL/PIE(핑 0ms)에서는 안 보이지만 Steam(30~100ms+)에서는 그대로 입력 지연이 된다.

### 4. 전투 몽타주를 전부 `Reliable Multicast`로 전송

- 연출용 몽타주를 Reliable로 자주 쏘면 채널이 밀려 끊김이 생긴다.

## 목표 구조

### 핵심 원칙

1. **단일 진입점**: 모든 액션 요청은 `UActionComponent::TryAction(FGameplayTag)` 하나로 들어온다.
2. **서버 단일 검증**: 상태(Idle/콤보 윈도우)와 코스트(스태미나) 판정은 서버에서 한 번만 한다.
3. **상태 복제로 표현**: 액션 실행은 RPC가 아니라 복제된 액션 상태(`FActiveAction`)와 `OnRep`로 모든 프록시에 전파한다. (AI의 `TimedMontagePlayRate`가 이미 검증한 패턴을 일반화)
4. **소유 클라 예측**: 입력을 넣은 본인 캐릭터는 서버 응답을 기다리지 않고 즉시 로컬 재생한다. 서버가 거부하면 복제 상태로 교정된다.
5. **판정은 서버 권위 유지**: 데미지/사망은 지금처럼 `TakeDamage`에서 서버만 처리한다(현행 유지).
6. **`Type`은 서버 단방향**: 상태는 서버에서만 쓰고 `OnRep`로 AnimBP에 반영한다. 클라 로컬 쓰기 제거.

### 새 타입

```cpp
USTRUCT()
struct FActiveAction
{
    GENERATED_BODY()

    UPROPERTY()
    FGameplayTag ActionTag;   // 어떤 액션인지 (DT의 FActionData::AbilityTag와 동일 키)

    UPROPERTY()
    uint8 Sequence = 0;       // 같은 액션 재요청도 OnRep을 확실히 트리거하는 카운터
};
```

### `UActionComponent` (신규 또는 MontageComponent 확장)

책임:

- `TryAction(FGameplayTag Tag)` — 입력단에서 호출하는 단일 진입점
- 소유 클라: 로컬 예측 재생 후 `Server_TryAction(Tag)` 전송
- 서버: 검증 → `ActiveAction = {Tag, ++Seq}` 설정 → `Type` 갱신
- `OnRep_ActiveAction()` — 시뮬레이트 프록시에서 데이터 조회 후 몽타주 재생
- 데이터 조회는 기존 `Equip->GetCurrentItem()`의 `FActionData`(이미 `AbilityTag` 보유)를 그대로 사용

### 런타임 흐름

```
[입력]                [소유 클라]              [서버]                  [다른 클라]
OnMouseL  ──►  ActionComp.TryAction(Tag)
                       │
                       ├─ 로컬 예측 재생 (즉시)
                       │
                       └─ Server_TryAction(Tag) ──►  검증(State/Stamina)
                                                          │ OK
                                                          ├─ ActiveAction={Tag,++Seq}
                                                          ├─ Type 갱신 (서버 권위)
                                                          └─ 복제 ──────────────►  OnRep_ActiveAction
                                                                                      └─ 몽타주 재생
                                                          │ 거부
                                                          └─ (복제 안 함) ──► 소유 클라는
                                                                              다음 복제로 자동 교정
```

### 데이터 매핑

- `FActionData::AbilityTag`를 액션 식별 키로 사용한다. 이미 DataTable에 존재하므로 신규 데이터 구조가 거의 필요 없다.
- `MontageComponent::PlayAnimMontage`가 하던 "몽타주 재생 + Type 변경 + 이동 허용" 로직은 유지하되, `Type` 변경은 **서버에서만** 수행하고 클라는 `OnRep`로 받은 상태로 AnimBP가 표현하게 한다.

## 마이그레이션 단계

큰 폭의 전환을 한 번에 하지 않는다. 액션을 하나씩 새 레이어로 옮기되, 옮긴 액션은 옛 RPC 경로를 제거한다.

| 단계 | 내용 | 완료 기준 |
| --- | --- | --- |
| 0 | `FActiveAction`, `UActionComponent` 골격 추가 (기존 동작 유지) | 빌드 성공, 기존 플레이 영향 없음 |
| 1 | `Type`을 서버 단방향 + OnRep로 전환, 클라 로컬 `ChangeType` 제거 | 2인 PIE에서 상태 튐 사라짐 |
| 2 | 평타/콤보를 `TryAction` + 예측으로 이전 | 소유 클라 즉시 반응, 서버/클라 결과 일치 |
| 3 | 회피/스텝백 이전 | 동일 |
| 4 | 스킬(R/Q)·궁극기 이전, 누락된 코스트/상태 검증 추가 | 동일 |
| 5 | AI 평타/스킬을 동일 상태 복제 패턴으로 이전 | 서버/클라 AI 타이밍 일치 |
| 6 | 전투 Multicast Reliable → 상태 복제 기반으로 정리, 남은 Reliable 최소화 | 끊김 감소 확인 |

## 호환성·위험

- **블루프린트 의존성**: 현재 입력→`OnMouseL` 등은 C++ 입력 핸들러에서 호출된다. AnimBP는 `StateComponent::Type`과 `Speed`만 읽으므로(확인됨: `PlayerAnimInstance`), `Type` 단방향 전환이 AnimBP 표현을 깨지 않는지 단계 1에서 우선 검증한다.
- **예측 롤백 범위**: 데미지·사망은 예측하지 않는다. 예측은 몽타주/상태 표현에만 적용해 롤백 복잡도를 낮춘다.
- **콤보 윈도우**: `IsCanCombo()`가 서버 무기 상태에 의존하므로, 예측 단계에서 콤보 가능 여부는 소유 클라도 로컬 판단 후 서버가 최종 확정한다. 불일치 시 서버 복제가 우선.
- **AI 경로**: AIController는 서버 전용이므로 AI는 예측이 불필요하다. AI는 서버 검증 → 상태 복제 → OnRep만 사용한다(예측 단계 생략).

## 검증 기준

- 2인 NULL 직접 접속(`Scripts\LaunchNullListenTest.bat`)에서 호스트/클라가 같은 액션 타이밍을 본다.
- 인위적 지연(`Net PktLag`) 환경에서 소유 클라 평타가 즉시 재생되고, 서버 결과와 어긋나지 않는다.
- 액션 추가 시 수정 범위가 "DataTable 행 + AbilityTag" 수준으로 줄어든다.
- `StateComponent::Type`이 클라에서 튀지 않는다.

## 남은 작업

- 단계 0 골격 구현 후 빌드 검증.
- 단계 1에서 AnimBP가 `Type` 단방향 전환에 영향받지 않는지 우선 확인.
- 각 단계 이전 시 옛 RPC 경로 제거를 함께 수행해 패턴이 둘로 갈라지지 않게 한다.
