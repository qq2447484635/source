#pragma once
#include <Windows.h>
#include <d2d1.h>
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
private:
	// Direct2D drawing components.
	Microsoft::WRL::ComPtr<ID2D1Factory>       m_d2dFactory;
	//Microsoft::WRL::ComPtr<ID2D1Device>        m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_d2dHwndRenderTarget;

	// DirectWrite drawing components.
	Microsoft::WRL::ComPtr<IDWriteFactory>     m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicFactory;

public:
	//Accessor
	ID2D1Factory*					   GetD2DFactory() const { return m_d2dFactory.Get(); }
//	ID2D1Device*				       GetD2DDevice() const { return m_d2dDevice.Get(); }
	ID2D1HwndRenderTarget*  GetD2DHwndRenderTarget() const { return m_d2dHwndRenderTarget.Get(); }
	DevicesResouce();

private:
	void CreateDeviceIndependentResources();
	void CreateDeviceResources();

};
