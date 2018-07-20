#include "stdafx.h"
#include "DevicesResource.h"

DevicesResouce::DevicesResouce()
{
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}

DevicesResouce::~DevicesResouce()
{
}

void DevicesResouce::CreateDeviceIndependentResources()
{
	// Initialize Direct2D resources.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	// Initialize the Direct2D Factory.
	DX::ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_MULTI_THREADED,
			__uuidof(ID2D1Factory),
			&options,
			&m_d2dFactory
		)
	);

	// Initialize the DirectWrite Factory.
	DX::ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			&m_dwriteFactory
		)
	);
	// Initialize the Windows Imaging Component (WIC) Factory.
	DX::ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_wicFactory)
		)
	);
}

void DevicesResouce::CreateDeviceResources()
{

}
