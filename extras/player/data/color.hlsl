struct VSInput
{
	float3 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_POSITION;
};

VSOutput vs_main(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 1);
	return output;
}

float4 fs_main(VSOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}