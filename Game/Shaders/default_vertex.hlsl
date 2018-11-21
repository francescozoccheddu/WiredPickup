cbuffer cbPerFrame
{
	float4x4 mProjView;
};

/*
cbuffer cbPerDrawable
{
	float4x4 mModel;
};
*/

struct VSIn
{
	float3 vPosition : POSITION;
};

struct VSOut
{
	float4 vPosition : SV_POSITION;
};

VSOut main (in VSIn _sIn)
{
	VSOut sOut;
	sOut.vPosition = mul (float4(_sIn.vPosition, 1.0), mProjView);
	return sOut;
}