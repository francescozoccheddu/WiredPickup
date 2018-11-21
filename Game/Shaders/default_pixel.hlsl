struct PSIn
{
	float4 vPosition : SV_POSITION;
};

struct PSOut
{
	float4 vColor : SV_TARGET;
};

PSOut main (in PSIn _sIn)
{
	PSOut sOut;
	sOut.vColor = float4(1.0, 1.0, 1.0, 1.0);
	return sOut;
}