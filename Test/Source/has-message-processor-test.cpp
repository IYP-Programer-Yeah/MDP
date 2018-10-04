#include <messenger.inl>
#include <gtest\gtest.h>

// Test name convention:
// <MessageDefinition>_<MessengerDefinition>
// MessageDefinition:
//  #1 CR: const message lvalue ref
//  #2 CC: const message copy
//  #3 CTR: const templated message ref
//  #4 CTC: const templated message copy
//  #5 UR: universal message ref
// MessengerDefinition:
//  #1 NONE: does not accept messenger
//  #2 CR: const messenger lvalue ref
//  #3 CC: const messenger copy
//  #4 UR: universal messenger ref

template<std::size_t, typename M = void> struct MessageType{};

struct HasProperMessageProcessor
{
	// Test#1 CR_NONE
	std::tuple<> process_message(const MessageType<1>& message);

	// Test#2 CC_NONE
	std::tuple<> process_message(const MessageType<2> message);

	// Test#3 CTR_NONE
	template <typename M> std::tuple<> process_message(const MessageType<3, M>& message);

	// Test#4 CTC_NONE
	template <typename M> std::tuple<> process_message(const MessageType<4, M> message);

	// Test#5 UR_NONE
	template <typename M> std::tuple<> process_message(MessageType<5, M>&& message);

	// Test#6 CR_CR
	template <typename T> std::tuple<int> process_message(const T& messenger, const MessageType<6>& message);

	// Test#7 CC_CR
	template <typename T> std::tuple<int> process_message(const T& messenger, const MessageType<7> message);

	// Test#8 CTR_CR
	template <typename T, typename M> std::tuple<int> process_message(const T& messenger, const MessageType<8, M>& message);

	// Test#9 CTC_CR
	template <typename T, typename M> std::tuple<int> process_message(const T& messenger, const MessageType<9, M> message);

	// Test#10 UR_CR
	template <typename T, typename M> std::tuple<int> process_message(const T& messenger, const MessageType<10, M>&& message);

	// Test#11 CR_CC
	template <typename T> std::tuple<int, char> process_message(const T messenger, const MessageType<11>& message);

	// Test#12 CC_CC
	template <typename T> std::tuple<int, char> process_message(const T messenger, const MessageType<12> message);

	// Test#13 CTR_CC
	template <typename T, typename M> std::tuple<int, char> process_message(const T messenger, const MessageType<13, M>& message);

	// Test#14 CTC_CC
	template <typename T, typename M> std::tuple<int, char> process_message(const T messenger, const MessageType<14, M> message);

	// Test#15 UR_CC
	template <typename T, typename M> std::tuple<int, char> process_message(const T messenger, const MessageType<15, M>&& message);

	// Test#16 CR_UR
	template <typename T> std::tuple<int, char, bool> process_message(T&& messenger, const MessageType<16>& message);

	// Test#17 CC_UR
	template <typename T> std::tuple<int, char, bool> process_message(T&& messenger, const MessageType<17> message);

	// Test#18 CTR_UR
	template <typename T, typename M> std::tuple<int, char, bool> process_message(T&& messenger, const MessageType<18, M>& message);

	// Test#19 CTC_UR
	template <typename T, typename M> std::tuple<int, char, bool> process_message(T&& messenger, const MessageType<19, M> message);

	// Test#20 UR_UR
	template <typename T, typename M> std::tuple<int, char, bool> process_message(T&& messenger, const MessageType<20, M>&& message);
};

struct HasNoProperMessageProcessor
{
	// Test#21 BAD_RETURN_TYPE_NONE
	int process_message(const MessageType<21>& message);

	// Test#22 NON_CONST_REF_MESSAGE
	std::tuple<> process_message(MessageType<22>& message);

	// Test#23 WRONG_MESSAGE_TYPE
	std::tuple<> process_message(const MessageType <0>& message);

	// Test#24 NON_CONST_REF_MESSAGE_CORRECT_MESSENGER
	template <typename T> std::tuple<> process_message(const T& messenger, MessageType<24>& message);

	// Test#25 CORRECT_NON_CONST_REF_MESSENGER
	template <typename T> std::tuple<> process_message(T& messenger, const MessageType<25>& message);

	// Test#26 WRONG_MESSAGE_TYPE_CORRECT_MESSENGER
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<0>& message);

	// Test#27 CORRECT_MESSAGE_WRONG_MESSENGER_TYPE
	std::tuple<> process_message(const MessageType<0>& messenger, const MessageType<27>& message);
};

