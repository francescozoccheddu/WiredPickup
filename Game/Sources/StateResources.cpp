#include <Game/Resources/StateResources.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include "Include\Game\Resources\StateResources.hpp"

void RasterizerStateResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.RSSetState (nullptr);
}

void RasterizerStateResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	_deviceContext.RSSetState (GetPointer ());
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

void DepthStencilStateResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	_deviceContext.OMSetDepthStencilState (GetPointer (), 0);
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

void BlendStateResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	_deviceContext.OMSetBlendState (GetPointer (), factor, sampleMask);
}

ID3D11BlendState * BlendStateResource::Create (ID3D11Device & _device) const
{
	ID3D11BlendState * pState;
	GAME_COMC (_device.CreateBlendState (&description, &pState));
	return pState;
}

void SamplerStateResource::Set (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, ShaderType _shaderType, const SamplerStateResource*const* _pSamplerStates, int _cSamplerState)
{
	GAME_ASSERT_MSG (_cSamplerState >= 0, "Sampler state count cannot be negative");
	if (_cSamplerState > 0)
	{
		std::vector<ID3D11SamplerState*> samplerStates (static_cast<size_t>(_cSamplerState));
		for (int iBuf { 0 }; iBuf < _cSamplerState; iBuf++)
		{
			if (_pSamplerStates[iBuf])
			{
				GAME_ASSERT_MSG (_pSamplerStates[iBuf]->IsCreated (), "Not created");
				samplerStates[iBuf] = _pSamplerStates[iBuf]->GetPointer ();
			}
			else
			{
				samplerStates[iBuf] = nullptr;
			}
		}
		Set (_deviceContext, _startingSlot, _shaderType, samplerStates.data (), _cSamplerState);
	}
}

void SamplerStateResource::Set (ID3D11DeviceContext & _deviceContext, UINT _slot, ShaderType _shaderType) const
{
	ID3D11SamplerState * pState { GetPointer () };
	Set (_deviceContext, _slot, _shaderType, &pState, 1);
}

ID3D11SamplerState * SamplerStateResource::Create (ID3D11Device & _device) const
{
	ID3D11SamplerState * pState;
	GAME_COMC (_device.CreateSamplerState (&description, &pState));
	return pState;
}

void SamplerStateResource::Set (ID3D11DeviceContext & _deviceContext, UINT _startingSlot, ShaderType _shaderType, ID3D11SamplerState*const* _pSamplerStates, int _cSamplerStates)
{
	switch (_shaderType)
	{
		case ShaderType::VertexShader:
			_deviceContext.VSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		case ShaderType::PixelShader:
			_deviceContext.PSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		case ShaderType::GeometryShader:
			_deviceContext.GSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		case ShaderType::HullShader:
			_deviceContext.HSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		case ShaderType::DomainShader:
			_deviceContext.DSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		case ShaderType::ComputeShader:
			_deviceContext.CSSetSamplers (_startingSlot, static_cast<UINT>(_cSamplerStates), _pSamplerStates);
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
	}
}
