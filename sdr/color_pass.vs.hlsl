struct VIn {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 texcoord : TEXCOORD0;
	float4 color : COLOR;
};

cbuffer uniforms {
	float4x4 MVP;
	float4x4 MV;
	float4x4 M;
	float4x4 V;
	float4x4 P;
	float4x4 ITMV;
	float4x4 texture_matrix;
	float4 diffuse;
	float4 specular;
	float fpower;
	float fbias;
	float pad;
	float pad1;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 vertexWorld : TEXCOORD1;
	float3 normal : NORMAL;
	float3 view_direction : VIEW_DIRECTION;
	float3 view_space_pos : VIEW_SPACE_POS;
	float fog_factor : TEXCOORD2;
	float4 texcoord : TEXCOORD0;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);
	output.vertexWorld = input.position;
	output.normal = mul(input.normal.xyz, (float3x3)ITMV);
	output.texcoord = mul(input.texcoord, texture_matrix);
	output.tangent = mul(input.tangent.xyz, (float3x3)ITMV);
	output.binormal = cross(output.normal, output.tangent);

	float4 vpos = mul(input.position, MV);
	output.view_space_pos = vpos.xyz;
	output.view_direction = -vpos.xyz;

	float fog_start = 0.0;
	float fog_end = 50.0;

	float vdist = length(output.view_space_pos);
	float fog_density = 0.0018;
	
	float exp = (vdist * fog_density) * (vdist * fog_density);
	output.fog_factor = 1.0 / pow(2.71828, exp);

	return output;
}
