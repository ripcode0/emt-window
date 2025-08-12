### EMT Window API Wrapper

TODO Lists

* 1 Avoid including windows.h directly in headers; instead, provide forward declarations for commonly used `WinAPI` types in a file -- DONE

* 2 Create a top-level base class named `Wnd` (all windows will be derived from this class).
    -- progressing

* 3 Design the system to be as clear and easy to understand as possible.

* 4 Separate the Application from the actual window objects.

* 5 Have only one global WndDefaultProc procedure function initially, and create one local procedure function for each object.