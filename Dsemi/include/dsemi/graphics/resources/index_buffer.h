#ifndef GRAPHCIS_INDEX_BUFFER_H
#define GRAPHCIS_INDEX_BUFFER_H

#include "Dsemi/graphics/api_include.h"

namespace dsemi {

	struct gfx_index_buffer_desc {
		void*    data_pointer = nullptr;
		uint32_t data_size = 0u;
		uint32_t data_stride = 0u;
	};

	class gfx_index_buffer {
		friend class gfx_device;
	public:
		gfx_index_buffer()
			: _dx_buffer(nullptr)
		{}

	private:

#if (defined GFX_USING_DX11)
	private:
		ComPtr<ID3D11Buffer> _dx_buffer;

#else

#endif
	};

}

#endif