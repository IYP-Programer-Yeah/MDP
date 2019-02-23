#include <MDP/messenger.inl>
#include <MDP/ModuleUtil/multiplexer.inl>
#include <gtest/gtest.h>

#include "../../Include/order-checker.inl"

TEST(MultiplexerTest, FiveConsecutiveMessages)
{
	Messenger::Messenger<ModuleUtil::Multiplexer<10>, OrderChecker> messenger;
	std::size_t count = 0;
	constexpr std::size_t Size = 5;
	bool properly_ordered = true;

	for (std::size_t i = 0; i < Size; i++)
		messenger.pass_message(ModuleUtil::Message::Multiplex<OrderCheckerMessage, std::size_t&, bool&>{i, std::tuple<std::size_t&, bool&>{ count, properly_ordered }});

	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, Size);
}