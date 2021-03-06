#include "Canvas2D.h"

Vec2 BezierV(std::deque<Vec2>&P,int stage,float t, int n)
{
	if (n == 1)
	{
		return P[0] * pow(1 - t, stage)+BezierV(P,stage,t,n+1);
	}
	else if (n == stage + 1)
	{
		return P[stage] * pow(t, stage);
	}
	else
	{
		return P[n - 1] * pow(1 - t, stage - n + 1)*pow(t, n - 1)*stage+BezierV(P,stage,t,n+1);
	}
}

ID2D1HwndRenderTarget *g_RT=NULL;

Canvas2D::Canvas2D(ID2D1HwndRenderTarget *pRT,HWND hWnd)
{
	_hWnd = hWnd;
	RT = pRT;
	RT->CreateCompatibleRenderTarget(D2D1::SizeF(4096,4096),&bitmapRT);
	RT->CreateCompatibleRenderTarget(D2D1::SizeF(4096, 4096), &AxisRT);
	CreateResource();
}

Canvas2D::~Canvas2D()
{
	SafeRelease(&Brush);
	SafeRelease(&wFac);
	SafeRelease(&bitmapRT);
	SafeRelease(&AxisRT);
	SafeRelease(&Axis);
}


void Canvas2D::CreateResource()
{
	RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &Brush);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&wFac));
}

void  Canvas2D::OnRender(float DeltaTime)
{
	//RT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	GetClientRect(_hWnd, &Rect);
	RT->Resize(D2D1::SizeU(Rect.right, Rect.bottom));
	Position = { 1,0,0,1,-x,-y };
	ToScreemCenter = { 1,0,0,1,(Rect.right - Rect.left) / 2.0f,(Rect.bottom - Rect.top) / 2.0f };
	RT->SetTransform(ToScreemCenter*Position);
//	MousePainting(DeltaTime);
	Debug_HUD(DeltaTime);
	if (Paper)
	{
		RT->DrawBitmap(Paper, D2D1::RectF(0, 0, Paper->GetSize().width, Paper->GetSize().height));
	//	SafeRelease(&Paper);
	}
}

void  Canvas2D::OnUpdata(float DeltaTime)
{

}

void Canvas2D::Debug_HUD(float DeltaTime)
{
	HRESULT hr;
	int fontsize = 20;
	static float fFPSflash=0;
	fFPSflash += DeltaTime;
	hr = wFac->CreateTextFormat(L"΢���ź�", NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontsize, L"chs", &Format);
	//RT->SetTransform(D2D1::IdentityMatrix());
	Brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	//swprintf_s(TEXTACHE, L"x=%.1lf,y=%.1lf", x, y);
	//RT->DrawTextW(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(0, 0, fontsize*wcslen(TEXTACHE), fontsize), Brush);
	if (fFPSflash > 0.5)
	{
		fFPSflash = 0;
		FPS = 1.0 / DeltaTime;
	}
	swprintf_s(TEXTACHE, L"FPS %.0f", FPS);
	//RT->DrawTextW(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(Rect.right, Rect.top, Rect.right - fontsize * wcslen(TEXTACHE), fontsize), Brush);
	SafeRelease(&Format);

	fontsize = 12;
	hr = wFac->CreateTextFormat(L"΢���ź�", NULL, DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontsize, L"chs", &Format);

	bool fix = true;
	static bool AxisDraw = false;
	if (AxisDraw == false)
	{
		AxisRT->BeginDraw();
		if (!fix)
		{
			RT->SetTransform(ToScreemCenter*Scale);
			RT->DrawLine(D2D1::Point2F(0, 1000), D2D1::Point2F(0, -1000), Brush);//y
			RT->DrawLine(D2D1::Point2F(1000, 0), D2D1::Point2F(-1000, 0), Brush);//x
			for (int i = (int)x / 100 * 100 - 1000; i < (int)x / 100 * 100 + 1000; i += 100)
			{
				swprintf_s(TEXTACHE, L"%d", i);
				RT->DrawTextA(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(i - x, 0, i - x + 40, 0 + 20), Brush);//x axis
				RT->DrawLine(D2D1::Point2F(i - x, -10), D2D1::Point2F(i - x, 10), Brush);
			}
			for (int i = (int)y / 100 * 100 - 1000; i < (int)y / 100 * 100 + 1000; i += 100)
			{
				swprintf_s(TEXTACHE, L"%d", i);
				RT->DrawTextA(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(0, i - y, 0 + 40, i - y + 20), Brush);//y axis
				RT->DrawLine(D2D1::Point2F(-10, i - y), D2D1::Point2F(10, i - y), Brush);
			}
		}
		else
		{
			AxisRT->SetTransform(ToCenterM(AxisRT));
			AxisRT->DrawLine(D2D1::Point2F(0, 2050), D2D1::Point2F(0, -2050), Brush);//y
			AxisRT->DrawLine(D2D1::Point2F(2050, 0), D2D1::Point2F(-2050, 0), Brush);//x
			for (int i = -2000; i < 2000; i += 100)
			{
				swprintf_s(TEXTACHE, L"%.1f", i / 100.0);
				AxisRT->DrawTextA(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(0, i, 40, i + 20), Brush);
				AxisRT->DrawTextA(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(i, 0, i + 40, 20), Brush);
			}
		}
		AxisDraw = true;
		AxisRT->EndDraw();
		AxisRT->GetBitmap(&Axis);
	}
	else if(Axis)
	{
		RT->DrawBitmap(Axis, D2D1::RectF(Axis->GetSize().width / -2.0, Axis->GetSize().height / -2.0, Axis->GetSize().width / 2.0, Axis->GetSize().height / 2.0));
		RT->FillRectangle(D2D1::RectF(0, 0, 100, 100), Brush);
	}

	SafeRelease(&Format);
}

