#pragma once
#include "dx_component.h"

struct texture_params
{
	std::string tex_path;
	UINT slot;
};

class texture : public dx_component
{
public:
	texture(const class surface& s, UINT slot, graphics& gfx);
	texture(const std::string& file_name, UINT slot, graphics& gfx);
	texture(const texture_params& params, graphics& gfx);
	void set(graphics& gfx) override;
	static std::string get_id(const surface& s, UINT slot);
	static std::string get_id(const std::string& file_name, UINT slot);
	static std::string get_id(const texture_params& params);
	bool has_alpha() const;
private:
	bool alpha_state;
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_texture_view;
};