#pragma once

// This is deprecated.

#include <tuple>

//Overloads message processor based on the number of arguments
//MESSAGE_PROCESSOR(Message declartion)
//MESSAGE_PROCESSOR(Messenger name, Message declartion)
//MESSAGE_PROCESSOR(Return type, Messenger name, Message declartion)
#define MESSAGE_PROCESSOR(...) __MESSENGER_PRIVATE_EXPAND(__MESSENGER_PRIVATE_GET_MESSAGE_PROCESSOR(__VA_ARGS__,\
																									__MESSENGER_PRIVATE_MESSAGE_PROCESSOR3,\
																									__MESSENGER_PRIVATE_MESSAGE_PROCESSOR2,\
																									__MESSENGER_PRIVATE_MESSAGE_PROCESSOR1)(__VA_ARGS__))

//Work around for __VA_ARGS__
#define __MESSENGER_PRIVATE_EXPAND(x) x
//Macro getter
#define __MESSENGER_PRIVATE_GET_MESSAGE_PROCESSOR(_1, _2, _3, MP, ...) MP
//Message processors
#define __MESSENGER_PRIVATE_MESSAGE_PROCESSOR1(message) auto process_message(message)
#define __MESSENGER_PRIVATE_MESSAGE_PROCESSOR2(messenger, message) template<typename MET> auto process_message(const MET & messenger, message)
#define __MESSENGER_PRIVATE_MESSAGE_PROCESSOR3(return_type, messenger, message) template<typename MET> ##return_type process_message(const MET & messenger, message)