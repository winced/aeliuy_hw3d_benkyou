#pragma once
#include "dx_component.h"


class vertex_shader : public dx_component
{
public:
	vertex_shader(const std::string& path, graphics& gfx);
	vertex_shader(ID3DBlob* plob, graphics& gfx);
	void set(graphics& gfx);
	ID3DBlob* get_blobp();
	static std::string get_id(const std::string& path);
private:
	Microsoft::WRL::ComPtr<ID3DBlob> p_blob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_vertex_shader;
};

