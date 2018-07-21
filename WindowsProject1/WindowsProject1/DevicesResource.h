#pragma once
#include <Windows.h>
#include <vector>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl.h>
#include <exception>

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在此行中设置断点，以捕获 Win32 API 错误。
			throw "error";
		}
	}
}

using namespace Microsoft::WRL;


class DevicesResouce
{
public:
	DevicesResouce();
	~DevicesResouce();
private:
	// Direct2D drawing components.
	Microsoft::WRL::ComPtr<ID2D1Factory>       m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_d2dRenderTarget;
	// DirectWrite drawing components.
	Microsoft::WRL::ComPtr<IDWriteFactory>     m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicFactory;
	//Direct2D Bitmap
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;


public:
	//Accessor
	ID2D1Factory*					   GetD2DFactory() const { return m_d2dFactory.Get(); }
	IDWriteFactory*        GetDWriteFactory() const { return m_dwriteFactory.Get(); }
	ID2D1Device*			GetD2DDevice()const { return m_d2dDevice.Get(); }
	IWICImagingFactory*    GetWicImagingFactory() const { return m_wicFactory.Get(); }
	void GetImage(LPCWSTR file, ID2D1HwndRenderTarget *rt, std::vector<ComPtr<ID2D1Bitmap>> &bitmap_arr)
	{
		ComPtr<IWICBitmapDecoder> Decoder;
		m_wicFactory->CreateDecoderFromFilename(file, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &Decoder);
		unsigned int count = 0;
		Decoder->GetFrameCount(&count);
		ComPtr<ID2D1Bitmap> temp;
		for (int i = 0; i < count; i++)
		{
			ComPtr<IWICBitmapFrameDecode> Frame;
			Decoder->GetFrame(i, &Frame);
			ComPtr<IWICFormatConverter> Converter;
			m_wicFactory->CreateFormatConverter(&Converter);
			Converter->Initialize(
				Frame.Get(),                          // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				NULL,                            // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			);
			HRESULT hr=rt->CreateBitmapFromWicBitmap(Converter.Get(), &temp);
			bitmap_arr.push_back(temp);
		}
	}

private:
	void CreateDeviceIndependentResources();
	void CreateDeviceResources();
};
