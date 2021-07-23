# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"
# include "global_methods.h"
# include "event_user.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<event_user>(
				const event_user& instance)
			{
				std::wstringstream ss;
				ss  << L"event_user { address : " 
					<< std::hex << &instance << std::dec 
					<< L" } ";

				return ss.str();
			}
		}
	}
}

namespace zx_test
{
	void on_something_happened()
	{
		ls << "Something happened. " << zx::endl;
	}

	int on_ready_to_calc()
	{
		return 12347654;
	}

	void on_added(double a, double b, double result)
	{
		Assert::IsTrue(a == 4.5);
		ls << L"Sum of " << a << " and " << b << L" is " << result << zx::endl;
	}

	const event_user& on_identity_requested(const event_user& sender)
	{
		return sender;
	}

	TEST_CLASS(EventTest)
	{
	public:
		TEST_METHOD(SubscribeFireUnsubscribe)
		{
			event_user user;

			user.something_complete += on_something_happened;
			user.readly_to_calc += on_ready_to_calc;
			user.added += on_added;

			user.do_something();
			const auto lastResult = user.get_last_result();
			Assert::AreEqual(12347654, lastResult);
			ls << L"get_last_result = " << lastResult << zx::endl;
			user.add(4.5, 3.2);

			user.something_complete -= on_something_happened;
			user.readly_to_calc -= on_ready_to_calc;
			user.added -= on_added;

			user.add(1.2, 3.4);
		}

		TEST_METHOD(EventIdentity)
		{
			event_user user1;
			user1.identity_requested += on_identity_requested;
			event_user user2(user1);

			// Copy result check
			Assert::AreEqual(user1.get_copyable_resource(), 
							 user2.get_copyable_resource());

			auto [result_user1, counter1] = user1.request_identity();
			Assert::AreSame(user1, result_user1);
			Assert::AreEqual(1ui32, counter1);

			auto [result_user2, counter2] = user2.request_identity();
			Assert::IsNull(&result_user2); // must be struct at <NULL>
			Assert::AreEqual(0ui32, counter2);

			user1.identity_requested -= on_identity_requested;
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
					thread = std::thread(subscribe_invoke_unsubscribe, 
										 del1.copy(), 
										 del2.copy(), 
										 del3.copy(), 
										 del4.copy());
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
