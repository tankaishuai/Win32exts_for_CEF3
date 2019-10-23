function test_activex(){
	//创建游戏控件
	bbb = new ActiveXControl("Tank90Control.Tank90", -2)
	bbb.Ax_MoveWindow(50, 50, 600, 600)
	bbb.Ax_ShowWindow(1)
	//external.Log(bbb.Ax_ListSym())

	//自动开始游戏
	bbb.StartGame()
}

setTimeout("test_activex()", 6000)
