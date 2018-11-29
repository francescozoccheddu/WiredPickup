struct VSIn
{
	float2 vPosition : POSITION;
};

struct VSOut
{
	float4 vPosition : SV_POSITION;
};

VSOut main (in VSIn _sIn)
{
	VSOut sOut;
	sOut.vPosition = float4(_sIn.vPosition, 1.0, 1.0);
	return sOut;
}