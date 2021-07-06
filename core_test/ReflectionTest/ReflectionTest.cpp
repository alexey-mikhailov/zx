# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace zx_test
{
	TEST_CLASS(ReflectionTest)
	{
	public:
		/// Instantiates new int number
		TEST_METHOD(InferType)
		{
			Assert::AreEqual(sizeof(zx::type), 40ui64);

			auto& int_type = zx::type::i<int32_t>();
			auto new_int =
				std::unique_ptr<int32_t>(
					static_cast<int32_t*>(int_type.instantiate()));

			Assert::AreEqual(0i32, *new_int);
		}

		/// Instantiates vector of int with size 10
		TEST_METHOD(InstantiatePerformanceTest)
		{
			constexpr zx::u64 size = 10ui64;

			// Default ctor will be added to metadata of zx::type
			// if type is default constructible. 
			const zx::type& vector_type = zx::type::i<std::vector<zx::i32>>();
			
			// Ensure all non-default ctors you need. Manually :(
			zx::refl<std::vector<zx::i32>>::ctor<zx::u64>::ensure();

			// Performance test
			constexpr zx::u64 instance_amount = 1000ui64;
			void* pointers[instance_amount]{};

			auto before = std::chrono::high_resolution_clock::now();

			// Instantiate number of instances. 
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				pointers[i] = new std::vector<zx::i32>(size);
			}

			auto after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> 
				spent_for_new = after - before;

			ls  << "[new T()]: " << zx::endl
				<< "Instantiated " << instance_amount << " "
				<< "instances of type \"" << vector_type.name << "\". "
				<< "Time spent: " << spent_for_new << "ms. " << zx::endl;

			// Test execution time convention
			Assert::IsTrue(spent_for_new < std::chrono::seconds(1));

			// Free instantiated memory
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				auto new_vector =
					std::unique_ptr<std::vector<zx::i32>>(
						static_cast<std::vector<zx::i32>*>(pointers[i]));

				Assert::IsNotNull(new_vector.get());
				Assert::AreEqual(size, new_vector->size());
			}

			before = std::chrono::high_resolution_clock::now();

			// Instantiate number of instances. 
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				pointers[i] = vector_type.instantiate(size);
			}

			after = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> spent_for_inst = after - before;
			ls  << "[zx::type::instantiate()]: " << zx::endl
				<< "Instantiated " << instance_amount << " "
				<< "instances of type \"" << vector_type.name << "\". "
				<< "Time spent: " << spent_for_inst << "ms. " << zx::endl;

			// Test execution time convention
			Assert::IsTrue(spent_for_inst < std::chrono::seconds(1));

			// Free instantiated memory
			for (auto i = zx::zero::u64; i < instance_amount; ++i)
			{
				auto new_vector =
					std::unique_ptr<std::vector<zx::i32>>(
						static_cast<std::vector<zx::i32>*>(pointers[i]));

				Assert::IsNotNull(new_vector.get());
				Assert::AreEqual(size, new_vector->size());
			}

			ls << spent_for_inst / spent_for_new
				<< " times slower than new(). ";
		}
	};
}