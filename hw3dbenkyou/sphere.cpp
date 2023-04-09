#include "sphere.h"
#include "transform_constant_buffer.h"

sphere::sphere(float radius, const std::string& id, DirectX::XMFLOAT4 sphere_col, bool light_source, codex& cdx, graphics& gfx)
{
	HRESULT hr;
	d_desc.blend_params.first  = false;
	d_desc.raster_params.first = false;
	d_desc.smplr_params.first = false;
	d_desc.ds_params.first = true;
	d_desc.has_indices = true;
	d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::obj_color] = true;
	d_desc.scale = 1.0f;
	d_desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d_desc.vertex_info[vertex_layout::type::position3f] = true;

	auto& normal_comps = owned_techs[technique::normal].get_stage_comps(0u);
	auto& normal_ps = owned_techs[technique::normal].get_stage_ps(0u);

	normal_ps[PS_cbuf_register::obj_color].get_buffer().add<dc_buf::type::float4>("color")(DirectX::XMFLOAT4{sphere_col});
	normal_comps.emplace_back(
		cdx.resolve_by_id<PS_dynamic_cbuf>("obj_color_cbuf#" + id,gfx, normal_ps[PS_cbuf_register::obj_color], (size_t)PS_cbuf_register::obj_color));

	std::vector<D3D11_INPUT_ELEMENT_DESC> input_descriptions;
	std::string shader_type; 
	if (light_source)
	{
		shader_type = "Solid";
		struct vertex
		{
			DirectX::XMFLOAT3 pos;
		};
		const auto sphere_model = basic_geometry<vertex>::make_sphere(radius, 16, 32);
		input_descriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		add_vertex_buffer(cdx.resolve_by_id<vertex_buffer>("vertex_buffer#" + id, gfx, sphere_model.get_vertices()));
		add_index_buffer(cdx.resolve_by_id<index_buffer>("index_buffer#" + id, gfx, sphere_model.get_indicies()));
	}
	else
	{
		shader_type = "Phong";
		d_desc.vertex_info[vertex_layout::type::normal3f] = true;
		struct vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal; 
		};
		d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::specular] = true;
		normal_ps[PS_cbuf_register::specular].get_buffer().add<dc_buf::type::float1>("specular")(0.8f);
		normal_ps[PS_cbuf_register::specular].get_buffer().add<dc_buf::type::float1>("power")(2.0f); 
		normal_comps.emplace_back(cdx.resolve_by_id<PS_dynamic_cbuf>("specular_cbuf#" + id, gfx, normal_ps[PS_cbuf_register::specular], (size_t)PS_cbuf_register::specular));
		const auto sphere_model = basic_geometry<vertex>::make_sphere_normals(radius, 16, 32);
		input_descriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		add_vertex_buffer(cdx.resolve_by_id<vertex_buffer>("vertex_buffer#" + id, gfx, sphere_model.get_vertices()));
		add_index_buffer(cdx.resolve_by_id<index_buffer>("index_buffer#" + id, gfx, sphere_model.get_indicies()));

	}
	//shaders
	if(light_source)
		normal_comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Solid__C_.cso")); // the pixel shader doesnt take in position because its not using phong shading
	else
		normal_comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_" + shader_type + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, d_desc.vertex_info)));
	
	auto vs_light = cdx.resolve<vertex_shader>(gfx, "VS_" + shader_type + get_name_ex_vs(d_desc.vertex_info));
	add_input_layout(cdx.resolve<input_layout>(gfx, input_descriptions, reinterpret_cast<vertex_shader*>(vs_light.get())->get_blobp()));
	normal_comps.emplace_back(std::move(vs_light));
	normal_comps.emplace_back(cdx.resolve<depth_stencil_state>(gfx, d_desc.ds_params.second));

	add_primitive_topology(cdx.resolve<primitive_topology>(gfx, d_desc.topology));
	add_transform_cbuf(cdx.resolve_by_id<transform_c_buf>("transform_cbuffer#" + id, gfx, *this));
}

DirectX::XMMATRIX sphere::get_transform_m() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}


void sphere::set_pos(const DirectX::XMFLOAT3& new_pos)
{
	this->pos = new_pos;
}

