#pragma once
#include <queue>
#include "DevicesResource.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

namespace Application
{
	static HWND hWnd = 0;
	static HINSTANCE m_hInstance;
}

class Framework
{
public:
	Framework();
	virtual ~Framework();
	virtual HRESULT Initialize();
	virtual ATOM                MyRegisterClass(HINSTANCE hInstance);
	virtual BOOL                InitInstance(HINSTANCE, int);
	virtual void RunMessageLoop();
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	void OnUpdata();
	void OnRender();
protected:
	DevicesResouce m_deviceresources;
};

inline Framework::Framework()
{
	
}

inline Framework::~Framework()
{
	
}

inline ATOM Framework::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Framework::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = 0;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "D2DDemoApp";
	return RegisterClassEx(&wcex);
}

inline BOOL Framework::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	FLOAT dpiX, dpiY;
	m_deviceresources.GetD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);
	Application::hWnd = CreateWindow(
		"D2DDemoApp",
		"Direct2D Geometries Examples",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(840.f * dpiX / 96.f)),
		static_cast<UINT>(ceil(700.f * dpiY / 96.f)),
		NULL,
		NULL,
		hInstance,
		this
	);
	if (!Application::hWnd)
	{
		return FALSE;
	}
	ShowWindow(Application::hWnd, SW_SHOW);
	UpdateWindow(Application::hWnd);

	return TRUE;
}

inline void Framework::RunMessageLoop()
{
	MSG msg;
	while (true)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			OnUpdata();
			OnRender();
		}
	}

}


inline LRESULT CALLBACK Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:{	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline void Framework::OnUpdata()
{
	
}

inline void Framework::OnRender()
{
}


inline HRESULT Framework::Initialize()
{
	HRESULT hr;
	// Register the window class.
	MyRegisterClass(HINST_THISCOMPONENT);
	// Create the application window.
	hr = InitInstance(Application::m_hInstance, SW_SHOW) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(Application::hWnd, SW_SHOWNORMAL);
		UpdateWindow(Application::hWnd);
	}
	return hr;
}