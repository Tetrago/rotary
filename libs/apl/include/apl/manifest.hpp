#pragma once

#include <string>

namespace apl
{
	class Manifest
	{
		
	};

	struct ResourceLocation
	{
		const Manifest* manifest;
		std::string exportLabel;
	};
}    
