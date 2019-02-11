#include <MDP/messenger.inl>
#include <MDP/MessageUtil/set-message.inl>
#include <gtest/gtest.h>

struct A {};
struct B {};
struct C {};
struct Sync {};

using SetA = MessageUtil::Set<int, A>;
using SetB = MessageUtil::Set<int, B>;
using SetC = MessageUtil::Set<int, C>;

struct ModuleA
{
	int a = 10;
	int b;
	int c;

	std::tuple<SetA> process_message(const Sync&)
	{
		return std::make_tuple(SetA(a));
	}

	std::tuple<> process_message(const SetB &set_b)
	{
		b = set_b.value;
		return std::make_tuple();
	}

	std::tuple<> process_message(const SetC &set_c)
	{
		c = set_c.value;
		return std::make_tuple();
	}
};

struct ModuleB
{
	int a;
	int b = 20;
	int c;

	std::tuple<SetB> process_message(const Sync&)
	{
		return std::make_tuple(SetB(b));
	}

	std::tuple<> process_message(const SetA &set_a)
	{
		a = set_a.value;
		return std::make_tuple();
	}

	std::tuple<> process_message(const SetC &set_c)
	{
		c = set_c.value;
		return std::make_tuple();
	}
};

struct ModuleC
{
	int a;
	int b;
	int c = 30;

	template <typename T> std::tuple<> process_message(const T& messenger, const Sync&)
	{
		messenger.pass_message(SetC(c));
		return std::make_tuple();
	}

	std::tuple<> process_message(const SetA &set_a)
	{
		a = set_a.value;
		return std::make_tuple();
	}

	std::tuple<> process_message(const SetB &set_b)
	{
		b = set_b.value;
		return std::make_tuple();
	}
};

using MessengerType = Messenger::Messenger<ModuleA, ModuleB, ModuleC>;

TEST(SetMessageTest, FunctionalityTest)
{
	MessengerType messenger;

	messenger.pass_message(Sync());

	EXPECT_EQ(messenger.get_module<0>().a, 10);
	EXPECT_EQ(messenger.get_module<0>().b, 20);
	EXPECT_EQ(messenger.get_module<0>().c, 30);

	EXPECT_EQ(messenger.get_module<1>().a, 10);
	EXPECT_EQ(messenger.get_module<1>().b, 20);
	EXPECT_EQ(messenger.get_module<1>().c, 30);

	EXPECT_EQ(messenger.get_module<2>().a, 10);
	EXPECT_EQ(messenger.get_module<2>().b, 20);
	EXPECT_EQ(messenger.get_module<2>().c, 30);
}