#pragma once

#include <Game/Engine/DeviceHolder.hpp>
#include <Game/Engine/EngineListener.hpp>
#include <Game/DirectXMath.hpp>
#include <Game/Direct3D.hpp>

class Looper : public EngineListener
{

public:

	Looper (const DeviceHolder & deviceHolder);

	~Looper ();

	void Render ();

private:

	const DeviceHolder & m_DeviceHolder;

	ID3D11Buffer * m_pVertexBuffer;
	ID3D11RasterizerState * m_pRasterizerState;
	ID3D11VertexShader * m_pVertexShader;
	ID3D11PixelShader * m_pPixelShader;
	ID3D11InputLayout * m_pInputLayout;
	D3D11_VIEWPORT m_Viewport;

	void OnDeviceCreated (const DeviceHolder & deviceHolder) override final;

	void OnDeviceDestroyed (const DeviceHolder& deviceHolder) override final;

	void OnSized (const DeviceHolder & deviceHolder) override final;

};