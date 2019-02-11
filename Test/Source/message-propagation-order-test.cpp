#include <MDP/messenger.inl>
#include <gtest/gtest.h>

#include <cstddef>

#include "../Include/order-checker.inl"

template <std::size_t N>
struct OrderedModule
{
	std::tuple<OrderCheckerMessage<N>> process_message(const OrderCheckerMessage<0>& message)
	{
		return std::make_tuple(OrderCheckerMessage<N>(message));
	}
};

TEST(MessagePropagationOrderTest, FiveConsecutiveMessages)
{
	Messenger::Messenger<OrderChecker, OrderedModule<1>, OrderedModule<2>, OrderedModule<3>, OrderedModule<4>> messenger;
	std::size_t count = 0;
	bool properly_ordered = true;
	messenger.pass_message(OrderCheckerMessage<0>(count, properly_ordered));
	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, 5);
}