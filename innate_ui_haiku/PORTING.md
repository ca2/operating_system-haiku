# Initial Haiku native UI port

Baseline commit: c9ed8a5 copies innate_ui_win32 unchanged from the local
operating_system-windows checkout. Legacy Windows sources and project files are
retained but excluded from the Haiku CMake target.

Implemented: native BWindow dialogs, BButton click callbacks, BStringView labels,
control positioning/sizing, label fonts and preferred sizes, show/hide/activation,
and dispatch through the existing Haiku application event loop.

Native BPopUpMenu support also provides the Haiku title-bar context menu.
This first adaptation does not implement icons, Windows resource-menu loading,
or text input. create_icon_still currently creates a label.
Windows icon source files are historical reference, not active factories.
