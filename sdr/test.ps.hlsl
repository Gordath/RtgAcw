struct VOut {
	float4 position : SV_POSITION;
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
	float4 view_position;
	uint4 light_count;
};

struct Light {
	float4 ambient_intensity;
	float4 diffuse_intensity;
	float4 specular_intensity;
	float spot_cutoff;
	float3 spot_direction;
	float spot_exponent;
	float3 attenuation;
	float3 position;
	uint4 flags;
};

StructuredBuffer<Light> lights : register(t0);

Texture2D diffuse_tex : register(t1);
Texture2D specular_tex : register(t2);
Texture2D normal_tex : register(t3);
Texture2D emission_tex : register(t4);
Texture2D ambient_occlusion_tex : register(t5);

float3 get_light_vector(Light light, float3 pos)
{
	if (light.flags.x == 0) { //not directional
		return light.position - pos;
	}
	
	return light.position;
}

void calculate_lighting(StructuredBuffer<Light> lights,
						float3 view_position,
						float3 normal,
						float3 view_direction,
						float shininess,
						inout float4 ambient_light,
						inout float4 diffuse_light,
						inout float4 specular_light)
{
	for (uint i = 0; i < light_count.x; i++) {
		if (lights[i].flags.y == 1) { //is enabled
			float3 light_vector = normalize(get_light_vector(lights[i], view_position));
			
			float n_dot_l = max(dot(normal.xyz, light_vector), 0.0);

			if (lights[i].flags.x == 0) { //not directional

			}

			float3 h = normalize(view_direction + light_vector);
			float n_dot_h = max(dot(normal, h), 0.0);
			float exponent = 60.0;

			float4 lit_result = lit(n_dot_l, n_dot_h, exponent);

			ambient_light += lights[i].ambient_intensity * lit_result.x;
			diffuse_light += lights[i].diffuse_intensity * lit_result.y;
			specular_light += lights[i].specular_intensity * lit_result.z;
		}
	}

}

float4 main(VOut input) : SV_TARGET
{

	float3 n = normalize(input.normal);
	float3 v = normalize(input.view_direction);

	float4 amb_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 diff_light = float4(0.0, 0.0, 0.0, 1.0);
	float4 spec_light = float4(0.0, 0.0, 0.0, 1.0);

	calculate_lighting(lights,
		input.view_space_pos,
		n,
		v,
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
