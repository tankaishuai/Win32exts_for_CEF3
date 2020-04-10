@echo off
regsvr32 "%cd%\test_Tigers5\Tigers5.ocx"
.\ExtWebView.exe --url="%cd%\test_Tigers5\Tigers5_Game.html"  --tab_ids=0 --tab_rect=0,0,1000,800
