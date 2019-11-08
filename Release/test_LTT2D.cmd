@echo off
regsvr32  /s ".\test_LTT2D\LTT2D.ocx"
".\ext_cef_client.exe"  "--url=%cd%\test_LTT2D\TestNew_CEF.html"
