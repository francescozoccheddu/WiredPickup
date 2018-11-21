#include <Game/Resources/InputLayoutResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void InputLayoutResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	GAME_COMC (_deviceContext.IASetInputLayout (m_pInputLayout));
}

void InputLayoutResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	GAME_ASSERT_MSG (pLayout, "Layout is nullptr");
	GAME_COMC (_device.CreateInputLayout (pLayout->GetDescriptions (), static_cast<UINT>(pLayout->GetDescriptionCount ()), pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), &m_pInputLayout));
}

void InputLayoutResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pInputLayout->Release ();
	m_pInputLayout = nullptr;
}

bool InputLayoutResource::IsCreated () const
{
	return m_pInputLayout;
}
