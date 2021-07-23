# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "smart_pointer_checker.h"
# include "zx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace zx_test
{
	TEST_CLASS(RttiSmartPointersTest)
	{
	public:
		TEST_METHOD(CheckRttiTypeSize)
		{
			# ifdef _WIN64
			Assert::AreEqual(24ui64, sizeof(zx::rtti::weak_ptr));
			Assert::AreEqual(24ui64, sizeof(zx::rtti::shared_ptr));
			# else ifdef _WIN32
			Assert::AreEqual(12ui32, sizeof(zx::rtti::weak_ptr));
			Assert::AreEqual(12ui32, sizeof(zx::rtti::shared_ptr));
			# endif
		}

		TEST_METHOD(RttiWeakPointer_Instantiate)
		{
			zx::rtti::weak_ptr rtti_wpt;

			// Test rtti pointer
			Assert::IsFalse(rtti_wpt ? true : false);
			Assert::AreEqual(0ul, rtti_wpt.riff_count());
			Assert::AreEqual(0ul, rtti_wpt.weak_count());

			auto shr1 = std::make_shared<std::vector<int>>
			(
				std::initializer_list{ 1, 2, 3 }
			);

			rtti_wpt = shr1;
			Assert::IsTrue(rtti_wpt ? true : false);
			Assert::AreEqual(1ul, rtti_wpt.riff_count());
			Assert::AreEqual(2ul, rtti_wpt.weak_count());

			std::shared_ptr<std::vector<int>> shr2 = rtti_wpt;
			Assert::IsTrue(shr2 ? true : false);
			Assert::AreEqual(2l, shr2.use_count());
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
			# endif

			std::shared_ptr<std::vector<int>> shr3;
			rtti_wpt.write_to(reinterpret_cast<void**>(&shr3));
			Assert::IsTrue(shr3 ? true : false);
			Assert::AreEqual(3l, shr3.use_count());
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr3->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr3->size());
			# endif
		}

		TEST_METHOD(TmplWeakPointer_OutOfScope)
		{
			smart_pointer_checker checker;

			{
				auto shr = std::make_shared<std::vector<int>>
				(
					std::initializer_list<int>{ 1, 2, 3 }
				);

				checker.look_at(shr);

				Assert::IsTrue(shr ? true : false);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);

				{
					std::weak_ptr<std::vector<int>> wkp1 = shr;

					Assert::AreEqual(1ul, checker.counter->riffs);
					Assert::AreEqual(2ul, checker.counter->weaks);

					{
						std::weak_ptr<std::vector<int>> wkp2 = shr;

						Assert::AreEqual(1ul, checker.counter->riffs);
						Assert::AreEqual(3ul, checker.counter->weaks);
					}

					Assert::AreEqual(1ul, checker.counter->riffs);
					Assert::AreEqual(2ul, checker.counter->weaks);
				}

				Assert::IsTrue(shr ? true : false);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);
			}

			// UB in release build
			# ifdef _DEBUG
			Assert::AreNotEqual(0ul, checker.counter->riffs);
			Assert::AreNotEqual(0ul, checker.counter->weaks);
			# endif 
		}

		TEST_METHOD(RttiWeakPointer_OutOfScope)
		{
			// Instantiate empty rtti pointer
			smart_pointer_checker checker;

			{
				auto shr1 = std::make_shared<std::vector<int>>
				(
					std::initializer_list<int>{ 1, 2, 3 }
				);
				
				checker.look_at(shr1);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);

				zx::rtti::weak_ptr rtti_wpt1 = shr1;
				Assert::IsTrue(rtti_wpt1 ? true : false);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(2ul, checker.counter->weaks);

				{
					zx::rtti::weak_ptr rtti_wpt2 = shr1;
					Assert::IsTrue(rtti_wpt1 ? true : false);
					Assert::AreEqual(1ul, checker.counter->riffs);
					Assert::AreEqual(3ul, checker.counter->weaks);

					{
						zx::rtti::weak_ptr rtti_wpt3 = rtti_wpt1;

						Assert::IsTrue(rtti_wpt1 ? true : false);
						Assert::AreEqual(1ul, checker.counter->riffs);
						Assert::AreEqual(4ul, checker.counter->weaks);
					}

					Assert::IsTrue(rtti_wpt1 ? true : false);
					Assert::AreEqual(1ul, checker.counter->riffs);
					Assert::AreEqual(3ul, checker.counter->weaks);
				}
			}

			// UB in release build
			# ifdef _DEBUG
			Assert::AreNotEqual(0ul, checker.counter->riffs);
			Assert::AreNotEqual(0ul, checker.counter->weaks);
			# endif 
		}

		TEST_METHOD(RttiWeakPointer_CopyMoveAssign)
		{
			zx::rtti::weak_ptr rtti_wpt1;
			zx::rtti::weak_ptr rtti_wpt2;

			auto shr0 = std::make_shared<std::vector<int>>
			(
				std::initializer_list<int>{ 1, 2, 3 }
			);

			// INSTANTIATE RTTI POINTER 1
			rtti_wpt1 = shr0;

			// Test rtti pointer 1
			Assert::IsTrue(rtti_wpt1 ? true : false);
			Assert::AreEqual(1ul, rtti_wpt1.riff_count());
			Assert::AreEqual(2ul, rtti_wpt1.weak_count());

			// INSTANTIATE RTTI POINTER 2
			rtti_wpt2 = shr0;

			// Test rtti pointer 2
			Assert::IsTrue(rtti_wpt2 ? true : false);
			Assert::AreEqual(1ul, rtti_wpt2.riff_count());
			Assert::AreEqual(3ul, rtti_wpt2.weak_count());

			// INSTANTIATE SHARED POINTER 1
			std::shared_ptr<std::vector<int>> shr1 = rtti_wpt1;

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
			# endif
			Assert::AreEqual(2l, shr1.use_count());

			// Test rtti pointers
			Assert::AreEqual(2ul, rtti_wpt1.riff_count());
			Assert::AreEqual(3ul, rtti_wpt1.weak_count());
			Assert::AreEqual(2ul, rtti_wpt2.riff_count());
			Assert::AreEqual(3ul, rtti_wpt2.weak_count());

			// INSTANTIATE SHARED POINTER 2
			std::shared_ptr<std::vector<int>> shr2 = rtti_wpt2;

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

			// Test rtti pointers
			Assert::AreEqual(3ul, rtti_wpt1.riff_count());
			Assert::AreEqual(3ul, rtti_wpt1.weak_count());
			Assert::AreEqual(3ul, rtti_wpt2.riff_count());
			Assert::AreEqual(3ul, rtti_wpt2.weak_count());

			// ASSIGN TO EACH OTHER
			rtti_wpt2 = rtti_wpt1;

			// Test rtti pointers
			Assert::AreEqual(3ul, rtti_wpt1.riff_count());
			Assert::AreEqual(3ul, rtti_wpt1.weak_count());
			Assert::AreEqual(3ul, rtti_wpt2.riff_count());
			Assert::AreEqual(3ul, rtti_wpt2.weak_count());

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

		TEST_METHOD(RttiSharedPointer_Instantiate)
		{
			auto shr1 = std::make_shared<std::vector<int>>
			(
				std::initializer_list<int>{ 1, 2, 3 }
			);

			zx::rtti::shared_ptr rtti_shr = shr1;
			Assert::IsTrue(rtti_shr ? true : false);
			Assert::AreEqual(2ul, rtti_shr.riff_count());
			Assert::AreEqual(1ul, rtti_shr.weak_count());

			std::shared_ptr<std::vector<int>> shr2 = rtti_shr;
			Assert::IsTrue(shr2 ? true : false);
			Assert::AreEqual(3l, shr2.use_count());
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
			# endif

			auto shr3 = std::make_shared<std::vector<int>>();
			rtti_shr.write_to(reinterpret_cast<void**>(&shr3));
			Assert::IsTrue(shr3 ? true : false);
			Assert::AreEqual(4l, shr3.use_count());
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr3->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr3->size());
			# endif
		}

		TEST_METHOD(TmplSharedPointer_OutOfScope)
		{
			smart_pointer_checker checker;

			{
				auto shr1 = std::make_shared<std::vector<int>>
				(
					std::initializer_list<int>{ 1, 2, 3 }
				);

				checker.look_at(shr1);

				Assert::IsTrue(shr1 ? true : false);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);

				{
					auto shr2 = shr1;

					Assert::AreEqual(2ul, checker.counter->riffs);
					Assert::AreEqual(1ul, checker.counter->weaks);

					{
						auto shr3 = shr1;

						Assert::AreEqual(3ul, checker.counter->riffs);
						Assert::AreEqual(1ul, checker.counter->weaks);
					}

					Assert::AreEqual(2ul, checker.counter->riffs);
					Assert::AreEqual(1ul, checker.counter->weaks);
				}

				Assert::IsTrue(shr1 ? true : false);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);
			}

			// UB in release build
			# ifdef _DEBUG
			Assert::AreNotEqual(0ul, checker.counter->riffs);
			Assert::AreNotEqual(0ul, checker.counter->weaks);
			# endif 
		}

		TEST_METHOD(RttiSharedPointer_OutOfScope)
		{
			// Instantiate empty rtti pointer
			smart_pointer_checker checker;

			{
				auto shr1 = std::make_shared<std::vector<int>>
				(
					std::initializer_list<int>{ 1, 2, 3 }
				);

				checker.look_at(shr1);
				Assert::AreEqual(1ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);

				zx::rtti::shared_ptr rtti_shr1 = shr1;
				Assert::IsTrue(rtti_shr1 ? true : false);
				Assert::AreEqual(2ul, checker.counter->riffs);
				Assert::AreEqual(1ul, checker.counter->weaks);

				{
					zx::rtti::shared_ptr rtti_shr2 = shr1;
					Assert::IsTrue(rtti_shr1 ? true : false);
					Assert::AreEqual(3ul, checker.counter->riffs);
					Assert::AreEqual(1ul, checker.counter->weaks);

					{
						zx::rtti::shared_ptr rtti_shr3 = rtti_shr1;

						Assert::IsTrue(rtti_shr1 ? true : false);
						Assert::AreEqual(4ul, checker.counter->riffs);
						Assert::AreEqual(1ul, checker.counter->weaks);
					}

					Assert::IsTrue(rtti_shr1 ? true : false);
					Assert::AreEqual(3ul, checker.counter->riffs);
					Assert::AreEqual(1ul, checker.counter->weaks);
				}
			}

			// UB in release build
			# ifdef _DEBUG
			Assert::AreNotEqual(0ul, checker.counter->riffs);
			Assert::AreNotEqual(0ul, checker.counter->weaks);
			# endif 
		}

		TEST_METHOD(RttiSharedPointer_CopyMoveAssign)
		{
			zx::rtti::shared_ptr rtti_shr1;
			zx::rtti::shared_ptr rtti_shr2;

			auto shr0 = std::make_shared<std::vector<int>>
			(
				std::initializer_list<int>{ 1, 2, 3 }
			);

			// INSTANTIATE RTTI POINTER 1
			rtti_shr1 = shr0;

			// Test rtti pointer 1
			Assert::IsTrue(rtti_shr1 ? true : false);
			Assert::AreEqual(2ul, rtti_shr1.riff_count());
			Assert::AreEqual(1ul, rtti_shr1.weak_count());

			// INSTANTIATE RTTI POINTER 2
			rtti_shr2 = shr0;

			// Test rtti pointer 2
			Assert::IsTrue(rtti_shr2 ? true : false);
			Assert::AreEqual(3ul, rtti_shr2.riff_count());
			Assert::AreEqual(1ul, rtti_shr2.weak_count());

			// INSTANTIATE SHARED POINTER 1
			std::shared_ptr<std::vector<int>> shr1 = rtti_shr1;

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
			# endif
			Assert::AreEqual(4l, shr1.use_count());

			// Test rtti pointers
			Assert::AreEqual(4ul, rtti_shr1.riff_count());
			Assert::AreEqual(1ul, rtti_shr1.weak_count());
			Assert::AreEqual(4ul, rtti_shr2.riff_count());
			Assert::AreEqual(1ul, rtti_shr2.weak_count());

			// INSTANTIATE SHARED POINTER 2
			std::shared_ptr<std::vector<int>> shr2 = rtti_shr2;

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
			# endif
			Assert::AreEqual(5l, shr1.use_count());

			// Test shared pointer 2
			Assert::IsTrue(shr2 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
			# endif
			Assert::AreEqual(5l, shr2.use_count());

			// Test rtti pointers
			Assert::AreEqual(5ul, rtti_shr1.riff_count());
			Assert::AreEqual(1ul, rtti_shr1.weak_count());
			Assert::AreEqual(5ul, rtti_shr2.riff_count());
			Assert::AreEqual(1ul, rtti_shr2.weak_count());

			// ASSIGN TO EACH OTHER
			rtti_shr2 = rtti_shr1;

			// Test rtti pointers
			Assert::AreEqual(5ul, rtti_shr1.riff_count());
			Assert::AreEqual(1ul, rtti_shr1.weak_count());
			Assert::AreEqual(5ul, rtti_shr2.riff_count());
			Assert::AreEqual(1ul, rtti_shr2.weak_count());

			// Test shared pointer 1
			Assert::IsTrue(shr1 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr1->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr1->size());
			# endif
			Assert::AreEqual(5l, shr1.use_count());

			// Test shared pointer 2
			Assert::IsTrue(shr2 ? true : false);
			# ifdef _WIN64
			Assert::AreEqual(3ui64, shr2->size());
			# else ifdef _WIN32
			Assert::AreEqual(3ui32, shr2->size());
			# endif
			Assert::AreEqual(5l, shr2.use_count());
		}
	};
}