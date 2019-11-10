#include "tests/cefsimple/simple_handler.h"

#include <windows.h>
#include <string>

#include "include/cef_browser.h"

void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  SetWindowText(hwnd, std::wstring(title).c_str());
}
