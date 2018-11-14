#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace Messenger
{
	namespace Private
	{
		// Itterates on FTs and calls the static execute function on each template instantiation of FT.
		template <template <std::size_t> class FT, std::size_t N, std::size_t M = 0> struct ForEach;

		// Final call, this does nothing.
		template <template <std::size_t> class FT, std::size_t M> struct ForEach <FT, M, M>
		{
			template <typename... AT> static void execute(AT&& ... args) {}
		};

		template <template <std::size_t> class FT, std::size_t N, std::size_t M> struct ForEach
		{
			template <typename... AT> static void execute(AT&& ... args)
			{
				FT<M>::execute(std::forward<AT>(args)...);
				ForEach<FT, N, M + 1>::execute(std::forward<AT>(args)...);
			}
		};

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
			static constexpr bool value = std::is_same<std::true_type, decltype(test_processor_method<MOT>(nullptr))>::value;
		};
		//Passes a tuple of messages to a module.
		template <std::size_t M> struct PassMessageTuple;
		//Passes a tuple of messages to a tuple of modules.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message_tuple(const MET& messenger, const MTT& messages_tuple, TT& modules_tuple);

		// Pass single message to a single module if the module has proper message processor.
		// After passing the message, the returned messages are passed to messenger.
		// This is a wrapper for class DoMessageProcessor for ease of use.
		template <typename ModuleType, typename MessengerType, typename MessageType> class PassMessage
		{
			template <std::size_t I, bool HasMessageProcessorWOMessenger, bool HasMessageProcessorWMessenger> struct DoMessageProcessor;

			// This is the case where the module, does not implement proper message processor for this message.
			template <std::size_t I> struct DoMessageProcessor <I, false, false>
			{
				template <typename MET, typename TT, typename MT> static void execute(const MET &messenger, const MT& message, TT& modules_tuple) {}
			};

			// The module implements message processor that takes only the message as input.
			template <std::size_t I> struct DoMessageProcessor <I, true, false>
			{
				template <typename MET, typename TT, typename MT> static void execute(const MET &messenger, const MT& message, TT& modules_tuple)
				{
					pass_tuple_message_tuple(messenger, std::get<I>(modules_tuple).process_message(message), modules_tuple);
				}
			};

			// This is the case the the module implementsa a single message processor that takes
			// messenger with message as the input.
			template <std::size_t I> struct DoMessageProcessor <I, false, true>
			{
				template <typename MET, typename TT, typename MT> static void execute(const MET &messenger, const MT& message, TT& modules_tuple)
				{
					pass_tuple_message_tuple(messenger, std::get<I>(modules_tuple).process_message(messenger, message), modules_tuple);
				}
			};

			// This is the case where the module implements both of the message processors for this message.
			template <std::size_t I> struct DoMessageProcessor <I, true, true>
			{
				template <typename MET, typename TT, typename MT> static void execute(const MET &messenger, const MT& message, TT& modules_tuple)
				{
					pass_tuple_message_tuple(messenger, std::get<I>(modules_tuple).process_message(message), modules_tuple);
					pass_tuple_message_tuple(messenger, std::get<I>(modules_tuple).process_message(messenger, message), modules_tuple);
				}
			};
		public:
			template <std::size_t I> using Type = DoMessageProcessor<I, HasMessageProcessor<ModuleType, MessageType>::value, HasMessageProcessor<ModuleType, MessengerType, MessageType>::value>;
		};

		// Passes a message to a tuple of modules. The first module in the tuple is the first to recieve the message.
		template <std::size_t M> struct PassMessageTuple
		{
			template <typename MET, typename MTT, typename TT> static void execute(const MET &messenger, const MTT& messages_tuple, TT& modules_tuple)
			{
				ForEach<PassMessageHelper<TT, MET, typename std::tuple_element<M, MTT>::type>::template Type, std::tuple_size<TT>::value>::execute(messenger, std::get<M>(messages_tuple), modules_tuple);
			}
		private:
			// A helper class to costumize the use of message passer for current loop.
			template <typename ModuleTupleType, typename MessengerType, typename MessageType> struct PassMessageHelper
			{
				template <std::size_t I>
				using Type = typename PassMessage<typename std::tuple_element<I, ModuleTupleType>::type, MessengerType, MessageType>::template Type<I>;
			};
		};

		//The helper function that passes a tuple of messages to a tuple of modules. First the first message on the tuple is passed to all of the modules then the rest of the messages are passed in the same manner.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message_tuple(const MET& messenger, const MTT& messages_tuple, TT& modules_tuple)
		{
			ForEach<PassMessageTuple, std::tuple_size<MTT>::value>::execute(messenger, messages_tuple, modules_tuple);
		}
	}

	//The messenger
	template <typename... Ts > class Messenger
	{
		//The tuple of modules
		using ModuleTupleType = std::tuple<Ts...>;
		mutable ModuleTupleType modules;


		// A helper class to costumize the use of message passer for current loop.
		template <typename MessageType> struct PassMessageHelper
		{
			template <std::size_t I> using Type = typename Private::PassMessage<typename std::tuple_element<I, ModuleTupleType>::type, Messenger<Ts...>, MessageType>::template Type<I>;
		};
	public:
		// The count of modules in messenger.
		static constexpr std::size_t module_count = sizeof... (Ts);

		Messenger() {}

		// Pass a message to internal modules.
		template <typename MT> void pass_message(const MT& message) const
		{
			Private::ForEach<PassMessageHelper<MT>::template Type, module_count>::execute(*this, message, modules);
		}

		// Get a certain module.
		template <std::size_t N> typename std::tuple_element<N, ModuleTupleType>::type &get_module()
		{
			return std::get<N>(modules);
		}
	};
}