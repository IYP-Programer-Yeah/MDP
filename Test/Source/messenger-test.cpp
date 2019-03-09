#include <MDP/messenger.inl>
#include <gtest/gtest.h>

template<std::size_t N> struct MessageType
{
	std::size_t &count;
	bool &properly_ordered;
	MessageType(std::size_t &message_count, bool &message_properly_ordered) : count(message_count), properly_ordered(message_properly_ordered) {}

	template <typename T> explicit MessageType(const T& message) : MessageType(message.count, message.properly_ordered) {}

	void check_message_order(const std::size_t message_number) const
	{
		properly_ordered = properly_ordered && (message_number == count);
		count++;
	}
};

template <std::size_t N> struct MessengerModule;

template <> struct MessengerModule<0>
{
	// Start
	template <typename T> std::tuple<MessageType<1>, MessageType<3>> process_message(const T& messenger, const MessageType<0> message)
	{
		message.check_message_order(0);
		// MessageType<2> is returned on MessengerModule<3> module's MessageType<1> message processor.
		return std::make_tuple(MessageType<1>(message), MessageType<3>(message));
	}
	// End
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<4> message)
	{
		message.check_message_order(10);
		return std::make_tuple();
	}
	
	int get_module_test = 0;
};

template <> struct MessengerModule<1>
{
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<1> message)
	{
		message.check_message_order(1);
		return std::make_tuple();
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<2> message)
	{
		message.check_message_order(4);
		return std::make_tuple();
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<3> message)
	{
		message.check_message_order(7);
		return std::make_tuple();
	}

	int get_module_test = 1;
};

template <> struct MessengerModule<2>
{
	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<1> message)
	{
		message.check_message_order(2);
		return std::make_tuple();
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<2> message)
	{
		message.check_message_order(5);
		return std::make_tuple();
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<3> message)
	{
		message.check_message_order(8);
		return std::make_tuple();
	}

	int get_module_test = 2;
};

template <> struct MessengerModule<3>
{
	template <typename T> std::tuple<MessageType<2>> process_message(const T& messenger, const MessageType<1> message)
	{
		message.check_message_order(3);
		// Test propegation.
		return std::make_tuple(MessageType<2>(message));
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<2> message)
	{
		message.check_message_order(6);
		return std::make_tuple();
	}

	template <typename T> std::tuple<> process_message(const T& messenger, const MessageType<3> message)
	{
		message.check_message_order(9);
		messenger.pass_message(MessageType<4>(message));
		return std::make_tuple();
	}

	int get_module_test = 3;
};

using MessengerType = Messenger::Messenger<MessengerModule<0>, MessengerModule<1>, MessengerModule<2>, MessengerModule<3>>;

TEST(MessengerTest, ModuleCountTest)
{
	EXPECT_EQ(MessengerType::ModuleCount, 4);
}

TEST(MessengerTest, GetModuleTest)
{
	MessengerType messenger;
	EXPECT_EQ(messenger.get_module<0>().get_module_test, 0);
	EXPECT_EQ(messenger.get_module<1>().get_module_test, 1);
	EXPECT_EQ(messenger.get_module<2>().get_module_test, 2);
	EXPECT_EQ(messenger.get_module<3>().get_module_test, 3);

	messenger.get_module<0>().get_module_test++;
	messenger.get_module<1>().get_module_test++;
	messenger.get_module<2>().get_module_test++;
	messenger.get_module<3>().get_module_test++;

	EXPECT_EQ(messenger.get_module<0>().get_module_test, 1);
	EXPECT_EQ(messenger.get_module<1>().get_module_test, 2);
	EXPECT_EQ(messenger.get_module<2>().get_module_test, 3);
	EXPECT_EQ(messenger.get_module<3>().get_module_test, 4);
}

TEST(MessengerTest, MessagePassingOrderAndCoverageTest)
{
	MessengerType messenger;
	std::size_t count = 0;
	bool properly_ordered = true;
	messenger.pass_message(MessageType<0>(count, properly_ordered));
	EXPECT_TRUE(properly_ordered);
	EXPECT_EQ(count, 11);
}