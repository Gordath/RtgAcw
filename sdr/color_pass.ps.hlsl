struct VOut {
	float4 position : SV_POSITION;
	float4 vertexWorld : TEXCOORD1;
	float3 normal : NORMAL;
	float3 view_direction : VIEW_DIRECTION;
	float3 view_space_pos : VIEW_SPACE_POS;
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

StructuredBuffer<Light> lights : register(t0);

Texture2D diffuse_tex : register(t1);
Texture2D specular_tex : register(t2);
Texture2D normal_tex : register(t3);

Texture2D depth_maps[4] : register(t4);

SamplerState texture_sampler_linear_wrap : register(s0);
SamplerState texture_sampler_linear_clamp : register(s1);

float3 get_light_vector(Light light, float3 pos)
{
	if (light.flags.x == 0) { //not directional
		return mul(float4(light.position.xyz, 1.0), V).xyz - pos;
	}
	
	return mul(light.position, (float3x3)V);
}

void calculate_lighting(StructuredBuffer<Light> lights,
						float3 view_position,
						float3 normal,
						float3 view_direction,
						float4 vertexWorld,
						float shininess,
						inout float4 ambient_light,
						inout float4 diffuse_light,
						inout float4 specular_light)
{
	uint buffer_size = 0;
	uint dummy = 0;

	lights.GetDimensions(buffer_size, dummy);

	for (uint i = 0; i < buffer_size; i++) {
		if (lights[i].flags.y == 1) { //is enabled
			float3 light_vector = normalize(get_light_vector(lights[i], view_position));
			
			float attenuation = 1.0;

			float n_dot_l = max(dot(normal.xyz, light_vector), 0.0);

			if (lights[i].flags.x == 0) { //not directional
				
				float3 spot_dir = normalize(mul(lights[i].spot_direction, (float3x3)V));

				float cos_cur_angle = dot(-light_vector, spot_dir);
				float cos_outer_angle = saturate(cos(radians(lights[i].spot_cutoff)));
				float cos_inner_angle = saturate(cos(radians((1.0 - (lights[i].spot_exponent / 128.0)) * lights[i].spot_cutoff)));
				attenuation *= saturate((cos_cur_angle - cos_outer_angle) /
					(cos_inner_angle - cos_outer_angle));

			}

			//SHADOWING ATTEMPT!!!!
			float4x4 offset_mat = float4x4(0.5, 0.0, 0.0, 0.0,
										   0.0, -0.5, 0.0, 0.0,
										   0.0, 0.0, 1.0, 0.0,
										   0.5, 0.5, 0.0, 1.0);

			float4 shadow_coords = mul(vertexWorld, M);
			shadow_coords = mul(shadow_coords, transpose(lights[i].light_view_matrix));
			shadow_coords = mul(shadow_coords, transpose(lights[i].light_projection_matrix));
			shadow_coords /= shadow_coords.w;
			shadow_coords = mul(shadow_coords, offset_mat);

			float shadow = 1.0;
			float depth_value = 1.0;
			if (i == 0) {
				depth_value = depth_maps[0].Sample(texture_sampler_linear_clamp, shadow_coords.xy).r;
			}

			if (i == 1) {
				depth_value = depth_maps[1].Sample(texture_sampler_linear_clamp, shadow_coords.xy).r;
			}

			if (i == 2) {
				depth_value = depth_maps[2].Sample(texture_sampler_linear_clamp, shadow_coords.xy).r;
			}

			if (i == 3) {
				depth_value = depth_maps[3].Sample(texture_sampler_linear_clamp, shadow_coords.xy).r;
			}

			if (shadow_coords.x < 0.0 || shadow_coords.x >= 1.0 || shadow_coords.y < 0.00 || shadow_coords.y >= 1.0) {
				depth_value = 100000000.0;
			}

			float light_depth_value = shadow_coords.z;
			light_depth_value = light_depth_value;

			

			if (light_depth_value > depth_value) {
				shadow = 0.0;
			}

			float3 h = normalize(view_direction + light_vector);
			float n_dot_h = max(dot(normal, h), 0.0);
			float exponent = 60.0;

			float4 lit_result = lit(n_dot_l, n_dot_h, exponent);

			ambient_light += attenuation * lights[i].ambient_intensity * lit_result.x * shadow;
			diffuse_light += attenuation * lights[i].diffuse_intensity * lit_result.y * shadow;
			specular_light += attenuation * lights[i].specular_intensity * lit_result.z * shadow;
		}
	}

}

float4 main(VOut input) : SV_TARGET
{
	float3 n = normalize(input.normal);

	float3 vdir = normalize(input.view_direction);

	float4 amb_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 diff_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 spec_light = float4(0.0, 0.0, 0.0, 1.0);

	calculate_lighting(lights,
		input.view_space_pos,
		n,
		vdir,
		input.vertexWorld,
		60.0,
		amb_light,
		diff_light,
		spec_light); 

	float4 diff_color = diffuse * diff_light;
	float4 spec_color = specular * spec_light;

	float4 final_color = diff_color + spec_color + amb_light;
	final_color.a = diffuse.a;

	return final_color;
}
