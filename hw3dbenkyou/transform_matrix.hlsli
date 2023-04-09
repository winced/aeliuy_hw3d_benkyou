

cbuffer transformation_matrix : register(b0)
{
    float4x4 model;
    float4x4 model_view_projection;
};