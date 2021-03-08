#include "tests/cefsimple/simple_app.h"
#include <CommonDef.h>
#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "tests/cefsimple/simple_handler.h"

void __InitWin32ExtsV8Core(CefRefPtr<CefV8Context> context);

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

  const bool use_views = false;

  CefRefPtr<SimpleHandler> handler(new SimpleHandler(use_views));

  CefBrowserSettings browser_settings;

  std::string url;
  url = command_line->GetSwitchValue("url");
  if (url.empty())
    url = "http://www.baidu.com?test=1";

  if (use_views) {
    // Create the BrowserView.
    //CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
    //    handler, url, browser_settings, NULL, NULL, NULL);
    //
    //// Create the Window. It will show itself after creation.
    //CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
  } else {
    CefWindowInfo window_info;
    window_info.SetAsPopup(NULL, "cefsimple");
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                                  NULL);
  }
}

bool SimpleApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {

  const CefString messageName = message->GetName();
  CefRefPtr<CefListValue> args = message->GetArgumentList();

  if (messageName == STR_INIT_TO_RENDER) {
    g_hRemoteBrowser = (HWND)(INT_PTR)(args->GetInt(0));
    __InitWin32ExtsV8Core(nullptr);
    return true;
  }
  return false;
}

UINT_PTR g_uTimerId = 0;

void APIENTRY TestTimeProc(HWND, UINT uMsg, UINT_PTR wParam, DWORD lParam)
{
  if (wParam == g_uTimerId)
  {
    KillTimer(NULL, g_uTimerId);
    MessageBoxW(NULL, L"__TestTimeProc", L"__TestTimeProc", MB_OK);
  }
}

void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context)
{
#ifdef USE_EMBED_WIN32EXTS_FOR_CEF3
  //win32exts 已内嵌，无需注册。
#else
  __InitWin32ExtsV8Core(context);
#endif
}
