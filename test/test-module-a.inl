#pragma once

#include "messenger-module.inl"
#include <string>
#include <iostream>

namespace Test
{
	class ModuleA
	{
	public:
		struct ModuleAReport
		{
			std::string report;
		};

		MESSAGE_PROCESSOR(const int& message)
		{
			ModuleAReport report;
			report.report = "Processed an int message.";
			return std::make_tuple(report, std::to_string(message));
		}

		MESSAGE_PROCESSOR(const ModuleAReport& message)
		{
			std::cout << "Module A: " << message.report << std::endl;
			return std::tuple<>();
		}
	};
}