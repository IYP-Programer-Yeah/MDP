#pragma once

namespace MessageUtil
{
	template<typename T, typename Entity> struct Set
	{
		T value;

		explicit Set(const T& i_value) : value(i_value){}
	};
}