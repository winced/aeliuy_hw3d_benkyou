#include "render_target.h"
#include "depth_stencil_buffer.h"


render_target::render_target(graphics& gfx, unsigned int width, unsigned int height)
	:
	width(width),
	height(height)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; 
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_tex;

	if ((hr = get_device(gfx)->CreateTexture2D(&tex_desc, nullptr, &p_tex)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	// create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
	rtv_desc.Format = tex_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	if ((hr = get_device(gfx)->CreateRenderTargetView(p_tex.Get(), &rtv_desc, &p_target_view) )!= S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	D3D11_SHADER_RESOURCE_VIEW_DESC rtv_resource_view_desc = {};
	rtv_resource_view_desc.Format = tex_desc.Format;
	rtv_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rtv_resource_view_desc.Texture2D.MipLevels = 1u;
	rtv_resource_view_desc.Texture2D.MostDetailedMip = 0u; 

	if ((hr = get_device(gfx)->CreateShaderResourceView(p_tex.Get(), &rtv_resource_view_desc, &p_resource_view)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

render_target::render_target(graphics& gfx, const render_target_params& params)
{
	*this = render_target(gfx, params.width, params.height);
}

void render_target::set(graphics& gfx)
{
	get_context(gfx)->PSSetShaderResources((UINT)PS_map_register::offscreen_m, 1u, p_resource_view.GetAddressOf());
}

void render_target::set_as_texture(graphics& gfx)
{
	get_context(gfx)->PSSetShaderResources((UINT)PS_map_register::offscreen_m, 1u, p_resource_view.GetAddressOf());
}

void render_target::set_as_target(graphics& gfx, ID3D11DepthStencilView* p_ds_view)
{
	get_context(gfx)->OMSetRenderTargets(1u, p_target_view.GetAddressOf(), p_ds_view);
}

void render_target::set_as_target(graphics& gfx)
{
	get_context(gfx)->OMSetRenderTargets(1u, p_target_view.GetAddressOf(), nullptr);
}

void render_target::clear(graphics& gfx, DirectX::XMFLOAT4 color) const noexcept
{
	const float color_arr[] = { color.x, color.y, color.z, color.w };
	get_context(gfx)->ClearRenderTargetView(p_target_view.Get(), color_arr);
}



