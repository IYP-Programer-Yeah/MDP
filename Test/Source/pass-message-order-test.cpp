#include <MDP/messenger.inl>
#include <gtest/gtest.h>

#include "../Include/order-checker.inl"

TEST(PassMessageOrderTest, FiveConsecutiveMessages)
{
	Messenger::Messenger<OrderChecker> messenger;
	std::size_t count = 0;
	bool properly_ordered = true;
	messenger.pass_message(OrderCheckerMessage<0>(count, properly_ordered),
		OrderCheckerMessage<1>(count, properly_ordered),
		OrderCheckerMessage<2>(count, properly_ordered),
		OrderCheckerMessage<3>(count, properly_ordered),
		OrderCheckerMessage<4>(count, properly_ordered));
	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, 5);
}