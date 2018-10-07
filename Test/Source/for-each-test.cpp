#include <messenger.inl>
#include <gtest/gtest.h>


static constexpr std::size_t N = 1000;
static bool execution_validator[N] = {false};
static bool out_of_bounds_access = false;

template <std::size_t M> struct ForEachCheck
{
	static void execute()
	{
		// Test for out of bound access.
		if (M >= N)
		{
			out_of_bounds_access = true;
			return;
		}

		// Checks order, since the (M-1)th should be true in order that Mth becomes true.
		if (M == 0 || execution_validator[M - 1])
			execution_validator[M] = !execution_validator[M];
	}
};

TEST(ForEachTest, OrderAndCoverageTest)
{
	Messenger::Private::ForEach<ForEachCheck, N + 1>::execute();
	bool result = true;

	// Checking coverage.
	for (std::size_t i = 0; i < N; i++)
		result = result && execution_validator[i];

	EXPECT_FALSE(out_of_bounds_access);
	EXPECT_TRUE(result);
}