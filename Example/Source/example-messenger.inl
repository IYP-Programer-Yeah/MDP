#pragma once

// The definition of Messenger::Messenger
#include <messenger.inl>
// The definition of Test::ModuleA
#include "example-module-a.inl"
// The definition of Test::ModuleB
#include "example-module-b.inl"

namespace Example
{
	// The specialization of Messenger for our use with proper modules.
	using MessengerType = Messenger::Messenger<Example::ModuleA, Example::ModuleB>;
}