using MessengerType = Messenger::Messenger<HasProperMessageProcessor, HasNoProperMessageProcessor>;

// Test#0 No proper message processor.
TEST(HasMessageProcessorTest, NO_PROCESSOR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<0>>::value;
	EXPECT_EQ(Result, false);
}

// Test#1
TEST(HasMessageProcessorTest, CR_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<1>>::value;
	EXPECT_EQ(Result, true);
}

// Test#2
TEST(HasMessageProcessorTest, CC_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<2>>::value;
	EXPECT_EQ(Result, true);
}

// Test#3
TEST(HasMessageProcessorTest, CTR_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<3>>::value;
	EXPECT_EQ(Result, true);
}

// Test#4
TEST(HasMessageProcessorTest, CTC_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<4>>::value;
	EXPECT_EQ(Result, true);
}

// Test#5
TEST(HasMessageProcessorTest, UR_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<5>>::value;
	EXPECT_EQ(Result, true);
}

// Test#6
TEST(HasMessageProcessorTest, CR_CR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<6>>::value;
	EXPECT_EQ(Result, true);
}

// Test#7
TEST(HasMessageProcessorTest, CC_CR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<7>>::value;
	EXPECT_EQ(Result, true);
}

// Test#8
TEST(HasMessageProcessorTest, CTR_CR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<8>>::value;
	EXPECT_EQ(Result, true);
}

// Test#9
TEST(HasMessageProcessorTest, CTC_CR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<9>>::value;
	EXPECT_EQ(Result, true);
}

// Test#10
TEST(HasMessageProcessorTest, UR_CR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<10>>::value;
	EXPECT_EQ(Result, true);
}

// Test#11
TEST(HasMessageProcessorTest, CR_CC_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<11>>::value;
	EXPECT_EQ(Result, true);
}

// Test#12
TEST(HasMessageProcessorTest, CC_CC_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<12>>::value;
	EXPECT_EQ(Result, true);
}

// Test#13
TEST(HasMessageProcessorTest, CTR_CC_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<13>>::value;
	EXPECT_EQ(Result, true);
}

// Test#14
TEST(HasMessageProcessorTest, CTC_CC_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<14>>::value;
	EXPECT_EQ(Result, true);
}

// Test#15
TEST(HasMessageProcessorTest, UR_CC_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<15>>::value;
	EXPECT_EQ(Result, true);
}

// Test#16
TEST(HasMessageProcessorTest, CR_UR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<16>>::value;
	EXPECT_EQ(Result, true);
}

// Test#17
TEST(HasMessageProcessorTest, CC_UR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<17>>::value;
	EXPECT_EQ(Result, true);
}

// Test#18
TEST(HasMessageProcessorTest, CTR_UR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<18>>::value;
	EXPECT_EQ(Result, true);
}

// Test#19
TEST(HasMessageProcessorTest, CTC_UR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<19>>::value;
	EXPECT_EQ(Result, true);
}

// Test#20
TEST(HasMessageProcessorTest, UR_UR_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessengerType, MessageType<20>>::value;
	EXPECT_EQ(Result, true);
}

// Test#21
TEST(HasMessageProcessorTest, BAD_RETURN_TYPE_NONE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<21>>::value;
	EXPECT_EQ(Result, false);
}

// Test#22
TEST(HasMessageProcessorTest, NON_CONST_REF_MESSAGE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<22>>::value;
	EXPECT_EQ(Result, false);
}

// Test#23
TEST(HasMessageProcessorTest, WORNG_MESSAGE_TYPE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<23>>::value;
	EXPECT_EQ(Result, false);
}

// Test#24
TEST(HasMessageProcessorTest, NON_CONST_REF_MESSAGE_CORRECT_MESSENGER_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<24>>::value;
	EXPECT_EQ(Result, false);
}

// Test#25
TEST(HasMessageProcessorTest, CORRECT_NON_CONST_REF_MESSENGER_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<25>>::value;
	EXPECT_EQ(Result, false);
}

// Test#26
TEST(HasMessageProcessorTest, WRONG_MESSAGE_TYPE_CORRECT_MESSENGER_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<26>>::value;
	EXPECT_EQ(Result, false);
}

// Test#27
TEST(HasMessageProcessorTest, CORRECT_MESSAGE_WRONG_MESSENGER_TYPE_TEST)
{
	constexpr bool Result = Messenger::Private::HasMessageProcessor<HasProperMessageProcessor, MessageType<27>>::value;
	EXPECT_EQ(Result, false);
}