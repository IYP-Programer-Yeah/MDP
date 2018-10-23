#pragma once

namespace MessageUtil
{
	template<typename T, typename Entity> struct Set
	{
		T value;

		explicit Set(const T& i_value) : value(i_value) {}
		explicit Set(T&& i_value) : value(std::move(i_value)){}
	};
}