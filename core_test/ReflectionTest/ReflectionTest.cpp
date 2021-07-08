# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace zx_test
{
	TEST_CLASS(ReflectionTest)
	{
	public:
		TEST_METHOD(CheckRttiTypeSize)
		{
# ifdef _WIN64
			Assert::AreEqual(40ui64, sizeof(zx::type));
			Assert::AreEqual(16ui64, sizeof(zx::rtti::weak_ptr));
# else ifdef _WIN32
			Assert::AreEqual(20ui32, sizeof(zx::type));
			Assert::AreEqual( 8ui32, sizeof(zx::rtti::weak_ptr));
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

		TEST_METHOD(RttiWeakPointer_SimpleTest)
		{
			auto shr1 = std::make_shared<std::vector<int>>
			(
				std::initializer_list<int>{ 1, 2, 3 }
			);

			auto rtti_wpt = zx::rtti::weak_ptr::from(shr1);
			Assert::IsTrue(rtti_wpt ? true : false);

			auto shr2 = zx::rtti::weak_ptr::to_shared_ptr<std::vector<int>>(rtti_wpt);
			Assert::IsTrue(shr2 ? true : false);
			Assert::AreEqual(2l, shr2.use_count());
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
# endif
		}

		TEST_METHOD(RttiWeakPointer_OutOfScopeTest)
		{
			// Instantiate empty weak pointer
			zx::rtti::weak_ptr rtti_wpt;

			// Test weak pointer
			Assert::IsFalse(rtti_wpt ? true : false);
			Assert::AreEqual(0ul, rtti_wpt.riff_count());
			Assert::AreEqual(0ul, rtti_wpt.weak_count());

			{
				auto shr1 = std::make_shared<std::vector<int>>
				(
					std::initializer_list<int>{ 1, 2, 3 }
				);

				rtti_wpt = zx::rtti::weak_ptr::from(shr1);
			}

			Assert::IsFalse(rtti_wpt ? true : false);
			Assert::AreEqual(0ul, rtti_wpt.riff_count());
			Assert::AreEqual(1ul, rtti_wpt.weak_count());

			// Ref counter is valid, but data lost. 
			auto shr2 = zx::rtti::weak_ptr::to_shared_ptr<std::vector<int>>(rtti_wpt);
			Assert::IsFalse(shr2 ? true : false);
			Assert::AreEqual(0l, shr2.use_count());
		}

		TEST_METHOD(RttiWeakPointer_CopyMoveAssignTest)
		{
			zx::rtti::weak_ptr rtti_wpt1;
			zx::rtti::weak_ptr rtti_wpt2;

			auto shr0 = std::make_shared<std::vector<int>>
			(
				std::initializer_list<int>{ 1, 2, 3 }
			);

			// INSTANTIATE WEAK POINTER 1
			rtti_wpt1 = zx::rtti::weak_ptr::from(shr0);

			// Test weak pointer 1
			Assert::IsTrue(rtti_wpt1 ? true : false);
			Assert::AreEqual(1ul, rtti_wpt1.riff_count());
			Assert::AreEqual(2ul, rtti_wpt1.weak_count());

			// INSTANTIATE WEAK POINTER 2
			rtti_wpt2 = zx::rtti::weak_ptr::from(shr0);

			// Test weak pointer 2
			Assert::IsTrue(rtti_wpt2 ? true : false);
			Assert::AreEqual(1ul, rtti_wpt2.riff_count());
			Assert::AreEqual(3ul, rtti_wpt2.weak_count());

			// INSTANTIATE SHARED POINTER 1
			auto shr1 = zx::rtti::weak_ptr::to_shared_ptr<std::vector<int>>(rtti_wpt1);

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
# endif
			Assert::AreEqual(2l, shr1.use_count());

			// Test RTTI weak pointers
			Assert::AreEqual(2ul, rtti_wpt1.riff_count());
			Assert::AreEqual(4ul, rtti_wpt1.weak_count());
			Assert::AreEqual(2ul, rtti_wpt2.riff_count());
			Assert::AreEqual(4ul, rtti_wpt2.weak_count());

			// INSTANTIATE SHARED POINTER 2
			auto shr2 = zx::rtti::weak_ptr::to_shared_ptr<std::vector<int>>(rtti_wpt2);

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
# endif
			Assert::AreEqual(3l, shr1.use_count());

			// Test shared pointer 2
			Assert::IsTrue(shr2 ? true : false);
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
# endif
			Assert::AreEqual(3l, shr2.use_count());

			// Test RTTI weak pointers
			Assert::AreEqual(3ul, rtti_wpt1.riff_count());
			Assert::AreEqual(5ul, rtti_wpt1.weak_count());
			Assert::AreEqual(3ul, rtti_wpt2.riff_count());
			Assert::AreEqual(5ul, rtti_wpt2.weak_count());

			// ASSIGN TO EACH OTHER
			rtti_wpt2 = rtti_wpt1;

			// Test RTTI weak pointers
			Assert::AreEqual(3ul, rtti_wpt1.riff_count());
			Assert::AreEqual(5ul, rtti_wpt1.weak_count());
			Assert::AreEqual(3ul, rtti_wpt2.riff_count());
			Assert::AreEqual(5ul, rtti_wpt2.weak_count());

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
# endif
			Assert::AreEqual(3l, shr1.use_count());

			// Test shared pointer 2
			Assert::IsTrue(shr2 ? true : false);
# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
# endif
			Assert::AreEqual(3l, shr2.use_count());
		}
	};
}