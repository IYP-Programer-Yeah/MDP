#pragma once

#include <cstddef>
#include <tuple>

namespace Messenger
{
	namespace Privates
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

		// TMP conditional call. Calls the pass_message function if the DoPass is true.
		template <bool DoPass> struct ConditionalPassMessage;

		template<> struct ConditionalPassMessage<true>
		{
			template <typename MOT, typename... MPAT> static auto pass_message(MOT& module, const MPAT & ... args) -> decltype(module.process_message(args...))
			{
				return module.process_message(args...);
			}
		};

		template<> struct ConditionalPassMessage<false>
		{
			template <typename MOT, typename... MPAT> static std::tuple<> pass_message(MOT& module, const MPAT & ... args)
			{
				return std::tuple<>();
			}
		};

		//Passes a single message to a module.
		template <std::size_t M> struct pass_message;
		//Passes a tuple of messages to a module.
		template <std::size_t M> struct pass_message_tuple;
		//Passes a tuple of messages to a tuple of modules.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message_tuple(const MET& messenger, const MTT& messages_tuple, TT& modules_tuple);

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

		// Passes a single message to a single module.
		// The result of processing a message is a tuple of messages to be passed to the modules, which is done using pass_tuple_message.
		template <std::size_t M> struct pass_message
		{
			template <typename MET, typename MT, typename TT> static void execute(const MET &messenger, const MT& message, TT& modules_tuple)
			{
				pass_tuple_message_tuple(messenger, ConditionalPassMessage<HasMessageProcessor<typename std::tuple_element<M, TT>::type, MT>::value>::pass_message((std::get<M>(modules_tuple)), message), modules_tuple);
				pass_tuple_message_tuple(messenger, ConditionalPassMessage<HasMessageProcessor<typename std::tuple_element<M, TT>::type, MET, MT>::value>::pass_message((std::get<M>(modules_tuple)), messenger, message), modules_tuple);
			}
		};

		// Passes a message to a tuple of modules. The first module in the tuple is the first to recieve the message.
		template <std::size_t M> struct pass_message_tuple
		{
			template <typename MET, typename MTT, typename TT> static void execute(const MET &messenger, const MTT& messages_tuple, TT& modules_tuple)
			{
				ForEach<pass_message, std::tuple_size<TT>::value>::execute(messenger, std::get<M>(messages_tuple), modules_tuple);
			}

		};

		//The helper function that passes a tuple of messages to a tuple of modules. First the first message on the tuple is passed to all of the modules then the rest of the messages are passed in the same manner.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message_tuple(const MET& messenger, const MTT& messages_tuple, TT& modules_tuple)
		{
			ForEach<pass_message_tuple, std::tuple_size<MTT>::value>::execute(messenger, messages_tuple, modules_tuple);
		}
	}

	//The messenger
	template <typename... Ts > class Messenger
	{
		//The tuple of modules
		using module_tuple_type = std::tuple<Ts...>;
		mutable module_tuple_type modules;

	public:
		Messenger() {}

		//pass a message to internal modules
		template <typename MT> void pass_message(const MT& message) const
		{
			Privates::ForEach<Privates::pass_message, sizeof... (Ts)>::execute(*this, message, modules);
		}

		//get a certain module
		template <std::size_t N> typename std::tuple_element<N, module_tuple_type>::type &get_module()
		{
			return std::get<N>(modules);
		}
	};
}