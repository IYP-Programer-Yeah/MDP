#pragma once

#include <string>
#include <iostream>

#include "messenger-module.inl"

namespace Test
{
	class module_b
	{
	public:

		std::tuple<> process_message(const std::string& message)
		{
			std::cout << message << std::endl;
			return std::tuple<>();
		}

		PROCESS_KNOWN_MESSAGES_ONLY
	};
}