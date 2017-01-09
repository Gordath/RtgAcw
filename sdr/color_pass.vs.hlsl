struct VIn {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tagent : TANGENT;
	float4 texcoord : TEXCOORD;
	float4 color : COLOR;
};

cbuffer uniforms {
	float4x4 MVP;
	float4x4 MV;
	float4x4 M;
	float4x4 V;
	float4x4 P;
	float4x4 ITMV;
	float4 diffuse;
	float4 specular;
};

struct VOut {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 view_direction : VIEW_DIRECTION;
	float3 view_space_pos : VIEW_SPACE_POS;
	float3 lpos : LPOS;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);
	output.normal = mul(input.normal.xyz, (float3x3)ITMV);

	float4 lpos = mul(float4(0.0, 0.0, -20.0, 1.0), V);

	float3 vpos = mul(input.position, MV).xyz;
	output.view_space_pos = vpos;
	output.view_direction = -vpos;
	output.lpos = lpos.xyz;// - vpos;

	return output;
}
