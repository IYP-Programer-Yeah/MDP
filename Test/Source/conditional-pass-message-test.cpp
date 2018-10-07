#include <messenger.inl>
#include <gtest/gtest.h>

struct ConditionalPassMessageModule
{
	std::tuple<int> process_message(int a, const int&b, const int&c)
	{
		return std::make_tuple(a + b + c);
	}
};

TEST(ConditionalPassMessageTest, CallCheck)
{
	ConditionalPassMessageModule module;
	const int a = 5;
	const int b = 10;
	const int c = 20;
	std::tuple<int> true_result = Messenger::Private::ConditionalPassMessage<true>::pass_message(module, a, b, b + c);
	std::tuple<> false_result = Messenger::Private::ConditionalPassMessage<false>::pass_message(module, a, b, b + c);
	EXPECT_EQ(a + b + b + c, std::get<0>(true_result));
}