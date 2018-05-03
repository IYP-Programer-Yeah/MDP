#pragma once

#include <cstddef>
#include <tuple>

namespace Messenger
{
	namespace Privates
	{
		//Passes a single message to a tuple of modules.
		template <std::size_t N, std::size_t M> struct message_passer;
		//Passes a tuple of messages to a tuple of modules.
		template <std::size_t N, std::size_t M> struct tuple_message_passer;
		//Passes a tuple of messages to a tuple of modules.
		template <typename MTT, typename TT> void pass_tuple_message(const MTT messages_tuple, TT& modules_tuple);

		template <std::size_t M> struct message_passer <0, M>
		{
			template <typename MT, typename TT> static void pass_message(const MT& message, TT& modules_tuple) {}
		};

		template <std::size_t N, std::size_t M> struct message_passer
		{
			//Pass a single message to a tuple of modules, the order of modules in the tuple, defines the order of the messages being passed.
			//The result of processing each message is a tuple of messages to be passed to the modules, which is done using pass_tuple_message.
			template <typename MT, typename TT> static void pass_message(const MT& message, TT& modules_tuple)
			{
				pass_tuple_message((std::get<M - N>(modules_tuple)).process_message(message), modules_tuple);
				message_passer<N - 1, M>::pass_message(message, modules_tuple);
			}
		};



		template <std::size_t M> struct tuple_message_passer <0, M>
		{
			template <typename MTT, typename TT> static void pass_message(const MTT& messages_tuple, TT& modules_tuple) {}
		};

		template <std::size_t N, std::size_t M> struct tuple_message_passer
		{
			//Pass a tuple of messages to a tuple of modules. First the first message on the tuple is passed to all of the modules then the rest of the messages are passed in the same manner.
			template <typename MTT, typename TT> static void pass_message(const MTT& messages_tuple, TT& modules_tuple)
			{
				message_passer<std::tuple_size<TT>::value, std::tuple_size<TT>::value>::pass_message(std::get<M - N>(messages_tuple), modules_tuple);
				tuple_message_passer<N - 1, M>::pass_message(messages_tuple, modules_tuple);
			}
		};


		//The helper function that passes a tuple of messages to a tuple of modules.
		template <typename MTT, typename TT> void pass_tuple_message(const MTT messages_tuple, TT& modules_tuple)
		{
			tuple_message_passer<std::tuple_size<MTT>::value, std::tuple_size<MTT>::value>::pass_message(messages_tuple, modules_tuple);
		}
	}

	//The messenger
	template <typename... Ts > class messenger
	{
		//The tuple of modules
		using module_tuple_type = std::tuple<Ts...>;
		mutable module_tuple_type modules;

	public:
		messenger() {}

		//pass a message to internal modules
		template <typename MT> void pass_message(const MT& message) const
		{
			Privates::message_passer<sizeof... (Ts), sizeof... (Ts)>::pass_message(message, modules);
		}

		//get a certain module
		template <std::size_t N> typename std::tuple_element<N, module_tuple_type>::type &get_module()
		{
			return std::get<N>(modules);
		}
	};
}