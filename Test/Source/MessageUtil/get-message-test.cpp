#include <MDP/messenger.inl>
#include <MDP/MessageUtil/get-message.inl>
#include <gtest/gtest.h>

namespace GetMessageTest
{
	struct A {};
	struct B {};
	struct C {};
	struct Sync {};

	using GetA = MessageUtil::Get<int, A>;
	using GetB = MessageUtil::Get<int, B>;
	using GetC = MessageUtil::Get<int, C>;

	struct ModuleA
	{
		int a = 10;
		int b;
		int c;

		std::tuple<GetB, GetC> process_message(const Sync&)
		{
			return std::make_tuple(GetB(b), GetC(c));
		}

		std::tuple<> process_message(const GetA &get_a)
		{
			get_a.ref = a;
			return std::make_tuple();
		}
	};

	struct ModuleB
	{
		int a;
		int b = 20;
		int c;

		std::tuple<GetA, GetC> process_message(const Sync&)
		{
			return std::make_tuple(GetA(a), GetC(c));
		}

		std::tuple<> process_message(const GetB &get_b)
		{
			get_b.ref = b;
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
			messenger.pass_message(GetA(a));
			messenger.pass_message(GetB(b));
			return std::make_tuple();
		}

		std::tuple<> process_message(const GetC &get_c)
		{
			get_c.ref = c;
			return std::make_tuple();
		}
	};

	using MessengerType = Messenger::Messenger<ModuleA, ModuleB, ModuleC>;
}

using namespace GetMessageTest;

TEST(GetMessageTest, FunctionalityTest)
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