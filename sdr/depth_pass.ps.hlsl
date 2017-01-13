struct VOut {
	float4 position : SV_POSITION;
};


void main(VOut input)
{
}

RasterizerState Depth {
	DepthBias = 100000;
	DepthBiasClamp = 0.0;
	SlopeScaledDepthBias = 1.0;
};
