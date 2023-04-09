#include "rasterizer.h"




rasterizer::rasterizer(bool disable_backface_culling, graphics& gfx)
{
	D3D11_RASTERIZER_DESC raster_desc = {};
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.CullMode = disable_backface_culling ? D3D11_CULL_NONE : D3D11_CULL_BACK;
	raster_desc.FrontCounterClockwise = false;
	//changes draw order to favor those with a larger z (for better working shadows) 
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0; 
	raster_desc.SlopeScaledDepthBias = 0; 
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	get_device(gfx)->CreateRasterizerState(&raster_desc, &p_rasterizor_state);
}

rasterizer::rasterizer(const rasterizer_params& params, graphics& gfx)
{
	*this = { params.disable_backface_culling, gfx };
}

void rasterizer::set(graphics& gfx)
{
	get_context(gfx)->RSSetState(p_rasterizor_state.Get());
}


std::string rasterizer::get_id(bool disable_backface_culling)
{
	return "rasterizer#" + std::to_string(disable_backface_culling);
}

std::string rasterizer::get_id(const rasterizer_params& params)
{
	return get_id(params.disable_backface_culling);
}
