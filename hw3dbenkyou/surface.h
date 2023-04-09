#pragma once
#include <objidl.h>
#include <iostream>
#include <memory>
#include "exeption.h"
#include "directx_tex/include/DirectXTex.h"




class surface
{
public:
	class color
	{
	public:
		constexpr color(const color& other)
			:
			c(c)
		{}
		constexpr color(unsigned int c)
			:
			c(c)
		{}
		constexpr color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			c = (b << 24) | (g << 16) | (r << 8) | a;
		}
		constexpr color(unsigned char r, unsigned char g, unsigned char b)
		{
			c = c = (b << 24) | (g << 16) | (r << 8) | 0xFFu;
		}
		unsigned char b()
		{
			return (c >> 24u) & 0xFFu;
		}
		unsigned char g()
		{
			return (c >> 16u) & 0xFFu;
		}
		unsigned char r()
		{
			return (c >> 8u) & 0xFFu;
		}
		unsigned char a()
		{
			return c & 0xFFu;
		}
	private:
		unsigned int c; 
	};
private:
	class shippai : public windows_sdk_shippai
	{
	public:
		shippai(int line, const char* file, HRESULT hr)
			:
			windows_sdk_shippai(line, file, hr)
		{}
	private:
	};
public:
	surface(const std::string& file);
	~surface();
	surface(const surface& other);
	surface(surface&& other) noexcept;
	surface& operator=(const surface& other) noexcept;
	surface& operator=(surface&& other) noexcept;
	static surface make_from_file(const std::string& file_path);
	UINT get_width() const;
	UINT get_height() const;
	std::string get_path() const;
	bool get_has_alpha() const;
	surface::color* get_buffer_ptr();
	const surface::color* get_buffer_ptr() const;
private:
	surface(DirectX::ScratchImage img, const std::string& path) noexcept;
private:
	static constexpr DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	std::string path = "unkown_path";
	DirectX::ScratchImage scratch_img; 
};