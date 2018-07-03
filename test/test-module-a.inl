#pragma once

#include "messenger-module.inl"
#include <string>

namespace Test
{
	class ModuleA
	{
	public:

		MESSAGE_PROCESSOR(messenger, const int& message)
		{
			messenger.pass_message(std::to_string(message));
			return std::tuple<>();
		}

		PROCESS_KNOWN_MESSAGES_ONLY
	};
}