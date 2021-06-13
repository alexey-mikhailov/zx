# include "CppUnitTest.h"
# include "TestDiContainer.h"
# include "SomeInjectee.h"
# include "SignalUser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace zx_test
{
	TEST_CLASS(DiContainerTest)
	{
	public:
		TEST_METHOD(CreateDiContainerTest)
		{
			// di_container::ctor -> di_container::bind_all
			auto diContainer = TestDiContainer();
		}

		TEST_METHOD(InjectInstancesTest)
		{
			auto type = zx::type::i<TypeToInject>();

			auto diContainer = TestDiContainer();
			auto injectee = SomeInjectee();
			const auto interfaceInst = injectee.GetInterfaceInstance();

			// Singleton container inject test
			Assert::IsNotNull(interfaceInst);

			const auto b = dynamic_cast<TypeToInject *>(interfaceInst);

			// Inheritance check
			Assert::IsTrue(b != nullptr);

			// Read-write check
			b->PublicField = 15;
			Assert::AreEqual(b->PublicField, 15);

			// Nominated container inject test
			Assert::IsNotNull(injectee.GetInjected1());

			// Nominated container inject test
			Assert::IsNotNull(injectee.GetInjected2());

			// Different named instances
			const auto bInst1 = injectee.GetInjected1();
			const auto bInst2 = injectee.GetInjected2();

			Assert::AreNotEqual((void *)(bInst1),
								(void *)(bInst2));
		}

		TEST_METHOD(SignalTest)
		{
			auto diContainer = TestDiContainer();
			auto injectee = SomeInjectee();
			auto signalUser = SignalUser();

			const auto result = injectee.DoSomething();
			Assert::AreEqual(result, 4.0);
		}
	};
}
