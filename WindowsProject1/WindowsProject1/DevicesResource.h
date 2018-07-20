#pragma once
#include <Windows.h>
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

private:
	void CreateDeviceIndependentResources();
	void CreateDeviceResources();

};
