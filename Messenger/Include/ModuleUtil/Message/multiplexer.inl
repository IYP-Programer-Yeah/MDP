#pragma once

#include <cstddef>

namespace ModuleUtil
{
	namespace Message
	{
		template <template<std::size_t> class T, typename... Args>
		struct Multiplex
		{
			std::size_t n;
			const std::tuple<Args...> &args;
		};
	}
}