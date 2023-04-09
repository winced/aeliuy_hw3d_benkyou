#pragma once
#include "dx_component.h"
#include "vertex.h"


class vertex_buffer : public dx_component
{
public:
	template<typename V>
	vertex_buffer(const std::vector<V>& vertices, graphics& gfx)
		:
		stride(sizeof(V))
	{
		HRESULT hr;
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = stride * vertices.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA resource_data = {};
		resource_data.pSysMem = vertices.data();
		if ((hr = get_device(gfx)->CreateBuffer(&bd, &resource_data, &p_vertex_buffer)) != S_OK)
			throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	}
	vertex_buffer(const element_vertex_buffer& v_buf, graphics& gfx);
	void set(graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_vertex_buffer;
	UINT stride;
};