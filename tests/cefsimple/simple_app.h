
#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include <CommonDef.h>

class SimpleApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {
 public:
  SimpleApp();

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() {
    return this;
  }

// browser
  virtual void OnContextInitialized() OVERRIDE;

// render
  virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
  }

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context);

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message);

 private:
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
