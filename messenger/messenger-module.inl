#pragma once

#include <cstddef>
#include <tuple>

//Note: Add to the end of the module class to process known messages only, other wise you need to define a message processor for each message type.
#ifndef PROCESS_KNOWN_MESSAGES_ONLY
#	define PROCESS_KNOWN_MESSAGES_ONLY public: template<typename MT> std::tuple<> process_message(const MT& message) {return std::tuple<>();}
#endif