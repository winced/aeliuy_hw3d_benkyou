#pragma once
#include "better_windows.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class vertex_layout
{
public:
	enum type
		{
			position3f,
			position2f,
			color3f,
			color4f,
			normal3f,
			texture2f,
			tangent3f,
			bitangent3f,
			n_vertex_types
		};
	template<type> struct Map;
	template<> struct Map<position2f>
		{
			using system_type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "POSITION";
		};
	template<> struct Map<position3f>
		{
			using system_type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "POSITION";
		};
	template<> struct Map<color3f>
		{
			using system_type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "COLOR";
		};
	template<> struct Map<color4f>
		{
			using system_type = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "COLOR";
		};
	template<> struct Map<normal3f>
		{
			using system_type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "NORMAL";
		};
	template<> struct Map<texture2f>
		{
			using system_type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXTURE";
		};
	template<> struct Map<tangent3f>
		{
			using system_type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "TANGENT";
		};
	template<> struct Map<bitangent3f>
		{
			using system_type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "BITANGENT";
		};
	class element
	{
	public:
		element(size_t offset, vertex_layout::type t)
			:
			offset(offset),
			element_type(t)
		{}
		static constexpr size_t type_size(vertex_layout::type t)
		{
			if (t == vertex_layout::type::position3f)
			{
				return sizeof(DirectX::XMFLOAT3);
			}
			else if (t == vertex_layout::type::position2f)
			{
				return sizeof(DirectX::XMFLOAT2);
			}
			else if (t == vertex_layout::type::color3f)
			{
				return sizeof(DirectX::XMFLOAT3);
			}
			else if (t == vertex_layout::type::color4f)
			{
				return sizeof(DirectX::XMFLOAT4);
			}
			else if (t == vertex_layout::type::normal3f)
			{
				return sizeof(DirectX::XMFLOAT3);
			}
			else if (t == vertex_layout::type::texture2f)
			{
				return sizeof(DirectX::XMFLOAT2);
			}
			else if (t == vertex_layout::type::tangent3f)
			{
				return sizeof(DirectX::XMFLOAT3);
			}
			else if (t == vertex_layout::type::bitangent3f)
			{
				return sizeof(DirectX::XMFLOAT3);
			}
		}
		vertex_layout::type get_type() const
			{
				return element_type;
			}
		size_t get_size() const
		{
			return type_size(get_type());
		}
		size_t get_next_offset() const
			{
				return offset + type_size(element_type);
			}
		size_t get_offset() const
			{
				return offset;
			}
		D3D11_INPUT_ELEMENT_DESC get_desc() const
			{
				switch (element_type)
				{
				case position2f:
					return GenerateDesc<position2f>(get_offset());
				case position3f:
					return GenerateDesc<position3f>(get_offset());
				case color4f:
					return GenerateDesc<color4f>(get_offset());
				case color3f:
					return GenerateDesc<color3f>(get_offset());
				case normal3f:
					return GenerateDesc<normal3f>(get_offset());
				case texture2f:
					return GenerateDesc<texture2f>(get_offset());
				case tangent3f:
					return GenerateDesc<tangent3f>(get_offset());
				case bitangent3f:
					return GenerateDesc<bitangent3f>(get_offset());
				}
			}
	private:
		template<vertex_layout::type type>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(UINT offset)
			{
				return { Map<type>::semantic,0,Map<type>::dxgiFormat,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		size_t offset;
		vertex_layout::type element_type;
	};
public:
	size_t size() const
	{
		return elements.empty() ? 0u : elements.back().get_next_offset();
	}
	size_t get_element_count() const
	{
		return elements.size();
	}
	element resolve(vertex_layout::type t) const
	{
		for (const auto& e : elements)
		{
			if (e.get_type() == t)
				return e;
		}
	}
	element resolve_by_index(size_t index) const
	{
		return elements[index];
	}
	vertex_layout& append(vertex_layout::type type)
		{
			elements.push_back(element(size(), type));
			return *this;
		}
	std::vector<D3D11_INPUT_ELEMENT_DESC> get_D3D11_layout() const 
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(get_element_count());
		for (const auto& e : elements)
		{
			desc.push_back(e.get_desc());
		}
		return desc;
	}
private:
	std::vector<element> elements;
};

class vertex
{
	friend class element_vertex_buffer;
public:
	template<vertex_layout::type et>
	auto& element()
	{
		auto p_element = p_vertex_info + layout_info.resolve(et).get_offset();
		return *reinterpret_cast<vertex_layout::Map<et>::system_type*>(p_element);
	}
	template<typename T>
	void set_by_index(size_t index, T&& val)
	{
		const auto& e = layout_info.resolve_by_index(index);
		auto p_vertex_element = p_vertex_info + e.get_offset();
		switch (e.get_type())
		{
		case vertex_layout::type::position2f:
			set_element<vertex_layout::type::position2f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::position3f:
			set_element<vertex_layout::type::position3f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::normal3f:
			set_element<vertex_layout::type::normal3f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::color3f:
			set_element<vertex_layout::type::color3f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::color4f:
			set_element<vertex_layout::type::color4f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::texture2f:
			set_element<vertex_layout::type::texture2f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::tangent3f:
			set_element<vertex_layout::type::tangent3f>(p_vertex_element, std::forward<T>(val));
			break;
		case vertex_layout::type::bitangent3f:
			set_element<vertex_layout::type::bitangent3f>(p_vertex_element, std::forward<T>(val));
			break;
		}
	}
protected:
	vertex(const vertex_layout& layout_info, char* p_vertex_add)
		:
		layout_info(layout_info),
		p_vertex_info(p_vertex_add)
	{}
private:
	template<typename first, typename... rest>
	void set_by_index(size_t index, first&& first_v, rest&&... rest_v)
	{
		set_by_index(index, std::forward<first>(first_v));
		set_by_index(index + 1, std::forward<rest>(rest_v)...);
	}
	template<vertex_layout::type DestLayoutType, typename SrcType>
	void set_element(char* pAttribute, SrcType&& val)
	{
		using Dest = typename vertex_layout::Map<DestLayoutType>::system_type;
		if constexpr (std::is_assignable<Dest, SrcType>::value)
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			assert("Parameter attribute type mismatch" && false);
		}
	}
private:
	const vertex_layout& layout_info;
	char* p_vertex_info;
};

