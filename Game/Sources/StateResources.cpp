#include <Game/Resources/StateResources.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void RasterizerStateResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.RSSetState (nullptr);
}

void RasterizerStateResource::ForceSet (ID3D11DeviceContext & _deviceContext, ID3D11RasterizerState * _pState) const
{
	_deviceContext.RSSetState (_pState);
}

ID3D11RasterizerState * RasterizerStateResource::Create (ID3D11Device & _device) const
{
	ID3D11RasterizerState * pState;
	GAME_COMC (_device.CreateRasterizerState (&description, &pState));
	return pState;
}

void DepthStencilStateResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.OMSetDepthStencilState (nullptr, 0);
}

void DepthStencilStateResource::ForceSet (ID3D11DeviceContext & _deviceContext, ID3D11DepthStencilState * _pState) const
{
	_deviceContext.OMSetDepthStencilState (_pState, 0);
}

ID3D11DepthStencilState * DepthStencilStateResource::Create (ID3D11Device & _device) const
{
	ID3D11DepthStencilState * pState;
	GAME_COMC (_device.CreateDepthStencilState (&description, &pState));
	return pState;
}

void BlendStateResource::Reset (ID3D11DeviceContext & _deviceContext, const FLOAT * _factor, UINT _sampleMask)
{
	_deviceContext.OMSetBlendState (nullptr, _factor, _sampleMask);
}

void BlendStateResource::ForceSet (ID3D11DeviceContext & _deviceContext, ID3D11BlendState * _pState) const
{
	_deviceContext.OMSetBlendState (_pState, factor, sampleMask);
}

ID3D11BlendState * BlendStateResource::Create (ID3D11Device & _device) const
{
	ID3D11BlendState * pState;
	GAME_COMC (_device.CreateBlendState (&description, &pState));
	return pState;
}
