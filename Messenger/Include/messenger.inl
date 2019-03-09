#pragma once

#include <cstddef>
#include <tuple>

#include <Utility/integer-sequence.inl>
#include <Utility/constexpr.inl>
#include <Utility/for-each.inl>
#include <Utility/tuple.inl>


namespace Messenger
{
	namespace Private
	{
		// Checks the module to see if it has the proper message processor.
		template <typename MOT, typename... MPAT> class HasMessageProcessor
		{
			template <typename, typename> struct Checker;

			// Checks the return type of the message prosessor.
			template <typename... T> static std::true_type test_processor_return_type(std::tuple<T...> arg);
			static std::false_type test_processor_return_type(...);

			// First checks the message processor's arguments, and then its return type.
			template <typename C> static auto test_processor_method(Checker<C, decltype(std::declval<C>().process_message(std::declval<MPAT>()...))>*) -> decltype(test_processor_return_type(std::declval<C>().process_message(std::declval<MPAT>()...)));
			template <typename C> static std::false_type test_processor_method(...);

		public:

			// If the module has the proper message processor, this value will be true, otherwise this will be false.
			static constexpr bool Value = std::is_same<std::true_type, decltype(test_processor_method<MOT>(nullptr))>::value;
		};

		// Pass a single message to a single module if the module has proper message processor.
		template <bool HasMessageProcessorWOMessenger, bool HasMessageProcessorWMessenger>
		struct DoMessageProcessor;

		// This is the case where the module, does not implement proper message processor for this message.
		template <>
		struct DoMessageProcessor <false, false>
		{
			template <typename ModuleType, typename MessengerType, typename MessageType>
			CPP_14_CONSTEXPR static void process_message(ModuleType& module, const MessengerType &messenger, const MessageType &message) {}
		};

		// The module implements message processor that takes only the message as input.
		template <>
		struct DoMessageProcessor <true, false>
		{
			template <typename ModuleType, typename MessengerType, typename MessageType>
			CPP_14_CONSTEXPR static void process_message(ModuleType& module, const MessengerType &messenger, const MessageType &message)
			{
				Utility::call_with_tuple_elements(messenger, module.process_message(message));
			}
		};

		// This is the case the the module implementsa a single message processor that takes
		// messenger with message as the input.
		template <>
		struct DoMessageProcessor <false, true>
		{
			template <typename ModuleType, typename MessengerType, typename MessageType>
			CPP_14_CONSTEXPR static void process_message(ModuleType& module, const MessengerType &messenger, const MessageType &message)
			{
				Utility::call_with_tuple_elements(messenger, module.process_message(messenger, message));
			}
		};

		// This is the case where the module implements both of the message processors for this message.
		template <>
		struct DoMessageProcessor <true, true>
		{
			template <typename ModuleType, typename MessengerType, typename MessageType>
			CPP_14_CONSTEXPR static void process_message(ModuleType& module, const MessengerType &messenger, const MessageType &message)
			{
				Utility::call_with_tuple_elements(messenger, module.process_message(message));
				Utility::call_with_tuple_elements(messenger, module.process_message(messenger, message));
			}
		};

		// Pass a single message to a single module if the module has proper message processor.
		// After passing the message, the returned messages are passed to messenger.
		// This is a wrapper for class DoMessageProcessor for ease of use.
		template <typename MessengerType, typename MessageType>
		struct PassMessage
		{
			template <typename ModuleType>
			CPP_14_CONSTEXPR void operator()(ModuleType& module) const
			{
				DoMessageProcessor<HasMessageProcessor<ModuleType, MessageType>::Value, HasMessageProcessor<ModuleType, MessengerType, MessageType>::Value>::process_message(module, messenger, message);
			}

			const MessengerType &messenger;
			const MessageType &message;
		};

		//The messenger implementation
		template <typename... Modules>
		class MessengerImpl
		{
			//The tuple of modules
			using ModuleTupleType = std::tuple<Modules...>;
			mutable ModuleTupleType modules;

		public:

			// The count of modules in messenger.
			static constexpr std::size_t ModuleCount = sizeof... (Modules);

			template <typename Message>
			CPP_14_CONSTEXPR void operator()(const Message& message) const
			{
				struct Lambda
				{
					CPP_14_CONSTEXPR void operator()(Modules &... i_modules) const
					{
						Utility::for_each(message_passer, i_modules...);
					}

					const Private::PassMessage<MessengerImpl<Modules...>, Message> message_passer;
				};
				Utility::call_with_tuple_elements(Lambda{ { *this, message } }, modules);
			}

			// Pass a message to internal modules.
			template <typename... Messages>
			CPP_14_CONSTEXPR void operator()(const Messages &... messages) const
			{
				Utility::for_each(*this, messages...);
			}

			// Pass a message to internal modules.
			template <typename... Messages>
			CPP_14_CONSTEXPR void pass_message(const Messages &... messages) const
			{
				this->operator()(messages...);
			}

			// Get a certain module.
			template <std::size_t N>
			constexpr typename std::tuple_element<N, ModuleTupleType>::type &get_module() const
			{
				return std::get<N>(modules);
			}
		};

		template<typename... Modules>
		constexpr typename std::size_t MessengerImpl<Modules...>::ModuleCount;

		template <typename... Modules>
		struct Messenger : private MessengerImpl<Modules...>
		{
			using MessengerImpl<Modules...>::ModuleCount;
			using MessengerImpl<Modules...>::pass_message;
			using MessengerImpl<Modules...>::get_module;
		};
	}

	using Private::Messenger;
}