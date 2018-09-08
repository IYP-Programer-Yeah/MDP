#pragma once

// Definition of MESSAGE_PROCESSOR macro
#include "messenger-module.inl"

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
		// This is one of the three macros that can be used to declare a message processor.
		// Here the return type is set to auto which is availble on C++14 and above.
		// The message to be processed should always be a const Lvalue reference.
		// "const int& message" defines the input argument of the module (the message),
		// and the name of the variable that the message is passed as.
		MESSAGE_PROCESSOR(const int& message)
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

		// This is a message processor that will process every message that is of type ModuleA::Report.
		// "const ModuleA::Report& message" defines the input argument of the module(the message),
		// and the name of the variable that the message is passed as.
		// The " -> std::tuple<>" declares the return type of the message processor, and is supported on C++11 and above.
		MESSAGE_PROCESSOR(const Report& message) -> std::tuple<>
		{
			std::cout << "Module A: " << message.report << std::endl;
			// There are no messages to be passed to modules after returning.
			return std::tuple<>();
		}
	};
}