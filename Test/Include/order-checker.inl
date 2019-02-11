#pragma once

#include <cstddef>
#include <tuple>

template<std::size_t N>
struct OrderCheckerMessage
{
	std::size_t &count;
	bool &properly_ordered;
	OrderCheckerMessage(std::size_t &message_count, bool &message_properly_ordered) : count(message_count), properly_ordered(message_properly_ordered) {}

	template <typename T>
	explicit OrderCheckerMessage(const T& message) : OrderCheckerMessage(message.count, message.properly_ordered) {}

	void check_message_order(const std::size_t message_number) const
	{
		properly_ordered = properly_ordered && (message_number == count);
		count++;
	}
};

struct OrderChecker
{
	template <std::size_t N>
	std::tuple<> process_message(const OrderCheckerMessage<N> &message)
	{
		message.check_message_order(N);
		return std::make_tuple();
	}
};