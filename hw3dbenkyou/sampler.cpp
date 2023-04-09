#include "sampler.h"

sampler::sampler(D3D11_FILTER sampler_filter, graphics& gfx)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC sampler_desc = {};
	//D3D11_FILTER_MIN_MAG_MIP_LINEAR is just linear sampling from the textrue to the destiation 
	//anistrophic fixes the issue when one destination dimesion is smaller than the other and linear sampling fucks up 
	sampler_desc.Filter = sampler_filter;
	sampler_desc.MinLOD = 0u;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	//three addresses for three dimensional texturing, defines how the texture is processed if texture coords (uv coords) go beyond the texture (eg. cord < 0.0f | cord > 1.0f)
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if ((hr = get_device(gfx)->CreateSamplerState(&sampler_desc, &p_sampler)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

sampler::sampler(const sampler_params& params, graphics& gfx)
{
	*this = { params.sampler_filter, gfx};
}

void sampler::set(graphics& gfx)
{
	// start slot correspondes directly to the slots on the shader side
	// we bind it to slot 0 
	get_context(gfx)->PSSetSamplers(0, 1, p_sampler.GetAddressOf());
}

std::string sampler::get_id(D3D11_FILTER sampler_filer)
{
	return "sampler#" + std::to_string((int)sampler_filer);
}

std::string sampler::get_id(const sampler_params& params)
{
	return get_id(params.sampler_filter);
}
