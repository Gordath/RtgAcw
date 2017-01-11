struct VIn {
	float4 position : POSITION;
};

struct VOut {
	float4 position : SV_POSITION;
};

cbuffer uniforms {
	float4x4 MVP;
};

VOut main(VIn input)
{
	VOut output;

	output.position = mul(input.position, MVP);

	return output;
}
