#include "stdafx.h"
#include "App.h"

App::App()
{

}

App::~App()
{
}


void App::RunMessageLoop()
{

	thread game_thread([&]() {
		double deltatime = 0.0;
		while (msg.message != WM_QUIT)
		{
			m_timer.Tick([&]() {
				deltatime = m_timer.GetElapsedSeconds();
				Update(deltatime);
				Render();
			});
			/*double interval = (1 / 60.0 - m_timer.GetElapsedSeconds())*1000;
			if(interval>0)
				Sleep(interval);*/
		}
	});
	
	this->Framework::RunMessageLoop();
	game_thread.join();
}

ComPtr<ID2D1Brush> App::GetDeafaultBrush()
{
	ComPtr<ID2D1SolidColorBrush>  Brush;
	m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LawnGreen), &Brush);
	return Brush;
}

ComPtr<IDWriteTextFormat> App::GetDeafaultWriteTextFomat()
{
	ComPtr<IDWriteTextFormat> fmt;
	m_deviceresources.GetDWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0, L"chs", &fmt);
	return fmt;
}

WCHAR*  App::GetFpsText()
{
	int fps = m_timer.GetFramesPerSecond();
	swprintf_s(fps_text, L"%d", fps);
	return fps_text;
}

int App::GetFps()
{
	return m_timer.GetFramesPerSecond();
}

HRESULT App::Initialize()
{
	HRESULT hr=this->Framework::Initialize();
	if (SUCCEEDED(hr))
	{
		RECT rect;
		GetClientRect(Application::hWnd, &rect);
		hr=m_deviceresources.GetD2DFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(Application::hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top),D2D1_PRESENT_OPTIONS_NONE),
			&m_RenderTarget
		);
	}

	D2D1_SIZE_F sizef = { m_RenderTarget->GetSize() };
	D2D1_SIZE_U sizeu = { sizef.width,sizef.height };
	m_RenderTarget->CreateCompatibleRenderTarget(&bitmapRT);
	return hr;
}



