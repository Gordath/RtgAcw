struct VIn {
	float4 position : POSITION;
	float4 normal : NORMAL;
};

struct VOut {
	float4 position : SV_POSITION;
	float3 normal : TEXCOORD0;
};

cbuffer uniforms
{
	float4x4 VP;
	float4x4 ITV;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, VP);
	output.normal = input.position.xyz;

	return output;
}
