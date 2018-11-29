#include <Game/Looper/Looper.hpp>

#include <Game/Utils/Storage.hpp>
#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>
#include <Game/DirectXMath.hpp>

Looper::Looper (const DeviceHolder & _deviceHolder) : m_DeviceHolder { _deviceHolder }
{}

Looper::~Looper ()
{}

void Looper::OnDeviceCreated (const DeviceHolder & _deviceHolder)
{
	ID3D11Device & device { *m_DeviceHolder.GetDevice () };
	ID3D11DeviceContext & context { *m_DeviceHolder.GetDeviceContext () };
	{
		static constexpr DirectX::XMFLOAT2 vertices[] { { -0.5f, -0.5f }, { 0.0f, 0.5f }, { 0.5f, -0.5f } };
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof (vertices);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof (DirectX::XMFLOAT2);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		GAME_COMC (device.CreateBuffer (&desc, &data, &m_pVertexBuffer));
		UINT offsets[] { 0 };
		UINT strides[] { sizeof (DirectX::XMFLOAT2) };
		context.IASetVertexBuffers (0, 1, &m_pVertexBuffer, strides, offsets);
	}
	{
		D3D11_RASTERIZER_DESC desc {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		GAME_COMC (device.CreateRasterizerState (&desc, &m_pRasterizerState));
		context.RSSetState (m_pRasterizerState);
		context.IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	{
		static const std::vector<char> data { Storage::LoadBinaryFile ("default_vertex.cso") };
		GAME_COMC (device.CreateVertexShader (data.data (), static_cast<SIZE_T>(data.size ()), nullptr, &m_pVertexShader));
		context.VSSetShader (m_pVertexShader, nullptr, 0);
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.AlignedByteOffset = 0;
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			desc.InputSlot = 0;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			desc.SemanticIndex = 0;
			desc.SemanticName = "POSITION";
			GAME_COMC (device.CreateInputLayout (&desc, 1, data.data (), static_cast<SIZE_T>(data.size ()), &m_pInputLayout));
			context.IASetInputLayout (m_pInputLayout);
		}
	}
	{
		static const std::vector<char> pData { Storage::LoadBinaryFile ("default_pixel.cso") };
		GAME_COMC (device.CreatePixelShader (pData.data (), static_cast<SIZE_T>(pData.size ()), nullptr, &m_pPixelShader));
		context.PSSetShader (m_pPixelShader, nullptr, 0);
	}
}

void Looper::OnSized (const DeviceHolder & _deviceHolder)
{
	ID3D11Device & device { *m_DeviceHolder.GetDevice () };
	WindowSize size = m_DeviceHolder.GetSize ();
	{
		m_Viewport.Width = static_cast<FLOAT>(size.width);
		m_Viewport.Height = static_cast<FLOAT>(size.height);
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}
	ID3D11DeviceContext & context { *m_DeviceHolder.GetDeviceContext () };
	context.RSSetViewports (1, &m_Viewport);
	ID3D11RenderTargetView * pRenderTargetView { m_DeviceHolder.GetRenderTargetView () };
	context.OMSetRenderTargets (1, &pRenderTargetView, nullptr);
}

void Looper::Render ()
{
	ID3D11DeviceContext & context { *m_DeviceHolder.GetDeviceContext () };
	ID3D11RenderTargetView * const pRenderTargetView { m_DeviceHolder.GetRenderTargetView () };

	{
		float color[4] { 1.0f, 0.2f, 0.2f, 1.0f };
		context.ClearRenderTargetView (pRenderTargetView, color);
	}

	{
		context.RSSetViewports (1, &m_Viewport);
		context.OMSetRenderTargets (1, &pRenderTargetView, nullptr);
		UINT offsets[] { 0 };
		UINT strides[] { sizeof (DirectX::XMFLOAT2) };
		context.IASetVertexBuffers (0, 1, &m_pVertexBuffer, strides, offsets);
		context.RSSetState (m_pRasterizerState);
		context.IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context.VSSetShader (m_pVertexShader, nullptr, 0);
		context.PSSetShader (m_pPixelShader, nullptr, 0);
		context.IASetInputLayout (m_pInputLayout);
	}

	context.Draw (3, 0);
}

void Looper::OnDeviceDestroyed (const DeviceHolder& _deviceHolder)
{
	m_pPixelShader->Release ();
	m_pVertexShader->Release ();
	m_pRasterizerState->Release ();
	m_pVertexBuffer->Release ();
	m_pInputLayout->Release ();
}
