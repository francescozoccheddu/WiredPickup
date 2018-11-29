#include <Game/Looper/Looper.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <Game/DirectXMath.hpp>

Looper::Looper (const DeviceHolder & _deviceHolder) : m_DeviceHolder { _deviceHolder }
{}

Looper::~Looper ()
{}

void Looper::OnDeviceCreated (const DeviceHolder & deviceHolder)
{
	ID3D11Device & device { *m_DeviceHolder.GetDevice () };

}

void Looper::OnSized (const DeviceHolder & deviceHolder)
{
	ID3D11Device * pDevice { m_DeviceHolder.GetDevice () };
	WindowSize size = m_DeviceHolder.GetSize ();
	{
		// Viewport
		m_Viewport.Width = static_cast<FLOAT>(size.width);
		m_Viewport.Height = static_cast<FLOAT>(size.height);
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}
}

void Looper::Render ()
{
	ID3D11DeviceContext & context { *m_DeviceHolder.GetDeviceContext () };
	ID3D11RenderTargetView * const pRenderTargetView { m_DeviceHolder.GetRenderTargetView () };

	{
		// Set viewport, primitive topology and sampler state
		context.RSSetViewports (1, &m_Viewport);
		context.IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ID3D11RenderTargetView * const views[] { m_DeviceHolder.GetRenderTargetView () };
		context.OMSetRenderTargets (1, views, nullptr);
	}

	{
		// Clear and set output render target
		float color[4] { 1.0f, 0.2f, 0.2f, 1.0f };
		context.ClearRenderTargetView (pRenderTargetView, color);
	}

}

void Looper::OnDeviceDestroyed (const DeviceHolder& _deviceHolder)
{

}
