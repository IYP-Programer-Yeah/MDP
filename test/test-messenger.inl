#pragma once

// The definition of Messenger::Messenger
#include "messenger.inl"
// The definition of Test::ModuleA
#include "test-module-a.inl"
// The definition of Test::ModuleB
#include "test-module-b.inl"

namespace Test
{
	// The specialization of Messenger for our use with proper modules.
	typedef Messenger::Messenger<Test::ModuleA, Test::ModuleB> TestMessenger;
}