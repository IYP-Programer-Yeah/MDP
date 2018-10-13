namespace MessageUtil
{
	template<typename T, typename Entity> struct Get
	{
		T& ref;
		Get() = delete;

		Get(const Get& that) : ref(that.ref) {}
		Get(Get&& that) : ref(that.ref) {}

		Get& operator =(const Get&) = delete;
		Get& operator =(Get&&) = delete;

		Get(T& i_ref) : ref(i_ref){}
	};
}