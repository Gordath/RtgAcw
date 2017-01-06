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
	float4x4 M;
	float4x4 V;
	float4x4 P;
	float4x4 ITMV;
	float4 diffuse;
	float4 specular;
	float4 view_position;
	uint4 light_count;
};

struct VOut {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 view_direction : VIEW_DIRECTION;
	float3 view_space_pos : VIEW_SPACE_POS;
};

struct Light {
	bool enabled;
	float4 ambient_intensity;
	float4 diffuse_intensity;
	float4 specular_intensity;
	float spot_cutoff;
	float3 spot_direction;
	float spot_exponent;
	float3 attenuation;
	float3 position;
	bool directional;
};

StructuredBuffer<Light> lights;

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);
	output.normal = mul(input.normal, ITMV).xyz;

	float3 vpos = mul(input.position, MV).xyz;
	output.view_space_pos = vpos;
	output.view_direction = -vpos;

	return output;
}
