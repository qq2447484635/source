//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// 供 WindowsProject1.rc 使用
//
#pragma once
#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_WINDOWSPROJECT1_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_WINDOWSPROJECT1			107
#define IDI_SMALL				108
#define IDC_WINDOWSPROJECT1			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif
// 新对象的下一组默认值
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

#define MAX_LOADSTRING 100

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

namespace Application
{
	static int RESOLUTION_W=900, RESOLUTION_H=600;
	static HWND hWnd = 0;
	static HINSTANCE hInst = HINST_THISCOMPONENT;                                // 当前实例
	static WCHAR szTitle[MAX_LOADSTRING] = L"Direct2D Geometries Examples";                  // 标题栏文本
	static WCHAR szWindowClass[MAX_LOADSTRING] = L"D2DDemoApp";            // 主窗口类名
}