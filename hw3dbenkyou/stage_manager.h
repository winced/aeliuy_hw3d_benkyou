#pragma once
#include "dx_component_base.h"
#include "fullscreen_blend.h"
#include <list>


class stage_manager
{
public:
	class tech_setup
	{
		friend stage_manager;
	public:
		void add_drawable(drawable* ref)
		{
			drawables.push_back(ref);
		}
		void add_blender(fullscreen_blender* blender)
		{
			blenders.push_back(blender);
		}
	private:
		std::list<drawable*> drawables; 
		std::list<fullscreen_blender*> blenders;
	};
	void init_ptrs(std::shared_ptr<render_target>& rt, std::shared_ptr<depth_stencil_buffer>& ds_buf)
	{
		p_rt_buffer = rt;
		p_ds_buffer = ds_buf;
	}
	void add_drawable(drawable* ref, technique t)	
	{
		tech_map[(size_t)t].add_drawable(ref);
	}
	void add_fs_blender(fullscreen_blender* ref, technique t)
	{
		tech_map[(size_t)t].add_blender(ref);
	}
	void set_rt(graphics& gfx, const stage_descriptor& s_desc)
	{
		if (s_desc.offscreen == rt_stage::target)
			p_rt_buffer->set_as_target(gfx, p_ds_buffer->get_ds_view());
		else if(s_desc.offscreen == rt_stage::texture)
			p_rt_buffer->set_as_texture(gfx);
		if (s_desc.swap_buffer == rt_stage::target)
			gfx.set_swapchain_render_target(p_ds_buffer->get_ds_view());
			
	}
	// return true if the stage is finished with just this function 
	void draw(graphics& gfx, technique t)
	{
		std::array<tech_stage, (size_t)tech_stage::n_stages> stages = get_tech_stages(t);
		for (size_t s = 0; s < stages.size() && stages[s] != tech_stage::null; s++)
		{
			stage_descriptor s_desc = get_stage_descriptor(stages[s]);
			set_rt(gfx, s_desc);
			if (s_desc.drawables)
			{
				for (const auto& d : tech_map[(size_t)t].drawables)
				{
					d->set_default(gfx);
					d->set_stage_comps(gfx, t, s);
					if (d->has_indices())
						gfx.draw_indexed(d->get_index_count());
					else
						gfx.draw_unindexed(d->get_vertex_count());

				}
				
			}
			if (s_desc.blender)
			{
				for (const auto& b : tech_map[(size_t)t].blenders)
				{
					b->set(gfx);
					gfx.draw_indexed(b->get_index_count());
				}
			}

		}

	}
private:
	std::shared_ptr<render_target> p_rt_buffer; 
	std::shared_ptr<depth_stencil_buffer> p_ds_buffer;
	std::array<tech_setup, (size_t)technique::n_tech> tech_map;

};