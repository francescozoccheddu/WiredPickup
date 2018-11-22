#include <Game/Resources/ShaderResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <Game/Utils/Storage.hpp>

ShaderBytecode::ShaderBytecode (const std::vector<char>& _bytecode)
	: bytecode { _bytecode }
{}

const ShaderBytecode ShaderBytecode::CreateFromFile (const std::string & _filename)
{
	return ShaderBytecode { Storage::LoadBinaryFile (_filename) };
}

ShaderResource::ShaderResource (ShaderType _type)
	: m_Type { _type }
{
	switch (_type)
	{
		case ShaderType::VertexShader:
		case ShaderType::PixelShader:
		case ShaderType::GeometryShader:
		case ShaderType::HullShader:
		case ShaderType::DomainShader:
		case ShaderType::ComputeShader:
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::Reset (ID3D11DeviceContext & _deviceContext, ShaderType _type)
{
	switch (_type)
	{
		case ShaderType::VertexShader:
			GAME_COMC (_deviceContext.VSSetShader (nullptr, nullptr, 0));
			break;
		case ShaderType::PixelShader:
			GAME_COMC (_deviceContext.PSSetShader (nullptr, nullptr, 0));
			break;
		case ShaderType::GeometryShader:
			GAME_COMC (_deviceContext.GSSetShader (nullptr, nullptr, 0));
			break;
		case ShaderType::HullShader:
			GAME_COMC (_deviceContext.HSSetShader (nullptr, nullptr, 0));
			break;
		case ShaderType::DomainShader:
			GAME_COMC (_deviceContext.DSSetShader (nullptr, nullptr, 0));
			break;
		case ShaderType::ComputeShader:
			GAME_COMC (_deviceContext.CSSetShader (nullptr, nullptr, 0));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	switch (m_Type)
	{
		case ShaderType::VertexShader:
			GAME_COMC (_deviceContext.VSSetShader (reinterpret_cast<ID3D11VertexShader*>(GetPointer ()), nullptr, 0));
			break;
		case ShaderType::PixelShader:
			GAME_COMC (_deviceContext.PSSetShader (reinterpret_cast<ID3D11PixelShader*>(GetPointer ()), nullptr, 0));
			break;
		case ShaderType::GeometryShader:
			GAME_COMC (_deviceContext.GSSetShader (reinterpret_cast<ID3D11GeometryShader*>(GetPointer ()), nullptr, 0));
			break;
		case ShaderType::HullShader:
			GAME_COMC (_deviceContext.HSSetShader (reinterpret_cast<ID3D11HullShader*>(GetPointer ()), nullptr, 0));
			break;
		case ShaderType::DomainShader:
			GAME_COMC (_deviceContext.DSSetShader (reinterpret_cast<ID3D11DomainShader*>(GetPointer ()), nullptr, 0));
			break;
		case ShaderType::ComputeShader:
			GAME_COMC (_deviceContext.CSSetShader (reinterpret_cast<ID3D11ComputeShader*>(GetPointer ()), nullptr, 0));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

ShaderType ShaderResource::GetType () const
{
	return m_Type;
}

ID3D11DeviceChild * ShaderResource::Create (ID3D11Device & _device) const
{
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	ID3D11DeviceChild * pShader;
	switch (m_Type)
	{
		case ShaderType::VertexShader:
			GAME_COMC (_device.CreateVertexShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11VertexShader**>(&pShader)));
			break;
		case ShaderType::PixelShader:
			GAME_COMC (_device.CreatePixelShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11PixelShader**>(&pShader)));
			break;
		case ShaderType::GeometryShader:
			GAME_COMC (_device.CreateGeometryShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11GeometryShader**>(&pShader)));
			break;
		case ShaderType::HullShader:
			GAME_COMC (_device.CreateHullShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11HullShader**>(&pShader)));
			break;
		case ShaderType::DomainShader:
			GAME_COMC (_device.CreateDomainShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11DomainShader**>(&pShader)));
			break;
		case ShaderType::ComputeShader:
			GAME_COMC (_device.CreateComputeShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11ComputeShader**>(&pShader)));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
	return pShader;
}
