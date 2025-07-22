#include "test_validate.h"

void checkStrings() {
	assert(LETTER.size() == 2*26);
	assert(LOWER.size() == 26);
	assert(UPPER.size() == 26);
	for (int i = 0; i < 26; i++) {
		assert(LOWER[i] == 'a' + i);
		assert(UPPER[i] == 'A' + i);
		assert(toLower(UPPER[i]) == LOWER[i]);
		assert(toUpper(LOWER[i]) == UPPER[i]);
	}

	assert(VOWEL.size() == 2*5);
	assert(LOWER_VOWELS.size() == 5);
	assert(UPPER_VOWELS.size() == 5);
	for (int i = 0; i < 5; i++) {
		assert(toLower(UPPER_VOWELS[i]) == LOWER_VOWELS[i]);
		assert(toUpper(LOWER_VOWELS[i]) == UPPER_VOWELS[i]);
	}

	assert(CONSONANT.size() == 2*(26-5));
	assert(LOWER_CONSONANT.size() == 26-5);
	assert(UPPER_CONSONANT.size() == 26-5);
	for (int i = 0; i < 26-5; i++) {
		assert(toLower(UPPER_CONSONANT[i]) == LOWER_CONSONANT[i]);
		assert(toUpper(LOWER_CONSONANT[i]) == UPPER_CONSONANT[i]);
	}

	assert(ALPHA_NUMERIC.size() == 10+2*26);
	assert(LOWER_ALPHA_NUMERIC.size() == 10+26);
	assert(UPPER_ALPHA_NUMERIC.size() == 10+26);
	for (int i = 0; i < 10+26; i++) {
		assert(toLower(UPPER_ALPHA_NUMERIC[i]) == LOWER_ALPHA_NUMERIC[i]);
		assert(toUpper(LOWER_ALPHA_NUMERIC[i]) == UPPER_ALPHA_NUMERIC[i]);
	}

	assert(DIGITS.size() == 10);
	for (int i = 0; i < 10; i++) {
		assert(toLower(DIGITS[i]) == DIGITS[i]);
		assert(toUpper(DIGITS[i]) == DIGITS[i]);
	}

	assert(isLower(LOWER));
	assert(isLower(LOWER_VOWELS));
	assert(isLower(LOWER_CONSONANT));
	assert(!isUpper(LOWER));
	assert(!isUpper(LOWER_VOWELS));
	assert(!isUpper(LOWER_CONSONANT));

	assert(isUpper(UPPER));
	assert(isUpper(UPPER_VOWELS));
	assert(isUpper(UPPER_CONSONANT));
	assert(!isLower(UPPER));
	assert(!isLower(UPPER_VOWELS));
	assert(!isLower(UPPER_CONSONANT));

	assert(isLetter(LETTER));
	assert(!isLetter(DIGITS));

	assert(isDigit(DIGITS));
	assert(!isDigit(LETTER));

	for (char c : VOWEL) assert(isVowel(c));
	for (char c : CONSONANT) assert(!isVowel(c));
	for (char c : VOWEL) assert(!isConsonant(c));
	for (char c : CONSONANT) assert(isConsonant(c));
	assert(isVowel(VOWEL));
	assert(!isVowel(LETTER));
	assert(!isConsonant(LETTER));
	assert(isConsonant(CONSONANT));

	assert(startsWith(LETTER, UPPER));
	assert(endsWith(LETTER, LOWER));
	assert(startsWith(VOWEL, UPPER_VOWELS));
	assert(endsWith(VOWEL, LOWER_VOWELS));
	assert(startsWith(CONSONANT, UPPER_CONSONANT));
	assert(endsWith(CONSONANT, LOWER_CONSONANT));

	assert(areIncreasing(LETTER));
	assert(areIncreasing(UPPER));
	assert(areIncreasing(LOWER));
	assert(areIncreasing(VOWEL));
	assert(areIncreasing(UPPER_VOWELS));
	assert(areIncreasing(LOWER_VOWELS));
	assert(areIncreasing(CONSONANT));
	assert(areIncreasing(UPPER_CONSONANT));
	assert(areIncreasing(LOWER_CONSONANT));
	assert(areIncreasing(ALPHA_NUMERIC));
	assert(areIncreasing(UPPER_ALPHA_NUMERIC));
	assert(areIncreasing(LOWER_ALPHA_NUMERIC));
	assert(areIncreasing(DIGITS));
}

