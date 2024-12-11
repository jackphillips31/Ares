#pragma once
#include <arespch.h>

namespace Ares {

	class Asset
	{
	public:
		virtual ~Asset() = default;
		virtual const std::string& GetName() const = 0;
	};

}