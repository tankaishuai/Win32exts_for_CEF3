
virtual void OnContextCreated(CefRefPtr<ClientAppRenderer> app,
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) OVERRIDE {

	CefRefPtr<CefV8Value> windowObject = context->GetGlobal();

	//
	// To register win32exts, just call RegisterActiveXObject_CEF() API.
	//
	typedef bool (* RegisterActiveXObject_CEF_T)(CefV8Value *windowObject, LPVOID lpReserved1, LPVOID lpReserved2);

	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	PathRemoveFileSpecW(szPath);
	PathAppendW(szPath, L"win32exts_web.dll");
	HMODULE hDll = LoadLibraryW(szPath);
	if (hDll){
		RegisterActiveXObject_CEF_T pfnRegisterActiveXObject_CEF =
			(RegisterActiveXObject_CEF_T)GetProcAddress(hDll, "RegisterActiveXObject_CEF");

		if (pfnRegisterActiveXObject_CEF)
		{
			pfnRegisterActiveXObject_CEF(windowObject, NULL, NULL);
		}
	}
}
