#pragma once
#include <DirectXMath.h>
#include "constant_buffers.h"
#include "drawable.h"

struct model_projection_mat
{
	DirectX::XMMATRIX model;
	DirectX::XMMATRIX project;
};
class transform_c_buf : public dx_component
{
public:
	transform_c_buf(const drawable& parent, graphics& gfx);
	void set(graphics& gfx) noexcept override;
private:
	static std::unique_ptr<vertex_constant_buffer<model_projection_mat>> vcbuf_p;
	const drawable& parent;
};