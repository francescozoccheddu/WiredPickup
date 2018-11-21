#pragma once

#include <Game/Direct3D.hpp>
#include <Game/Resources/Resource.hpp>
#include <Game/Resources/ShaderResource.hpp>
#include <vector>

struct InputLayoutProvider
{

	virtual ~InputLayoutProvider () = default;

	virtual const D3D11_INPUT_ELEMENT_DESC * GetDescriptions () const = 0;

	virtual int GetDescriptionCount () const = 0;

};

struct InputLayout : public InputLayoutProvider
{

	virtual inline const D3D11_INPUT_ELEMENT_DESC * GetDescriptions () const override
	{
		return descriptions.data ();
	}

	virtual inline int GetDescriptionCount () const override
	{
		return static_cast<int>(descriptions.size ());
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> descriptions;

};

class InputLayoutResource : public AtomicResource
{

public:

	static void Reset (ID3D11DeviceContext & deviceContext);

	void Set (ID3D11DeviceContext & deviceContext) const;

	void ForceCreate (ID3D11Device & device) override final;

	void ForceDestroy () override final;

	bool IsCreated () const override final;

	const InputLayoutProvider * pLayout { nullptr };
	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11InputLayout * m_pInputLayout { nullptr };

};
