#include <Game/Engine/DeviceHolder.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <Game/Utils/COMExceptions.hpp>

#define SWAP_CHAIN_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM
#define FIRE_EVENT(x) { for (EngineListener* listener : Listeners) listener -> x ; }


DeviceHolder::DeviceHolder () {}

DeviceHolder::~DeviceHolder ()
{
	ReleaseAll ();
}

void DeviceHolder::Present ()
{
#if GAME_D3D11_HEADER_VERS >= 1
	DXGI_PRESENT_PARAMETERS pars;
	pars.DirtyRectsCount = 0;
	pars.pDirtyRects = nullptr;
	pars.pScrollOffset = nullptr;
	pars.pScrollRect = nullptr;
	const HRESULT hResPresent { m_pSwapChain->Present1 (1, 0, &pars) };
	com_ptr<ID3D11DeviceContext1> pDeviceContext1;
	m_pDeviceContext.try_as (pDeviceContext1);
	if (pDeviceContext1)
	{
		pDeviceContext1->DiscardView1 (m_pRenderTargetView.get (), nullptr, 0);
	}
#else
	const HRESULT hResPresent { m_pSwapChain->Present (1, 0) };
#endif
	if (hResPresent == DXGI_ERROR_DEVICE_REMOVED || hResPresent == DXGI_ERROR_DEVICE_RESET)
	{
		HandleDeviceLost ();
	}
	else
	{
		GAME_COMC (hResPresent);
	}
}

void DeviceHolder::Size (WindowSize _size, WindowRotation _rotation, bool _bForce)
{
	if (_bForce || _size != m_Size || _rotation != m_Rotation)
	{
		m_pRenderTargetView = nullptr;
		m_Size = _size;
		m_Rotation = _rotation;
		bool recreated { false };
		if (m_pSwapChain)
		{
			const HRESULT hr { m_pSwapChain->ResizeBuffers (2, _size.width, _size.height, SWAP_CHAIN_FORMAT, 0) };
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				HandleDeviceLost ();
				recreated = true;
			}
			else
			{
				GAME_COMC (hr);
			}
		}
		else
		{
			CreateSwapChain ();
		}
#if GAME_D3D11_HEADER_VERS >= 1
		DXGI_MODE_ROTATION dxgiRotation;
		switch (m_Rotation)
		{
			case WindowRotation::IDENTITY:
				dxgiRotation = DXGI_MODE_ROTATION_IDENTITY;
				break;
			case WindowRotation::ROTATE_90:
				dxgiRotation = DXGI_MODE_ROTATION_ROTATE90;
				break;
			case WindowRotation::ROTATE_180:
				dxgiRotation = DXGI_MODE_ROTATION_ROTATE180;
				break;
			case WindowRotation::ROTATE_270:
				dxgiRotation = DXGI_MODE_ROTATION_ROTATE270;
				break;
			default:
				GAME_THROW_MSG ("Unknown rotation");
				break;
		}
		GAME_COMC (m_pSwapChain->SetRotation (dxgiRotation));
#endif
		if (!recreated)
		{
			CreateRenderTarget ();
		}
		FIRE_EVENT (OnSized (*this));
	}
}

void DeviceHolder::Destroy ()
{
	ReleaseAll ();
	FIRE_EVENT (OnDeviceDestroyed ());
}

void DeviceHolder::SetWindow (GAME_NATIVE_WINDOW_T _window, WindowSize _size, WindowRotation _rotation)
{
	if (!m_pDevice)
	{
		CreateDeviceAndDeviceContext ();
		FIRE_EVENT (OnDeviceCreated (*this));
	}
	m_NativeWindow = _window;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDeviceContext->ClearState ();
	m_pDeviceContext->Flush ();
	Size (_size, _rotation, true);
}

void DeviceHolder::Trim ()
{
#if GAME_D3D11_HEADER_VERS >= 3
	if (m_pDevice)
	{
		m_pDeviceContext->ClearState ();
		com_ptr<IDXGIDevice3> pDevice;
		if (m_pDevice.try_as (pDevice))
		{
			pDevice->Trim ();
		}
	}
#endif
}

void DeviceHolder::ValidateDevice ()
{
#if GAME_D3D11_HEADER_VERS >= 3
	com_ptr<IDXGIFactory2> pFactory;
	{
		com_ptr<IDXGIDevice> pDevice;
		com_ptr<IDXGIAdapter> pAdapter;
		m_pDevice.as (pDevice);
		pDevice->GetAdapter (pAdapter.put ());
		pAdapter->GetParent (IID_PPV_ARGS (pFactory.put ()));
	}
	DXGI_ADAPTER_DESC previousDesc;
	{
		com_ptr<IDXGIAdapter1> previousDefaultAdapter;
		GAME_COMC (pFactory->EnumAdapters1 (0, previousDefaultAdapter.put ()));
		GAME_COMC (previousDefaultAdapter->GetDesc (&previousDesc));
	}

	DXGI_ADAPTER_DESC currentDesc;
	{
		com_ptr<IDXGIFactory2> currentFactory;
		GAME_COMC (CreateDXGIFactory2 (0, IID_PPV_ARGS (currentFactory.put ())));

		com_ptr<IDXGIAdapter1> currentDefaultAdapter;
		GAME_COMC (currentFactory->EnumAdapters1 (0, currentDefaultAdapter.put ()));

		GAME_COMC (currentDefaultAdapter->GetDesc (&currentDesc));
	}

	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart
		|| previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart
		|| FAILED (m_pDevice->GetDeviceRemovedReason ()))
	{
		HandleDeviceLost ();
	}
