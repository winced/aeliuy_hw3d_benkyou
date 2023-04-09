
cbuffer obj_color_cbuf : register(b1)
{
    float4 obj_color;
};


float4 main() : SV_TARGET
{
    return obj_color;
}