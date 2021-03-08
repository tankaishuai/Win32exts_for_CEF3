
#include "include/cef_app.h"
#include <Shlwapi.h>
#include <CommonDef.h>
#pragma comment(lib, "shlwapi.lib")

void __InitWin32ExtsV8Core(CefRefPtr<CefV8Context> context) {

  CefRefPtr<CefV8Value> windowObject = (context ? context->GetGlobal() : nullptr);

  //
  // To register win32exts, just call RegisterActiveXObject_CEF_***() API.
  //
  typedef bool(*RegisterActiveXObject_CEF_T)(CefV8Value *windowObject, HWND hWnd, LPVOID lpReserved);
  typedef void(*SetRemoteBrowser_CEF_T)(HWND hWnd);

  WCHAR szPath[MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, szPath, MAX_PATH);
  PathRemoveFileSpecW(szPath);
#ifdef USE_EMBED_WIN32EXTS_FOR_CEF3
  PathAppendW(szPath, L"libcef.dll");
#else
  PathAppendW(szPath, L"win32exts_web.dll");
#endif
  HMODULE hDll = LoadLibraryW(szPath);

  if (hDll) {
    RegisterActiveXObject_CEF_T pfnRegisterActiveXObject_CEF =
      (RegisterActiveXObject_CEF_T)GetProcAddress(hDll, "RegisterActiveXObject_CEF");

    if (pfnRegisterActiveXObject_CEF && windowObject) {
      pfnRegisterActiveXObject_CEF(windowObject, g_hRemoteBrowser, NULL);
    }

    //
    // Set the browser window handle for ActiveX Control.
    //
    SetRemoteBrowser_CEF_T pfnSetRemoteBrowser_CEF =
      (SetRemoteBrowser_CEF_T)GetProcAddress(hDll, "_SetRemoteBrowser_CEF");
    if (pfnSetRemoteBrowser_CEF) {
      pfnSetRemoteBrowser_CEF(g_hRemoteBrowser);
    }
  }
}
