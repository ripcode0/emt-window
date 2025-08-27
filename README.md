### EMT Window API Wrapper

#### TODO Lists

| # | Task | Details | Status | Priority |
|---|------|---------|--------|----------|
| 1 | Avoid including `windows.h` in headers | Use a separate header with forward declarations for common WinAPI types | Done | High |
| 2 | Create base class `Wnd` | All windows should be derived from this class | In progress | High |
| 3 | Design for clarity | Keep the system simple and easy to understand | Planned | Medium |
| 4 | Separate Application from Windows | Application logic should be independent of window objects | Planned | High |
| 5 | Message handling | Start with one global `WndDefaultProc`, later give each object its own local procedure | Planned | Medium |
