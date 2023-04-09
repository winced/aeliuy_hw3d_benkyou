#include "texture.h"
#include "surface.h"


texture::texture(const surface& s, UINT slot, graphics& gfx)
	:
	slot(slot),
	alpha_state(s.get_has_alpha())
{
	HRESULT hr;
	// texture descriptor 
	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = s.get_width();
	texture_desc.Height = s.get_height();
	//set mip levels to 0, so we can maximum mip levels 
	texture_desc.MipLevels = 0;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//anti aliasing
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	//bind it as render target so the gpu can write to it (when generating mip levels) 
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture_desc.CPUAccessFlags = 0;
	//tell the gpu to generate mip levels for us 
	texture_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//data for texture 
	//with mip mapping, you cannot intialize the texture with just one subresource data, since if you initialize it sets the array of data 
	//and with mip mapping you need more than one element in the array, so when you initialize it with just one, it shits the bed

	// so we pass in a nullptr as the data and define it later 
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_texture;
	if ((hr = get_device(gfx)->CreateTexture2D(&texture_desc, nullptr, &p_texture)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	//so instead of forcing it to one subresource data, we update the data in slot 0 to our texture 
	get_context(gfx)->UpdateSubresource(p_texture.Get(), 0u, nullptr, s.get_buffer_ptr(), s.get_width() * sizeof(surface::color), 0u);

	/*create view for the texture, which can be bound to the pipeline*/
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	view_desc.Format = texture_desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	// put mip levels to -1 so the gpu figures it out itself (so it will probably use all the mip levels) 
	view_desc.Texture2D.MipLevels = -1;
	if ((hr = get_device(gfx)->CreateShaderResourceView(p_texture.Get(), &view_desc, &p_texture_view)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	//tell the gpu to generate the mips for our texture view 
	//I feel like these views are just a way of accesing the texture they are refrencing. A middle-man between the texture and the gpu 
	get_context(gfx)->GenerateMips(p_texture_view.Get());

}

texture::texture(const std::string& file_name, UINT slot, graphics& gfx)
	:
	slot(slot)
{
	surface s = surface::make_from_file(file_name);
	*this = { s, slot, gfx };
}

texture::texture(const texture_params& params, graphics& gfx)
	:
	slot(params.slot)
{
	surface s = surface::make_from_file(params.tex_path);
	*this = { s, slot, gfx };
}

void texture::set(graphics& gfx)
{
	get_context(gfx)->PSSetShaderResources(slot, 1u, p_texture_view.GetAddressOf());
}

std::string texture::get_id(const surface& s, UINT slot)
{
	return "texture#" + std::to_string(slot) + "#" + s.get_path();
}

std::string texture::get_id(const std::string& file_name, UINT slot)
{
	return "texture#" + std::to_string(slot) + "#" + file_name;
}

std::string texture::get_id(const texture_params& params)
{
	return get_id(params.tex_path, params.slot);
}

bool texture::has_alpha() const
{
	return alpha_state;
}
