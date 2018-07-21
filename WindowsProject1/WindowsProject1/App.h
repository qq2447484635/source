#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "DevicesResource.h"
#include "Framework.h"
#include "threadsafe_queue.h"

class App :
	public	Framework,
	public Component
{
public:
	App();
	virtual ~App();
	virtual void Render(ID2D1HwndRenderTarget *rt)
	{ 
		//rt->Clear(); 
		Component::Render(rt);
	};
	virtual void Update(double deltatime)
	{
		Component::Update(deltatime);
	};
	virtual void RunMessageLoop();
	virtual ComPtr<ID2D1Brush> GetDeafaultBrush();
	virtual ComPtr<IDWriteTextFormat> GetDeafaultWriteTextFomat();
	virtual WCHAR* GetFpsText();
	virtual int GetFps();
	virtual HRESULT Initialize();
private:
	threadsafe_queue<ComPtr<ID2D1Bitmap>> frame_queue;
	WCHAR fps_text[8];
};