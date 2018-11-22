#include <Game/Resources/StateResources.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void RasterizerStateResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	_deviceContext.RSSetState (nullptr);
}

void RasterizerStateResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	_deviceContext.RSSetState (GetState ());
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
	_deviceContext.OMSetDepthStencilState (GetState (), 0);
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
	_deviceContext.OMSetBlendState (GetState (), factor, sampleMask);
}

ID3D11BlendState * BlendStateResource::Create (ID3D11Device & _device) const
{
	ID3D11BlendState * pState;
	GAME_COMC (_device.CreateBlendState (&description, &pState));
	return pState;
}

void SamplerStateResource::Set (ID3D11DeviceContext & _deviceContext, int _startingSlot, ShaderType _shaderType, const std::vector<const SamplerStateResource*> _samplers)
{
	if (!_samplers.empty ())
	{
		std::vector<ID3D11SamplerState*> samplers { _samplers.size () };
		for (int iBuf { 0 }; iBuf < _samplers.size (); iBuf++)
		{
			if (_samplers[iBuf])
			{
				GAME_ASSERT_MSG (_samplers[iBuf]->IsCreated (), "Not created");
				samplers[iBuf] = _samplers[iBuf]->GetState ();
			}
			else
			{
				samplers[iBuf] = nullptr;
			}
		}
		switch (_shaderType)
		{
			case ShaderType::VertexShader:
				_deviceContext.VSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			case ShaderType::PixelShader:
				_deviceContext.PSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			case ShaderType::GeometryShader:
				_deviceContext.GSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			case ShaderType::HullShader:
				_deviceContext.HSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			case ShaderType::DomainShader:
				_deviceContext.DSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			case ShaderType::ComputeShader:
				_deviceContext.CSSetSamplers (static_cast<UINT>(_startingSlot), static_cast<UINT>(_samplers.size ()), samplers.data ());
				break;
			default:
				GAME_THROW_MSG ("Unknown type");
				break;
		}
	}
}

void SamplerStateResource::Set (ID3D11DeviceContext & _deviceContext, int _slot, ShaderType _shaderType) const
{
	ID3D11SamplerState * pSamplers[] { GetState () };
	switch (_shaderType)
	{
		case ShaderType::VertexShader:
			GAME_COMC (_deviceContext.VSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		case ShaderType::PixelShader:
			GAME_COMC (_deviceContext.PSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		case ShaderType::GeometryShader:
			GAME_COMC (_deviceContext.GSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		case ShaderType::HullShader:
			GAME_COMC (_deviceContext.HSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		case ShaderType::DomainShader:
			GAME_COMC (_deviceContext.DSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		case ShaderType::ComputeShader:
			GAME_COMC (_deviceContext.CSSetSamplers (static_cast<UINT>(_slot), 1, pSamplers));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

ID3D11SamplerState * SamplerStateResource::Create (ID3D11Device & _device) const
{
	ID3D11SamplerState * pState;
	GAME_COMC (_device.CreateSamplerState (&description, &pState));
	return pState;
}
