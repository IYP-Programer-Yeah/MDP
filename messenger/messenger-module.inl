#pragma once

#include <tuple>

//Note: Add to the end of the module class to process known messages only, other wise you need to define a message processor for each message type.
#ifndef PROCESS_KNOWN_MESSAGES_ONLY
#	define PROCESS_KNOWN_MESSAGES_ONLY public: template<typename MET, typename MT> std::tuple<> process_message(const MET &messenger, const MT& message) {return std::tuple<>();}
#endif

//Overloads message processor based on the number of arguments
//MESSAGE_PROCESSOR(Message declartion)
//MESSAGE_PROCESSOR(Messnger name, Message declartion)
//MESSAGE_PROCESSOR(Return type, Messnger name, Message declartion)
#define MESSAGE_PROCESSOR(...) __MESSAGE_PASSING_PRIVATE_EXPAND(__MESSAGE_PASSING_PRIVATE_GET_MESSAGE_PROCESSOR(__VA_ARGS__,\
																												__MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR3,\
																												__MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR2,\
																												__MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR1)(__VA_ARGS__))

//Work around for __VA_ARGS__
#define __MESSAGE_PASSING_PRIVATE_EXPAND(x) x
//Macro getter
#define __MESSAGE_PASSING_PRIVATE_GET_MESSAGE_PROCESSOR(_1, _2, _3, MP, ...) MP
//Message processors
#define __MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR1(message) template<typename MET> auto process_message(const MET &, ##message)
#define __MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR2(messenger, message) template<typename MET> auto process_message(const MET & ##messenger, ##message)
#define __MESSAGE_PASSING_PRIVATE_MESSAGE_PROCESSOR3(return_type, messenger, message) template<typename MET> ##return_type process_message(const MET & ##messenger, ##message)