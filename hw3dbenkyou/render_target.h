#pragma once
#include "dx_component.h"


struct render_target_params
{
	unsigned int width;
	unsigned int height; 
};

class depth_stencil_buffer;


class render_target : public dx_component
{
public:
	render_target(graphics& gfx, unsigned int width, unsigned int height);
	render_target(graphics& gfx, const render_target_params& params);
	//sets the render target as a texture resource to pixel shader
	void set(graphics& gfx) override;
	void set_as_texture(graphics& gfx);
	void set_as_target(graphics& gfx);
	void set_as_target(graphics& gfx, ID3D11DepthStencilView* p_ds_view);
	void clear(graphics& gfx, DirectX::XMFLOAT4 color) const noexcept;
private:
	unsigned int width;
	unsigned int height; 
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_target_view = nullptr; // view as an output 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_resource_view = nullptr; // a view as an input, we can read this in our shaders (which is required for the blur effect for example) 
};
