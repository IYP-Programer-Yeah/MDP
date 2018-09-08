// Includes the Test::TestMessenger type for the messenger.
#include "test-messenger.inl"

// The entery point of the program.
int main()
{
	// Instantiation of messenger.
	Test::TestMessenger test_messenger;
	// Pass an int message to messenger.
	test_messenger.pass_message(12);
	// Example of accessing a module.
	std::cout << "The last int message Module A recieved had value: " << test_messenger.get_module<0>().last_recieved_message << std::endl;
	return 0;
}