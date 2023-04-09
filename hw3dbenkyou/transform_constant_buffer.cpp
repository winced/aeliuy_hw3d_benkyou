#include "transform_constant_buffer.h"


transform_c_buf::transform_c_buf(const drawable& parent, graphics& gfx)
	:
	parent(parent)
{
	if (!vcbuf_p)
	{
		vcbuf_p = std::make_unique<vertex_constant_buffer<model_projection_mat>>(0u, gfx);
	}
}

void transform_c_buf::set(graphics& gfx) noexcept
{
	model_projection_mat mat;
	mat.model = DirectX::XMMatrixTranspose(parent.get_transform_m());
	mat.project = DirectX::XMMatrixTranspose(parent.get_transform_m() * gfx.get_camera() * gfx.get_projection());
	vcbuf_p->update(std::move(mat), gfx);
	vcbuf_p->set(gfx);
}


std::unique_ptr<vertex_constant_buffer<model_projection_mat>> transform_c_buf::vcbuf_p;