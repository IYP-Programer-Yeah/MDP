#pragma once

#include <cstddef>
#include <tuple>

namespace Messenger
{
	namespace Privates
	{
		//Passes a single message to a tuple of modules.
		template <std::size_t N, std::size_t M> struct MessagePasser;
		//Passes a tuple of messages to a tuple of modules.
		template <std::size_t N, std::size_t M> struct TupleMessagePasser;
		//Passes a tuple of messages to a tuple of modules.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message(const MET &messenger, const MTT messages_tuple, TT& modules_tuple);

		template <std::size_t M> struct MessagePasser <0, M>
		{
			template <typename MET, typename MT, typename TT> static void pass_message(const MET &messenger, const MT& message, TT& modules_tuple) {}
		};

		template <std::size_t N, std::size_t M> struct MessagePasser
		{
			//Pass a single message to a tuple of modules, the order of modules in the tuple, defines the order of the messages being passed.
			//The result of processing each message is a tuple of messages to be passed to the modules, which is done using pass_tuple_message.
			template <typename MET, typename MT, typename TT> static void pass_message(const MET &messenger, const MT& message, TT& modules_tuple)
			{
				pass_tuple_message(messenger, (std::get<M - N>(modules_tuple)).process_message(messenger, message), modules_tuple);
				MessagePasser<N - 1, M>::pass_message(messenger, message, modules_tuple);
			}
		};



		template <std::size_t M> struct TupleMessagePasser <0, M>
		{
			template <typename MET, typename MTT, typename TT> static void pass_message(const MET &messenger, const MTT& messages_tuple, TT& modules_tuple) {}
		};

		template <std::size_t N, std::size_t M> struct TupleMessagePasser
		{
			//Pass a tuple of messages to a tuple of modules. First the first message on the tuple is passed to all of the modules then the rest of the messages are passed in the same manner.
			template <typename MET, typename MTT, typename TT> static void pass_message(const MET &messenger, const MTT& messages_tuple, TT& modules_tuple)
			{
				MessagePasser<std::tuple_size<TT>::value, std::tuple_size<TT>::value>::pass_message(messenger, std::get<M - N>(messages_tuple), modules_tuple);
				TupleMessagePasser<N - 1, M>::pass_message(messenger, messages_tuple, modules_tuple);
			}
		};


		//The helper function that passes a tuple of messages to a tuple of modules.
		template <typename MET, typename MTT, typename TT> void pass_tuple_message(const MET &messenger, const MTT messages_tuple, TT& modules_tuple)
		{
			TupleMessagePasser<std::tuple_size<MTT>::value, std::tuple_size<MTT>::value>::pass_message(messenger, messages_tuple, modules_tuple);
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
			Privates::MessagePasser<sizeof... (Ts), sizeof... (Ts)>::pass_message(*this, message, modules);
		}

		//get a certain module
		template <std::size_t N> typename std::tuple_element<N, module_tuple_type>::type &get_module()
		{
			return std::get<N>(modules);
		}
	};
}