#endif
}

ID3D11Device * DeviceHolder::GetDevice () const
{
	return m_pDevice.get ();
}

ID3D11DeviceContext * DeviceHolder::GetDeviceContext () const
{
	return m_pDeviceContext.get ();
}

ID3D11RenderTargetView * DeviceHolder::GetRenderTargetView () const
{
	return m_pRenderTargetView.get ();
}

WindowSize DeviceHolder::GetSize () const
{
	return m_Size;
}

D3D_FEATURE_LEVEL DeviceHolder::GetSupportedFeatureLevel () const
{
	return m_SupportedFeatureLevel;
}

WindowRotation DeviceHolder::GetRotation () const
{
	return m_Rotation;
}

void DeviceHolder::CreateDeviceAndDeviceContext ()
{
#if GAME_D3D11_HEADER_VERS >= 1
	const D3D_FEATURE_LEVEL featureLevels[] { D3D_FEATURE_LEVEL_11_1 };
#else
	const D3D_FEATURE_LEVEL featureLevels[] { D3D_FEATURE_LEVEL_11_0 };
#endif
	UINT flags { D3D11_CREATE_DEVICE_SINGLETHREADED };
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr { D3D11CreateDevice (nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, ARRAYSIZE (featureLevels), D3D11_SDK_VERSION, m_pDevice.put (), &m_SupportedFeatureLevel, m_pDeviceContext.put ()) };
	if (FAILED (hr))
	{
		GAME_COMC (D3D11CreateDevice (nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, featureLevels, ARRAYSIZE (featureLevels), D3D11_SDK_VERSION, m_pDevice.put (), &m_SupportedFeatureLevel, m_pDeviceContext.put ()));
	}
}

void DeviceHolder::CreateSwapChain ()
{
#if GAME_D3D11_HEADER_VERS >= 1
	m_pSwapChain = nullptr;
	com_ptr<IDXGIFactory2> pFactory;
	{
		com_ptr<IDXGIDevice> pDevice;
		com_ptr<IDXGIAdapter> pAdapter;
		m_pDevice.as (pDevice);
		pDevice->GetAdapter (pAdapter.put ());
		pAdapter->GetParent (IID_PPV_ARGS (pFactory.put ()));
	}

	DXGI_SWAP_CHAIN_DESC1 desc;
	desc.BufferCount = 2;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = m_Size.width;
	desc.Height = m_Size.height;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.Format = SWAP_CHAIN_FORMAT;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Stereo = FALSE;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = 0;
#if GAME_PLATFORM == GAME_PLATFORM_UWP
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	GAME_COMC (pFactory->CreateSwapChainForCoreWindow (m_pDevice.get (), m_NativeWindow, &desc, nullptr, m_pSwapChain.put ()));
#elif GAME_PLATFORM == GAME_PLATFORM_WIN32
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	GAME_COMC (pFactory->CreateSwapChainForHwnd (m_pDevice.get (), m_NativeWindow, &desc, nullptr, nullptr, m_pSwapChain.put ()));
#else
#error Unknown platform
#endif
#else
	com_ptr<IDXGIFactory> pFactory;
	{
		com_ptr<IDXGIDevice> pDevice;
		com_ptr<IDXGIAdapter> pAdapter;
		m_pDevice.as (pDevice);
		pDevice->GetAdapter (pAdapter.put ());
		pAdapter->GetParent (IID_PPV_ARGS (pFactory.put ()));
	}

	DXGI_SWAP_CHAIN_DESC desc {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = SWAP_CHAIN_FORMAT;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Windowed = TRUE;
	desc.OutputWindow = m_NativeWindow;
	desc.Flags = 0;
	desc.BufferDesc.Width = m_Size.width;
	desc.BufferDesc.Height = m_Size.height;
	GAME_COMC (pFactory->CreateSwapChain (m_pDevice.get (), &desc, m_pSwapChain.put ()));
#endif
}

void DeviceHolder::CreateRenderTarget ()
{
	m_pRenderTargetView = nullptr;
	com_ptr<ID3D11Texture2D> pTexture;
	GAME_COMC (m_pSwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), pTexture.put_void ()));
	GAME_COMC (m_pDevice->CreateRenderTargetView (pTexture.get (), nullptr, m_pRenderTargetView.put ()));
}

void DeviceHolder::HandleDeviceLost ()
{
	ReleaseAll ();
	FIRE_EVENT (OnDeviceDestroyed ());
	CreateDeviceAndDeviceContext ();
	FIRE_EVENT (OnDeviceCreated (*this));
	CreateSwapChain ();
	CreateRenderTarget ();
}

void DeviceHolder::ReleaseAll ()
{
	m_pRenderTargetView = nullptr;
	m_pSwapChain = nullptr;
	m_pDevice = nullptr;
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState ();
		m_pDeviceContext->Flush ();
		m_pDeviceContext = nullptr;
	}
}
