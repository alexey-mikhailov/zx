# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace zx_test
{
	TEST_CLASS(ZxTypeTest)
	{
	public:
		TEST_METHOD(CheckRttiTypeSize)
		{
# ifdef _WIN64
			Assert::AreEqual(48ui64, sizeof(zx::type));
# else ifdef _WIN32
			Assert::AreEqual(24ui32, sizeof(zx::type));
# endif
		}

		/// Instantiates new int number
		TEST_METHOD(InferType)
		{
			auto& int_type = zx::type::i<int32_t>();
			auto new_int =
				std::unique_ptr<int32_t>(
					static_cast<int32_t*>(int_type.instantiate()));

			Assert::AreEqual(0i32, *new_int);
		}

		/// Instantiates vector of int with size 10
		TEST_METHOD(InstantiatePerformanceMeasurement)
		{
			typedef std::vector<zx::i32> some_t;
			typedef size_t ctor_arg_t;
			constexpr ctor_arg_t ctor_arg = 10ui64;

			// Default ctor will be added to metadata of zx::type
			// if type is default constructible. 
			const zx::type& some_type = zx::type::i<some_t>();
			
			// Ensure all non-default ctors you need. Manually :(
			zx::refl<some_t>::ctor<ctor_arg_t>::ensure();

			// Performance test
			constexpr zx::u64 instance_amount = 1000ui64;
			void* pointers[instance_amount]{};

			auto before = std::chrono::high_resolution_clock::now();

			// Instantiate number of instances. 
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				pointers[i] = new std::vector<zx::i32>(ctor_arg);
			}

			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> 
				spent_for_new = after - before;

			ls  << "[new T()]: " << zx::endl
				<< "Instantiated " << instance_amount << " "
				<< "instances of type \"" << some_type.name << "\". "
				<< "Time spent: " << spent_for_new << "ms. " << zx::endl;

			// Test execution time convention
			Assert::IsTrue(spent_for_new < std::chrono::seconds(1));

			// Free instantiated memory
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				auto some_instance =
					std::unique_ptr<some_t>(
						static_cast<some_t*>(pointers[i]));

				Assert::IsNotNull(some_instance.get());
				Assert::AreEqual(ctor_arg, some_instance->size());
			}

			before = std::chrono::high_resolution_clock::now();

			// Instantiate number of instances. 
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				pointers[i] = some_type.instantiate(ctor_arg);
			}

			after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> spent_for_inst = after - before;
			ls  << "[zx::type::instantiate()]: " << zx::endl
				<< "Instantiated " << instance_amount << " "
				<< "instances of type \"" << some_type.name << "\". "
				<< "Time spent: " << spent_for_inst << "ms. " << zx::endl;

			// Test execution time convention
			Assert::IsTrue(spent_for_inst < std::chrono::seconds(1));

			// Free instantiated memory
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				auto some_instance =
					std::unique_ptr<some_t>(
						static_cast<some_t*>(pointers[i]));

				Assert::IsNotNull(some_instance.get());
				Assert::AreEqual(ctor_arg, some_instance->size());
			}

			ls << spent_for_inst / spent_for_new
				<< " times slower than new(). ";
		}
	};
}