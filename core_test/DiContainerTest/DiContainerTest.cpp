# include "CppUnitTest.h"
# include "test_di_container.h"
# include "some_injectee.h"
# include "signal_user.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<type_to_inject>(
				const type_to_inject& instance)
			{
				return wide_nameof(type_to_inject);
			}
		}
	}
}

namespace zx_test
{
	TEST_CLASS(DiContainerTest)
	{
	public:
		TEST_METHOD(CreateDiContainerTest)
		{
			// di_container::ctor -> di_container::bind_all
			auto di_container = test_di_container();
		}

		TEST_METHOD(InjectInstancesTest)
		{
			auto type = zx::type::i<type_to_inject>();

			auto di_container = test_di_container();
			auto injectee = some_injectee();
			const auto interface_inst = injectee.get_interface_instance();

			// Singleton container inject test
			Assert::IsNotNull(interface_inst);

			const auto b = dynamic_cast<type_to_inject *>(interface_inst);

			// Inheritance check
			Assert::IsTrue(b != nullptr);

			// Read-write check
			b->public_field = 15;
			Assert::AreEqual(b->public_field, 15);

			// Nominated container inject test
			Assert::IsNotNull(injectee.get_injected_1());

			// Nominated container inject test
			Assert::IsNotNull(injectee.get_injected_2());

			// Different named instances
			const auto b_inst_1 = injectee.get_injected_1();
			const auto b_inst_2 = injectee.get_injected_2();

			Assert::AreNotSame(*b_inst_1, *b_inst_2);
		}

		TEST_METHOD(SignalTest)
		{
			auto di_container = test_di_container();
			auto injectee = some_injectee();
			auto signal_user = ::signal_user();

			const auto result = injectee.do_something();
			Assert::AreEqual(result, 4.0);
		}
	};
}
