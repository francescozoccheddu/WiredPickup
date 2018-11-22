#include <Game/Resources/DepthBufferResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void DepthBufferResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = size.width;
	desc.Height = size.height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = bHalfPrecision ? DXGI_FORMAT_D16_UNORM : DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	ID3D11Texture2D * pTexture;
	GAME_COMC (_device.CreateTexture2D (&desc, nullptr, &pTexture));
	GAME_COMC (_device.CreateDepthStencilView (pTexture, nullptr, &m_pDepthView));
	pTexture->Release ();
}

void DepthBufferResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
}

bool DepthBufferResource::IsCreated () const
{
	return m_pDepthView;
}

ID3D11DepthStencilView * DepthBufferResource::Get () const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	return m_pDepthView;
}

void DepthBufferResource::Clear (ID3D11DeviceContext & _deviceContext, float _value)
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	_deviceContext.ClearDepthStencilView (m_pDepthView, D3D11_CLEAR_DEPTH, static_cast<FLOAT>(_value), 0);
}

