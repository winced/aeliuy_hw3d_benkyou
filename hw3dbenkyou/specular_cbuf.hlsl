
cbuffer specular_cbuf : register(b3)
{
    float obj_specular_intensity;
    float obj_specular_power;
    float padding[2];
};