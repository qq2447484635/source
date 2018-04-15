#pragma once
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
#include "M.h"
#include "Framework.h"



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
	:public WinComponent
{
public:
	Canvas2D(ID2D1HwndRenderTarget *pRT,HWND hWnd);
	~Canvas2D();

	ID2D1HwndRenderTarget *RT = NULL;
	HWND _hWnd;

	void  OnRender(float DeltaTime=0);
	void  OnUpdata(float DeltaTime=0);
	void CreateResource();
	void Debug_HUD( float DeltaTime=0);
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
