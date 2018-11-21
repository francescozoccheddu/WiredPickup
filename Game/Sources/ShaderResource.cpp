#include <Game/Resources/ShaderResource.hpp>

#include <Game/Resources/MeshResource.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

ShaderBytecode::ShaderBytecode (const std::vector<char>& _bytecode)
	: bytecode { _bytecode }
{}

const void * ShaderBytecode::GetData () const
{
	return bytecode.data ();
}

size_t ShaderBytecode::GetSize () const
{
	return static_cast<size_t>(bytecode.size ());
}

const ShaderBytecode ShaderBytecode::CreateFromFile (const std::string & _filename)
{
	GAME_THROW_MSG ("Unimplemented");
}


VertexShaderResource::VertexShaderResource (const D3D11_INPUT_ELEMENT_DESC * _pDescs, int _cDescs) :
	m_pDescriptions { _pDescs }, m_cDescriptions { _cDescs }
{}

VertexShaderResource::~VertexShaderResource ()
{
	if (VertexShaderResource::IsCreated ())
	{
		VertexShaderResource::Destroy ();
	}
}

void VertexShaderResource::SetShaderAndInputLayout (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	GAME_COMC (_deviceContext.VSSetShader (m_pShader, nullptr, 0));
	GAME_COMC (_deviceContext.IASetInputLayout (m_pInputLayout));
}

void VertexShaderResource::Create (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	const void * pBytecodeData = pBytecode->GetData ();
	const SIZE_T bytecodeSize = static_cast<SIZE_T>(pBytecode->GetSize ());
	GAME_COMC (_device.CreateVertexShader (pBytecodeData, bytecodeSize, nullptr, &m_pShader));
	GAME_COMC (_device.CreateInputLayout (m_pDescriptions, static_cast<UINT>(m_cDescriptions), pBytecodeData, bytecodeSize, &m_pInputLayout));
}

void VertexShaderResource::Destroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pShader->Release ();
	m_pInputLayout->Release ();
	m_pShader = nullptr;
	m_pInputLayout = nullptr;
}

bool VertexShaderResource::IsCreated () const
{
	return m_pShader != nullptr;
}

PixelShaderResource::~PixelShaderResource ()
{
	if (PixelShaderResource::IsCreated ())
	{
		PixelShaderResource::Destroy ();
	}
}

void PixelShaderResource::SetShader (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	GAME_COMC (_deviceContext.PSSetShader (m_pShader, nullptr, 0));
}

void PixelShaderResource::Create (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	GAME_COMC (_device.CreatePixelShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, &m_pShader));
}

void PixelShaderResource::Destroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pShader->Release ();
	m_pShader = nullptr;
}

bool PixelShaderResource::IsCreated () const
{
	return m_pShader != nullptr;
}

GeometryShaderResource::~GeometryShaderResource ()
{
	if (GeometryShaderResource::IsCreated ())
	{
		GeometryShaderResource::Destroy ();
	}
}

void GeometryShaderResource::SetShader (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	GAME_COMC (_deviceContext.GSSetShader (m_pShader, nullptr, 0));
}

void GeometryShaderResource::Create (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	GAME_COMC (_device.CreateGeometryShader (pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), nullptr, &m_pShader));
}

void GeometryShaderResource::Destroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pShader->Release ();
	m_pShader = nullptr;
}

bool GeometryShaderResource::IsCreated () const
{
	return m_pShader != nullptr;
}
