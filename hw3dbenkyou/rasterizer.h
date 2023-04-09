#pragma once
#include "dx_component.h"


struct rasterizer_params
{
	bool disable_backface_culling = false;
};

class rasterizer : public dx_component
{
public:
	rasterizer(bool disable_backface_culling, graphics& gfx);
	rasterizer(const rasterizer_params& params, graphics& gfx);
	void set(graphics& gfx) override;
	static std::string get_id(bool disable_backface_culling);
	static std::string get_id(const rasterizer_params& params);
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> p_rasterizor_state; 
};