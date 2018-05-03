#pragma once

#include "messenger.inl"
#include "test-module-a.inl"
#include "test-module-b.inl"

namespace Test
{
	Messenger::messenger<Test::module_a, Test::module_b> test_messenger;
	template <typename MT> void pass_message(const MT &message)
	{
		test_messenger.pass_message(message);
	}
}