#pragma once

#include <Game/Resources/Resource.hpp>
#include <Game/Resources/ShaderType.hpp>
#include <string>

struct ShaderBytecodeProvider
{

	virtual ~ShaderBytecodeProvider () = default;

	virtual const void * GetData () const = 0;

	virtual size_t GetSize () const = 0;

};

struct ShaderBytecode : public ShaderBytecodeProvider
{

	ShaderBytecode () = default;

	ShaderBytecode (const std::vector<char>& bytecode);

	virtual inline const void * GetData () const override
	{
		return bytecode.data ();
	}

	virtual inline size_t GetSize () const override
	{
		return static_cast<size_t>(bytecode.size ());
	}

	std::vector<char> bytecode;

	static const ShaderBytecode CreateFromFile (const std::string& filename);

};

class ShaderResource : public SimpleResource<ID3D11DeviceChild>
{

public:

	ShaderResource (ShaderType type);

	static void Reset (ID3D11DeviceContext & deviceContext, ShaderType type);

	void Set (ID3D11DeviceContext & deviceContext) const;

	ShaderType GetType () const;

	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11DeviceChild * Create (ID3D11Device & device) const override final;

	const ShaderType m_Type;

};