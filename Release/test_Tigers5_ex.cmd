@echo off
regsvr32 "%cd%\test_Tigers5\Tigers5.ocx"
start "" .\ExtWebView.exe --url="%cd%\test_Tigers5\Tigers5_Game.html"  --transparent_window1 --tab_ids=0 --tab_rect=0,0,1000,800
