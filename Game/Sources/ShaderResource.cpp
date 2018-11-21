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

ShaderResource::ShaderResource (Type _type)
	: m_Type { _type }
{
	switch (_type)
	{
		case Type::VertexShader:
		case Type::PixelShader:
		case Type::GeometryShader:
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::ResetShader (ID3D11DeviceContext & _deviceContext, Type _type)
{
	switch (_type)
	{
		case Type::VertexShader:
			GAME_COMC (_deviceContext.VSSetShader (nullptr, nullptr, 0));
			break;
		case Type::PixelShader:
			GAME_COMC (_deviceContext.PSSetShader (nullptr, nullptr, 0));
			break;
		case Type::GeometryShader:
			GAME_COMC (_deviceContext.GSSetShader (nullptr, nullptr, 0));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::SetShader (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	switch (m_Type)
	{
		case Type::VertexShader:
			GAME_COMC (_deviceContext.VSSetShader (reinterpret_cast<ID3D11VertexShader*>(m_pShader), nullptr, 0));
			break;
		case Type::PixelShader:
			GAME_COMC (_deviceContext.PSSetShader (reinterpret_cast<ID3D11PixelShader*>(m_pShader), nullptr, 0));
			break;
		case Type::GeometryShader:
			GAME_COMC (_deviceContext.GSSetShader (reinterpret_cast<ID3D11GeometryShader*>(m_pShader), nullptr, 0));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	switch (m_Type)
	{
		case Type::VertexShader:
			GAME_COMC (_device.CreateVertexShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11VertexShader**>(&m_pShader)));
			break;
		case Type::PixelShader:
			GAME_COMC (_device.CreatePixelShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11PixelShader**>(&m_pShader)));
			break;
		case Type::GeometryShader:
			GAME_COMC (_device.CreateGeometryShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, reinterpret_cast<ID3D11GeometryShader**>(&m_pShader)));
			break;
		default:
			GAME_THROW_MSG ("Unknown type");
			break;
	}
}

void ShaderResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pShader->Release ();
	m_pShader = nullptr;
}

bool ShaderResource::IsCreated () const
{
	return m_pShader != nullptr;
}

ShaderResource::Type ShaderResource::GetType () const
{
	return m_Type;
}
