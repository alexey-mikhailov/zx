# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"
# include "property_user_1.h"
# include "property_user_2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<ref_type>(
				const ref_type& instance) 
			{
				auto a = &instance ? instance.a : 0;

				std::wstringstream ss;
				ss << L"ref_type { a : " << a << L" } ";

				return ss.str(); 
			}
		}
	}
}

namespace zx_test
{
	TEST_CLASS(PropertyTest)
	{
	public:
		TEST_METHOD(SimplePropertyTest)
		{
			property_user_1 user;
			Assert::AreEqual(property_user_order,
							 *user.order);
			Assert::AreEqual(property_user_balance,
							 *user.balance);
			Assert::AreEqual(property_user_inflation_index,
							 *user.inflation_index);

			constexpr float new_balance = 8.0f;
			user.balance = new_balance;
			Assert::AreEqual(new_balance, *user.balance);

			constexpr double new_index = 1.4;
			user.inflation_index = new_index;
			Assert::AreEqual(2.0 * new_index,
							 *user.inflation_index);
		}

		TEST_METHOD(ValueGetterTest)
		{
			constexpr zx::i32 val_arg = 2i32;
			property_user_2 u({ val_arg, val_arg });

			Assert::AreEqual(property_user_ro_i32,
							 *u.ro_i32);

			// operator *
			auto ro_i32 = *u.ro_i32;
			Assert::AreEqual(property_user_ro_i32,
							 ro_i32);

			// operator convert
			zx::i32 ro_i32_explicit = u.ro_i32;
			Assert::AreEqual(property_user_ro_i32,
							 ro_i32_explicit);

			// operator ->
			Assert::AreEqual(val_arg, u.ro_val->x);
			Assert::AreEqual(val_arg, u.ro_val->y);
		}

		TEST_METHOD(ValueWrapperTest)
		{
			constexpr zx::i32 val_arg = 5i32;
			property_user_2 u({ val_arg, val_arg });

			// operator *
			auto rw_val = *u.rw_val;
			Assert::AreEqual(val_type_x, rw_val.x);

			// operator convert
			val_type rw_val_explicit = u.rw_val;
			Assert::AreEqual(val_type_x, rw_val_explicit.x);

			// operator ->
			Assert::AreEqual(val_type_x, u.rw_val->x);

			// operator =
			constexpr zx::i32 new_val_arg = 6i32;
			u.rw_val = { new_val_arg, new_val_arg }; // write
			Assert::AreEqual(new_val_arg, u.rw_val->x);
		}

		TEST_METHOD(ValueFormulaTest)
		{
			constexpr zx::i32 val_arg = 5i32;
			property_user_2 u({ val_arg, val_arg });

			auto expected = val_type_x - val_arg;

			// operator *
			auto diff = *u.diff;
			Assert::AreEqual(expected, diff.x);

			// operator convert
			val_type diff_explicit = u.diff;
			Assert::AreEqual(expected, diff_explicit.x);

			// operator ->
			Assert::AreEqual(expected, u.diff->x);
		}

		TEST_METHOD(ValueComputer)
		{
			constexpr zx::i32 val_arg = 5i32;
			property_user_2 u({ val_arg, val_arg });

			// operator *
			auto sum = *u.sum;
			Assert::AreEqual(val_type_x + val_arg,
							 sum.x);

			// operator convert
			val_type sum_explicit = u.sum;
			Assert::AreEqual(val_type_x + val_arg,
							 sum_explicit.x);

			// operator ->
			Assert::AreEqual(val_type_x + val_arg,
							 u.sum->x);
			
			// operator = (write to property)
			constexpr zx::i32 new_val = 6i32;
			u.sum = { new_val, new_val }; 

			Assert::AreEqual(new_val, u.sum->x);
		}

		TEST_METHOD(ReferenceGetterTest)
		{
			property_user_2 u;

			// operator *
			auto& ref = *u.ref_getter;
			Assert::AreSame(*u.ref_getter,
							ref);
			
			// operator convert to type&
			ref_type& explicit_ref = u.ref_getter;
			Assert::AreSame(*u.ref_getter,
							explicit_ref);

			// operator convert to type*
			ref_type* explicit_ptr = u.ref_getter;
			Assert::AreSame(*u.ref_getter,
							*explicit_ptr);

			// operator ->
			Assert::AreEqual(property_user_ref_arg,
							 u.ref_getter->a);
		}

		TEST_METHOD(ReferenceWrapperTest)
		{
			property_user_2 u;

			// operator *
			auto& ref = *u.ref_wrapper;
			Assert::AreSame(*u.ref_wrapper,
							ref);

			// operator convert to type&
			ref_type& explicit_ref = u.ref_wrapper;
			Assert::AreSame(*u.ref_wrapper,
							explicit_ref);

			// operator convert to type*
			ref_type* explicit_ptr = u.ref_wrapper;
			Assert::AreSame(*u.ref_wrapper,
							*explicit_ptr);

			// operator =
			auto& old_ref = *u.ref_wrapper;
			constexpr zx::i32 new_val = 123i32;
			u.ref_wrapper = new ref_type(new_val); 
			Assert::AreNotSame(old_ref, *u.ref_wrapper);

			// operator ->
			Assert::AreEqual(new_val,
							 u.ref_wrapper->a);
		}

		TEST_METHOD(ReferenceFormulaTest)
		{
			property_user_2 u;

			// operator *
			auto& ref = *u.ref_formula;
			Assert::AreSame(*u.ref_formula,
							ref);

			// operator convert to type&
			ref_type& explicit_ref = u.ref_formula;
			Assert::AreSame(*u.ref_formula,
							explicit_ref);

			// operator convert to type*
			ref_type* explicit_ptr = u.ref_formula;
			Assert::AreSame(*u.ref_formula,
							*explicit_ptr);

			// operator ->
			Assert::AreEqual(zx::zero::i32,
							 u.ref_formula->a);
		}

		TEST_METHOD(ReferenceComputerTest)
		{
			property_user_2 u;

			// operator *
			auto& ref = *u.ref_computer;
			Assert::AreSame(*u.ref_computer,
							ref);

			// operator convert to type&
			ref_type& explicit_ref = u.ref_computer;
			Assert::AreSame(*u.ref_computer,
							explicit_ref);

			// operator convert to type*
			ref_type* explicit_ptr = u.ref_computer;
			Assert::AreSame(*u.ref_computer,
							*explicit_ptr);

			// operator =
			auto& old_ref = *u.ref_computer;
			constexpr zx::i32 new_val = 123i32;
			u.ref_computer = new ref_type(new_val); 
			Assert::AreNotSame(old_ref, *u.ref_computer);

			// operator ->
			Assert::AreEqual(new_val,
							 u.ref_computer->a);
		}

		TEST_METHOD(UniquePointerGetterTest)
		{
			property_user_2 u;
			
			// operator *
			auto& ref = *u.unq_getter;
			Assert::AreSame(*u.unq_getter,
							ref);

			// operator convert to type&
			ref_type& explicit_ref = u.unq_getter;
			Assert::AreSame(*u.unq_getter,
							explicit_ref);

			// operator convert to type*
			ref_type* explicit_ptr = u.unq_getter;
			Assert::AreSame(*u.unq_getter,
							*explicit_ptr);

			// operator ->
			Assert::AreEqual(property_user_unq_arg,
							 u.unq_getter->a);
		}

		TEST_METHOD(SharedPointerGetterTest)
		{
			property_user_2 u;

			// operator *
			auto shr = *u.shr_getter;
			Assert::AreSame(**u.shr_getter,
							*shr);

			// operator convert to std::shared_pointer
			std::shared_ptr<ref_type> explicit_shr = u.shr_getter;
			Assert::AreSame(**u.shr_getter,
							*explicit_shr);

			// operator ->
			Assert::AreEqual(property_user_shr_arg,
							 u.shr_getter->a);
		}

		TEST_METHOD(SharedPointerWrapperTest)
		{
			property_user_2 u;

			// operator *
			auto shr = *u.shr_wrapper;
			Assert::AreSame(**u.shr_wrapper,
							*shr);

			// operator convert to std::shared_pointer
			std::shared_ptr<ref_type> explicit_shr = u.shr_wrapper;
			Assert::AreSame(**u.shr_wrapper,
							*explicit_shr);

			// operator =
			constexpr zx::i32 new_val = 321i32;
			u.shr_wrapper = std::make_shared<ref_type>(new_val);
			Assert::AreNotSame(*shr, **u.shr_wrapper);

			// operator ->
			Assert::AreEqual(new_val,
							 u.shr_wrapper->a);
		}

		TEST_METHOD(SharedPointerFormulaTest)
		{
			property_user_2 u;
			
			// operator *
			auto shr = *u.shr_formula;
			Assert::AreSame(**u.shr_formula,
							*shr);

			// operator convert to std::shared_pointer
			std::shared_ptr<ref_type> explicit_shr = u.shr_formula;
			Assert::AreSame(**u.shr_formula,
							*explicit_shr);

			// operator ->
			Assert::AreEqual(property_user_shr_arg,
							 u.shr_formula->a);
		}

		TEST_METHOD(SharedPointerComputerTest)
		{
			property_user_2 u;

			// operator *
			auto shr = *u.shr_computer;
			Assert::AreSame(**u.shr_computer,
							*shr);

			// operator convert to std::shared_pointer
			std::shared_ptr<ref_type> explicit_shr = u.shr_computer;
			Assert::AreSame(**u.shr_computer,
							*explicit_shr);

			// operator =
			constexpr zx::i32 new_val = 321i32;
			u.shr_computer = std::make_shared<ref_type>(new_val);
			Assert::AreNotSame(*shr, **u.shr_computer);

			// operator ->
			Assert::AreEqual(new_val,
							 u.shr_computer->a);
		}
	};
}
