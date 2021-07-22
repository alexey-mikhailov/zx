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
			// 
			// di_container::ctor -> di_container::bind_all
			// 
			auto di_container = test_di_container();

			auto field_name = nameof(_interface_inst);
			auto field = zx::meta::registry::get_field(
				zx::type::i<some_injectee>(),
				field_name);

			Assert::AreEqual(std::string(field_name),
							 field.get_name());

			# ifdef _WIN64
			Assert::AreEqual(16ui64, sizeof(field));
			# else ifdef _WIN32
			Assert::AreEqual(8ui64, sizeof(field));
			# endif
		}

		TEST_METHOD(InjectInstancesTest)
		{
			auto& type = zx::type::i<type_to_inject>();

			auto di_container = test_di_container();
			auto injectee = some_injectee();
			const auto abstract_instance = injectee.get_interface_instance();

			// Singleton container inject test
			Assert::AreEqual(true, (bool)abstract_instance);

			const auto concrete_instance = dynamic_cast<type_to_inject *>(abstract_instance.get());

			// Inheritance check
			Assert::IsTrue(concrete_instance != nullptr);

			// Read-write check
			Assert::AreEqual(singleton_arg, concrete_instance->public_field);
			constexpr int new_value = 15;
			concrete_instance->public_field = new_value;
			Assert::AreEqual(new_value, concrete_instance->public_field);

			// Nominated container inject test
			auto concrete_instance_1 = injectee.get_class_instance_1();
			Assert::AreEqual(true, (bool)concrete_instance_1);

			// Read-write check
			Assert::AreEqual(nominated1_arg, concrete_instance_1->public_field);
			concrete_instance_1->public_field = new_value;
			Assert::AreEqual(new_value, concrete_instance_1->public_field);

			// Nominated container inject test
			auto concrete_instance_2 = injectee.get_class_instance_2();
			Assert::AreEqual(true, (bool)concrete_instance_2);

			// Read-write check
			Assert::AreEqual(nominated2_arg, concrete_instance_2->public_field);
			concrete_instance_2->public_field = new_value;
			Assert::AreEqual(new_value, concrete_instance_2->public_field);

			// Different named instances check
			Assert::AreNotSame(*concrete_instance_1, *concrete_instance_2);
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
