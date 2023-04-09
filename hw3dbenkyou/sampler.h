#pragma once
#include "dx_component.h"

struct sampler_params
{
	D3D11_FILTER sampler_filter = D3D11_FILTER_ANISOTROPIC;
};

class sampler : public dx_component
{
public:
	sampler(D3D11_FILTER sampler_filter, graphics& gfx);
	sampler(const sampler_params& params, graphics& gfx);
	void set(graphics& gfx) override;
	static std::string get_id(D3D11_FILTER sampler_filer);
	static std::string get_id(const sampler_params& params);
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> p_sampler;
};