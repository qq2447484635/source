#include "stdafx.h"
#include "DemoApp.h"
#include <cstring>

DemoApp::DemoApp()
{
	/************FIX FPS****************************/
	/*m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 30);
	/***********************************************/
	center.x = center.y = 350;
	Current_Radian = 0.0f;
	RadianPerSec = _2PI / 3.0f;
	length = 150;

	AddComponent(rain);
	thread r2([&]() 
	{
		int a = 5;
		while (a > 0)
		{
			Sleep(2500);
			rain.AddTears(100);
			a--;
		}
	});
	r2.detach();
}


DemoApp::~DemoApp()
{
}

void DemoApp::Render(ID2D1HwndRenderTarget *RT)
{
	/********************************************TITLE***********************************/
	SetWindowTextW(m_RenderTarget->GetHwnd(), GetFpsText());
	/************************************************************************************/
	int start = clock();
	RT->BeginDraw();
	/******************************************PAINT***********************************/
	ComPtr<ID2D1Brush> Brush = GetDeafaultBrush();
	RT->Clear(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));
	static bool init = false;
	static double f = 0;
	static double speed = 1.0 / 15.0;
	static DX::StepTimer atimer;
	if (!init)
	{
		m_deviceresources.GetImage(L"D:\\image\\back.png", RT, am);
		thread s([&]() 
		{
			while (msg.message != WM_QUIT)
			{
				atimer.Tick([&]()
				{
					double tf = f + 0.02*speed;
					if (tf > am.size()) f = 0;
					else f = tf;
				});
			}
		});
		s.detach();
		init = true;		
	}
	if (f > am.size())
		return;
	RT->DrawBitmap(am[(int)f].Get(), D2D1::RectF(0, 100, Application::RESOLUTION_W,Application::RESOLUTION_H));
	/*******************************************END************************************/
	App::Render(RT);
	RT->EndDraw();
}

void DemoApp::Update(double deltatime)
{
	App::Update(deltatime);
	Current_Radian += RadianPerSec*deltatime;
}
