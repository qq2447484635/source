#pragma once
#define UNICODE
#define debug_info
//#define ShowInConsole
#include <time.h>
#include <deque>
#include <windows.h>
#include <d2d1.h>
#include <math.h>
#include <wincodec.h>
#include <vector>
#include <process.h>
#include <cstring>
#include <dwrite.h>
#include "Polygon.h"

#ifndef ShowInConsole
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif 


template<class Interface>
inline void
SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

struct _LoadPictureResource
{
	ID2D1HwndRenderTarget *RT;
	IWICImagingFactory *pWICImagingFactory;
	IWICBitmapDecoder *pDecoder;
	IWICBitmapFrameDecode *pFrame;
	IWICFormatConverter *pConverter;
	HRESULT operator ()(LPCWSTR Path, ID2D1Bitmap **ppBitmap);
	HRESULT operator ()(LPCWSTR Path, std::vector<ID2D1Bitmap *>& BitmapArr);
	UINT FrameCount;
};

class Canvas2D
{
public:
	Canvas2D();
	~Canvas2D();

	HINSTANCE _hInstance;
	HWND _hWnd;
	BOOL initWin();
	ID2D1HwndRenderTarget *RT = NULL;
	ID2D1Factory *Factory = NULL;
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	void _cdecl OnRender(float DeltaTime=0);
	static void _cdecl RunMessageLoop(void *arg=NULL);
	static void Show();
	void OnUpdata(float DeltaTime=0);
	void CreateResource();
	void Debug_HUD( float DeltaTime=0);
	void FrameUpdata(float DeltaTime=0);
	void MousePainting(float DeltaTime);
	D2D1_MATRIX_3X2_F ToCenterM(ID2D1RenderTarget *RT);
	_LoadPictureResource LoadPictureResource;

public:
	RECT Rect;
	ID2D1SolidColorBrush *Brush;
	IDWriteFactory *wFac;
	IDWriteTextFormat *Format;
	WCHAR TEXTACHE[200];
	FLOAT x=0, y=0;
	Vec2 CursorPos;
	float FPS=0;
	ID2D1BitmapRenderTarget *bitmapRT;
	ID2D1BitmapRenderTarget *AxisRT;
	ID2D1Bitmap *Paper;
	ID2D1Bitmap *Axis=NULL;
	int stage = 3;
public:
	D2D1_MATRIX_3X2_F ToScreemCenter;
	D2D1_MATRIX_3X2_F Position;
	D2D1_MATRIX_3X2_F Scale;
	D2D1_MATRIX_3X2_F Extra;
	float ScaleRate=0.6;
};

extern ID2D1HwndRenderTarget *g_RT;
Vec2 BezierV(std::deque<Vec2>&P, int stage, float t, int n = 1);