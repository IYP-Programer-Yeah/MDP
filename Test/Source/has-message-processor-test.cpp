#include <messenger.inl>
#include <gtest\gtest.h>

template<std::size_t> struct MessageType{};

struct HasProperMessageProcessor
{
	std::tuple<> process_message(const MessageType<0>& message) {}
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<1>& message) {}
	std::tuple<> process_message(const MessageType<2>& message) {}
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<2>& message) {}
};

using MessengerType = Messenger::Messenger<HasProperMessageProcessor>;

TEST(HasMessageProcessorTest, HasProperMessageProcessorWithoutMessengerTest)
{
	constexpr bool Result = Messenger::Privates::HasMessageProcessor<HasProperMessageProcessor, MessageType<0>>::value;
	EXPECT_EQ(Result, true);
}

TEST(HasMessageProcessorTest, HasProperMessageProcessorWithMessengerTest)
{
	constexpr bool Result = Messenger::Privates::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<1>>::value;
	EXPECT_EQ(Result, true);
}

TEST(HasMessageProcessorTest, HasProperMessageProcessorWithAndWithoutMessengerTest)
{
	constexpr bool Result = Messenger::Privates::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<2>>::value && Messenger::Privates::HasMessageProcessor<HasProperMessageProcessor, MessageType<2>>::value;
	EXPECT_EQ(Result, true);
}
