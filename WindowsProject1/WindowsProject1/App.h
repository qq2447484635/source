#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "DevicesResource.h"
#include "Framework.h"
#include "threadsafe_queue.h"

class App :
	public
	Framework
{
public:
	App();
	~App();
	virtual void Render() {};
	virtual void Update(double) {};
	virtual void RunMessageLoop();
	virtual ComPtr<ID2D1Brush> GetDeafaultBrush();
	virtual ComPtr<IDWriteTextFormat> GetDeafaultWriteTextFomat();
	WCHAR* GetFpsText();
	int GetFps();
	HRESULT Initialize();
protected:
	ComPtr<ID2D1HwndRenderTarget> m_RenderTarget;
	ComPtr<ID2D1BitmapRenderTarget> bitmapRT;
private:
	threadsafe_queue<ComPtr<ID2D1Bitmap>> frame_queue;
	WCHAR fps_text[8];
};