void checkVerdict() {
	using namespace Verdicts;
	assert(WA.exitCode != AC.exitCode);
	assert(PE.exitCode != AC.exitCode);
	assert(FAIL.exitCode != AC.exitCode);
	assert(FAIL.exitCode != WA.exitCode);
	assert(WA.exitCode != 0);
	assert(PE.exitCode != 0);
	assert(FAIL.exitCode != 0);
}

void checkUtility() {
	assert(!shorter{}(LETTER, LETTER));
	assert(!longer{}(LETTER, LETTER));
	assert(shorter{}(DIGITS, LETTER));
	assert(!longer{}(DIGITS, LETTER));
	assert(!shorter{}(LETTER, DIGITS));
	assert(longer{}(LETTER, DIGITS));

	assert(isPerm(LOWER, 'a'));
	assert(isPerm(UPPER, 'A'));
	assert(!isPerm(LOWER, 'A'));
	assert(!isPerm(UPPER, 'a'));
	assert(isPerm(DIGITS, '0'));

	assert(isPerm(DIGITS, DIGITS));
	assert(!isPerm(LOWER, UPPER));
	assert(areNonDecreasing(DIGITS));
	assert(areIncreasing(DIGITS));
	assert(areDistinct(DIGITS));
	assert(range(5) == std::vector<Integer>{0, 1, 2, 3, 4});
	assert(range(0, 1) == std::vector<Integer>{0});
	assert(range(-3, -3) == std::vector<Integer>{});
	assert(range(0, -1) == std::vector<Integer>{});
	assert(range(1, 3, 2) == std::vector<Integer>{1});
	assert(range(1, 4, 2) == std::vector<Integer>{1, 3});
	assert(range(1, -1, -2) == std::vector<Integer>{1});
	assert(range(1, -2, -2) == std::vector<Integer>{1, -1});
	assert(range(0x7FFF'FFFF'FFFF'FFFE_int, 0x7FFF'FFFF'FFFF'FFFF_int, 5) == std::vector<Integer>{0x7FFF'FFFF'FFFF'FFFE_int});
	assert(range(-0x7FFF'FFFF'FFFF'FFFE_int, -0x7FFF'FFFF'FFFF'FFFF_int, -5) == std::vector<Integer>{-0x7FFF'FFFF'FFFF'FFFE_int});
}

void checkFloat() {
	Real inf = std::numeric_limits<Real>::infinity();
	assert(details::floatEqual(std::nan("1"), std::nan("2"), 0, 0));
	assert(!details::floatEqual(1, std::nan("2"), 1, 1));
	assert(!details::floatEqual(std::nan("1"), 2, 1, 1));
	assert(!details::floatEqual(-1, std::nan("2"), 1, 1));
	assert(!details::floatEqual(std::nan("1"), -2, 1, 1));
	assert(!details::floatEqual(inf, std::nan("2"), 1, 1));
	assert(!details::floatEqual(std::nan("1"), inf, 1, 1));
	assert(!details::floatEqual(-inf, std::nan("2"), 1, 1));
	assert(!details::floatEqual(std::nan("1"), -inf, 1, 1));

	assert(details::floatEqual(inf, inf, 0, 0));
	assert(!details::floatEqual(inf, -inf, 1, 1));
	assert(!details::floatEqual(1, inf, 1, 1));
	assert(!details::floatEqual(inf, 1, 1, 1));
	assert(!details::floatEqual(-1, inf, 1, 1));
	assert(!details::floatEqual(inf, -1, 1, 1));

	assert(details::floatEqual(0.1, 0.1, 0, 0));
	assert(!details::floatEqual(0.1, -0.1, 0, 1));
	assert(!details::floatEqual(0.1, -0.1, 0.1, 0));
	assert(!details::floatEqual(0.1, 0.2, 0, 0));
	assert(details::floatEqual(0.1, 0.2, 0.1, 0));
	assert(details::floatEqual(0.2, 0.1, 0, 1));
	assert(!details::floatEqual(0.2, 0.1, 0, 0.9999));
}

void checkMath() {
	assert(applyMod(1, 3) == 1);
	assert(applyMod(-1, 3) == 2);

	assert(mulMod(0x7FFF'FFFF'FFFF'FFFE_int, 0x7FFF'FFFF'FFFF'FFFE_int, 0x7FFF'FFFF'FFFF'FFFF_int) == 1);
	assert(mulMod(0x7FFF'FFFF'FFFF'FFFE_int, 0x7FFF'FFFF'FFFF'FFFD_int, 0x7FFF'FFFF'FFFF'FFFF_int) == 2);
	assert(mulMod(0x7FFF'FFFF'FFFF'FFFD_int, 0x7FFF'FFFF'FFFF'FFFD_int, 0x7FFF'FFFF'FFFF'FFFF_int) == 4);

	assert(powMod(0xFFFF'FFFF_int, 2, 0x1'0000'0000_int) == 1);
	assert(powMod(0xFFFF'FFFD_int, 31, 0xFFFF'FFFF_int) == 0x7FFF'FFFF_int);
	assert(powMod(0xFFFF'FFFD_int, 63, 0xFFFF'FFFF_int) == 0x7FFF'FFFF_int);
	assert(powMod(0x7FFF'FFFF'FFFF'FFFE_int, 2, 0x7FFF'FFFF'FFFF'FFFF_int) == 1);
	assert(powMod(0x7FFF'FFFF'FFFF'FFFE_int, 3, 0x7FFF'FFFF'FFFF'FFFF_int) == 0x7FFF'FFFF'FFFF'FFFE_int);
	assert(powMod(0x7FFF'FFFF'FFFF'FFFD_int, 63, 0x7FFF'FFFF'FFFF'FFFF_int) == 0x7FFF'FFFF'FFFF'FFFE_int);

	assert(multInv(5, 17) == powMod(5, 15, 17));
	assert(multInv(5, 10) == -1);

	assert(!isPrime(-3));
	assert(!isPrime(-2));
	assert(!isPrime(0));
	assert(!isPrime(1));
	assert(isPrime(2));
	assert(isPrime(3));
	assert(!isPrime(4));
	assert(!isPrime(512461));
	assert(isPrime(0x7FFF'FFFF'FFFF'FFFF_int-164));
	assert(!isPrime(0x7FFF'FFFF'FFFF'FFFF_int-25));
	assert(isPrime(0x7FFF'FFFF'FFFF'FFFF_int-24));
	assert(!isPrime(0x7FFF'FFFF'FFFF'FFFF_int-23));
	assert(!isPrime(0x7FFF'FFFF'FFFF'FFFF));
	assert(!isPrime(2147483647_int * 4294967291_int));
}

template<typename T, typename F>
void checkUniform(const F& f, Integer iterations, Integer different, const std::source_location location = std::source_location::current()) {
	std::map<T, Integer> count;
	for (Integer i = 0; i < iterations; i++) count[f()]++;
	assert(std::ssize(count) == different, location);
	std::vector<Integer> counts;
	for (auto e : count) counts.push_back(e.second);
	sort(counts.begin(), counts.end());
	Integer diff = counts.back() - counts.front();
	Integer ma = std::llround(4*std::sqrt(counts.front() + counts.back()));
	assert(diff < ma, location);
}

void checkRandom() {
	for (Integer i = 0; i < 100000; i++) {
		Integer x = Random::integer(-5, 15);
		assert(x >= -5 && x < 15);
	}
	{
		Real min = 0.5;
		Real max = 0.5;
		Real sum = 0;
		for (Integer i = 0; i < 2000000; i++) {
			Real x = Random::real();
			min = std::min(min, x);
			max = std::max(max, x);
			sum += x;
		}
		assert(min >= 0.0l);
		assert(max < 1.0l);
		assert(std::abs(0.5l - sum / 2000000.0l) < 0.0001l);
	}
	for (Integer i = 0; i < 100000; i++) {
		std::string tmp(LOWER);
		Random::shuffle(tmp);
		assert(isPerm(LOWER, tmp));
	}
	for (Integer i = 0; i < 10000; i++) {
		auto tmp = Random::partition(17, 5);
		Integer sum = 0;
		for (auto x : tmp) {
			assert(x >= 0 && x < 17);
			sum += x;
		}
		assert(sum == 17);
	}
	for (Integer i = 0; i < 10000; i++) {
		auto tmp = Random::bracketSequence(17);
		Integer sum = 0;
		for (auto x : tmp) {
			if (x == '(') sum++;
			else sum--;
			assert(sum >= 0);
		}
		assert(sum == 0);
	}

	for (Integer i = 0; i < 10000; i++) assert(isPerm(Random::perm(13)));
	for (Integer i = 0; i < 10000; i++) assert(isPerm(Random::perm(Random::partition(13, 3))));
	for (Integer i = 0; i < 10000; i++) assert(isPerm(Random::perm(13, Random::distinct(3, 13))));
	for (Integer i = 0; i < 10000; i++) assert(areIncreasing(Random::increasing(13, 100)));
	for (Integer i = 0; i < 10000; i++) assert(areDecreasing(Random::decreasing(13, 100)));
	for (Integer i = 0; i < 10000; i++) assert(areNonDecreasing(Random::nonDecreasing(13, 100)));
	for (Integer i = 0; i < 10000; i++) assert(areNonIncreasing(Random::nonIncreasing(13, 100)));

	checkUniform<Integer>([](){return Random::integer(4'000'000'000'000);}, 1'000'000, 1'000'000);
	checkUniform<Integer>([](){return Random::integer(3, 17);}, 1'000'000, 14);
	checkUniform<Integer>([](){return Random::prime(3, 17);}, 1'000'000, 5);
	checkUniform<Integer>([](){return Random::select({1,5,7,8,11});}, 1'000'000, 5);
	std::string test = "test";
	checkUniform<std::string>([&](){return Random::shuffle(test), test;}, 1'000'000, 12);
	checkUniform<std::vector<Integer>>([](){return Random::perm(4);}, 1'000'000, 24);
	checkUniform<std::vector<Integer>>([](){return Random::perm(std::vector<Integer>{4});}, 1'000'000, 6);
	checkUniform<std::vector<Integer>>([](){return Random::perm({3,1});}, 1'000'000, 8);
	checkUniform<std::vector<Integer>>([](){return Random::perm({2,2});}, 1'000'000, 3);
	checkUniform<std::vector<Integer>>([](){return Random::perm({2,1,1});}, 1'000'000, 6);
	checkUniform<std::vector<Integer>>([](){return Random::perm({1,1,1,1});}, 1'000'000, 1);
	checkUniform<std::vector<Integer>>([](){return Random::multiple(3, 3);}, 1'000'000, 3*3*3);
	checkUniform<std::vector<Integer>>([](){return Random::partition(10, 3);}, 1'000'000, 6*6);
	checkUniform<std::string>([](){return Random::bracketSequence(4);}, 1'000'000, 14);

	Random::seed(123456789u);
	//this sequence may change but all compilers should generate the same sequence!
	std::vector<Integer> expected = {43070660, 32948942, 16870018, 3525658, 107275727, 43315498, 8778520};
	assert(expected == Random::multiple(expected.size(), 123, 123456789));
}

void checkCommandParser() {
	char arg0[] = "prog";
    char arg1[] = "--test1";
    char arg2[] = "1";
    char arg3[] = "1.5";
    char arg4[] = "test";
    char arg5[] = "--test2";
    char arg6[] = "--test3";
    char* argv[] = {&arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0], &arg5[0], &arg6[0], nullptr};
	CommandParser arguments(7, argv);

	assert(!arguments["--test0"]);
	assert(!!arguments["--test1"]);
	assert(!!arguments["--test2"]);
	assert(!!arguments["--test3"]);
	assert(!arguments["--test4"]);

	assert(arguments["--test0"].parameterCount() == 0);
	assert(arguments["--test1"].parameterCount() == 3);
	assert(arguments["--test2"].parameterCount() == 0);
	assert(arguments["--test3"].parameterCount() == 0);
	assert(arguments["--test4"].parameterCount() == 0);

	assert(arguments["--test1"].asInteger() == 1);
	assert(arguments["--test1"].asInteger(5) == 1);
	assert(arguments["--test1"].asReal() == 1);
	assert(arguments["--test1"][1].asReal() == 1.5);
	assert(arguments["--test1"][2].asString() == "test");
	assert(arguments["--test1"][3].asInteger(5) == 5);
	assert(arguments["--test2"].asInteger(5) == 5);
	assert(arguments["--test4"].asInteger(5) == 5);
}

void checkInputStream() {
	assertNoException([](){
		std::istringstream rawIn("Das   ist 1 test\n");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::FAIL);

		in.expectString("Das");
		in.space();
		in.space();
		in.space();
		assert(in.string(3, 4) == "ist");
		in.space();
		assert(in.integer(1, 2) == 1);
		in.space();
		assert(in.string(std::regex("[a-z]*"), 4, 5) == "test");
		in.newline();
		in.eof();
	});
	assertNoException([](){
		std::istringstream rawIn("Das   ist 1 test\n");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::FAIL);

		in.expectString("Das");
		in.space();
		in.space();
		in.space();
		assert(in.string(3, 4) == "ist");
		in.space();
		assert(in.integer(1, 2) == 1);
		in.space();
		assert(in.string(std::regex("[a-z]*"), 4, 5) == "test");
		in.newline();
		in.eof();
	});
	assertNoException([](){
		std::istringstream rawIn("123");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectInt(123);
	});
	assertNoException([](){
		std::istringstream rawIn("Das");
		InputStream in(rawIn, true, false, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("Das  das");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("Das");
		in.space();
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("Das\ndas");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("Das");
		in.space();
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("Das das");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("Das");
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("Das  das");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("Das");
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("Das das");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("Das");
		in.space();
		in.space();
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("\n");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.eof();
	});
	assertNoException([](){
		std::istringstream rawIn("das");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.space();
		in.expectString("das");
	});
	assertNoException([](){
		std::istringstream rawIn("1.5");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.real();
	});
	assertNoException([](){
		std::istringstream rawIn("0.00000000000000000000001");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.real();
	});
	assertNoException([](){
		std::istringstream rawIn("9223372036854775807");
		InputStream in(rawIn, false, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	});
	assertNoException([](){
		std::istringstream rawIn("00.1");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.real();
	});
	assertNoException([](){
		std::istringstream rawIn("+0.1");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.real();
	});

	assertExit([](){
		std::istringstream rawIn("Das");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("das");
	}, 23);
	assertExit([](){
		std::istringstream rawIn;
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("das");
	}, 23);
	assertExit([](){
		std::istringstream rawIn("das\n");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectString("das");
		in.eof();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("0123");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("0x123");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("1.0");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("1.5");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("1-1");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("+123");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.expectInt(123);
	}, 23);
	assertExit([](){
		std::istringstream rawIn("123");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer(0, 123);
	}, 23);
	assertExit([](){
		std::istringstream rawIn("9223372036854775808");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.integer();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("Das  ist");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));

		in.string();
		in.space();
		in.string();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("Das ist");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));

		in.string();
		in.space();
		in.space();
		in.string();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("\n");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.eof();
	}, 23);
	assertExit([](){
		std::istringstream rawIn(" ");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.eof();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("x");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.eof();
	}, 23);
	assertExit([](){
		std::istringstream rawIn("0.123456");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.realStrict(-10, 10, 0, 3);
	}, 23);
	assertExit([](){
		std::istringstream rawIn("00.1");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.realStrict(-10, 10, 0, 3);
	}, 23);
	assertExit([](){
		std::istringstream rawIn("+0.1");
		InputStream in(rawIn, true, true, ValidateBase::juryOut, Verdicts::Verdict(23));
		in.realStrict(-10, 10, 0, 3);
	}, 23);
	assertNoException([](){
		std::istringstream rawIn("Das   ist 1\n2 3 test 3.1 4.1 5.9 ende\n");
		InputStream in(rawIn, false, false, ValidateBase::juryOut, Verdicts::FAIL);

		in.strings(2, 3, 4);
		in.integers(3, 1, 4);
		in.expectString("test");
		in.reals(3, 3.0, 6.0);
		in.expectString("ende");
		in.eof();
	});
}

void checkJoin() {
	std::vector<Integer> a = {1,2,0};
	std::array<Integer, 3> b = {1, 2, 0};
	Integer c[3] = {1, 2, 0};
	std::string d = "120";
	char e[4] = "120";
	std::tuple<char, Integer, Integer> f = {'1', 2, 0};
	std::pair<char, Integer> g = {'1', 2};

	using namespace std::literals;
	assert(join(std::begin(a), std::end(a)).asString() == "1 2 0"sv);
	assert(join(std::begin(b), std::end(b)).asString() == "1 2 0"sv);
	assert(join(std::begin(c), std::end(c)).asString() == "1 2 0"sv);
	assert(join(std::begin(d), std::end(d)).asString() == "1 2 0"sv);
	assert(join(std::begin(e), std::end(e)).asString() == "1 2 0 \0"sv);

	assert(join(std::cbegin(a), std::cend(a)).asString() == "1 2 0"sv);
	assert(join(std::cbegin(b), std::cend(b)).asString() == "1 2 0"sv);
	assert(join(std::cbegin(c), std::cend(c)).asString() == "1 2 0"sv);
	assert(join(std::cbegin(d), std::cend(d)).asString() == "1 2 0"sv);
	assert(join(std::cbegin(e), std::cend(e)).asString() == "1 2 0 \0"sv);

	assert(join(a).asString() == "1 2 0"sv);
	assert(join(b).asString() == "1 2 0"sv);
	assert(join(c).asString() == "1 2 0"sv);
	assert(join(d).asString() == "1 2 0"sv);
	assert(join(e).asString() == "1 2 0"sv);
	assert(join(f).asString() == "1 2 0"sv);
	assert(join(g).asString() == "1 2"sv);

	assert(join(const_cast<const decltype(a)&>(a)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(b)&>(b)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(c)&>(c)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(d)&>(d)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(e)&>(e)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(f)&>(f)).asString() == "1 2 0"sv);
	assert(join(const_cast<const decltype(g)&>(g)).asString() == "1 2"sv);

	assert(join(std::move(a)).asString() == "1 2 0"sv);
	assert(join(std::move(b)).asString() == "1 2 0"sv);
	assert(join(std::move(c)).asString() == "1 2 0"sv);
	assert(join(std::move(d)).asString() == "1 2 0"sv);
	assert(join(std::move(e)).asString() == "1 2 0"sv);
	assert(join(std::move(f)).asString() == "1 2 0"sv);
	assert(join(std::move(g)).asString() == "1 2");

	assert(join({1, 2, 0}).asString() == "1 2 0"sv);
	assert(join({1, "2", '0'}).asString() == "1 2 0"sv);
	assert(join("120").asString() == "1 2 0"sv);
	assert(join(std::make_tuple(1, "2", '0')).asString() == "1 2 0"sv);
	assert(join(std::make_pair(1, "2")).asString() == "1 2"sv);
}

template<typename T, typename = void>
struct CompileJoin : std::false_type {};

template<typename T>
struct CompileJoin<T, std::void_t<decltype(join(std::declval<T>()))>> : std::true_type {};

static_assert(!CompileJoin<int>{}, "join(int) should be invalid!");

void checkGeometry() {
	using pts = std::vector<std::pair<Integer, Integer>>;
	assert(!isConvex(pts{}));
	assert(!isConvex(pts{{1,1}}));
	assert(!isConvex(pts{{0,0}, {1,1}}));
	assert(!isConvex(pts{{0,0}, {1,1}, {-1,-1}}));
	assert(!isConvex(pts{{0,0}, {1,1}, {0,0}, {1,1}}));
	assert(!isConvex(pts{{0,0}, {2,0}, {2,2}, {1,2}, {1,1}, {3,1}, {3,3}, {0,3}}));
	assert(isConvex(pts{{0,0}, {1,0}, {2,0}, {2,2}, {0,2}, {0,1}}));
	assert(isConvex(pts{{0,0}, {2,0}, {2,2}}));
	assert(!isConvex(pts{{0,0}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0}, {1,1}, {0,1}}));
	assert(!isConvex(pts{{0,0}, {1,0}, {1,1}, {0,1}, {1,0}, {1,1}, {0,1}}));
	assert(!isConvex(pts{{0,0}, {0,-1}, {1,-1}, {1,1}, {0,1}, {0,-1}, {1,-1}, {1,1}, {0,1}}));
	assert(!isConvex(pts{{0,0}, {3,0}, {3,3}, {1,1}, {2,2}, {0,2}}));
	assert(isConvex(pts{{0,0}, {3,0}, {2,2}, {0,3}}));
	assert(!isConvex(pts{{0,0}, {2,0}, {2,1}, {1,2}, {2,1}, {1,2}, {0,2}}));

	assert(!isStrictlyConvex(pts{}));
	assert(!isStrictlyConvex(pts{{1,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,1}, {-1,-1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,1}, {0,0}, {1,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {2,0}, {2,2}, {1,2}, {1,1}, {3,1}, {3,3}, {0,3}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,0}, {2,0}, {2,2}, {0,2}, {0,1}}));
	assert(isStrictlyConvex(pts{{0,0}, {2,0}, {2,2}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0}, {1,1}, {0,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {1,0}, {1,1}, {0,1}, {1,0}, {1,1}, {0,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {0,-1}, {1,-1}, {1,1}, {0,1}, {0,-1}, {1,-1}, {1,1}, {0,1}}));
	assert(!isStrictlyConvex(pts{{0,0}, {3,0}, {3,3}, {1,1}, {2,2}, {0,2}}));
	assert(isStrictlyConvex(pts{{0,0}, {3,0}, {2,2}, {0,3}}));
	assert(!isStrictlyConvex(pts{{0,0}, {2,0}, {2,1}, {1,2}, {2,1}, {1,2}, {0,2}}));

	assert(isConvex(Random::convex(100, 1000'000'000)));//this is always true
	assert(isStrictlyConvex(Random::convex(100, 1000'000'000)));//this is likely true

	// implementation was also checked against https://open.kattis.com/problems/polygon
	assert(isSimple(pts{{0, 0}, {1, 0}, {0, 1}}));
	assert(isSimple(pts{{2, 0}, {1, 1}, {0, 2}, {1, 3}, {2, 4}, {3, 3}, {4, 2}, {3, 1}}));
	assert(isSimple(pts{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}, {0, 1}}));
	assert(isSimple(pts{{1, 6}, {5, 7}, {9, 4}, {4, 3}, {7, 4}, {4, 6}, {3, 1}}));

	assert(!isSimple(pts{{0, 0}, {0, 1}, {0, 0}, {0, 1}}));
	assert(!isSimple(pts{{0, 0}, {1, 0}, {0, 0}, {1, 0}}));
	assert(!isSimple(pts{{0, 0}, {2, 0}, {1, 0}}));
	assert(!isSimple(pts{{2, 0}, {1, 0}, {0, 0}}));
	assert(!isSimple(pts{{1, 0}, {0, 0}, {2, 0}}));
	assert(!isSimple(pts{{0, 0}, {1, 0}, {2, 0}}));
	assert(!isSimple(pts{{1, 0}, {2, 0}, {0, 0}}));
	assert(!isSimple(pts{{2, 0}, {0, 0}, {1, 0}}));
	assert(!isSimple(pts{{0, 0}, {1, 0}, {0, 1}, {1, 1}}));
	assert(!isSimple(pts{{0, 0}, {2, 0}, {0, 2}, {2, 2}, {1, 1}}));
	assert(!isSimple(pts{{0, 0}, {2, 0}, {1, 1}, {0, 2}, {2, 2}}));
	assert(!isSimple(pts{{0 ,0}, {2, 0}, {1, 1}, {0, 2}, {2, 2}, {1, 1}}));
	assert(!isSimple(pts{{0, 0}, {2, 0}, {2, 1}, {1, 0}, {0, 1}}));
	assert(!isSimple(pts{{1, 0}, {1, 1}, {0, 1}, {2, 1}, {1, 1}, {1, 2}, {3, 2}, {3, 0}}));
	assert(!isSimple(pts{{0, 1}, {1, 1}, {1, 0}, {1, 2}, {1, 1}, {2, 1}, {2, 3}, {0, 3}}));
}

int main() {
	details::initialized(true);
	std::ostringstream err, out;
	ValidateBase::juryErr = OutputStream(err);
	ValidateBase::juryOut = OutputStream(out);

	checkStrings();
	checkVerdict();
	checkUtility();
	checkFloat();
	checkMath();
	checkRandom();
	checkCommandParser();
	checkInputStream();
	checkJoin();
	checkGeometry();

	std::cout << "All tests passed!" << std::endl;
}