void Canvas2D::MousePainting(float DeltaTime)
{
	HRESULT hr;
	ID2D1LinearGradientBrush *LBrush = NULL;
	D2D1_GRADIENT_STOP gradientStops[2];
	ID2D1GradientStopCollection *stopcollection = NULL;
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
	gradientStops[0].position = 10.f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradientStops[1].position = 0.0f;
	hr = RT->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_MIRROR, &stopcollection
	);
	hr = RT->CreateLinearGradientBrush(D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES(), stopcollection, &LBrush);

	D2D1_SIZE_F bs = bitmapRT->GetSize();
	D2D1_MATRIX_3X2_F Tr = { 1,0,0,1,bs.width / 2.0f,bs.height / 2.0f };
	bitmapRT->BeginDraw();
	bitmapRT->SetTransform(Tr);
	Vec2 B;
	static std::vector<Vec2> L;
	static std::deque<Vec2> P;
	static bool clicked = false;
	static bool ctrl = false;
	POINT cpos;
	GetCursorPos(&cpos);
	ScreenToClient(_hWnd,&cpos);
	CursorPos = { (cpos.x - Rect.right / 2.0f) / ScaleRate + x,(cpos.y - Rect.bottom / 2.0f) / ScaleRate + y };
	{//draw Pos
		Brush->SetColor(D2D1::ColorF(D2D1::ColorF::DodgerBlue));
		RT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(CursorPos.x, CursorPos.y), 3, 3), Brush);
		swprintf_s(TEXTACHE, L"Paint Demo\nx :%.0f \ny : %.0f \nPress Key 'C' to clear all \nPress Key 'R' to return (0,0)\nHold Ctrl and select %d points to create a BezierCurve\nUse Q and E to change Order\nOrder: %d\nx:%d,y%d",CursorPos.x,CursorPos.y, stage + 1, stage,cpos.x,cpos.y);
		wFac->CreateTextFormat(L"΢���ź�", NULL, DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			18, L"chs", &Format);
		RT->DrawTextA(TEXTACHE, wcslen(TEXTACHE), Format, D2D1::RectF(-700, -400, 200, 90), Brush);
		SafeRelease(&Format);
	}

	if (GetKeyState(VK_LCONTROL) < 0)
	{
		ctrl = true;
		P.push_back(CursorPos);
		if (P.size() >= 2)
		{
			L.clear();
			for (float t = 0.0f; t < 1.f; t += 0.02)
			{
				B = BezierV(P, P.size() - 1, t,0);
				L.push_back(B);
			}
		}
		if (!((GetKeyState(VK_LBUTTON) < 0) && clicked == false))
		{
			P.pop_back();
		}
		if (!L.empty())
		{
			for (int i = 0; i < L.size() - 1; i++)
			{
				RT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), LBrush);
			}
			L.clear();
		}
	}
	else if (ctrl == true)
	{
		ctrl = false;
		if (P.size() >= 2)
		{
			for (float t = 0.0f; t < 1.f; t += 0.02)
			{
				B = BezierV(P, P.size() - 1, t,0);
				L.push_back(B);
			}
			for (int i = 0; i < L.size() - 1; i++)
			{
				bitmapRT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), LBrush);
			}
			L.clear();
		}
		P.clear();
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			P.push_back(CursorPos);
		}
		else
		{
			P.clear();
		}
		if (P.size() >= stage+1)
		{
			for (float t = 0.0f; t < 1.f; t += 0.02)
			{
				B = BezierV(P, P.size() - 1, t,0);
				L.push_back(B);
			}
			for (int i = 0; i < stage; i++)
			{
				P.pop_front();
			}
			for (int i = 0; i < L.size() - 1; i++)
			{
				bitmapRT->DrawLine(D2D1::Point2F(L[i].x, L[i].y), D2D1::Point2F(L[i + 1].x, L[i + 1].y), LBrush);
			}
			L.clear();
		}
	}

	bitmapRT->EndDraw();
	SafeRelease(&stopcollection);
	SafeRelease(&LBrush);
	if (GetKeyState(VK_LBUTTON) >= 0)
	{
		clicked = false;
	}
	else
	{
		clicked = true;
	}
}

