#pragma once

#include <Game/Resources/Resource.hpp>
#include <string>

class ShaderBytecodeProvider
{

public:

	virtual ~ShaderBytecodeProvider () = default;

	virtual const void * GetData () const = 0;

	virtual size_t GetSize () const = 0;

};

class ShaderBytecode final : public ShaderBytecodeProvider
{

public:

	ShaderBytecode () = default;

	ShaderBytecode (const std::vector<char>& bytecode);

	virtual const void * GetData () const override;

	virtual size_t GetSize () const override;

	std::vector<char> bytecode;

	static const ShaderBytecode CreateFromFile (const std::string& filename);

};

class VertexShaderResource final : public Resource
{

public:

	~VertexShaderResource () override;

	VertexShaderResource (const D3D11_INPUT_ELEMENT_DESC * pDescriptions, int cDescriptions);

	void SetShaderAndInputLayout (ID3D11DeviceContext & deviceContext) const;

	void Create (ID3D11Device & device) override final;

	void Destroy () override final;

	bool IsCreated () const override final;

	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11VertexShader * m_pShader { nullptr };
	ID3D11InputLayout * m_pInputLayout { nullptr };
	const D3D11_INPUT_ELEMENT_DESC * m_pDescriptions;
	const int m_cDescriptions;

};

class PixelShaderResource final : public Resource
{

public:

	~PixelShaderResource () override;

	void SetShader (ID3D11DeviceContext & deviceContext) const;

	void Create (ID3D11Device & device) override final;

	void Destroy () override final;

	bool IsCreated () const override final;

	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11PixelShader * m_pShader { nullptr };

};

class GeometryShaderResource final : public Resource
{

public:

	~GeometryShaderResource () override;

	void SetShader (ID3D11DeviceContext & deviceContext) const;

	void Create (ID3D11Device & device) override final;

	void Destroy () override final;

	bool IsCreated () const override final;

	const ShaderBytecodeProvider * pBytecode { nullptr };

private:

	ID3D11GeometryShader * m_pShader { nullptr };
};