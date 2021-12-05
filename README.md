
Win32exts_for_CEF3 是一个有用的工具，它能够允许你在你的谷歌嵌入式框架（CEF）项目内部的 js 脚本中，

调用任意 Win32API 或者用户DLL导出API，乃至任意COM/ActiveX控件。

======================================

# 让CEF框架支持 COM/ActiveX控件

2019-08-17 11:22:14 tan_kaishuai 阅读数 222 文章标签： CEF ActiveX COM 插件

编辑

版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。

本文链接：https://blog.csdn.net/tan_kaishuai/article/details/99690082

通常各种浏览器都支持某种插件机制，以方便扩充页面脚本的能力，丰富页面内容。

比如：对于 Trident 内核（IE浏览器），可以使用 ActiveX控件。

对于Gecko内核（火狐浏览器）和Webkit/Blink内核（Safari，Chrome系列浏览器），可以使用NPAPI插件，以及PPAPI插件。

 

遗憾的是，各种浏览器的插件机制不仅不能够兼容统一，而且高版本的Chrome系列浏览器非但不再支持传统的NPAPI插件，

甚至PPAPI插件（Flash除外）亦难以部署发布，通常仅能用作本机调试之用，实为无奈。╮(╯﹏╰）╭

这里笔者不打算讨论如何统一各类浏览器插件机制的问题，貌似有现成的厂商做这个，比如某厂的 "牛插件"：

就是通过WebSocket实现的插件与浏览器的通信调用。

 

只简要介绍下在CEF框架中支持 COM/ActiveX控件，以兼容传统IE内核ActiveX插件的方法。

项目地址：

             https://github.com/tankaishuai/Win32exts_for_CEF3

为了让CEF框架支持 COM/ActiveX控件，只需要 在渲染进程的 OnContextCreated（）回调中添加如下代码：


virtual void OnContextCreated(...) {

    CefRefPtr<CefV8Value> windowObject = context->GetGlobal();

    //

    // 仅仅只需要调用 RegisterActiveXObject_CEF() API 即可.

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


然后，该CEF浏览器即支持了在js脚本 中加载ActiveX控件的能力，


![win32exts_for_cef3](https://github.com/tankaishuai/Win32exts_for_CEF3/blob/master/cef_load_ActiveX_demo/test_case1.jpg)


创建控件示例代码：

![win32exts_for_cef3](https://github.com/tankaishuai/Win32exts_for_CEF3/blob/master/cef_load_ActiveX_demo/test_js.png)


![win32exts_for_cef3](https://github.com/tankaishuai/Win32exts_for_CEF3/blob/master/cef_load_ActiveX_demo/test_case2.png)
![win32exts_for_cef3](https://github.com/tankaishuai/Win32exts_for_CEF3/blob/master/cef_load_ActiveX_demo/test_case3.png)
![win32exts_for_cef3](https://github.com/tankaishuai/Win32exts_for_CEF3/blob/master/cef_load_ActiveX_demo/test_case4.png)

 

/*********************************  ver 31.2019.10.20  ******************************

## 除原来的 ActiveXObject 之外， 新增对 ActiveX 控件的支持： 

创建：

var activex_ctrl = new ActiveXControl("prog_id", parent_wnd, x, y, r, b)


（parent_wnd 非常重要，将直接影响控件风格。

如果指定 0，将在一个弹出窗口创建；如果指定一个负数，则将优先选择

引擎传入的 SetRemoteBrowser( hwnd ) 浏览器句柄。）



## 每个控件都含有下列几个 Ax_*** 开头的内置的成员函数：

ptr = activex_ctrl.Ax_GetRaw()

pUnk = activex_ctrl.Ax_GetHost()

pDisp = activex_ctrl.Ax_GetControl()

hwnd = activex_ctrl.Ax_GetWindow()

activex_ctrl.Ax_ShowWindow( nCmdShow )

activex_ctrl.Ax_MoveWindow( [z_order] or [x, y] or [x, y, r, b] )

strInfo = activex_ctrl.Ax_ListSym()

cookie = activex_ctrl.Ax_ListenEvent( "event_iid", event_sink )



## 一个控件有哪些方法、属性可以通过 Ax_ListSym() 函数查看，当然也可以用其他工具。

例如笔者开发的 90坦克大战 游戏控件：

----Function----

QueryInterface=1610612736(1)

...

StartGame=1610809351(0)

EndGame=1610809352(0)

GetCurCount=1610809353(0)

GetCurScore=1610809354(0)

GetCurLevel=1610809355(0)

Execute=1610809356(0)

System=1610809357(0)

GetHelp=1610809358(0)

GetAbout=1610809359(0)

GetAppPath=1610809360(0)

InputMsgBox=1610809361(0)



调用控件方法：

ret = activex_ctrl.FunctionName( args ... )



获取属性：

ret = activex_ctrl.get_AttributeName()



修改属性：

activex_ctrl.put_AttributeName( new_val )



设置事件监听：

var event_sink = {}

event_sink.handler_259 = function( args ){

	// do something

}

event_sink.handler_s102 = function( args ){

	// do something

}

activex_ctrl.Ax_ListenEvent( "{34A715A0-6587-11D0-924A-0020AFC7AC4D}", event_sink )



取消事件监听：

activex_ctrl.Ax_ListenEvent( "{34A715A0-6587-11D0-924A-0020AFC7AC4D}", null )


*************************************************************/

 

## 示例用法如下：

    //创建游戏控件

    bbb = new ActiveXControl("Tank90Control.Tank90", -2)

    bbb.Ax_MoveWindow(50, 50, 600, 600)

    bbb.Ax_ShowWindow(1)

    //external.Log(bbb.Ax_ListSym())

    //自动开始游戏

    bbb.StartGame()

 

加载普通COM组建示例：

var active_x = new ActiveXObject("prog_appid");     //创建COM

ret = active_x.func1(1, 2);                                         //调用方法

ret = active_x.func2( "test", false, 0 ); 

与IE中的用法基本一致。实现了不改动 ActiveX控件 情况下的完美支持。可喜可贺可喜可贺。。





======================================

# win32exts 多语言 外部编程接口扩展

======================================

**脚本语言通用 Win32 FFI调用引擎简介**


![markdown](https://www.mdeditor.com/images/logos/markdown.png "markdown")


    一直希望有这样的一种统一江湖的脚本语言：
    
既拥有脚本语言的动态解释执行能力，又拥有接近如C/C++般高效的执行效率，同时还具有如C/C++甚至汇编语言般强大的功能，可以直接调用系统功能。语言干净简练、子功能模块库丰富多彩，同时作为嵌入式脚本也能得心应手。

**可惜没有！**

    常见的语言如lua, js, vbs, Python, PowerShell 都或多或少存在一些问题。PowerShell 语法过于诡异，
    
    更接近于一种增强版的批处理脚本。Js/vbs/lua 使用方便，但原生功能较弱。Python拥有强大的第三方库，但打包部署麻烦。
    
    于是，才有了大千世界、芸芸众生，各种语言的各领风骚。


## 言归正传。

    笔者在开发过程中各种语言均有接触（当然更亲赖Python和Lua，毕竟作为嵌入式脚本非常合适）。经常摆在面前的问题是，
    
    某语言在特定平台上需要直接调用系统API才能实现某些功能的问题。
    
    当然，各种语言都有提供一些外部函数扩展（FFI）的方式。通常是需要自己编写一个模块(DLL)或者额外的脚本，难免有点愚拙。
    
    所以此处只考虑一种无须额外写太多第三方库，实现统一调用任意系统API的机制。
    
**目前主要流行的语言中，已有一些FFI支持库：**


![Pandao editor.md](https://pandao.github.io/editor.md/images/logos/editormd-logo-180x180.png "Pandao editor.md")

    各种扩展库语法差异极大，并且有些需要带上庞大数量的支持库，有些功能又极为简略，不能适应较为复杂的函数调用
    
    （典型的如EnumWindows类带回调的，以及匿名函数、变参数目等的调用）。

    因此，笔者开发的win32exts扩展框架旨在谋求一种 【发布简单】的(一个模块)，【统一的】(各种脚本中语法基本一致)，
    
    【高效的】、【功能强大】 的(原生API,COM,C++非COM接口成员均支持)模式。

**一句话，有了 win32exts，真正实现 Do whatever you want，放之四海而皆准是也。**


### 以Python为例(其他语言其实差不多长一个样吧)，

### win32exts调用的基本用法如下：

（1）常规具名函数（以MessageBoxA/W为例）调用：

导入模块符号。第一个参数为待加载的模块名，可以带路径，传入"*"表示当前进程的所有模块；

第二个参数表示函数符号名称，传入"\*"表示该模块的所有符号。

win32exts.load_sym("\*", "\*")

或 win32exts.load_sym("C:\\windows\\system32\\user32.dll", "MessageBoxW")

或 win32exts.load_sym("user32", "MessageBoxA")

或 win32exts.load_sym("user32", "\*")

win32exts.MessageBoxA(0, "call MessageBoxA", "", 1)

宽字符需要用 win32exts.L() 包装，与C/C++雷同。

win32exts.MessageBoxW(0, win32exts.L("call MessageBoxW"), None, 1)


（2）带回调的函数（以EnumWindows为例）调用：

先分配一块内存后面用：

	g_buf = win32exts.malloc(2*260)

定义一个回调函数：
	
	
	def EnumWndProc(args):   
		【args为参数包，以下取参数】
		hWnd = win32exts.arg(args, 1)
		lParam = win32exts.arg(args, 2)
		
		win32exts.GetWindowTextW(hWnd, g_buf, 260)
		【读取内存中的宽字符串】
		【read_***系列接口读内存，write_***系列接口写内存】
		strText = win32exts.read_wstring(g_buf, 0, -1)
	
		win32exts.MessageBoxW(0, win32exts.L(strText), g_buf, 1)

		strRetVal = "1, 8"
		g_index = g_index + 1
		if g_index > 3:   【假设只弹框三次】
			strRetVal = "0, 8"
	
		【返回值是形如这样的字符串: "回调返回值, 参数字节数",
		对于 cdecl 调用约定，参数字节数总是取 0 】
		return strRetVal

然后调用：

	win32exts.EnumWindows(win32exts.callback("EnumWndProc"), 0)

win32exts.callback（）用于包装一个Python回调函数。

对于js/lua 等等语言来说，没有提供该函数，直接传入即可，亦即：

	win32exts.EnumWindows(EnumWndProc, 0)

（3）匿名（非具名）函数调用：

假设通过某个接口获取了某函数的地址 lFuncAddr，然后可以类似下述方式调用：

	win32exts.push_value(arg1)     【参数是整数】
	win32exts.push_wstring("arg2") 【参数是宽字符串】
	win32exts.push_astring(arg3)   【参数是多字节字符串】
	win32exts.push_double(arg4)    【参数是双精度浮点数】
	win32exts.push_float(arg5)     【参数是单精度浮点数】
	iRetVal = win32exts.call( lFuncAddr )

当然具名函数也可以类似调用，例如：

	win32exts.push_value(0)
	
	win32exts.push_astring("Py_MessageBoxA_V1")
	
	win32exts.push_value(0)
	
	win32exts.push_value(0)
	
	iRetVal = win32exts.sym_call("MessageBoxA")  【或用 func_call】



# 实在编不下去了（←___←b'），下面是 win32exts 提供的接口分类清单, 至于典型用法请参考git上的 win32exts_for_Xxxx 仓库的demo 示例。

![win32exts](https://github.com/tankaishuai/win32exts_for_Python/blob/master/API/b1.jpg)
![win32exts](https://github.com/tankaishuai/win32exts_for_Python/blob/master/API/b2.jpg)

当然，也可以通过 VS的对象查看器 或者 Python 的 win32exts.help() 命令查看详细API信息：

![win32exts](https://github.com/tankaishuai/win32exts_for_Python/blob/master/API/a1.jpg)
![win32exts](https://github.com/tankaishuai/win32exts_for_Python/blob/master/API/a2.jpg)
![win32exts](https://github.com/tankaishuai/win32exts_for_Python/blob/master/API/a3.jpg)
