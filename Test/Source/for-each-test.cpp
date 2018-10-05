#include <messenger.inl>
#include <gtest/gtest.h>


static constexpr std::size_t N = 1000;
static constexpr std::size_t M = 1000;
static bool execution_validator[N + M] = {false};

template <std::size_t M> struct ForEachCheck
{
	static void execute()
	{
		// Checks order, since the (M-1)th should be true in order that Mth becomes true.
		if (M == 0 || execution_validator[M - 1])
			execution_validator[M] = !execution_validator[M];
	}
};

TEST(ForEachTest, OrderAndCoverageTest)
{
	Messenger::Private::ForEach<ForEachCheck, N>::execute();
	bool result = true;

	// Checking coverage.
	for (std::size_t i = 0; i < N; i++)
		result = result && execution_validator[i];
	// Make sure it hasnt gone too far.
	for (std::size_t i = N; i < M; i++)
		result = result && !execution_validator[i];
	EXPECT_TRUE(result);
}