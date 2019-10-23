
void OnContextCreated() {

	CefRefPtr<CefV8Value> windowObject = context->GetGlobal();

	//
	// To register win32exts, just call RegisterActiveXObject_CEF() API.
	//
	typedef bool (* RegisterActiveXObject_CEF_T)(CefV8Value *windowObject, HWND hWnd, LPVOID lpReserved);
	typedef void (* SetRemoteBrowser_CEF_T)(HWND hWnd);

	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	PathRemoveFileSpecW(szPath);
	PathAppendW(szPath, L"win32exts_web.dll");
	HMODULE hDll = LoadLibraryW(szPath);
	if (hDll){
		RegisterActiveXObject_CEF_T pfnRegisterActiveXObject_CEF =
			(RegisterActiveXObject_CEF_T)GetProcAddress(hDll, "RegisterActiveXObject_CEF");

		if (pfnRegisterActiveXObject_CEF){
			pfnRegisterActiveXObject_CEF(windowObject, g_hRemoteBrowser, NULL);
		}

		  //
  		// Set the browser window handle for ActiveX Control.
  		//
 	 	SetRemoteBrowser_CEF_T pfnSetRemoteBrowser_CEF =
   			 (SetRemoteBrowser_CEF_T)GetProcAddress(hDll, "SetRemoteBrowser_CEF");
 		 if (pfnSetRemoteBrowser_CEF) {
    			pfnSetRemoteBrowser_CEF(g_hRemoteBrowser);
 	 	}
	}
}
