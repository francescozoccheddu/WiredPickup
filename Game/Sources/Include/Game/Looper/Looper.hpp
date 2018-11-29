#pragma once

#include <Game/Engine/EngineListener.hpp>
#include <Game/Engine/DeviceHolder.hpp>
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

	D3D11_VIEWPORT m_Viewport;

	void OnDeviceCreated (const DeviceHolder & deviceHolder) override final;

	void OnDeviceDestroyed (const DeviceHolder& deviceHolder) override final;

	void OnSized (const DeviceHolder & deviceHolder) override final;

};