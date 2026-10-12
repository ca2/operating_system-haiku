# Initial Haiku accessibility backend

Baseline commit: 2bd703d, an unchanged copy of accessibility_windows.

The active build registers accessibility, application, and main_window implementations. It uses the Haiku application roster for executable-path lookup and native window scripting for the first window and its title. It also supports application activation and cooperative close requests. Requests waiting for scripting replies have a 500 ms timeout.

Applications must support the requested scripting properties. Window index zero is a provisional main-window choice; it is not a semantic main-window discovery algorithm. Windows numeric child-control IDs and indexed text traversal are unsupported and report an error. Desktop automation trees and input blocking still need a separate Haiku implementation.

Windows source bodies are retained as line comments in application.cpp, main_window.cpp, and _.cpp. Other copied legacy files are retained for reference and excluded from the active CMake source list. The original Windows build configuration is preserved as comments above the Haiku configuration.
