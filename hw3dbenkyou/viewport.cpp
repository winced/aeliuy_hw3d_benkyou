#include "viewport.h"

viewport::viewport(D3D11_VIEWPORT viewport_info)
	:
	viewport_info(viewport_info)
{
}

viewport::viewport(int width, int height, int min_depth, int max_depth, int topleft_x, int topleft_y)
{
	viewport_info.Width = width;
	viewport_info.Height = height;
	viewport_info.MinDepth = min_depth;
	viewport_info.MaxDepth = max_depth;
	viewport_info.TopLeftX = topleft_x;
	viewport_info.TopLeftY = topleft_y;
}

void viewport::set(graphics& gfx)
{
	get_context(gfx)->RSSetViewports(1u, &viewport_info);
}
