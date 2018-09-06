#pragma once

#include <string>
#include <iostream>

#include "messenger-module.inl"

namespace Test
{
	class ModuleB
	{
	public:
		struct ModuleBReport
		{
			std::string report;
		};

		MESSAGE_PROCESSOR(const ModuleBReport& message)
		{
			std::cout << "Module B: " << message.report << std::endl;
			return std::tuple<>();
		}

		MESSAGE_PROCESSOR(messenger, const std::string& message)
		{
			ModuleBReport report;
			report.report = "Recieved a std::string message.";
			messenger.pass_message(report);
			std::cout << message << std::endl;
			return std::tuple<>();
		}
	};
}