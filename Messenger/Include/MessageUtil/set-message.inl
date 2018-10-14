#pragma once

namespace MessageUtil
{
	template<typename T, typename Entity> struct Set
	{
		const T value;
		Set() = delete;

		Set(const Set& that) : value(that.value) {}
		Set(Set&& that) : value(that.value) {}

		Set& operator =(const Set&) = delete;
		Set& operator =(Set&&) = delete;

		Set(T& i_value) : value(i_value){}
	};
}