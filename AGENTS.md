1. Think Before Coding
Don't assume. Don't hide confusion. Surface tradeoffs.

Before implementing:

State your assumptions explicitly. If uncertain, ask.
If multiple interpretations exist, present them - don't pick silently.
If a simpler approach exists, say so. Push back when warranted.
If something is unclear, stop. Name what's confusing. Ask.
2. Simplicity First
Minimum code that solves the problem. Nothing speculative.

No features beyond what was asked.
No abstractions for single-use code.
No "flexibility" or "configurability" that wasn't requested.
No error handling for impossible scenarios.
If you write 200 lines and it could be 50, rewrite it.
Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

3. Surgical Changes
Touch only what you must. Clean up only your own mess.

When editing existing code:

Don't "improve" adjacent code, comments, or formatting.
Don't refactor things that aren't broken.
Match existing style, even if you'd do it differently.
If you notice unrelated dead code, mention it - don't delete it.
When your changes create orphans:

Remove imports/variables/functions that YOUR changes made unused.
Don't remove pre-existing dead code unless asked.
The test: Every changed line should trace directly to the user's request.

4. Goal-Driven Execution
Define success criteria. Loop until verified.

Transform tasks into verifiable goals:

"Add validation" → "Write tests for invalid inputs, then make them pass"
"Fix the bug" → "Write a test that reproduces it, then make it pass"
"Refactor X" → "Ensure tests pass before and after"
For multi-step tasks, state a brief plan:

1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]
Strong success criteria let you loop independently. Weak criteria ("make it work") require constant clarification.

5. Korean Commit and Push Convention
When the user asks to commit, push, or "커밋 푸시", use a Korean Conventional Commit style by default.

Commit by meaningful task unit, not by every tiny file edit. Keep unrelated work in separate commits.

Commit message format:

`type(scope): 한국어 요약`

Use a short Korean summary that explains the outcome, not just the touched file.

Recommended types:

- `feat`: new gameplay, UI, system, or tool feature
- `fix`: bug fix or broken behavior correction
- `refactor`: structure cleanup without intended behavior change
- `chore`: asset cleanup, project setup, tooling, MCP, Git, or maintenance work
- `build`: build setting, module, plugin, or engine-version related changes
- `test`: test additions or verification-only changes
- `docs`: documentation or instruction updates

Recommended scopes:

- `asset`: Unreal content/assets
- `mcp`: UE-MCP bridge or MCP workflow
- `build`: Unreal build, target, module, or plugin build config
- `ui`: UI widgets and menus
- `ai`: AI, behavior tree, perception, or enemy logic
- `player`: player characters, controls, or player abilities
- `network`: multiplayer/session/online subsystem work
- `docs`: documentation and agent instructions

Examples:

- `chore(asset): 보스 에셋 정리 및 스피어 참조 이전`
- `fix(player): 스피어 투척 메시 참조 경로 수정`
- `build: 언리얼 5.7 빌드 설정 적용`
- `docs: 에이전트 커밋 규약 추가`

When committing:

1. Check `git status` first.
2. Stage only files related to the requested work.
3. Run a relevant verification when practical, such as an Unreal build for C++ changes.
4. Commit with the Korean Conventional Commit format above.
5. Push only when the user explicitly asks to push, or explicitly asks for "커밋 푸시".
6. After committing or pushing, report the commit hash, message, branch state, and verification result.

For this SoulLike modernization project, the user has requested an automatic commit/push workflow for completed task units. When a meaningful task unit is completed, and the work is no longer an unfinished experiment, automatically commit and push it unless the user explicitly says not to.

Automatic commit/push applies after:

- documentation baseline updates, such as roadmap, work-log, or architecture records
- C++ changes that have been built or otherwise verified
- asset cleanup that has been checked with `git status`
- MCP/tooling setup changes that are stable enough to preserve

Before automatic commit/push:

1. Confirm the change is a coherent task unit.
2. Keep unrelated dirty files out of the commit.
3. Prefer running verification when practical.
4. Use the Korean Conventional Commit format.
5. Push to the current working branch, normally `main`.

6. Project Documentation Convention
This project is an AI-assisted legacy Unreal project modernization effort. Documentation should record how the project improves over time, not just static setup instructions.

Use `docs/` for ongoing Korean project records. Prefer date-based work logs that explain what changed, why it changed, how AI was used, and what remains to do.

Recommended structure:

- `docs/README.md`: documentation purpose, index, and how to read the records
- `docs/work-log/YYYY-MM-DD.md`: daily or session-based work record
- `docs/architecture/YYYY-MM-DD-topic.md`: current or target code architecture, responsibility maps, runtime flows, and refactor baselines
- `docs/decisions/YYYY-MM-DD-topic.md`: important design or technical decisions, only when a decision needs separate explanation
- `docs/summaries/`: milestone summaries, portfolio-ready summaries, or company-facing summaries

For each `docs/work-log/YYYY-MM-DD.md`, include these sections when relevant:

- `작업 목표`: what the session tried to improve
- `기존 문제`: legacy issue, build issue, asset issue, design weakness, or workflow pain point
- `AI 활용 방식`: how Codex, UE-MCP, search, build logs, or Git analysis were used
- `구현/수정 내용`: concrete code, asset, config, or workflow changes
- `검증 결과`: build result, editor check, git status, runtime check, or known unverified areas
- `의사결정`: why this approach was chosen over alternatives
- `남은 작업`: follow-up tasks and risks

Do not create generic install manuals unless the user explicitly asks for them or the setup itself was part of the session. If MCP, Git LFS, build setup, asset cleanup, or commit rules matter, record them inside the relevant dated work log or milestone summary instead of making standalone setup documents by default.

When asked to document progress, update or create the appropriate dated document and keep it grounded in actual completed work. Avoid vague claims like "improved quality"; name the specific improvement, verification, and remaining risk.
