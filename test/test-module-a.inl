#pragma once

#include "messenger-module.inl"
#include "test-module.inl"
#include <string>

namespace Test
{
	class module_a
	{
	public:

		std::tuple<> process_message(const int& message)
		{
			pass_message(std::to_string(message));
			return std::tuple<>();
		}

		PROCESS_KNOWN_MESSAGES_ONLY
	};
}