#pragma once

#include <Utility/integer-sequence.inl>
#include <Utility/tuple.inl>
#include <Utility/constexpr.inl>

#include <type_traits>
#include <cstddef>
#include <tuple>

#include "Message/multiplexer.inl"

namespace ModuleUtil
{
	template <std::size_t Max>
	struct Multiplexer
	{
		template<typename Messenger, template<std::size_t> class T, typename... Args>
		CPP_14_CONSTEXPR std::tuple<> process_message(const Messenger& messenger, const Message::Multiplex<T, Args...> &message)
		{
			generate_message_and_pass_message<Messenger, T>(typename Utility::MakeIntegerSequence<std::size_t, Max>(),
				message.n, messenger, message.args);
			return std::make_tuple();
		}
	private:

		template<typename Messenger, template<std::size_t> class T, typename TupleType, std::size_t... FuncIndices>
		CPP_14_CONSTEXPR static void generate_message_and_pass_message(Utility::IntegerSequence<std::size_t, FuncIndices...>,
			const std::size_t n, const Messenger &messenger, const TupleType &args)
		{
			using FuncType = void(*)(const Messenger &, const TupleType&);
			using FuncArrayType = FuncType[Max];
			(FuncArrayType{ generate_message_and_pass_message<FuncIndices, Messenger, T, TupleType>... })[n](messenger, args);
		}

		template<std::size_t N, typename Messenger, template<std::size_t> class T, typename TupleType>
		CPP_14_CONSTEXPR static void generate_message_and_pass_message(const Messenger &messenger, const TupleType &args)
		{
			messenger.pass_message(Utility::make_from_tuple<T<N>>(args));
		}
	};
}