/******************************************************************************
License: BSD Zero Clause License
Copyright:
	(C) 2008 - 2022 Alexander Shaduri <ashaduri@gmail.com>
******************************************************************************/
/// \file
/// \author Alexander Shaduri
/// \ingroup hz_tests
/// \weakgroup hz_tests
/// @{

// Catch2 v3
//#include "catch2/catch_test_macros.hpp"

// Catch2 v2
#include "catch2/catch.hpp"

// disable libdebug, we don't link to it
#undef HZ_USE_LIBDEBUG
#define HZ_USE_LIBDEBUG 0
// enable libdebug emulation through std::cerr
#undef HZ_EMULATE_LIBDEBUG
#define HZ_EMULATE_LIBDEBUG 1

// The first header should be then one we're testing, to avoid missing
// header pitfalls.
#include "hz/string_algo.h"

#include <vector>



/// Main function for the test
TEST_CASE("StringAlgorithms", "[hz][string]")
{
	using namespace hz;

	SECTION("string_split, single-character") {
		std::string s = "/aa/bbb/ccccc//dsada//";
		std::vector<std::string> result;
		string_split(s, '/', result, false);

		std::vector<std::string> expected = {
			"",
			"aa",
			"bbb",
			"ccccc",
			"",
			"dsada",
			"",
			""
		};
		REQUIRE(result == expected);
	}

	SECTION("string_split, single-character, skip empty") {
		std::string s = "/aa/bbb/ccccc//dsada//";
		std::vector<std::string> result;
		string_split(s, '/', result, true);

		REQUIRE(result == std::vector<std::string> {
			"aa",
			"bbb",
			"ccccc",
			"dsada",
		});
	}

	SECTION("string_split, multi-character") {
		std::string s = "//aa////bbb/ccccc//dsada////";
		std::vector<std::string> result;
		string_split(s, "//", result, false);

		REQUIRE(result == std::vector<std::string> {
			"",
			"aa",
			"",
			"bbb/ccccc",
			"dsada",
			"",
			"",
		});
	}

	SECTION("string_remove_adjacent_duplicates") {
		std::string s = "  a b bb  c     d   ";
		REQUIRE(string_remove_adjacent_duplicates_copy(s, ' ') == " a b bb c d ");
		REQUIRE(string_remove_adjacent_duplicates_copy(s, ' ', 2) == "  a b bb  c  d  ");
	}

	SECTION("string_replace single-character") {
		std::string s = "/a/b/c/dd//e/";
		string_replace(s, '/', ':');
		REQUIRE(s == ":a:b:c:dd::e:");
	}

	SECTION("string_replace multi-character") {
		std::string s = "112/2123412";
		string_replace(s, "12", "AB");
		REQUIRE(s == "1AB/2AB34AB");
	}

	SECTION("string_replace_array multi -> multi") {
		std::vector<std::string> from, to;

		from.emplace_back("12");
		from.emplace_back("abc");

		to.emplace_back("345");
		to.emplace_back("de");

		std::string s = "12345678abcdefg abc ab";
		string_replace_array(s, from, to);
		REQUIRE(s == "345345678dedefg de ab");
	}

	SECTION("string_replace_array multi -> single") {
		std::vector<std::string> from, to;

		from.emplace_back("12");
		from.emplace_back("abc");

		std::string s = "12345678abcdefg abc ab";
		string_replace_array(s, from, ":");
		REQUIRE(s == ":345678:defg : ab");
	}
}






/// @}
