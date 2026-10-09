# Initial Haiku native UI port

Baseline commit: c9ed8a5 copies innate_ui_win32 unchanged from the local
operating_system-windows checkout. Legacy Windows sources and project files are
retained but excluded from the Haiku CMake target.

Implemented: native BWindow dialogs, BButton click callbacks, BStringView labels,
control positioning/sizing, label fonts and preferred sizes, show/hide/activation,
and dispatch through the existing Haiku application event loop.

Native BPopUpMenu support also provides the Haiku title-bar context menu.
Native bitmap icons use Haiku's Translation Kit and a BView for display.
Top-level dialogs are retained by innate_ui so they survive the creation callback.
This first adaptation does not implement Windows resource-menu loading or text input.
