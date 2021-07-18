# include "CppUnitTest.h"
# include "../logger_stream.h"
# include "zx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace zx_test
{
	TEST_CLASS(IterablesTest)
	{
	public:
		TEST_METHOD(IterateIterables)
		{
			int checker;

			std::vector<int> v { 1, 2, 3, 4, 5 };

			const std::vector<int>& rv = v;

			auto citbl = zx::iterable::imm::generic(rv);
			checker = 1;
			for (auto&& item : citbl)
			{
				Assert::AreEqual(checker++, item);
			}

			auto itbl = zx::iterable::mut::generic(v);
			checker = 1;
			for (auto&& item : itbl)
			{
				Assert::AreEqual(checker++, item);
			}

			auto vtrg = zx::iterable::mut::vector<int>(v);
			checker = 2;
			for (auto&& item : vtrg)
			{
				++item;
				Assert::AreEqual(checker++, item);
			}

			checker = 2;
			auto vcrg = zx::iterable::imm::vector<int>(rv);
			for (auto&& item : vcrg)
			{
				Assert::AreEqual(checker++, item);
			}

			std::unordered_map<std::string, int> um
			{
				std::make_pair("1", 1),
				std::make_pair("2", 10),
				std::make_pair("3", 100),
				std::make_pair("4", 1000),
				std::make_pair("5", 10000)
			};

			checker = 1;
			auto umitbl = zx::iterable::imm::unordered_map<std::string, int>(um);
			for (auto&& [key, value] : umitbl)
			{
				Assert::AreEqual(checker, value);
				checker *= 10;
			}
		}

		TEST_METHOD(IterateRanges)
		{
			int checker;

			std::vector<int> v { 1, 2, 3, 4, 5 };
			auto begin = v.begin() + 1;
			auto end = v.end() - 1;

			const std::vector<int>& rv = v;
			auto cbegin = rv.begin() + 1;
			auto cend = rv.end() - 1;

			auto citrg = zx::ranges::generic(cbegin, cend);
			checker = 2;
			for (auto&& item : citrg)
			{
				Assert::AreEqual(checker++, item);
			}

			auto itrg = zx::ranges::generic(begin, end);
			checker = 2;
			for (auto&& item : itrg)
			{
				Assert::AreEqual(checker++, item);
			}

			citrg = zx::ranges::imm::vector<int>(cbegin, cend);
			checker = 2;
			for (auto&& item : citrg)
			{
				Assert::AreEqual(checker++, item);
			}

			itrg = zx::ranges::mut::vector<int>(begin, end);
			checker = 2;
			for (auto&& item : itrg)
			{
				Assert::AreEqual(checker++, item);
			}

			std::unordered_map<std::string, int> um
			{
				std::make_pair("1", 1),
				std::make_pair("2", 10),
				std::make_pair("3", 100),
				std::make_pair("4", 1000),
				std::make_pair("5", 10000)
			};

			checker = 1;
			auto umitrg = zx::ranges::generic(um.begin(), um.end());
			for (auto&& [key, value] : umitrg)
			{
				Assert::AreEqual(checker, value);
				checker *= 10;
			}

			checker = 1;
			umitrg = zx::ranges::mut::unordered_map<std::string, int>(um.begin(), um.end());
			for (auto&& [key, value] : umitrg)
			{
				Assert::AreEqual(checker, value);
				checker *= 10;
			}
		}
	};
}
