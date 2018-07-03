#pragma once

#include <string>
#include <iostream>

#include "messenger-module.inl"

namespace Test
{
	class ModuleB
	{
	public:

		MESSAGE_PROCESSOR(const std::string& message)
		{
			std::cout << message << std::endl;
			return std::tuple<>();
		}

		PROCESS_KNOWN_MESSAGES_ONLY
	};
}