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
	RadianPerSec = _2PI/3.0f;
	length = 150;
}


DemoApp::~DemoApp()
{
}

void DemoApp::Render()
{
	/********************************************TITLE***********************************/
	SetWindowTextW(m_RenderTarget->GetHwnd(), GetFpsText());
	/************************************************************************************/
	int start = clock();

	m_RenderTarget->BeginDraw();
	m_RenderTarget->Clear();
	/******************************************PAINT***********************************/
	ComPtr<ID2D1Brush> Brush = GetDeafaultBrush();

	/*for (int i = 0; i < 1200; i++)
	{
		D2D1_POINT_2F p1 = {i,100*sin(i/200.0*_2PI-Current_Radian)+450};
		D2D1_POINT_2F p2 = {i+1,100*sin((i+1)/200.0*_2PI-Current_Radian)+450};
		m_RenderTarget->DrawLine(p1, p2, Brush.Get(),2.0);
	}

	m_RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(600+ 400 * sin(Current_Radian), 300), 10, 10), Brush.Get(),3.0);

	WCHAR mms[8] = { 0 };
	int elapsed = clock() - start;
	swprintf_s(mms, L"%d ms", elapsed-elapsed%2);
	m_RenderTarget->DrawTextW(mms, wcslen(mms), GetDeafaultWriteTextFomat().Get(), D2D1::RectF(0, 0, 250, 50), Brush.Get());*/
	((ID2D1SolidColorBrush*)Brush.Get())->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	rain.render(m_RenderTarget.Get(), Brush.Get());

	/*******************************************END************************************/
	m_RenderTarget->EndDraw();
}

void DemoApp::Update(double deltatime)
{
	Current_Radian += RadianPerSec*deltatime;
	rain.update(deltatime);
}
