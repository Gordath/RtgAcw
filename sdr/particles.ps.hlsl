struct VOut {
	float4 position : SV_POSITION;
	float4 texcoord : TEXCOORD0;
};

float4 main(VOut input) : SV_TARGET
{
	return float4(input.texcoord.x, input.texcoord.y, 0.0f, 1.0f);
}
