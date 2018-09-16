#pragma once

// Used STL libraries
#include <string>
#include <iostream>

namespace Test
{
	// Defintion of Module A
	class ModuleA
	{
		// The structure that is passed as a message. This type is processed by the module itself so it does not need to be visible to other modules.
		struct Report
		{
			// The message holds a string that the processesor will print.
			std::string report; 
		};
	public:

		int last_recieved_message;

		// This is a message processor that will process every message that is of type int.
		// A message processor is any function named "message_processor" that can take either
		// a single parameter of const lvalue reference of message type, or a const lvalue 
		// reference of a messanger as the first parameter and a const lvalue referemce of
		// message type as the second parameter and returns a tuple of messages.
		auto message_processor(const int& message)
		{
			last_recieved_message = message;
			Report report;
			report.report = "Processed an int message.";
			// The returned tuple holds a set of messages that will be passed to modules all over again.
			// It is important to note that the order of messages in tuple defines the order in which the messages are passed.
			// In this case first the report, a message of type ModuleA::Report is passed to all the modules, and then a message
			// of type std::string is passed to all the module.
			return std::make_tuple(report, std::to_string(message));
		}

		// This is a message processor that will process every message that is of type
		//ModuleA::Report.
		auto message_processor(const Report& message) -> std::tuple<>
		{
			std::cout << "Module A: " << message.report << std::endl;
			// There are no messages to be passed to modules after returning.
			return std::tuple<>();
		}
	};
}