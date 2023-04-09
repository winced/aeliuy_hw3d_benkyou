#pragma once
#include "dx_component.h"


class viewport : public dx_component
{
public:
	viewport(D3D11_VIEWPORT viewport_info);
	viewport(int width, int height, int min_depth, int max_depth, int topleft_x, int topleft_y);
	void set(graphics& gfx) override;
private:
	D3D11_VIEWPORT viewport_info;
};


