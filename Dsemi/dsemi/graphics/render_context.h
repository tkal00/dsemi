#ifndef HEADER_DSEMI_GRAPHICS_RENDERCONTEXT
#define HEADER_DSEMI_GRAPHICS_RENDERCONTEXT

#include "dsemi/graphics/resources/resource_include.h"
#include "dsemi/graphics/color.h"

namespace dsemi {
	namespace graphics {
		class Device;
		class render_target;

		// an abstraction of the directx device context
		// to allow the set up of either an immediate
		// or deferred context for render commands with
		// pre-set outputs and optionally a swapchain
		// if outputting to a window
		//
		class render_context {
		public:
			render_context(Device* device);

			void set_render_target(render_target* render_target);
			render_target* get_render_target() const;

			Device* get_device() const;

			void set_clear_color(float r, float g, float b, float a);
			void set_clear_color(color32 rgba);

			// todo: implement render commands to be sent to the device as a queued up list for non-immediate mode
			void begin();
			void clear();
			void draw(unsigned int num_vertices);
			void draw_triangle();
			void present();
			void end();

		public:
			ID3D11DeviceContext* get_dx_context() const;

		protected:
			Device* _device;
			render_target* _render_target;
		};

	}
}

#endif
