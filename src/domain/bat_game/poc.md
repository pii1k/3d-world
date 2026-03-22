🦇 프로젝트명: 에코 나이트 (Echo Knight) - POC 설계안
1. 핵심 컨셉 (Core Concept)
시각의 제한: 초기 POC는 밝게 시작(전체 가시). 이후 단계에서 완전한 어둠(Blackout) 모드 전환.

에코로케이션: 초음파(Sound Pulse)가 지형에 반사될 때만 세상이 보임.

콜로니 서바이벌: 무리와 함께 이동하며 생존하고, 전설의 안식처로 이주하는 여정.

2. 플레이어 시점 및 조작 (TPP 시스템)
시점: 3인칭 백뷰 (Third-Person Pursuit Camera).

박쥐의 뒤쪽 상단에 카메라 위치.

카메라 스프링 암(Spring Arm): 가속 시 카메라가 멀어지고, 감속 시 가까워지는 연출로 속도감 부여.

기본 조작:

Mouse: 조준 및 회전 (박쥐가 바라보는 방향으로 비행 방향 결정).

W (Hold): 날갯짓(추진력 발생). 누를 때마다 스태미나 소모.

Space (Tap): 초음파 발사. 짧은 쿨타임 존재.

Shift: 급강하 또는 급가속 (파워형 종 특화).

3. 종별 능력치 및 체감 차이 (Presettings)
능력치	파워형 (불독박쥐)	지구력형 (작은숲박쥐)
비행 메커니즘	폭발적인 가속, 짧은 비행	일정한 속도 유지, 장거리 비행
스태미나	총량이 적고 소모가 빠름	총량이 많고 회복이 빠름
카메라 연출	가속 시 FOV가 70도 이상 급팽창	FOV 변화가 적고 안정적인 시야
초음파 성능	좁지만 멀리 나가는 직선형 파동	넓게 퍼지는 방사형 파동
4. 핵심 시스템 로직 (System Logic)
① 초음파 렌더링 (Echolocation Shader)

동작 방식: 1.  플레이어가 발사 버튼을 누르면 Origin(발사 위치)와 CurrentRadius(현재 파동 거리)를 쉐이더로 전달.
2.  CurrentRadius는 시간에 따라 일정 속도로 증가.
3.  쉐이더에서 distance(PixelPos, Origin)가 CurrentRadius 근처일 때만 해당 픽셀의 색상을 출력(나머지는 검은색).

시각적 피드백: 벽면에 닿은 파동은 '점구름(Point Cloud)' 효과로 잔상이 남았다가 1~2초 후 서서히 사라짐(Fade out).

② 콜로니 시스템 (Colony & Social Echo)

공명(Resonance): 플레이어가 쏜 음파가 NPC 박쥐에게 닿으면, 해당 NPC도 자동으로 작은 음파를 발사.

효과: "도미노 효과"처럼 음파가 퍼져나가며 거대 동굴의 전체 윤곽이 한순간에 드러나는 장관 연출.

군집 AI: Boids 알고리즘을 간소화하여 플레이어의 후방 일정 거리 내에서 무리를 지어 비행.

③ 생존 루프 (Survival Loop)

에너지 관리: 스태미나가 0이 되면 날갯짓 불가(추락).

사냥: 초음파 파동에 감지된 '나방(밝게 빛나는 점)'에 박쥐 모델이 닿으면 스태미나 즉시 회복.

5. POC 구현 우선순위 (Implementation Roadmap)
[Week 1] 기초 물리 및 카메라: 3차원 공간에서 마우스 방향으로 비행하는 TPP 박쥐 컨트롤러 구현.

[Week 2] 에코 쉐이더: 초기에는 전체 가시 유지. 이후 거리 기반 마스킹 쉐이더를 만들어 클릭 시 주변 지형이 보이게 함.

[Week 3] 콜로니 및 타겟: NPC 박쥐 배치 및 공명 시스템, 사냥 가능한 나방 객체 추가.

[Week 4] 수치 밸런싱: 파워형/지구력형 변수를 조정하여 두 종의 '손맛' 차이 테스트.

6. 설계 시 고려할 기술 키워드 (for C++/OpenGL)
Camera: glm::lookAt, glm::perspective, Spring Arm Logic.

Rendering: GLSL Fragment Shader, Distance Functions, Additive Blending (음파 중첩용).

Physics: Acceleration, Friction (공기 저항), Gravity.


---

목표 스코프 (POC)

초기 POC는 전체 가시, 이후 암흑 + 초음파로만 가시화되는 3D 비행 체험
기본 비행/카메라/스태미나/초음파/간단 군집/사냥 루프까지
모듈 구조 제안

Game Loop
Update(dt) → Input → Player → NPC → World → Render
Input
마우스 방향, 키 입력(W/Space/Shift)
InputState를 만들어 시스템에 전달
Player (BatController)
비행 물리(가속/감속/중력/마찰)
스태미나 소모/회복
종별 프리셋(파워형/지구력형) 데이터
Camera (Spring Arm TPP)
타겟: 박쥐 위치 + 후방 오프셋
속도에 따른 거리/FOV 변화
Echolocation System
초음파 이벤트 생성(Origin, StartTime, Speed, MaxRadius, Type)
파동 리스트 관리 (여러 파동 중첩)
쉐이더에 현재 파동 정보를 전달
World Render
기본 지형/오브젝트는 “보이지 않는 상태”
쉐이더에서 distance(Pixel, Origin) 근처만 출력
점구름 잔상(간단 잔상 텍스처/버퍼)
Colony (NPC Boids Lite)
플레이어 뒤쪽 집합 유지(간단한 Separation/Alignment/Cohesion 중 최소 1~2개)
플레이어의 파동 감지 시 NPC 파동 발사
Huntables (Moth)
위치 + 히트체크(거리 충돌)
초음파로 감지될 때 발광 처리
먹으면 스태미나 회복
핵심 데이터 구조

BatStats
max_stamina, stamina_regen, flap_cost, accel, drag, gravity, fov_boost, sonar_range, sonar_spread
SonarPulse
origin, start_time, speed, max_radius, spread_type(linear|radial), owner_id
InputState
mouse_dir, flap_hold, sonar_tap, boost
NPCState
pos, vel, cooldown, group_offset
시스템 흐름 (프레임 단위)

입력 수집 → InputState
플레이어 비행 업데이트 (가속/스태미나)
카메라 위치/FOV 보정
초음파 발사 처리 (SonarPulse 추가)
NPC 업데이트(간단 보이드 + 공명)
사냥 대상 충돌 체크 (스태미나 회복)
렌더링: 쉐이더에 현재 파동 정보 전달 → 가시화
렌더 파이프라인 포인트

쉐이더 uniform 구성:
pulse_count, pulse_origin[i], pulse_radius[i], pulse_type[i]
거리 기반 마스킹 + Additive Blending
점구름 잔상:
방법 A: 현재 프레임 에코 결과를 별도 텍스처에 누적 + 감쇠
방법 B: 에코 히트된 메시를 파티클 스프라이트로 짧게 생성
POC 마일스톤 구조 (재정리)

비행 물리 + 카메라 + 스태미나
에코 쉐이더 + 파동 데이터 전달
NPC 간단 군집 + 공명 파동
나방 사냥 루프 + 종별 밸런싱
