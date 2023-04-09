#include "vertex_shader.h"
#include "basic_utility.h"
#include <d3dcompiler.h>

vertex_shader::vertex_shader(const std::string& path, graphics& gfx)
{
	HRESULT hr;
	if ((hr = D3DReadFileToBlob(convert_multibyte_to_wide(path).c_str(), &p_blob)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	if ((hr = get_device(gfx)->CreateVertexShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_vertex_shader)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

vertex_shader::vertex_shader(ID3DBlob* plob, graphics& gfx)
{
	HRESULT hr;
	p_blob = plob;
	if ((hr = get_device(gfx)->CreateVertexShader(plob->GetBufferPointer(), plob->GetBufferSize(), nullptr, &p_vertex_shader)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

void vertex_shader::set(graphics& gfx)
{
	get_context(gfx)->VSSetShader(p_vertex_shader.Get(), nullptr, 0u);
}

ID3DBlob* vertex_shader::get_blobp()
{
	return p_blob.Get();
}

std::string vertex_shader::get_id(const std::string& path)
{
	return "vertex_shader#" + path;
}
