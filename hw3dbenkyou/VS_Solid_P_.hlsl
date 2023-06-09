#include "transform_matrix.hlsli"


float4 main( float3 pos : POSITION ) : SV_POSITION
{
    return mul(float4(pos, 1.0f), model_view_projection);
}