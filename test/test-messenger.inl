#pragma once

#include "messenger.inl"
#include "test-module-a.inl"
#include "test-module-b.inl"

namespace Test
{
	typedef Messenger::Messenger<Test::ModuleA, Test::ModuleB> TestMessenger;
}