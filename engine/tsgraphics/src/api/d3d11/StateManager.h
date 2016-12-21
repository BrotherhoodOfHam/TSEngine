/*
	Render API
	
	State manager caches states
*/

#pragma once

#include "base.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

namespace ts
{
	class D3D11StateManager
	{
	private:
		
		
	public:
		
		int demandDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc, ID3D11DepthStencilState** state);
	};
}

/////////////////////////////////////////////////////////////////////////////////////////////////
