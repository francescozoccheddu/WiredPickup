#pragma once

#include <Game/Resources/Resource.hpp>
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

class ShaderResource : public AtomicResource
{

public:

	enum class Type
	{
		VertexShader, PixelShader, GeometryShader
	};

	ShaderResource (Type type);

	static void ResetShader (ID3D11DeviceContext & deviceContext, Type type);

	void SetShader (ID3D11DeviceContext & deviceContext) const;

	void ForceCreate (ID3D11Device & device) override final;

	void ForceDestroy () override final;

	bool IsCreated () const override final;

	Type GetType () const;

	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11DeviceChild * m_pShader { nullptr };
	const Type m_Type;

};