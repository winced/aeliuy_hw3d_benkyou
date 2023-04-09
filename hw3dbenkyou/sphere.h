#pragma once
#include "drawable.h"


class sphere : public drawable
{
public:
	sphere(float radius, const std::string& id, DirectX::XMFLOAT4 sphere_col, bool light_source, codex& cdx, graphics& gfx);
	DirectX::XMMATRIX get_transform_m() const noexcept override;
	void set_pos(const DirectX::XMFLOAT3& new_pos);
private:
	DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
};
