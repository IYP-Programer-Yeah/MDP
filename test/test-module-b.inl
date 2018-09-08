#pragma once

// Used STL libraries
#include <string>
#include <iostream>

// Definition of MESSAGE_PROCESSOR macro
#include "messenger-module.inl"

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

		// This is a message processor that will process every std::string message, this type of message processor
		// declration allows the access to messenger. The messeger is passed as a const Lvalue refrence, the first parameter
		// of the macro specifies the name of the variable the messenger will be passed as. The second parameter of the macro
		// "const std::string& message" defines the input argument of the module(the message), and the name of the variable
		// that the message is passed as.
		// The return type of the message processor is auto and is supported on C++14 and above.
		MESSAGE_PROCESSOR(messenger, const std::string& message)
		{
			Report report;
			report.report = "Recieved a std::string message.";
			// Use messenger to pass a message before returning.
			messenger.pass_message(report);
			std::cout << message << std::endl;
			// There are no messages to be passed after returning.
			return std::tuple<>();
		}

		// This type of message processor declration allows definition of return type, as well as access to messenger.
		// The first parameter of the macro is the return type, the second is the name of the variable that the messenger
		// is passed as, and the third parameter defines the input argument of the module(the message), and the name of 
		// the variable that the message is passed as.
		MESSAGE_PROCESSOR(std::tuple<>, messenger, const Report& message)
		{
			std::cout << "Module B: " << message.report << std::endl;
			// There are no messages to be passed after returning.
			return std::tuple<>();
		}
	};
}