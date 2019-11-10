
#include <windows.h>

#include "include/cef_sandbox_win.h"
#include "simple_app.h"

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {

  //CefEnableHighDPISupport();

  void* sandbox_info = NULL;
  CefMainArgs main_args(hInstance);
  CefRefPtr<SimpleApp> app(new SimpleApp);

  int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
  if (exit_code >= 0) {
    return exit_code;
  }

  CefSettings settings;
  settings.no_sandbox = true;
  CefInitialize(main_args, settings, app.get(), sandbox_info);

  CefRunMessageLoop();

  CefShutdown();
  return 0;
}
