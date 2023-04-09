#include "vertex_buffer.h"



vertex_buffer::vertex_buffer(const element_vertex_buffer& v_buf, graphics& gfx)
	:
	stride(v_buf.get_layout().size())
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = v_buf.size_in_bytes();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA resource_data = {};
	resource_data.pSysMem = v_buf.get_data();
	if ((hr = get_device(gfx)->CreateBuffer(&bd, &resource_data, &p_vertex_buffer)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}
void vertex_buffer::set(graphics& gfx)
{
	const UINT offset = 0u;
	get_context(gfx)->IASetVertexBuffers(0u, 1u, p_vertex_buffer.GetAddressOf(), &stride, &offset);
}
