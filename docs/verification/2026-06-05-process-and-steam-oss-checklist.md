# 2026-06-05 별도 프로세스 및 Steam OSS 검증 체크리스트

## 목적

PIE 2 Players로 확인하기 어려운 백그라운드 FPS 문제와 Steam OSS 세션 흐름을 별도 프로세스에서 검증한다.

## 1. NULL 직접 접속 테스트

### 실행

```bat
Scripts\LaunchNullListenTest.bat
```

### 기대 결과

- 리슨서버 창이 `/Game/_dev/Level/MainWorld?listen`으로 열린다.
- 클라이언트 창이 `127.0.0.1`로 접속한다.
- 두 창 모두 포커스 상태와 무관하게 비교 가능한 FPS를 유지한다.

### 확인 항목

- 서버/클라이언트가 같은 월드에 있는가.
- 서로의 캐릭터 이동이 보이는가.
- AI가 양쪽 화면에서 보이는가.
- AI 몽타주 속도 변경이 양쪽 화면에서 같은 타이밍으로 보이는가.
- 클라이언트 접속 실패 시 로그에 `TravelFailure`, `NetworkFailure`, `PendingNetGame` 관련 메시지가 있는가.

## 2. Steam 메뉴 세션 테스트

### 실행

```bat
Scripts\LaunchSteamMenuTest.bat
```

### 전제 조건

- Steam 클라이언트가 실행 중이어야 한다.
- 현재 AppID는 `480`이다.
- 같은 PC와 같은 Steam 계정에서 두 인스턴스를 띄우면 실패할 수 있다.
- 실제 Steam 세션 검증은 두 PC 또는 두 Steam 계정이 가장 안정적이다.

### 테스트 순서

1. 첫 번째 창에서 Host를 실행한다.
2. 두 번째 창에서 Refresh 또는 Join 메뉴를 연다.
3. 서버 목록에 호스트 세션이 보이는지 확인한다.
4. Join을 눌러 세션에 접속한다.
5. 접속 후 같은 월드에 스폰되는지 확인한다.

### 통과 기준

- 로그에 `OSS Pointer Found. Name : STEAM` 또는 Steam OSS 이름이 출력된다.
- Host 시 `Create Session` 로그가 출력된다.
- Join 시 `GetResolvedConnectString`이 주소를 반환한다.
- 클라이언트가 `ClientTravel`로 서버에 들어간다.

### 실패로 볼 증상

- 두 번째 인스턴스에서 Steam 초기화 실패.
- 서버 목록이 비어 있음.
- `CreateSession` 실패.
- `GetResolvedConnectString` 실패.
- Join 후 메뉴 맵으로 돌아감.

## 3. 기록 항목

테스트할 때 아래 내용을 작업 로그에 남긴다.

- 실행 스크립트
- Steam 사용 여부
- Steam 계정/PC 수
- Host 성공 여부
- Find Sessions 성공 여부
- Join 성공 여부
- AI 동기화 확인 여부
- 실패 로그 핵심 메시지

## 현재 결론

Steam OSS 설정과 세션 분기 코드는 적용했지만, 실제 Steam 세션 성공 여부는 런타임 환경에 의존한다.

게임플레이 동기화 수정은 계속 NULL 직접 접속 테스트와 Steam 세션 테스트를 병행해서 확인한다.
