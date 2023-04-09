#include "func_u.hlsl"
#include "cam_u.hlsl"
#include "light_u.hlsl"
#include "backface_cbuf.hlsl"
#include "specular_map_consts.hlsl"

SamplerState smplr : register(s0);

Texture2D tex : register(t0);

Texture2D specular_map : register(t1);

Texture2D normal_map : register(t2);


float4 main(float3 pixel_world_pos : POSITION, float3 n : NORMAL, float2 tex_cord : TEXTURE, float3 tangent : TANGENT, float3 bitangent : BITANGENT) : SV_TARGET
{
// multiply * operator in hlsl is hadamard 
// to dot, call dot 
// saturate, saturates a vector/scalar from 0.0f to 1.0f
// these functions are called intrinsic functions 
    float4 tex_sample = tex.Sample(smplr, tex_cord);
    if (has_alpha)
    {
        clip(tex_sample.a < 0.1 ? -1 : 1);
    }
    n = normalize(n);
    n = map_normal(smplr, normal_map, n, tex_cord, tangent, bitangent);
    if (backface_enabled)
    {
        n = back_face_normal(n, cam_pos, pixel_world_pos);
    }
	float4 specular_sample = specular_map.Sample(smplr, tex_cord);
    float specular_intensity = calc_specular_intensity(cam_pos, light_pos, light_specular_intensity, pixel_world_pos, n, specular_sample.rgb, specular_sample.a * specular_power_factor);
	float3 diffuse = calc_diffuse(light_diffuse_intensity, light_diffuse_color, light_pos, pixel_world_pos, n, dist_c_a, dist_c_b, dist_c_c);
// specular is not affected by the color of the object, since it is just the reflection of the light. Which is also why it needs to be affected by the diffuse
    return float4(saturate(tex_sample.rgb * (ambient + diffuse) + (diffuse * specular_intensity)), tex_sample.a);
}