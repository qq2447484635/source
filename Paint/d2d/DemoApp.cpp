#include "DemoApp.h"



DemoApp::DemoApp()
{
	
}


DemoApp::~DemoApp()
{

}

void DemoApp::Load()
{
	ObjectPanel.push_back(new Canvas2D(m_pHwndRenderTarget, m_hwnd));
}