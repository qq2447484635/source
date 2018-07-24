#pragma once
#include "stdafx.h"
#include <thread>
#include <memory>
#include <ctime>
#include "Resource.h"
#include <queue>
#include<list>
#include "DevicesResource.h"
#include "StepTimer.h"
#include "Component.h"


using namespace std;


class Framework
{
public:
	Framework();
	virtual ~Framework();
	virtual HRESULT Initialize();
	virtual void RunMessageLoop();
	virtual void Render() {};
	virtual void Update(double) {};
	virtual DevicesResouce& GetResource() { return m_deviceresources; };
private:
	virtual ATOM                MyRegisterClass(HINSTANCE hInstance);
	virtual BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
protected:
	ComPtr<ID2D1HwndRenderTarget> m_RenderTarget;
	DevicesResouce m_deviceresources;
	MSG msg = {0};
	DX::StepTimer m_timer;
};

inline Framework::Framework()
{

}

inline Framework::~Framework()
{

}

inline ATOM Framework::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = Application::szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

inline BOOL Framework::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	FLOAT dpiX, dpiY;
	Application::hInst = hInstance;
	m_deviceresources.GetD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);
	Application::hWnd = CreateWindowW(Application::szWindowClass, Application::szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(Application::RESOLUTION_W * dpiX / 96.f)),
		static_cast<UINT>(ceil(Application::RESOLUTION_H * dpiY / 96.f)), nullptr, nullptr, hInstance, nullptr);
	if (!Application::hWnd)
	{
		return FALSE;
	}
	return TRUE;
}

inline void Framework::RunMessageLoop()
{
	
	HACCEL hAccelTable = LoadAccelerators(Application::hInst, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	//MesssageLoop
	while (msg.message != WM_QUIT)
	{
		//if (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE))
		if (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	
}

inline LRESULT CALLBACK Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/*case WM_COMMAND:
		{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		break;*/
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline HRESULT Framework::Initialize()
{
	HRESULT hr;
	// Register the window class.
	MyRegisterClass(HINST_THISCOMPONENT);
	// Create the application window.
	hr = InitInstance(Application::hInst, SW_SHOW) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(Application::hWnd, SW_SHOWNORMAL);
		UpdateWindow(Application::hWnd);
	}
	return hr;
}

