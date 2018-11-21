#include <Game/Resources/RasterizerStateResource.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

void RasterizerStateResource::ForceCreate (ID3D11Device & _device)
{
	GAME_ASSERT_MSG (!IsCreated (), "Already created");
	GAME_COMC (_device.CreateRasterizerState (&description, &m_pRasterizerState));
}

void RasterizerStateResource::ForceDestroy ()
{
	GAME_ASSERT_MSG (IsCreated (), "Not created");
	m_pRasterizerState->Release ();
	m_pRasterizerState = nullptr;
}

bool RasterizerStateResource::IsCreated () const
{
	return m_pRasterizerState;
}