class element_vertex_buffer
{
public:
	element_vertex_buffer() = default; 
	element_vertex_buffer(vertex_layout v_l)
		:
		layout_info(std::move(v_l))
	{}
	vertex operator[](size_t n)
	{
		return vertex(layout_info, v_buffer.data() + n * layout_info.size());
	}
	size_t size() const
	{
		return v_buffer.size() / layout_info.size();
	}
	size_t size_in_bytes() const
	{
		return v_buffer.size();
	}
	const char* get_data() const
	{
		return v_buffer.data();
	}
	template<typename... args>
	void emplace_back(args... vertex_info)
	{
		v_buffer.resize(v_buffer.size() + layout_info.size());
		back().set_by_index(0u, std::forward<args>(vertex_info)...);
	}
	template<typename single_v>
	void emplace_back_single(size_t index, single_v&& vert)
	{
		if(index == 0)
			v_buffer.resize(v_buffer.size() + layout_info.size());
		back().set_by_index(index, std::forward<single_v>(vert));
	}
	vertex back()
	{
		return vertex(layout_info, v_buffer.data() + v_buffer.size() - layout_info.size());
	}
	vertex front()
	{
		return vertex(layout_info, v_buffer.data());
	}
	const vertex_layout& get_layout() const
	{
		return layout_info;
	}
private:
	std::vector<char> v_buffer;
	vertex_layout layout_info;
};

