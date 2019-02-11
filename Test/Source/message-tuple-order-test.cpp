#include <MDP/messenger.inl>
#include <gtest/gtest.h>

#include "../Include/order-checker.inl"

struct SenderModule
{
	std::tuple<OrderCheckerMessage<1>, OrderCheckerMessage<2>, OrderCheckerMessage<3>, OrderCheckerMessage<4>>process_message(const OrderCheckerMessage<0> &message)
	{
		return std::make_tuple(OrderCheckerMessage<1>(message),
			OrderCheckerMessage<2>(message),
			OrderCheckerMessage<3>(message),
			OrderCheckerMessage<4>(message));
	}
};

TEST(TupleMessageOrderTest, FiveConsecutiveMessages)
{
	Messenger::Messenger<OrderChecker, SenderModule> messenger;
	std::size_t count = 0;
	bool properly_ordered = true;
	messenger.pass_message(OrderCheckerMessage<0>(count, properly_ordered));
	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, 5);
}