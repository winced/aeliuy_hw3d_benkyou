#pragma once
#include <algorithm>
#include <array>
#include <exception>
#include "shader_component_register.h"




enum class tech_stage {
	null,
	normal,
	write_mask,
	render_offscreen,
	render_from_offscreen_blend,
	n_stages
};

enum class technique {
	normal,
	outline,
	n_tech
};
enum class rt_stage {
	texture,
	target,
	off
};


struct stage_descriptor
{
	bool drawables = false;
	bool blender = false;
	rt_stage offscreen = rt_stage::off;
	rt_stage swap_buffer = rt_stage::target;
};


static constexpr stage_descriptor get_stage_descriptor(tech_stage s)
{
	switch (s)
	{
	case tech_stage::null:
		break;
	case tech_stage::normal:
		return { .drawables = true, .blender = false, .offscreen = rt_stage::off, .swap_buffer = rt_stage::target };
		break;
	case tech_stage::write_mask:
		return { .drawables = true, .blender = false, .offscreen = rt_stage::off, .swap_buffer = rt_stage::off };
		break;
	case tech_stage::render_offscreen:
		return { .drawables = true, .blender = false, .offscreen = rt_stage::target, .swap_buffer = rt_stage::off };
		break;
	case tech_stage::render_from_offscreen_blend:
		return { .drawables = true, .blender = true, .offscreen = rt_stage::texture, .swap_buffer = rt_stage::target };
		break;
	case tech_stage::n_stages:
		break;
	default:
		break;
	}
	return stage_descriptor();
}

static constexpr std::array<tech_stage, (size_t)tech_stage::n_stages> get_tech_stages(technique t)
{
	std::array<tech_stage, (size_t)tech_stage::n_stages> stg;
	stg.fill(tech_stage::null);
	switch (t)
	{
		case technique::normal:
		{
			stg[0] = tech_stage::normal;
			break;
		}
		case technique::outline:
		{
			stg[0] = tech_stage::write_mask;
			stg[1] = tech_stage::render_offscreen;
			stg[2] = tech_stage::render_from_offscreen_blend;
			break;
		}	
	}
	return stg;
}

static constexpr size_t get_n_stage(technique t)
{
	switch (t)
	{
	case technique::normal:
	{
		return 1;
		break;
	}
	case technique::outline:
	{
		return 3;
		break;
	}
	}
}




class tech_c
{
public:
	enum class stage {};
};