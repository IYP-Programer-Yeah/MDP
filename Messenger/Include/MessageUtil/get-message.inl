#pragma once

namespace MessageUtil
{
	template<typename T, typename Entity> struct Get
	{
		T& ref;

		explicit Get(T& i_ref) : ref(i_ref){}
	};
}