Texture2D tex : register(t0);

SamplerState samplr;

float4 main(float2 tex_cord : texture_coordinate) : SV_TARGET
{
	return tex.Sample(samplr, tex_cord);
}