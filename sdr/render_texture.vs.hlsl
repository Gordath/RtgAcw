/*struct VIn {
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VOut {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VOut main(VIn input)
{
	VOut output;

	output.position = input.position;
	output.texcoord = input.texcoord;

	return output;
}*/

struct VOut {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VOut main(uint vI : SV_VERTEXID)
{
	VOut output;

	float2 texcoord = float2(vI & 1,vI >> 1); //you can use these for texture coordinates later

	output.position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0, 1);
	output.texcoord = texcoord;

	return output;
}
