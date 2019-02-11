#include <MDP/messenger.inl>
#include <gtest/gtest.h>

#include "../Include/order-checker.inl"


struct LoopModule
{
	std::tuple<OrderCheckerMessage<1>> process_message(const OrderCheckerMessage<0>& message)
	{
		return std::make_tuple(OrderCheckerMessage<1>(message));
	}
};

TEST(PassMessageSendReveiveTest, SingleMessage)
{
	Messenger::Messenger<OrderChecker, LoopModule> messenger;
	std::size_t count = 0;
	bool properly_ordered = true;
	messenger.pass_message(OrderCheckerMessage<0>(count, properly_ordered));
	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, 2);
}