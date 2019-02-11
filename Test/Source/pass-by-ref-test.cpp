#include <MDP/messenger.inl>
#include <gtest/gtest.h>

#include <tuple>

static const void * messenger_ptr;
static const void * message_ptr;
static const void * module_ptr;

static const void * message_only_message_ptr;
static const void * message_only_module_ptr;

struct MessageOnly {};
struct MessengerAndMessage {};
struct DoubleProcessor {};

struct RefCatcher
{
	std::tuple<> process_message(const MessageOnly& message)
	{
		message_only_message_ptr = &message;
		message_only_module_ptr = this;

		return std::make_tuple();
	}

	template <typename Messenger>
	std::tuple<> process_message(const Messenger& messenger, const MessengerAndMessage& message)
	{
		message_ptr = &message;
		messenger_ptr = &messenger;
		module_ptr = this;

		return std::make_tuple();
	}

	std::tuple<> process_message(const DoubleProcessor& message)
	{
		message_only_message_ptr = &message;
		message_only_module_ptr = this;

		return std::make_tuple();
	}

	template <typename Messenger>
	std::tuple<> process_message(const Messenger& messenger, const DoubleProcessor& message)
	{
		message_ptr = &message;
		messenger_ptr = &messenger;
		module_ptr = this;

		return std::make_tuple();
	}
};

TEST(PassByRefTest, MessageOnlyProcessor)
{
	message_only_message_ptr = nullptr;
	message_only_module_ptr = nullptr;

	Messenger::Messenger<RefCatcher> messenger;
	
	MessageOnly message;
	messenger.pass_message(message);
	
	EXPECT_EQ(message_only_message_ptr, &message);
	EXPECT_EQ(message_only_module_ptr, &messenger.get_module<0>());
}

TEST(PassByRefTest, MessageAndMessengerProcessor)
{
	message_ptr = nullptr;
	messenger_ptr = nullptr;
	module_ptr = nullptr;

	Messenger::Messenger<RefCatcher> messenger;

	MessengerAndMessage message;
	messenger.pass_message(message);

	EXPECT_EQ(message_ptr, &message);
	EXPECT_EQ(messenger_ptr, &messenger);
	EXPECT_EQ(module_ptr, &messenger.get_module<0>());
}



TEST(PassByRefTest, DoubleMessageProcessor)
{
	message_only_message_ptr = nullptr;
	message_only_module_ptr = nullptr;

	message_ptr = nullptr;
	messenger_ptr = nullptr;
	module_ptr = nullptr;

	Messenger::Messenger<RefCatcher> messenger;

	DoubleProcessor message;
	messenger.pass_message(message);

	EXPECT_EQ(message_only_message_ptr, &message);
	EXPECT_EQ(message_only_module_ptr, &messenger.get_module<0>());

	EXPECT_EQ(message_ptr, &message);
	EXPECT_EQ(messenger_ptr, &messenger);
	EXPECT_EQ(module_ptr, &messenger.get_module<0>());
}
