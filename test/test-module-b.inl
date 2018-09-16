#pragma once

// Used STL libraries
#include <string>
#include <iostream>

namespace Test
{
	// Defintion of Module B
	class ModuleB
	{
		// The structure that is passed as a message. This type is processed by the module itself so it does not need to be visible to other modules.
		struct Report
		{
			std::string report;
		};
	public:

		// This is a message processor that takes a messenger and a message, since messenger
		// type is not availbe here, a template parameter is used for messenger type.
		template <typename T> auto process_message(const T& messenger, const std::string& message)
		{
			Report report;
			report.report = "Recieved a std::string message.";
			// Use messenger to pass a message before returning.
			messenger.pass_message(report);
			std::cout << message << std::endl;
			// There are no messages to be passed after returning.
			return std::tuple<>();
		}

		// A message processor for messages of type ModuleB::Report.
		template <typename T> auto process_message(const T& messenger, const Report& message)
		{
			std::cout << "Module B: " << message.report << std::endl;
			// There are no messages to be passed after returning.
			return std::tuple<>();
		}
	};
}