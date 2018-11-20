#pragma once

#include <Game/Engine/DeviceHolder.hpp>
#include <Game/DirectXMath.hpp>
#include <Game/Direct3D.hpp>

class Renderer : public EngineListener
{

public:

	Renderer (const DeviceHolder & deviceHolder);

	~Renderer ();

	void Render (/*const Scene& scene*/);

private:

	const DeviceHolder & m_DeviceHolder;

	D3D11_VIEWPORT m_Viewport;

	void OnDeviceCreated (const DeviceHolder & deviceHolder) override;

	void OnDeviceDestroyed () override;

	void OnSized (const DeviceHolder & deviceHolder) override;

};