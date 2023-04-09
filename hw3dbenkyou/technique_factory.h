#pragma once
#include "drawable.h"
#include "fullscreen_blend.h"


namespace technique_factory
{
	// configures the setting for the different dx_components 
	// could be just some argument packs for each of the dx_components 
	static dc_buf create_obj_color_dc_buf(DirectX::XMFLOAT4 val = { 1.0f, 1.0f, 1.0f, 1.0f }, const std::string& col_name = "color")
	{
		dc_buf temp_cbuf;
		temp_cbuf.add<dc_buf::type::float4>(col_name)(std::move(val));
		return temp_cbuf;
	}
	static dc_buf create_specular_dc_buf(float obj_specular = 0.8f, float obj_specular_power = 60.0f, const std::string& specular_name = "specular", const std::string& power_name = "power")
	{
		dc_buf temp_cbuf;
		temp_cbuf.add<dc_buf::type::float1>(specular_name)(std::move(obj_specular));
		temp_cbuf.add<dc_buf::type::float1>(power_name)(std::move(obj_specular_power));
		return temp_cbuf;
	}
	static dc_buf create_scale_cbuf(float scale = 1.0f)
	{
		dc_buf temp_cbuf;
		temp_cbuf.add<dc_buf::type::float1>("scale")(std::move(scale));
		return temp_cbuf;
	}
	template<typename... args>
	static dc_buf create_ps_cbuf(PS_cbuf_register cb, args&&... arguments)
	{
		if (cb == PS_cbuf_register::specular)
			return create_specular_dc_buf(std::forward<args>(arguments)...);
		if (cb == PS_cbuf_register::obj_color)
			return create_obj_color_dc_buf(std::forward<args>(arguments)...);
	}
	template<typename... args>
	static dc_buf create_vs_cbuf(VS_cbuf_register cb, args&&... arguments)
	{
		if (cb == VS_cbuf_register::scale_cbuf)
			return create_scale_cbuf(std::forward<args>(arguments)...);
	}
	static fullscreen_blender_description create_outline_blend_desc()
	{
		//std::array<bool, (size_t)PS_map_register::n_PS_maps> ps_map_list = { false };
		////depth_stencil_params depths_params; 
		//D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		//std::pair<bool, sampler_params> smplr_params = { false, {.sampler_filter = D3D11_FILTER_ANISOTROPIC } };
		//std::pair<bool, blender_params> blend_params = { false, {.blend_enabled = false } };
		//std::pair<bool, ds_state_params> ds_params = { false, {.depth_enabled = true, .s_mode = stencil_mode::off, .ref_val = 1u} };
		//std::string PS;
		//std::string VS;
		//std::vector<texture_params> tex_params;
		fullscreen_blender_description b_desc;
		b_desc.smplr_params.first = true;
		b_desc.smplr_params.second = { .sampler_filter = D3D11_FILTER_ANISOTROPIC };
		b_desc.blend_params.first = true;
		b_desc.blend_params.second = { .blend_enabled = true };
		b_desc.ds_params.first = true;
		b_desc.ds_params.second = { .depth_enabled = false, .s_mode = stencil_mode::mask, .ref_val = 0u };
		b_desc.PS = "PS_FullBlend.cso";
		b_desc.VS = "VS_FullBlend.cso";
		return b_desc;
	}
	static drawable_description create_line_desc(float x0, float y0, float z0, float x1, float y1, float z1)
	{

		drawable_description d_desc;

		d_desc.blend_params.first = false;
		d_desc.smplr_params.first = false;
		d_desc.raster_params.first = false;

		d_desc.has_indices = false;
		d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::obj_color] = true;
		d_desc.scale = 1.0f;
		d_desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		d_desc.vertex_info[vertex_layout::position3f] = true;
		d_desc.ds_params.first = true;
		d_desc.ds_params.second.depth_enabled = true;
		d_desc.ds_params.second.ref_val = 1.0f;
		d_desc.ds_params.second.s_mode = stencil_mode::off;
		vertex_layout vl;
		vl.append(vertex_layout::position3f);
		d_desc.vbuf = { vl };
		d_desc.vbuf.emplace_back(DirectX::XMFLOAT3(x0, y0, z0));
		d_desc.vbuf.emplace_back(DirectX::XMFLOAT3(x1, y1, z1));
		return d_desc;
	}
	static std::vector<drawable_description> create_stage_descs(const drawable_description& default_desc, technique t )
	{
		std::vector<drawable_description> d_descs;
		switch (t)
		{
		case technique::outline:
		{
			d_descs.resize(2);
			d_descs[0].blend_params.first = false;
			d_descs[0].raster_params.first = false;
			d_descs[0].smplr_params.first = false;
			d_descs[0].scale = default_desc.scale;
			d_descs[0].vertex_info[vertex_layout::position3f] = true;
			d_descs[0].has_indices = default_desc.has_indices;
			d_descs[0].ps_map_list; // just drawing the mask, we dont need anything 
			d_descs[0].ps_cbuf_list;
			d_descs[0].vs_cbuf_list;
			d_descs[0].ds_params.first = true;
			d_descs[0].ds_params.second = { .depth_enabled = true, .s_mode = stencil_mode::write, .ref_val = 1u };
							
			d_descs[1].blend_params.first = false;
			d_descs[1].raster_params.first = false;
			d_descs[1].smplr_params.first = false;
			d_descs[1].scale = default_desc.scale;
			d_descs[1].vertex_info[vertex_layout::position3f] = true;
			d_descs[1].has_indices = default_desc.has_indices;
			d_descs[1].ps_map_list;
			d_descs[1].vs_cbuf_list;
			d_descs[1].ps_cbuf_list[(size_t)PS_cbuf_register::obj_color] = true;
			d_descs[1].vs_cbuf_list; // we dont want anything (***unless it affects the positions, which none do right now, so I might need to change this later) 
			d_descs[1].ds_params.first = true;
			d_descs[1].ds_params.second = { .depth_enabled = false, .s_mode = stencil_mode::off, .ref_val = 1u }; // just write to offscreen *** (not sure if I should have depth on here) 

		}
		}
		return d_descs;
	}
	//TODO: make similar to this but for the fullsceren blend 
	static std::vector<std::shared_ptr<dx_component>> create_stage_comps(graphics& gfx, codex& cdx, drawable& parent, drawable_description d_desc, std::unordered_map<PS_cbuf_register, cbuf_state>& ps_cbufs, std::unordered_map<VS_cbuf_register, cbuf_state>& vs_cbufs, std::string id)
	{
		std::vector<std::shared_ptr<dx_component>> comps;
		if (d_desc.blend_params.first)
			comps.emplace_back(cdx.resolve<blender>(gfx, d_desc.blend_params.second));
		if (d_desc.raster_params.first)
			comps.emplace_back(cdx.resolve<rasterizer>(gfx, d_desc.raster_params.second));
		if (d_desc.smplr_params.first)
			comps.emplace_back(cdx.resolve<sampler>(gfx, d_desc.smplr_params.second));
		if (d_desc.ds_params.first)
			comps.emplace_back(cdx.resolve<depth_stencil_state>(gfx, d_desc.ds_params.second));
		for (const auto& t : d_desc.tex_params)
		{
			comps.emplace_back(cdx.resolve<texture>(gfx, t));
		}
		if (d_desc.vertex_info[vertex_layout::normal3f] || d_desc.ps_map_list[(size_t)PS_map_register::normal_m])
		{
			comps.emplace_back(cdx.resolve<vertex_shader>(gfx, "VS_Phong" + get_name_ex_vs(d_desc.vertex_info, d_desc.vs_cbuf_list)));
			comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Phong" + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, d_desc.vertex_info)));
		}
		else
		{
			comps.emplace_back(cdx.resolve<vertex_shader>(gfx, "VS_Solid" + get_name_ex_vs(d_desc.vertex_info, d_desc.vs_cbuf_list)));
			comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Solid" + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, {})));
		}
		
		// I expect that you can have more veretex info components in the vertex buffer than the shader accepts. At least I hope so 
		for (int i = 0; i < (size_t)PS_cbuf_register::n_PS_cbuf; i++)
		{
			if (d_desc.ps_cbuf_list[i])
			{
				ps_cbufs[(PS_cbuf_register)i] = cbuf_state(create_ps_cbuf((PS_cbuf_register)i));
				comps.emplace_back(cdx.resolve_by_id<PS_dynamic_cbuf>(std::string(parent.get_name() + "#ps_cbuf#" + id + "#" + std::to_string(i) + "#"), gfx, ps_cbufs[(PS_cbuf_register)i], i));
			}
		}
		for (int i = 0; i < (size_t)VS_cbuf_register::n_VS_cbuf; i++) // maybe get rid of arrays all together? *** 
		{
			if (d_desc.vs_cbuf_list[i])
			{
				vs_cbufs[(VS_cbuf_register)i] = cbuf_state(create_vs_cbuf((VS_cbuf_register)i));
				comps.emplace_back(cdx.resolve_by_id<VS_dynamic_cbuf>(std::string(parent.get_name() + "#vs_cbuf#" + id + "#" + std::to_string(i) + "#"), gfx, vs_cbufs[(VS_cbuf_register)i], i));
			}
		}
		return comps;

	}
}