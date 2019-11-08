@echo off
regsvr32  /s ".\test_Tigers5\Tigers5.ocx"
".\ext_cef_client.exe"  "--url=%cd%\test_Tigers5\Tigers5_Game.html"
