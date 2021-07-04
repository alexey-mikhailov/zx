# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"
# include "property_user_1.h"
# include "property_user_2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace zx_test
{
	TEST_CLASS(PropertyTest)
	{
	public:
		TEST_METHOD(SimplePropertyTest)
		{
			property_user_1 propertyUser;
			Assert::AreEqual(*propertyUser.Order, 1234);
			Assert::AreEqual(*propertyUser.Balance, 5.0f);
			Assert::AreEqual(*propertyUser.InflationIndex, 1.0);

			propertyUser.Balance = 8.0f;
			Assert::AreEqual(*propertyUser.Balance, 8.0f);

			propertyUser.InflationIndex = 1.4;
			Assert::AreEqual(*propertyUser.InflationIndex, 2.8);
		}

		TEST_METHOD(MoreComplicatedPropertyTest)
		{
			property_user_2 u1({ 2, 2 });
			property_user_2 u2({ 5, 5 });

			// Value getter test
			// operator *
			auto ro_val1 = *u1.ro_val1;
			// operator convert
			int ro_val1_explicit = u1.ro_val1;
			// operator ->
			ls << "ro_val2: " << u1.ro_val2->x << " " << u1.ro_val2->y << zx::endl;

			// Value wrapper test
			ls << "property type is: " << typeid(u2.rw_val).name() << zx::endl;
			// operator *
			auto rw_val = *u2.rw_val;
			// operator convert
			val_type rw_val_explicit = u2.rw_val;
			// operator ->
			ls << "rw_val before modification: " << u2.rw_val->x << zx::endl; // read9
			// operator =
			u2.rw_val = { 6, 6 }; // write
			ls << "rw_val after modification: " << u2.rw_val->x << zx::endl; // read again

			// Value formula test
			// operator *
			auto diff = *u2.diff;
			// operator convert
			val_type diff_explicit = u2.diff;
			// operator ->
			ls << "diff x value: " << u2.diff->x << zx::endl;

			// Value computer test
			// operator *
			auto sum = *u2.sum;
			// operator convert
			val_type sum_explicit = u2.sum;
			// operator ->
			ls << "sum before modification: " << u2.sum->x << zx::endl; // read
			// operator =
			u2.sum = { 6, 6 }; // write
			ls << "sum after modification: " << u2.sum->x << zx::endl; // read again



			// Reference getter test
			// operator *
			auto& ro_ref1 = *u2.ro_ref1;
			// operator convert
			ref_type& ro_ref1_explicit_ref = u2.ro_ref1;
			ref_type* ro_ref1_explicit_ptr = u2.ro_ref1;
			// operator ->
			ls << "ro_ref1: " << u1.ro_ref1->a << zx::endl;

			// Reference wrapper test
			// operator *
			auto& rw_ref1 = *u2.rw_ref1;
			// operator convert
			ref_type& rw_ref_Explicit_ref = u2.rw_ref1;
			ref_type* rw_ref_explicit_ptr = u2.rw_ref1;
			// operator =
			ls << "rw_ref1 before modification: " << &*u2.rw_ref1 << zx::endl; // read
			u2.rw_ref1 = new ref_type(321); // write
			ls << "rw_ref1 after modification: " << &*u2.rw_ref1 << zx::endl; // read again
			// operator ->
			auto a = u2.rw_ref1->a;

			// Refrerence fornula test
			// operator *
			auto ro_ref2 = *u2.ro_ref2;
			// operator convert
			ref_type& ro_ref2_explicit_ref = u2.ro_ref2;
			ref_type& ro_ref2_explicit_ptr = u2.ro_ref2;
			// operator ->
			ls << "ro_ref2: " << &*u2.ro_ref2 << zx::endl; // read

			// Reference computer test
			// operator *
			auto rw_ref2 = *u2.rw_ref2;
			// operator convert
			ref_type& rw_ref2_explicit_ref = u2.rw_ref2;
			ref_type* re_ref2_explicit_ptr = u2.rw_ref2;
			// operator =
			ls << "rw_ref2 before modification: " << &*u2.rw_ref2 << zx::endl; // read
			u2.rw_ref2 = new ref_type(321); // write
			ls << "rw_ref2 after modification: " << &*u2.rw_ref2 << zx::endl; // read again
			// operator ->
			a = u2.rw_ref2->a;



			// Shared ptr getter test
			// operator *
			auto ro_shr1 = *u2.ro_shr1;
			// operator convert
			std::shared_ptr<ref_type> ro_shr1_explicit = u2.ro_shr1;
			// operator ->
			ls << "ro_ref1: " << u1.ro_shr1->a << zx::endl;

			// Shared ptr wrapper test
			// operator *
			auto rw_shr = *u2.rw_shr1;
			// operator convert
			std::shared_ptr<ref_type> rw_shr1_explicit = u2.rw_shr1;
			// operator =
			ls << "rw_shr1 before modification: " << &**u2.rw_shr1 << zx::endl; // read
			u2.rw_shr1 = std::make_shared<ref_type>(321); // write
			// operator ->
			ls << "rw_shr1 after modification: " << &**u2.rw_shr1 << zx::endl; // read again

			// Shared ptr formula test
			// operator *
			auto ro_shr2 = *u2.ro_shr2;
			// operator convert
			std::shared_ptr<ref_type> ro_shr2_explicit = u2.ro_shr2;
			// operator ->
			ls << "ro_shr2.a: " << u2.ro_shr2->a << zx::endl; // read

			// Reference computer test
			// operator *
			auto rw_shr2 = *u2.rw_shr2;
			// operator convert
			std::shared_ptr<ref_type> rw_shr2_explicit = u2.rw_shr2;
			// operator =
			ls << "rw_shr2 before modification: " << &**u2.rw_shr2 << zx::endl; // read
			u2.rw_shr2 = std::make_shared<ref_type>(321); // write
			ls << "rw_shr2 after modification: " << &**u2.rw_shr2 << zx::endl; // read again
			// operator ->
			a = u2.rw_shr2->a;
		}
	};
}