D2D1_MATRIX_3X2_F Canvas2D::ToCenterM(ID2D1RenderTarget * RT)
{
	return D2D1::Matrix3x2F(1,0,0,1,RT->GetSize().width/2.0f,RT->GetSize().height/2.0f);
}

HRESULT _LoadPictureResource::operator()(LPCWSTR Path, ID2D1Bitmap ** ppBitmap)
{
	HRESULT hr;
	hr=CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICImagingFactory));
	if (SUCCEEDED(hr))
	{
		hr=pWICImagingFactory->CreateDecoderFromFilename(
			Path,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder);
	}
	if (SUCCEEDED(hr))
	{
		pDecoder->GetFrameCount(&FrameCount);
		hr = pDecoder->GetFrame(0, &pFrame);
	}
	if (SUCCEEDED(hr))
	{
		hr=pWICImagingFactory->CreateFormatConverter(&pConverter);
		
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
		);
	}
	if (SUCCEEDED(hr))
	{
		hr=RT->CreateBitmapFromWicBitmap(pConverter, ppBitmap);
	}
	SafeRelease(&pWICImagingFactory);
	SafeRelease(&pDecoder);
	SafeRelease(&pFrame);
	SafeRelease(&pConverter);
	if (!SUCCEEDED(hr))
	{
		MessageBox(0, TEXT("Unknow fail when loading resource"), TEXT("ERROR"), MB_OK);
		exit(-1);
	}
	return hr;
}

HRESULT _LoadPictureResource::operator()(LPCWSTR Path, std::vector<ID2D1Bitmap*> &BitmapArr)
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICImagingFactory));
	if (SUCCEEDED(hr))
	{
		hr = pWICImagingFactory->CreateDecoderFromFilename(
			Path,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder);
	}
	if (SUCCEEDED(hr))
	{
		pDecoder->GetFrameCount(&FrameCount);
	}
	if (SUCCEEDED(hr))
	{
		ID2D1Bitmap *pBitmap = NULL;
		for (int i = 0; i < FrameCount; i++)
		{
			hr = pDecoder->GetFrame(i, &pFrame);
			if (SUCCEEDED(hr))
			{
				hr = pWICImagingFactory->CreateFormatConverter(&pConverter);
			}
			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					pFrame,                          // Input bitmap to convert
					GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
					WICBitmapDitherTypeNone,         // Specified dither pattern
					NULL,                            // Specify a particular palette 
					0.f,                             // Alpha threshold
					WICBitmapPaletteTypeCustom       // Palette translation type
				);
			}
			if (SUCCEEDED(hr))
			{
				hr = RT->CreateBitmapFromWicBitmap(pConverter, &pBitmap);
			}
			if (SUCCEEDED(hr))
			{
				BitmapArr.push_back(pBitmap);
			}
			SafeRelease(&pFrame);
			SafeRelease(&pConverter);
		}
	}
	SafeRelease(&pWICImagingFactory);
	SafeRelease(&pDecoder);
	if (!SUCCEEDED(hr))
	{
		MessageBox(0, TEXT("Unknow fail when loading resource"), TEXT("ERROR"), MB_OK);
		exit(-1);
	}
	return hr;
}
