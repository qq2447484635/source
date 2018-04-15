#pragma once
#include "M.h"
#include <deque>
#include <thread>
class WinComponent
{
public:
	virtual void OnUpdata(float DeltaTime)
	{

	}
	virtual void OnRender(float DeltaTime)
	{

	}
	virtual ~WinComponent()
	{

	}
};

class Framework
{
public:

	Framework();
	virtual ~Framework();
	virtual HRESULT Initialize();
	virtual ATOM                MyRegisterClass(HINSTANCE hInstance);
	virtual BOOL                InitInstance(HINSTANCE, int);
	virtual void RunMessageLoop();
	virtual HRESULT CreateDeviceIndependentResources();
	virtual HRESULT CreateDeviceResources();
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	void OnUpdata();
	virtual void Load();
	std::deque<WinComponent*> ObjectPanel;
protected:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	ID2D1Factory *m_pD2DFactory;
	ID2D1HwndRenderTarget *m_pHwndRenderTarget;
};

inline Framework::Framework()
{
	
}

inline Framework::~Framework()
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pHwndRenderTarget);
}

inline ATOM Framework::MyRegisterClass(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Framework::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "D2DDemoApp";
	return RegisterClassEx(&wcex);
}

inline BOOL Framework::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	FLOAT dpiX, dpiY;
	m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	m_hwnd = CreateWindow(
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
	if (!m_hwnd)
	{
		return FALSE;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return TRUE;
}

inline void Framework::RunMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		OnUpdata();
	}
	
}

inline HRESULT Framework::CreateDeviceIndependentResources()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	return hr;
}

inline HRESULT Framework::CreateDeviceResources()
{
	HRESULT hr;
	RECT Rect;
	GetClientRect(m_hwnd, &Rect);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(Rect.right, Rect.bottom)),
		&m_pHwndRenderTarget
	);
	return hr;
}

inline LRESULT CALLBACK Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{

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

inline void Framework::OnUpdata()
{
	m_pHwndRenderTarget->BeginDraw();
	m_pHwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	for (std::deque<WinComponent*>::iterator i = ObjectPanel.begin(); i != ObjectPanel.end(); i++)
	{
		/*std::thread updata(&WinComponent::OnUpdata,*i,0.0f);
		updata.detach();
		std::thread render(&WinComponent::OnRender, *i,0.0f);
		render.detach();*/
		(*i)->OnRender(0);
	}
	m_pHwndRenderTarget->EndDraw();
}

inline void Framework::Load()
{
}

inline HRESULT Framework::Initialize()
{
	HRESULT hr;
	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		// Register the window class.
		MyRegisterClass(HINST_THISCOMPONENT);
		// Create the application window.
		hr=InitInstance(m_hInstance, SW_SHOW) ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}
	//Initialize DeviceResource
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources();
	}
	Load();
	return hr;
}