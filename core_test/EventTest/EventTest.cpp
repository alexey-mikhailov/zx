# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"
# include "GlobalMethods.h"
# include "EventUser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace zx_test
{
	void OnSomethingHappened()
	{
		ls << "Something happened. " << zx::endl;
	}

	int OnReadyToCalc()
	{
		return rand();
	}

	void OnAdded(double a, double b, double result)
	{
		Assert::IsTrue(a == 4.5);
		ls << L"Sum of " << a << " and " << b << L" is " << result << zx::endl;
	}

	TEST_CLASS(EventTest)
	{
	public:
		TEST_METHOD(SubscribeFireUnsubscribe)
		{
			std::wstringstream ss;
			EventUser user;

			user.SomethingComplete += OnSomethingHappened;
			user.ReadyToCalc += OnReadyToCalc;
			user.Added += OnAdded;

			user.DoSomething();
			const auto lastResult = user.GetLastResult();
			ls << L"GetLastResult = " << lastResult << zx::endl;
			user.Add(4.5, 3.2);

			user.SomethingComplete -= OnSomethingHappened;
			user.ReadyToCalc -= OnReadyToCalc;
			user.Added -= OnAdded;

			user.Add(1.2, 3.4);
		}

		TEST_METHOD(MultithreadingTest)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(250));

			{
				zx::multicast_delegate<int(int, int)> del1;
				zx::multicast_delegate<void(int, int)> del2;
				zx::multicast_delegate<int()> del3;
				zx::multicast_delegate<void()> del4;

				std::this_thread::sleep_for(std::chrono::seconds(1));

				std::vector<std::thread> threads(150);
				for (auto&& thread : threads)
				{
					thread = std::thread(SubscribeInvokeUnsubscribe, del1, del2, del3, del4);
				}

				for (auto&& thread : threads)
				{
					thread.join();
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	};
}
