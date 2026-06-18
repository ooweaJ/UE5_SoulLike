# 2026-06-18 사망/부활 및 공격 입력 보정

## 작업 목표

- 사망 후 플레이어 상태, 표시, 입력, 위치가 정상 복구되지 않을 수 있는 코드를 정리한다.
- BAT 별도 프로세스 테스트에서 공격/콤보 입력이 불안정해지는 기초 오류를 줄인다.

## 기존 문제

- 플레이어 사망 완료가 `AN_PlayerDead` 애니메이션 notify에만 의존해 notify가 실행되지 않으면 부활 타이머가 시작되지 않을 수 있었다.
- `MultiCompletePlayerDeath()`가 서버와 모든 클라이언트에서 직접 부활 타이머를 걸고, `HandlePlayerRevival()` 안에서 HP/State 같은 서버 권한 상태와 visibility/input 같은 로컬 표현을 함께 처리하고 있었다.
- `SetAttachedActorsVisiblity()`가 부착 액터가 아니라 플레이어 자신의 컴포넌트를 반복 조회해 무기/부착물 visibility가 제대로 바뀌지 않을 수 있었다.
- `AMainWorldGameMode`가 모든 플레이어 리스폰에 하나의 `RespawnTimerHandle`을 공유해 여러 플레이어가 죽으면 타이머가 덮일 수 있었다.
- `ABaseWeapon::bCanCombo`, `bSucceed`가 명시 초기화되지 않아 첫 공격/콤보 판정이 비결정적으로 시작될 수 있었다.

## AI 활용 방식

- Codex로 사망, 부활, 리스폰, 콤보 notify, 액션 컴포넌트 흐름을 코드 기준으로 추적했다.
- UE-MCP `project(get_status)`로 에디터 연결 및 프로젝트 상태를 확인했다.
- UnrealBuildTool 직접 빌드로 C++ 컴파일 결과를 검증했다.

## 구현/수정 내용

- `ABasePlayer::CompletePlayerDeath()`가 서버에서 한 번만 사망 완료 처리를 예약하도록 `bDeathCompletionQueued`와 `PlayerRevivalTimerHandle`을 추가했다.
- 사망 완료 멀티캐스트는 visibility와 사망 이펙트만 처리하고, 3초 후 서버 타이머가 `MultiHandlePlayerRevival()`을 호출해 모든 인스턴스의 표시/입력/몽타주 정리를 맞추도록 분리했다.
- HP 회복과 `State->SetIdleMode()`는 authority 인스턴스에서만 수행하고, HP는 현재값과 최대값 차이만큼 회복해 정확히 MaxHP로 맞추도록 했다.
- `SetAttachedActorsVisiblity()`가 `AttachedActor->GetComponents()`를 사용하도록 고쳤다.
- `AMainWorldGameMode::Respawn()`에서 리스폰 타이머 핸들을 함수 로컬로 사용해 플레이어별 타이머가 서로 덮이지 않도록 했다.
- `BaseCharacter`, `AIBaseCharacter`, `Spearman`의 널 가드를 보강했다.
- `ABaseWeapon` 콤보 플래그를 `false`로 명시 초기화했다.

## 검증 결과

- `C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat SoulLikeEditor Win64 Development -Project=C:\jaewoo\Unreal\UE5_SoulLike\SoulLike.uproject -WaitMutex -FromMsBuild`
- 결과: `Result: Succeeded`

## 의사결정

- 부활 로직은 서버 권한 상태 복구와 클라이언트 표현 복구를 완전히 새 시스템으로 갈아엎지 않고, 기존 함수 구조 안에서 책임만 분리했다.
- death notify 자체는 유지하되, 중복 RPC와 클라이언트 로컬 상태 변경 문제를 막는 방향으로 최소 수정했다.
- 콤보 복제 구조 전체 개편은 범위가 커서 이번 변경에서는 명시 초기화와 액션 잠금 보정 범위까지만 적용했다.

## 남은 작업

- BAT 서버/클라이언트를 완전히 종료 후 재실행해서 새 DLL로 사망 후 3초 복구, 위치 리스폰, 입력 복구를 확인해야 한다.
- 콤보 단계 자체는 아직 명시적인 서버 확정 combo index를 복제하지 않으므로, 지연 환경에서 콤보 단계가 어긋나면 `ActiveAction`에 combo index/action tag를 포함하는 후속 작업이 필요하다.
- 데드 몽타주 notify가 빠진 캐릭터/몽타주가 있는지 에셋 검사가 필요하다.

## 부활 후 이동 불가 보정

### 문제

- 부활 후 입력 ignore는 풀리지만 플레이어 이동 입력이 계속 막힐 수 있었다.
- 이동 입력 처리에서 `Status->IsCanMove()`를 확인하는데, 사망/데드 몽타주 중 `SetStop()` 된 값을 부활 시 `SetMove()`로 복구하지 않았다.

### 수정

- `ABasePlayer::HandlePlayerRevival()`에서 `Status->SetMove()`를 호출해 부활 직후 이동 가능 상태를 복구했다.
- 서버 권한 복구 구간의 `Status`, `State` 널 가드를 보강했다.

### 검증

- `SoulLikeEditor Win64 Development` 빌드 성공.
- UBT 결과: `Result: Succeeded`

## Grux 메테오 바닥 폭발 판정 보정

### 문제

- `AGruxMeteor`가 플레이어 overlap에서 직접 데미지를 주고, 폭발 이펙트와 범위 데미지는 바닥 등 블로킹 hit에서만 실행되는 구조였다.
- 플레이어와 먼저 닿는 경우 폭발이 안 보이거나 의도와 다르게 직접 피격처럼 느껴질 수 있었다.
- 기존 범위 데미지는 같은 플레이어가 여러 컴포넌트로 trace되면 중복 데미지가 들어갈 여지가 있었다.

### 수정

- 플레이어 hit/overlap에서는 직접 데미지를 주지 않고 메테오가 계속 진행하도록 했다.
- 바닥 등 블로킹 대상에 맞아 `Explode()`가 호출될 때만 이펙트와 범위 데미지가 발생하도록 정리했다.
- 폭발 반경을 `ExplosionRadius`로 분리하고, 폭발 범위 안 플레이어에게 actor 기준 한 번만 데미지와 넉백을 적용하도록 했다.

### 검증

- `SoulLikeEditor Win64 Development` 빌드 성공.
- UBT 결과: `Result: Succeeded`

## Grux 메테오 폭발 이펙트 기본값 보정

### 문제

- `/Game/_dev/Actor/Projectile/BP_GruxMeteor`의 `ImpactParticle` 기본값이 `None`이었다.
- 기존 코드는 `ImpactParticle`이 있을 때만 `MultiCast_SpawnImpactEffect()`를 호출해서, 폭발 판정이 나도 파티클이 보이지 않았다.

### 수정

- `AGruxMeteor::Explode()`에서 폭발 시 항상 이펙트 멀티캐스트를 호출하도록 했다.
- 멀티캐스트 실행 시 `ImpactParticle`이 비어 있으면 `/Game/MegaMagicVFXBundle/VFX/MagicAuraVFX/VFX/Meteor/Systems/N_Meteor`를 기본 Niagara로 로드해 재생하도록 했다.

### 검증

- `SoulLikeEditor Win64 Development` 빌드 성공.
- UBT 결과: `Result: Succeeded`
