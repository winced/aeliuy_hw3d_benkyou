#pragma once
#include <string>
#include "vertex.h"
//cbuffer light_cbuf : register(b0)
//{
//    float3 light_pos;
//    float3 ambient;
//    float3 light_diffuse_color;
//    float light_diffuse_intensity;
//    float dist_c_a;
//    float dist_c_b;
//    float dist_c_c;
//    float light_specular_intensity;
//};
//
//
//cbuffer obj_color_cbuf : register(b1)
//{
//    float4 obj_color;
//};
//
//cbuffer camera_pos_cbuf : register(b2)
//{
//    float3 cam_pos;
//};
//
//cbuffer specular_cbuf : register(b3)
//{
//    float obj_specular_intensity;
//    float obj_specular_power;
//    float padding[2];
//};
//
//cbuffer constant_color : register(b4)
//{
//    float4 const_col;
//};
////vertex constant buffers
//
//cbuffer transformation_matrix : register(b0)
//{
//    float4x4 model;
//    float4x4 model_view_projection;
//};
//SamplerState smplr : register(s0);
//
//Texture2D tex : register(t0);
//
//Texture2D specular_map : register(t1);
//
//Texture2D normal_map : register(t2);


//the order of these enums needs to match the order in registers and the order in the shader names 
enum class PS_cbuf_register {
	point_light,
	obj_color,
	camera_pos,
	specular,
	blend_kernel,
	n_PS_cbuf
};
enum class VS_cbuf_register {
	scale_cbuf,
	n_VS_cbuf
};

enum class PS_map_register {
	texture_m,
	specular_m,
	normal_m,
	offscreen_m,
	depth_stencil_m,
	n_PS_maps
};

// TODO: this shit needs to differentiate constatn buffers and maps in the pixel shaders, and so does the naming of them 
static std::string get_vertex_ex(vertex_layout::type t)
{
	switch (t)
	{
	case vertex_layout::type::normal3f:
		return "N";
	case vertex_layout::type::tangent3f:
		return "Ta";
	case vertex_layout::type::bitangent3f:
		return "B";
	case vertex_layout::type::texture2f:
		return "T";
	case vertex_layout::type::position3f:
		return "P";
	case vertex_layout::type::color4f:
		return "C";
	case vertex_layout::type::color3f:
		return "C";
	}
	return "";
}

static std::string get_vs_cbuf_ex(VS_cbuf_register cbuf)
{
	switch (cbuf)
	{
	case VS_cbuf_register::scale_cbuf:
		return "Sc";
		break;
	default:
		break;
	}
	return "";
}

static std::string get_ps_cbuf_ex(PS_cbuf_register cbuf)
{
	switch (cbuf)
	{
	case PS_cbuf_register::obj_color:
		return "C";
	case PS_cbuf_register::specular:
		return "S";
	}
	return "";
}
static std::string get_ps_map_ex(PS_map_register map)
{
	switch (map)
	{
	case PS_map_register::texture_m:
		return "T";
	case PS_map_register::specular_m:
		return "S";
	case PS_map_register::normal_m:
		return "N";
	}
	return "";
}

static std::string get_name_ex_ps(const std::array<bool, (size_t)PS_map_register::n_PS_maps>& maps, const std::array<bool, (size_t)PS_cbuf_register::n_PS_cbuf>& cbufs, const std::array<bool, vertex_layout::type::n_vertex_types>& vertices)
{
	std::string extension;
	extension += "_";
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i])
			extension += get_vertex_ex( vertex_layout::type(i));
	}
	extension += "_";
	for (int i = 0; i < cbufs.size(); i++)
	{
		if (cbufs[i])
			extension += get_ps_cbuf_ex(PS_cbuf_register(i));
	}
	extension += "_";
	for (int i = 0; i < maps.size(); i++)
	{
		if (maps[i])
			extension += get_ps_map_ex(PS_map_register(i));
	}
	extension += ".cso";
	return std::move(extension);
}

static std::string get_name_ex_vs(const std::array<bool, vertex_layout::type::n_vertex_types>& vertex_info_map, const std::array<bool, (size_t)VS_cbuf_register::n_VS_cbuf>& cbufs = {})
{
	std::string extension;
	extension += "_";
	for (size_t i = 0; i < vertex_info_map.size(); i++)
	{
		if(vertex_info_map[i])
			extension += get_vertex_ex(vertex_layout::type(i));
	}
	extension += "_";
	for (size_t i = 0; i < (size_t)VS_cbuf_register::n_VS_cbuf; i++)
	{
		if (cbufs[i])
			extension += get_vs_cbuf_ex(VS_cbuf_register(i));
	}
	extension += ".cso";
	return std::move(extension);
}