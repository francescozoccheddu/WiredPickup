#include <Game/Resources/InputLayoutResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void InputLayoutResource::Reset (ID3D11DeviceContext & _deviceContext)
{
	GAME_COMC (_deviceContext.IASetInputLayout (nullptr));
}

void InputLayoutResource::Set (ID3D11DeviceContext & _deviceContext) const
{
	GAME_COMC (_deviceContext.IASetInputLayout (GetPointer ()));
}

ID3D11InputLayout * InputLayoutResource::Create (ID3D11Device & _device) const
{
	GAME_ASSERT_MSG (pBytecode, "Bytecode is nullptr");
	GAME_ASSERT_MSG (pLayout, "Layout is nullptr");
	ID3D11InputLayout * pResource;
	GAME_COMC (_device.CreateInputLayout (pLayout->GetDescriptions (), static_cast<UINT>(pLayout->GetDescriptionCount ()), pBytecode->GetData (), static_cast<SIZE_T>(pBytecode->GetSize ()), &pResource));
	return pResource;
}


