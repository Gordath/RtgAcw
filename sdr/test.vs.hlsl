struct VIn {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tagent : TANGENT;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

cbuffer uniforms {
	float4x4 MVP;
	float4x4 MV;
	float4x4 ITMV;
	float4 diffuse;
	float4 specular;
	float4 view_position;
};

struct VOut {
	float4 position : SV_POSITION;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);

	return output;
}
