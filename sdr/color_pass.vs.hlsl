struct VIn {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tagent : TANGENT;
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
	float4 diffuse;
	float4 specular;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 vertexWorld : TEXCOORD1;
	float3 normal : NORMAL;
	float3 view_direction : VIEW_DIRECTION;
	float3 view_space_pos : VIEW_SPACE_POS;
	float fog_factor : TEXCOORD2;
	float fresnel_term : TEXCOORD3;
	float4 texcoord : TEXCOORD0;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);
	output.vertexWorld = input.position;
	output.normal = mul(input.normal.xyz, (float3x3)ITMV);
	output.texcoord = input.texcoord;

	float4 vpos = mul(input.position, MV);
	output.view_space_pos = vpos.xyz;
	output.view_direction = -vpos.xyz;

	float fog_start = 0.0;
	float fog_end = 50.0;

	float vdist = length(output.view_space_pos);
	float fog_density = 0.0006;
	float exp = (vdist * fog_density) ;
	output.fog_factor = 1.0 / pow(2.71828, exp);


	float fresnel_power = 5.0;
	float fresnel_bias = 0.10;
	float fresnel_scale = 1.0 - fresnel_bias;

	float3 i = normalize(output.view_space_pos);
	float3 n = normalize(output.normal);
	output.fresnel_term = fresnel_bias + fresnel_scale * pow(1.0 + dot(i, n), fresnel_power);

	return output;
}
