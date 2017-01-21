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

struct Light {
	float4 ambient_intensity;
	float4 diffuse_intensity;
	float4 specular_intensity;
	float spot_cutoff;
	float3 spot_direction;
	float spot_exponent;
	float3 attenuation;
	uint4 flags;
	float4x4 light_view_matrix;
	float4x4 light_projection_matrix;
	float3 position;
	float pad;
};



Texture2D diffuse_tex : register(t0);
Texture2D specular_tex : register(t1);
Texture2D normal_tex : register(t2);

StructuredBuffer<Light> lights : register(t4);

Texture2D depth_maps[4] : register(t5);

SamplerState texture_sampler_linear_wrap : register(s0);
SamplerComparisonState depth_comparison_sampler : register(s1);

float get_light_attenuation(Light lgt, float dist)
{
	float attenuation_at_lpos_infty = 1.0;
	return ((lgt.flags.x == 0)
		? 1.0 / ((lgt.attenuation.x) +
		(lgt.attenuation.y * dist) +
			(lgt.attenuation.z * dist * dist))
		: attenuation_at_lpos_infty);
}

float3 get_light_vector(Light light, float3 pos)
{
	if (light.flags.x == 0) { //not directional
		return mul(float4(light.position, 1.0), V).xyz - pos;
	}
	
	return mul(light.position, (float3x3)V);
}

void calculate_lighting(StructuredBuffer<Light> lights,
						float3 view_position,
						float3 normal,
						float3 view_direction,
						float4 vertexWorld,
						float shininess,
						float3x3 TBN,
						inout float4 ambient_light,
						inout float4 diffuse_light,
						inout float4 specular_light)
{
	uint buffer_size = 0;
	uint dummy = 0;

	lights.GetDimensions(buffer_size, dummy);

	for (uint i = 0; i < buffer_size; i++) {
		if (lights[i].flags.y == 1) { //is enabled
			float3 light_vector = get_light_vector(lights[i], view_position);
			
			float attenuation = get_light_attenuation(lights[i], length(light_vector));// 1.0;

			light_vector = normalize(light_vector);

			if (lights[i].flags.x == 0) { //not directional
				
				float3 spot_dir = normalize(mul(lights[i].spot_direction, (float3x3)V));

				float cos_cur_angle = dot(-light_vector, spot_dir);
				float cos_outer_angle = saturate(cos(radians(lights[i].spot_cutoff)));
				float cos_inner_angle = saturate(cos(radians((1.0 - (lights[i].spot_exponent / 128.0)) * lights[i].spot_cutoff)));
				attenuation *= saturate((cos_cur_angle - cos_outer_angle) /
					(cos_inner_angle - cos_outer_angle));

			}

			float4x4 offset_mat = float4x4(0.5, 0.0, 0.0, 0.0,
										   0.0, -0.5, 0.0, 0.0,
										   0.0, 0.0, 1.0, 0.0,
										   0.5, 0.5, 0.0, 1.0);

			float4 shadow_coords = mul(vertexWorld, M);
			shadow_coords = mul(shadow_coords, transpose(lights[i].light_view_matrix));
			shadow_coords = mul(shadow_coords, transpose(lights[i].light_projection_matrix));
			shadow_coords /= shadow_coords.w;
			shadow_coords = mul(shadow_coords, offset_mat);

			float shadow = 0.0;

			float pixel_size = 1.0 / 2048.0;

			const float2 offsets[9] = {
				float2(-pixel_size, -pixel_size), float2(0.0, -pixel_size), float2(pixel_size, -pixel_size),
				float2(-pixel_size, 0.0), float2(0.0, 0.0), float2(pixel_size, 0.0),
				float2(-pixel_size, +pixel_size), float2(0.0, +pixel_size), float2(pixel_size, +pixel_size)
			};

			if (i == 0) {
				[unroll]
				for (int i = 0; i < 9; ++i) {
					shadow += depth_maps[0].SampleCmpLevelZero(depth_comparison_sampler, shadow_coords.xy + offsets[i], shadow_coords.z).r;
				}
			}

			if (i == 1) {
				[unroll]
				for (int i = 0; i < 9; ++i) {
					shadow += depth_maps[1].SampleCmpLevelZero(depth_comparison_sampler, shadow_coords.xy + offsets[i], shadow_coords.z).r;
				}
			}

			if (i == 2) {
				[unroll]
				for (int i = 0; i < 9; ++i) {
					shadow += depth_maps[2].SampleCmpLevelZero(depth_comparison_sampler, shadow_coords.xy + offsets[i], shadow_coords.z).r;
				}
			}

			if (i == 3) {
				[unroll]
				for (int i = 0; i < 9; ++i) {
					shadow += depth_maps[3].SampleCmpLevelZero(depth_comparison_sampler, shadow_coords.xy + offsets[i], shadow_coords.z).r;
				}
			}

			//Light in Tangent space here
			light_vector = normalize(mul(light_vector, TBN));

			float n_dot_l = max(dot(normal.xyz, light_vector), 0.0);
			float3 h = normalize(view_direction + light_vector);
			float n_dot_h = max(dot(normal, h), 0.0);

			float4 lit_result = lit(n_dot_l, n_dot_h, shininess);

			shadow /= 9.0;

			ambient_light += attenuation * lights[i].ambient_intensity * lit_result.x * shadow;
			diffuse_light += attenuation * lights[i].diffuse_intensity * lit_result.y * shadow;
			specular_light += attenuation * lights[i].specular_intensity * lit_result.z * shadow;
		}
	}

}

float4 main(VOut input) : SV_TARGET
{
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	float3 binormal = normalize(input.binormal);

	float3x3 TBN = transpose(float3x3(tangent,
							binormal,
							normal));

	float4 norm_texel = normal_tex.Sample(texture_sampler_linear_wrap, input.texcoord.xy);
	float3 n = normalize(norm_texel.xyz * 2.0 - 1.0);

	float3 vdir = normalize(mul(input.view_direction, TBN));

	float4 amb_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 diff_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 spec_light = float4(0.0, 0.0, 0.0, 1.0);

	calculate_lighting(lights,
		input.view_space_pos,
		n,
		vdir,
		input.vertexWorld,
		specular.a,
		TBN,
		amb_light,
		diff_light,
		spec_light); 

	float4 diff_texel = diffuse_tex.Sample(texture_sampler_linear_wrap, input.texcoord.xy);
	float4 spec_texel = specular_tex.Sample(texture_sampler_linear_wrap, input.texcoord.xy);

	float4 diff_color = diff_texel * diffuse * diff_light;
	float4 spec_color = spec_texel * specular * spec_light;

	float4 final_color = diff_color + spec_color + amb_light;
	//final_color = lerp(float4(0.0470588235294118f, 0.3019607843137255f, 0.4117647058823529f, 1.0), final_color, input.fog_factor);
	
	final_color.a = diffuse.a;


	return final_color;
}
