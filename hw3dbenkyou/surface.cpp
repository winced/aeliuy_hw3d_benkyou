#include "surface.h"
#include "basic_utility.h"

surface::surface(const std::string& file)
	:
	path(file)
{
	*this = surface::make_from_file(file);
}

surface::~surface()
{
	scratch_img.Release();
}

surface::surface(const surface& other)
{
	*this = other;
}

surface::surface(DirectX::ScratchImage img, const std::string& path) noexcept
	:
	scratch_img(std::move(img)),
	path(path)
{}

surface::surface(surface&& other) noexcept
	:
	scratch_img(std::move(other.scratch_img))
{
}

surface& surface::operator=(const surface& other) noexcept
{
	DirectX::Rect r = { 0, 0, other.get_width(), other.get_height() };
	DirectX::CopyRectangle(*other.scratch_img.GetImage(0, 0, 0), r, *(this->scratch_img.GetImage(0, 0, 0)), DirectX::TEX_FILTER_DEFAULT, 0, 0);
	return *this;
}

surface& surface::operator=(surface&& other) noexcept 
{
	scratch_img = std::move(other.scratch_img);
	return *this;
}

surface surface::make_from_file(const std::string& file_path)
{
	DirectX::ScratchImage scratch;
	HRESULT hr; 
	if ((hr = DirectX::LoadFromWICFile(convert_multibyte_to_wide(file_path).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratch)) != S_OK)
		throw surface::shippai(__LINE__, __FILE__, hr);
	if (scratch.GetImage(0, 0, 0)->format != format)
	{
		DirectX::ScratchImage converted_scratch;
		if((hr = DirectX::Convert(*scratch.GetImage(0,0,0), format, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, converted_scratch)) != S_OK)
			throw surface::shippai(__LINE__, __FILE__, hr);
		return surface(std::move(converted_scratch), file_path);
	}
	return surface(std::move(scratch), file_path);
}


UINT surface::get_width() const
{
	return scratch_img.GetImage(0,0,0)->width;
}

UINT surface::get_height() const
{
	return scratch_img.GetImage(0, 0, 0)->height;
}

std::string surface::get_path() const
{
	return path;
}

bool surface::get_has_alpha() const
{
	return !scratch_img.IsAlphaAllOpaque();
}

surface::color* surface::get_buffer_ptr()
{
	return reinterpret_cast<surface::color*>(scratch_img.GetPixels());

}

const surface::color* surface::get_buffer_ptr() const
{
	return const_cast<surface*>(this)->get_buffer_ptr();
}

