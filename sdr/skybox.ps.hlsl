struct VOut {
	float4 position : SV_POSITION;
	float3 normal : TEXCOORD0;
};

TextureCube cubemap : register(t0);

SamplerState linear_clamp  : register(s0);

float4 main(VOut input) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float4 diff_texel = cubemap.SampleLevel(linear_clamp, normal, 0);

	float4 final_color;
	final_color.rgb = diff_texel.rgb;
	final_color.a = 1.0;

	return final_color;
}
