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
	float fresnel_term : TEXCOORD3;
	float4 texcoord : TEXCOORD0;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
	float3 reflected_view_dir : TEXCOORD6;
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

	output.reflected_view_dir = reflect(output.view_direction, input.normal.xyz);

	float fresnel_power = fpower;
	float fresnel_bias = fbias;
	float fresnel_scale = 1.0 - fresnel_bias;

	float3 i = normalize(output.view_space_pos);
	float3 n = normalize(output.normal);
	output.fresnel_term = fresnel_bias + fresnel_scale * pow(saturate(1.0 + dot(i, n)), fresnel_power);

	return output;
}

