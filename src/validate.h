//============================================================================//
// validate.h                                                                 //
//============================================================================//
// This header can be used to safely parse team output tokenwise. We support: //
// - string tokens (get converted to lowercase)                               //
// - integer tokens in [-2^63, 2^63)                                          //
// - float tokens (relative and absolute error of 10^6 is allowed)            //
// Tokens need to be separated by whitespace (any amount). The follwoing      //
// command line flags allow stricter checking:                                //
// - caseSensitive: string tokens don't get converted to lowercase            //
// - space_change_sensitive: tokens need to be separated by the corecct       //
//   amount of whitespaces                                                    //
// - FLOAT_{|RELATIVE|ABSOLUTE}_TOLERANCE: allowed relative/absolute error    //
//                                                                            //
// This header can be used to safely verify input files. In this case tokens  //
// are case sensitive and all whitespaces have to be checked. Also            //
// whitespaces are not interchangeable.                                       //
//                                                                            //
// This header can be used to generate random numbers in a deterministic and  //
// reproducable fashion. (The randomness is consistent across compilers and   //
// machines)                                                                  //
//============================================================================//
//version 2.2.9                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef VALIDATE_H
#define VALIDATE_H

//#define DOUBLE_FALLBACK

#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <cmath>
#include <charconv>
#include <complex>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>


//============================================================================//
// Basic definitions and constants                                            //
//============================================================================//
// default types
using Integer = std::int64_t;
using Real = long double;

// derived types
using UInteger = std::make_unsigned<Integer>::type;
constexpr Integer operator ""_int(unsigned long long int value) {return static_cast<Integer>(value);}
constexpr UInteger operator ""_uint(unsigned long long int value) {return static_cast<UInteger>(value);}
constexpr Real operator ""_real(unsigned long long int value) {return static_cast<Real>(value);}
constexpr Real operator ""_real(long double value) {return static_cast<Real>(value);}

// settings which can be overwritten before the include!
namespace Settings {
	namespace details {
		using RandomEngine                              = std::mt19937_64;
		constexpr Integer LARGE                         = 0x3FFF'FFFF'FFFF'FFFF;
		constexpr bool DEFAULT_CASE_LOWER               = true;
		constexpr int DEFAULT_PRECISION                 = 6;
		constexpr Real DEFAULT_EPS                      = 1e-6_real;

		[[noreturn]] void exitVerdict(int exitCode) {
			//throw exitCode;
			//quick_exit(exitCode);
			std::exit(exitCode);
		}
	}
	using namespace details;
}
// make settings publically available
using Settings::RandomEngine;
using Settings::LARGE;
using Settings::DEFAULT_CASE_LOWER;
using Settings::DEFAULT_PRECISION;
using Settings::DEFAULT_EPS;
using Settings::exitVerdict;

// useful constants
constexpr std::string_view LETTER                       = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view LOWER                        = LETTER.substr(0, 26);
constexpr std::string_view UPPER                        = LETTER.substr(26);
constexpr std::string_view VOWEL                        = "aeiouAEIOU";
constexpr std::string_view LOWER_VOWELS                 = VOWEL.substr(0, 5);
constexpr std::string_view UPPER_VOWELS                 = VOWEL.substr(5);
constexpr std::string_view CONSONANT                    = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
constexpr std::string_view LOWER_CONSONANT              = CONSONANT.substr(0, 21);
constexpr std::string_view UPPER_CONSONANT              = CONSONANT.substr(21);
constexpr std::string_view ALPHA_NUMERIC                = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view LOWER_ALPHA_NUMERIC          = ALPHA_NUMERIC.substr(0, 10 + 26);
constexpr std::string_view UPPER_ALPHA_NUMERIC          = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view DIGITS                       = ALPHA_NUMERIC.substr(0, 10);
constexpr std::string_view BRACKETS                     = "()[]{}<>";
constexpr char NEWLINE                                  = '\n';
constexpr char SPACE                                    = ' ';
constexpr char NOSEP                                    = '\0';
constexpr Real PI                                       = 3.1415926535897932384626433832795028_real;


//============================================================================//
// internal definitions and constants                                         //
//============================================================================//
constexpr UInteger DEFAULT_SEED                         = 3141592653589793238_uint;
constexpr std::string_view CASE_SENSITIVE               = "case_sensitive";
constexpr std::string_view SPACE_SENSITIVE              = "space_change_sensitive";
constexpr std::string_view FLOAT_ABSOLUTE_TOLERANCE     = "float_absolute_tolerance";
constexpr std::string_view FLOAT_RELATIVE_TOLERANCE     = "float_relative_tolerance";
constexpr std::string_view FLOAT_TOLERANCE              = "float_tolerance";
constexpr std::string_view JUDGE_MESSAGE                = "judgemessage.txt";
constexpr char DEFAULT_SEPARATOR                        = SPACE;
constexpr std::string_view EMPTY_COMMAND                = "";
constexpr std::string_view COMMAND_PREFIX               = "--";
constexpr std::string_view CONSTRAINT_COMMAND           = "--constraints_file";
constexpr std::string_view SEED_COMMAND                 = "--seed";
constexpr auto REGEX_OPTIONS                            = std::regex::nosubs | std::regex::optimize;
inline const std::regex INTEGER_REGEX("0|-?[1-9][0-9]*", REGEX_OPTIONS);
inline const std::regex REAL_REGEX("-?(0|([1-9][0-9]*))(\\.[0-9]*)?([eE][+-]?(0|([1-9][0-9]*)))?", REGEX_OPTIONS);
inline const std::regex STRICT_REAL_REGEX("-?(0|([1-9][0-9]*))\\.?[0-9]*", REGEX_OPTIONS);

static_assert(2'000'000'000'000'000'000_int < LARGE / 2, "LARGE too small");
static_assert(LARGE <= std::numeric_limits<Integer>::max() / 2, "LARGE too big");

static_assert(-1 == 0xFFFF'FFFF'FFFF'FFFF_int, "Two's complement for signed numbers is required" );
static_assert(std::is_convertible_v<Integer, UInteger>, "Incompatible Integer and UInteger types?!");
static_assert(std::is_convertible_v<UInteger, Integer>, "Incompatible Integer and UInteger types?!");
static_assert(sizeof(Integer) == sizeof(UInteger), "Incompatible Integer and UInteger types?!");

template<typename T = std::logic_error>
constexpr void judgeAssert(bool asserted, std::string_view message) {
	if (!asserted) throw T(message.data());
}


//============================================================================//
// SFINAE                                                                     //
//============================================================================//
namespace details {
	template<typename T, typename = void>
	struct IsContainer : std::false_type {};

	template<typename T>
	struct IsContainer<T, std::void_t<decltype(std::begin(std::declval<std::add_lvalue_reference_t<T>>()))>> : std::true_type {
		using value_type = std::remove_reference_t<decltype(*std::begin(std::declval<std::add_lvalue_reference_t<T>>()))>;
	};

	template<typename T>
	struct IsStdArray : std::false_type {};

	template<typename T, std::size_t N>
	struct IsStdArray<std::array<T, N>> : std::true_type {};

	template<typename T, typename = void>
	struct IsTupleLike : std::false_type {};

	template<typename T>
	struct IsTupleLike<T, std::void_t<decltype(sizeof(std::tuple_size<T>))>> : std::true_type {};

	template<typename T, typename = void>
	struct HasOstreamOperator : std::false_type {};

	template<typename T>
	struct HasOstreamOperator<T, std::void_t<decltype(std::declval<std::ostream>() << std::declval<T>())>> : std::true_type {};
}


//============================================================================//
// Verdicts                                                                   //
//============================================================================//
struct Verdict final {
	int exitCode;

	constexpr explicit Verdict(int exitCode_ = 1) : exitCode(exitCode_) {}

	constexpr operator int() const {
		return exitCode;
	}

	[[noreturn]] void exit() const {
		exitVerdict(exitCode);
	}

	friend void operator<<(std::ostream& os, const Verdict& v) {
		os << std::endl;
		v.exit();
	}
};

// default verdicts (we do not support scoring)
constexpr Verdict AC(42);
constexpr Verdict WA(43);
constexpr Verdict PE = WA;
constexpr Verdict FAIL(1);


//============================================================================//
// Output streams                                                             //
//============================================================================//
class NullStream final : public std::ostream {
	class NullBuffer final : public std::streambuf {
	protected:
		std::streamsize xsputn(const char* /**/, std::streamsize n) override {
			return n;
		}
		int overflow(int c = std::char_traits<char>::eof()) override {
			return std::char_traits<char>::not_eof(c);
		}
	} nullBuffer;
public:
	NullStream() : std::ostream(&nullBuffer) {}
};

namespace details {
	NullStream nullStream;
}

class OutputStream final {
	std::unique_ptr<std::ofstream> managed;
	std::ostream* os;

	void init() {
		*os << std::boolalpha;
		*os << std::fixed;
		*os << std::setprecision(DEFAULT_PRECISION);
	}

public:
	OutputStream() : os(&details::nullStream) {}
	OutputStream(std::ostream& os_) : os(&os_) {init();}
	explicit OutputStream(const std::filesystem::path& path) : managed(std::make_unique<std::ofstream>(path)), os(managed.get()) {
		judgeAssert<std::runtime_error>(managed->good(), "OutputStream: Could not open File: " + path.string());
		init();
	}

	OutputStream(OutputStream&& other) = default;
	OutputStream& operator=(OutputStream&& other) = default;

	OutputStream(const OutputStream&) = delete;
	OutputStream& operator=(const OutputStream&) = delete;


	template<typename L, typename R>
	OutputStream& operator<<(const std::pair<L, R>& t) {
		return *this << t.first << DEFAULT_SEPARATOR << t.second;
	}

	template<typename... Args>
	OutputStream& operator<<(const std::tuple<Args...>& t) {
		return join(t, std::index_sequence_for<Args...>(), DEFAULT_SEPARATOR);
	}

	template<typename T>
	OutputStream& operator<<(const T& x) {
		if constexpr ((std::is_array_v<T> and !std::is_same_v<std::decay_t<T>, char*>) or
		              (details::IsContainer<T>{} and !details::HasOstreamOperator<T>{})) {
			return join(std::begin(x), std::end(x), DEFAULT_SEPARATOR);
		} else {
			*os << x;
			return *this;
		}
	}

	OutputStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
		*os << manip;
		return *this;
	}

	template<typename Tuple, std::size_t... Is>
	OutputStream& join(const Tuple& t, std::index_sequence<Is...> /**/, char separator) {
		static_assert(std::tuple_size_v<Tuple> == sizeof...(Is));
		if (separator != NOSEP) ((*os << (Is == 0 ? std::string_view() : std::string_view(&separator, 1)), *this << std::get<Is>(t)), ...);
		else ((*this << std::get<Is>(t)), ...);
		return *this;
	}

	template<typename T>
	OutputStream& join(T first, T last, char separator) {
		for (auto it = first; it != last; it++) {
			if (it != first and separator != NOSEP) *os << separator;
			*this << *it;
		}
		return *this;
	}
};

namespace ValidateBase {
	// define this early so everyone can use it!
	OutputStream juryErr(std::cerr);
	OutputStream juryOut(std::cout);
}

// allow printing colletions as:
// join(begin(), end(), [sep])
namespace details {
	template<typename C>
	class TempWriter final {
		C callable;
	public:
		constexpr explicit TempWriter(const C& callable_) : callable(callable_) {}

		TempWriter(const TempWriter&) = delete;
		TempWriter(TempWriter&&) = delete;
		TempWriter& operator=(const TempWriter&) = delete;
		TempWriter& operator=(TempWriter&&) = delete;

		std::string asString() const {
			std::ostringstream os;
			OutputStream tmp(os);
			tmp << *this;
			return os.str();
		}

		explicit operator std::string() const {
			return asString();
		}

		friend OutputStream& operator<<(OutputStream& os, const TempWriter<C>& writer) {
			writer.callable(os);
			return os;
		}

		friend OutputStream& operator<<(std::ostream& os, const TempWriter<C>& writer) = delete; //news OutputStream
	};

	struct JoinListCapture {
		std::function<void(OutputStream&, char separator)> callable;

		template<typename... Args>
		JoinListCapture(Args&&... args)
			: callable([t = std::forward_as_tuple(args...)](OutputStream& os, char separator) {
			os.join(t, std::index_sequence_for<Args...>(), separator);
		}) {}
	};
}

template<typename T>
constexpr auto join(T first, T last, char separator = DEFAULT_SEPARATOR) {
	return details::TempWriter([=](OutputStream& os) {
		os.join(first, last, separator);
	});
}

template<typename CR,
         typename = std::enable_if_t<details::IsContainer<CR>{}>,
         typename = std::enable_if_t<!details::IsStdArray<std::remove_cv_t<std::remove_reference_t<CR>>>{}>>
constexpr auto join(CR&& c, char separator = DEFAULT_SEPARATOR) {
	if constexpr(std::is_rvalue_reference_v<CR&&>) {
		if constexpr (std::is_array_v<CR>) {
			return details::TempWriter([c, separator](OutputStream& os) {
				os.join(std::begin(c), std::end(c), separator);
			});
		} else {
			return details::TempWriter([c = std::move(c), separator](OutputStream& os) {
				os.join(std::begin(c), std::end(c), separator);
			});
		}
	} else {
		return join(std::begin(c), std::end(c), separator);
	}
}

template<typename CR, std::size_t N = std::tuple_size<std::decay_t<CR>>::value>
constexpr auto join(CR&& c, char separator = DEFAULT_SEPARATOR) {
	if constexpr(std::is_rvalue_reference_v<CR&&>) {
		return details::TempWriter([c = std::move(c), separator](OutputStream& os) {
			os.join(c, std::make_index_sequence<N>{}, separator);
		});
	} else {
		return details::TempWriter([&c, separator](OutputStream& os) {
			os.join(c, std::make_index_sequence<N>{}, separator);
		});
	}
}

template<typename T, std::size_t N,
         typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, char>>>
constexpr auto join(T (&c)[N], char separator = DEFAULT_SEPARATOR) {
	static_assert(N > 0, "c-strings should be null terminated!");
	return join(std::begin(c), std::prev(std::end(c)), separator);
}

template<typename T, std::size_t N,
         typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, char>>>
constexpr auto join(T (&&c)[N], char separator = DEFAULT_SEPARATOR) {
	static_assert(N > 0, "c-strings should be null terminated!");
	return details::TempWriter([c, separator](OutputStream& os) {
		os.join(std::begin(c), std::prev(std::end(c)), separator);
	});
}

template<typename T,
         typename = std::enable_if_t<!std::is_array<T>{}>,
         typename = std::enable_if_t<!details::IsContainer<T>{}>,
         typename = std::enable_if_t<!details::IsTupleLike<T>{}>>
constexpr auto join(const T& t, char separator = DEFAULT_SEPARATOR) = delete;

auto join(details::JoinListCapture c, char separator = DEFAULT_SEPARATOR) {
	return details::TempWriter([c = std::move(c), separator](OutputStream& os) {
		c.callable(os, separator);
	});
}


//============================================================================//
// Basic datastructures                                                       //
//============================================================================//
// make usage of std::priority_queue easier...
namespace details {
	template<typename T, typename Compare = std::less<T>>
	struct invertCompare {
		constexpr bool operator()(const T &lhs, const T &rhs) const {
			return Compare{}(rhs, lhs);
		}
	};
}
template<typename T, typename Compare = std::less<T>>
using MinPQ = std::priority_queue<T, std::vector<T>, details::invertCompare<T, Compare>>;
template<typename T, typename Compare = std::less<T>>
using MaxPQ = std::priority_queue<T, std::vector<T>, Compare>;

template<typename C, typename K>
bool contains(const C& container, const K& key) {
	return container.find(key) != container.end();
}

template<typename C1, typename C2>
void append(C1& c1, const C2& c2) {
	static_assert(std::is_same_v<typename details::IsContainer<C1>::value_type,
	                             typename details::IsContainer<C2>::value_type>, "cannot append container of different value type!");
	if (static_cast<const void*>(&c1) != static_cast<const void*>(&c2)) {
		for (auto&& e : c2) c1.emplace(c1.end(), e);
	} else {
		C2 tmp = c2;
		for (auto&& e : tmp) c1.emplace(c1.end(), e);
	}
}

template<typename C1, std::size_t N>
void append(C1& c1, const typename C1::value_type(&c2)[N]) {
	for (auto&& e : c2) c1.emplace(c1.end(), e);
}

struct shorter {
	template<typename U, typename V>
	bool operator()(const U& a, const V& b) const {
		return std::size(a) < std::size(b);
	}
};

struct longer {
	template<typename U, typename V>
	bool operator()(const U& a, const V& b) const {
		return std::size(b) < std::size(a);
	}
};

namespace details {
	template<typename T, typename = void>
	struct Flatten {using value_type = T;};

	template<typename T>
	struct Flatten<T, std::enable_if_t<IsContainer<T>{}>> : Flatten<typename IsContainer<T>::value_type> {};

	template<typename CR, typename V>
	void flatAppend(CR&& c, std::vector<V>& res) {
		using C = std::remove_reference_t<CR>;
		if constexpr(std::is_same_v<C, V>) {
			res.emplace_back(std::forward<CR>(c));
		} else if constexpr (!IsContainer<C>{}) {
			static_assert(IsContainer<C>{}, "invalid base type for flatten()!");
		} else {
			if constexpr (std::is_rvalue_reference_v<CR&&>) {
				for (auto&& v : c) flatAppend(std::move(v), res);
			} else {
				for (auto&& v : c) flatAppend(v, res);
			}
		}
	}
}

template<typename R, typename CR>
auto flatten(CR&& c) {
	std::vector<R> res;
	details::flatAppend(std::forward<CR>(c), res);
	return res;
}

template<typename CR>
auto flatten(CR&& c) {
	using C = std::remove_reference_t<CR>;
	return flatten<typename details::Flatten<C>::value_type, CR>(std::forward<CR>(c));
}


//============================================================================//
// Utility                                                                    //
//============================================================================//
// for sequences
template<typename RandomIt,
         typename = std::enable_if_t<std::is_integral_v<typename std::iterator_traits<RandomIt>::value_type>>>
bool isPerm(RandomIt first, RandomIt last, typename std::iterator_traits<RandomIt>::value_type offset = 0) {
	auto count = std::distance(first, last);
	std::vector<bool> seen(count, false);
	for (; first != last; first++) {
		auto x = *first;
		if (x < offset) return false;
		x -= offset;
		if (x >= count) return false;
		if (seen[x]) return false;
		seen[x] = true;
	}
	return true;
}
template<typename C, typename std::enable_if_t<std::is_integral_v<typename details::IsContainer<C>::value_type>, bool> = true>
bool isPerm(const C& c, typename details::IsContainer<C>::value_type offset = 0) {
	return isPerm(std::begin(c), std::end(c), offset);
}

template<typename itA, typename itB>
bool isPerm(itA firstA, itA lastA, itB firstB, itB lastB) {
	std::vector<typename std::iterator_traits<itA>::value_type> a(firstA, lastA);
	std::vector<typename std::iterator_traits<itB>::value_type> b(firstB, lastB);
	if (a.size() != b.size()) return false;
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());
	for (std::size_t i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) return false;
	}
	return true;
}
template<typename C1,
         typename C2,
         typename = std::enable_if_t<details::IsContainer<C1>{}>,
         typename = std::enable_if_t<details::IsContainer<C2>{}>>
bool isPerm(const C1& c1, const C2& c2) {
	return isPerm(std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
}

template<typename RandomIt, typename BinaryPredicate>
constexpr bool anyAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	return std::adjacent_find(first, last, p) != last;
}
template<typename C>
constexpr bool anyAdjacent(const C& c) {
	return anyAdjacent(std::begin(c), std::end(c));
}

template<typename RandomIt, typename BinaryPredicate>
constexpr bool noneAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	return std::adjacent_find(first, last, p) == last;
}
template<typename C>
constexpr bool noneAdjacent(const C& c) {
	return noneAdjacent(std::begin(c), std::end(c));
}

template<typename RandomIt, typename BinaryPredicate>
constexpr bool allAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	return std::adjacent_find(first, last, std::not_fn(p)) == last;
}
template<typename C>
constexpr bool allAdjacent(const C& c) {
	return allAdjacent(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areIncreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::less<T>());
}
template<typename C>
constexpr bool areIncreasing(const C& c) {
	return areIncreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areNonDecreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::less_equal<T>());
}
template<typename C>
constexpr bool areNonDecreasing(const C& c) {
	return areNonDecreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areDecreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::greater<T>());
}
template<typename C>
constexpr bool areDecreasing(const C& c) {
	return areDecreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areNonIncreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::greater_equal<T>());
}
template<typename C>
constexpr bool areNonIncreasing(const C& c) {
	return areNonIncreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areDistinct(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	std::vector<T> tmp(first, last);
	std::sort(tmp.begin(), tmp.end());
	return areIncreasing(tmp.begin(), tmp.end());
}
template<typename C>
constexpr bool areDistinct(const C& c) {
	return areDistinct(std::begin(c), std::end(c));
}


// for strings (cctype functions are not safe to use with char...)
constexpr bool isLower(char c) {
	return c >= 'a' and c <= 'z';
}

constexpr bool isUpper(char c) {
	return c >= 'A' and c <= 'Z';
}

constexpr bool isLetter(char c) {
	return isLower(c) or isUpper(c);
}

constexpr bool isDigit(char c) {
	return c >= '0' and c <= '9';
}

constexpr char toLower(char c) {
	if (isUpper(c)) c += 'a' - 'A';
	return c;
}

constexpr bool isVowel(char c) {
	c = toLower(c);
	for (char x : LOWER_VOWELS) {
		if (c == x) return true;
	}
	return false;
}

constexpr bool isConsonant(char c) {
	return isLetter(c) and !isVowel(c);
}

constexpr char toUpper(char c) {
	if (isLower(c)) c -= 'a' - 'A';
	return c;
}

constexpr char toDefaultCase(char c) {
	if constexpr (DEFAULT_CASE_LOWER) return toLower(c);
	return toUpper(c);
}

void toLower(std::string& s) {
	for (char& c : s) c = toLower(c);
}

void toUpper(std::string& s) {
	for (char& c : s) c = toUpper(c);
}

void toDefaultCase(std::string& s) {
	if constexpr (DEFAULT_CASE_LOWER) return toLower(s);
	return toUpper(s);
}

constexpr bool isLower(std::string_view s) {
	for (char c : s) if (!isLower(c)) return false;
	return true;
}

constexpr bool isUpper(std::string_view s) {
	for (char c : s) if (!isUpper(c)) return false;
	return true;
}

constexpr bool isLetter(std::string_view s) {
	for (char c : s) if (!isLetter(c)) return false;
	return true;
}

constexpr bool isDigit(std::string_view s) {
	for (char c : s) if (!isDigit(c)) return false;
	return true;
}

constexpr bool isVowel(std::string_view s) {
	for (char c : s) if (!isVowel(c)) return false;
	return true;
}

constexpr bool isConsonant(std::string_view s) {
	for (char c : s) if (!isConsonant(c)) return false;
	return true;
}

std::vector<Integer> thueMorse(Integer lower, Integer upper) {
	judgeAssert<std::invalid_argument>(lower < upper, "thueMorse(): Lower must be less than upper!");
	std::vector<Integer> res(upper - lower);
	for (Integer i = lower; i < upper; i++) {
		res[i] = std::bitset<64>(i).count() % 2;
	}
	return res;
}

std::vector<Integer> thueMorse(Integer upper) {
	return thueMorse(0, upper);
}

// allow using std::pair and std::complex similiar
// (may be useful for geometric problem)
template<typename T>
constexpr auto& getX(T& point) {
	return std::get<0>(point);
}
template<typename T>
constexpr auto& getY(T& point) {
	return std::get<1>(point);
}
template<typename T>
constexpr auto& getZ(T& point) {
	return std::get<2>(point);
}
template<typename T>
constexpr auto getX(const T& point) {
	return std::get<0>(point);
}
template<typename T>
constexpr auto getY(const T& point) {
	return std::get<1>(point);
}
template<typename T>
constexpr auto getZ(const T& point) {
	return std::get<2>(point);
}

template<typename T>
constexpr auto& getX(std::complex<T>& point) {
	return reinterpret_cast<T(&)[2]>(point)[0];
}
template<typename T>
constexpr auto& getY(std::complex<T>& point) {
	return reinterpret_cast<T(&)[2]>(point)[1];
}
template<typename T>
constexpr auto getX(const std::complex<T>& point) {
	return reinterpret_cast<const T(&)[2]>(point)[0];
}
template<typename T>
constexpr auto getY(const std::complex<T>& point) {
	return reinterpret_cast<const T(&)[2]>(point)[1];
}

template<typename T>
constexpr std::pair<T,T> convert(const std::complex<T>& t) {
	return {getX(t), getY(t)};
}

template<typename L, typename R>
constexpr std::complex<typename std::common_type<L, R>::type>
convert(const std::pair<L,R>& t) {
	return {getX(t), getY(t)};
}


namespace details {
	// Test two numbers for equality, accounting for +/-INF, NaN and precision.
	// Real expected is considered the reference value for relative error.
	bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol) {
		judgeAssert<std::domain_error>(floatAbsTol >= 0.0_real, "floatEqual(): floatAbsTol must be positive!");
		judgeAssert<std::domain_error>(floatRelTol >= 0.0_real, "floatEqual(): floatRelTol must be positive!");
		// Finite values are compared with some tolerance
		if (std::isfinite(given) and std::isfinite(expected)) {
			Real absDiff = std::abs(given-expected);
			Real relDiff = std::abs((given-expected)/expected);
			return absDiff <= floatAbsTol or relDiff <= floatRelTol;
		}
		// NaN is equal to NaN (-NaN is also equal NaN)
		if (std::isnan(given) and std::isnan(expected)) {
			return true;
		}
		// Infinite values are equal if their sign matches
		if (std::isinf(given) and std::isinf(expected)) {
			return std::signbit(given) == std::signbit(expected);
		}
		// Values in different classes are always different.
		return false;
	}

	constexpr bool stringEqual(std::string_view a, std::string_view b, bool caseSensitive) {
		if (a.size() != b.size()) {
			return false;
		} else if (caseSensitive) {
			return a == b;
		} else {
			for (std::size_t i = 0; i < a.size(); i++) {
				if (toDefaultCase(a[i]) != toDefaultCase(b[i])) {
					return false;
				}
			}
			return true;
		}
	}

	constexpr bool isToken(std::string_view a) {
		for (char c : a) {
			if (c == ' ') return false;
			if (c == '\n') return false;
			if (c == '\r') return false;
			if (c == '\t') return false;
			if (c == '\f') return false;
			if (c == '\v') return false;
		}
		return true;
	}

	template<typename T>
	bool parse(std::string_view s, T& res) {
		const auto* begin = s.data();
		const auto* end = s.data() + s.size();
		auto [ptr, ec] = std::from_chars(begin, end, res);
		return ptr == end and ec == std::errc();
	}
	#ifdef DOUBLE_FALLBACK
	template<>
	bool parse(std::string_view s, Real& res) {
		try {
			std::size_t pos = 0;
			res = std::stold(std::string(s), &pos);
			return pos == s.size();
		} catch(...) {
			return false;
		}
	}
	#endif

}


//============================================================================//
// Math                                                                       //
//============================================================================//
namespace details {
	constexpr std::array<Integer, 32> TRIAL_PRIMES = {
		 2,  3,  5,  7, 11, 13, 17, 19, 23,  29,  31,  37,  41,  43,  47,  53,
		59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
	};
	constexpr std::array<UInteger, 7> MILLER_RABIN_WITNESS = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

	// these operations are safe as long as the value would fit in Integer
	constexpr UInteger mulMod(UInteger lhs, UInteger rhs, UInteger mod) {
		UInteger res = 0;
		while (rhs > 0) {
			if (rhs & 1) res = (lhs + res) % mod;
			lhs = (lhs + lhs) % mod;
			rhs /= 2;
		}
		return res;
	}

	constexpr UInteger powMod(UInteger base, UInteger exp, UInteger mod) {
		UInteger res = 1;
		if (mod <= 0x1'0000'0000) {
			while (exp > 0) {
				if (exp & 1) res = (base * res) % mod;
				base = (base * base) % mod;
				exp /= 2;
			}
		} else {
			while (exp > 0) {
				if (exp & 1) res = mulMod(base, res, mod);
				base = mulMod(base, base, mod);
				exp /= 2;
			}
		}
		return res;
	}

	constexpr Integer extendedEuclid(Integer a, Integer b, Integer& x, Integer& y) {
		if (a == 0) {
			x = 0;
			y = 1;
			return b;
		} else {
			Integer x1 = 0;
			Integer y1 = 0;
			Integer d = extendedEuclid(b % a, a, x1, y1);
			x = y1 - (b / a) * x1;
			y = x1;
			return d;
		}
	}
}

constexpr Integer applyMod(Integer x, Integer mod) {
	x %= mod;
	if (x < 0) x += mod;
	return x;
}

constexpr Integer mulMod(Integer lhs, Integer rhs, Integer mod) {
	judgeAssert<std::domain_error>(mod > 0, "mulMod(): mod must be positive!");
	UInteger ul = static_cast<UInteger>(applyMod(lhs, mod));
	UInteger ur = static_cast<UInteger>(applyMod(rhs, mod));
	UInteger um = static_cast<UInteger>(mod);
	return static_cast<Integer>(details::mulMod(ul, ur, um));
}

constexpr Integer powMod(Integer base, Integer exp, Integer mod) {
	judgeAssert<std::domain_error>(mod > 0, "powMod(): mod must be positive!");
	judgeAssert<std::domain_error>(exp >= 0, "powMod(): exp must be non negative!");
	UInteger ub = static_cast<UInteger>(applyMod(base, mod));
	UInteger ue = static_cast<UInteger>(exp);
	UInteger um = static_cast<UInteger>(mod);
	return static_cast<Integer>(details::powMod(ub, ue, um));
}

constexpr Integer multInv(Integer n, Integer mod) {
	judgeAssert<std::domain_error>(mod > 0, "multInv(): mod must be positive!");
	Integer x = 0;
	Integer y = 0;
	Integer g = details::extendedEuclid(n, mod, x, y);
	if (g != 1) return -1;
	else return applyMod(x, mod);
}

constexpr bool isPrime(Integer n) {
	for (Integer p : details::TRIAL_PRIMES) {
		if (n <= p or n % p == 0) {
			return n == p;
		}
	}
	if (details::powMod(details::TRIAL_PRIMES.back() + 1, n - 1, n) != 1) {
		return false;
	}
	UInteger un = static_cast<UInteger>(n);
	UInteger d = un - 1;
	UInteger j = 0;
	while (d % 2 == 0) {
		d /= 2;
		j++;
	}
	for (UInteger a : details::MILLER_RABIN_WITNESS) {
		if (a % un == 0) continue;
		UInteger v = details::powMod(a, d, un);
		if (v == 1 or v == un - 1) continue;
		for (UInteger i = 1; i < j; i++) {
			v = details::mulMod(v, v, un);
			if (v == un - 1 or v <= 1) break;
		}
		if (v != un - 1) return false;
	}
	return true;
}

std::vector<Integer> primes(Integer lower, Integer upper) {
	judgeAssert<std::invalid_argument>(lower < upper, "primes(): Lower must be less than upper!");
	lower = std::max<Integer>(2, lower);
	upper = std::max<Integer>(2, upper);
	Integer count = upper - lower;
	Integer cache = (count + 1) / 2;

	std::vector<bool> notPrime(cache), notPrimeSegment(cache);
	for (Integer i = 3; i < count; i += 2) {
		if (!notPrime[i / 2]) {
			for (Integer j = i * i; j < count; j += 2 * i) {
				notPrime[j / 2] = true;
			}
			Integer lowest = lower - (lower % (2*i)) + i;
			if (lowest < lower) lowest += 2*i;
			for (Integer j = std::max(i * i, lowest); j < upper; j += 2 * i) {
				notPrimeSegment[(j - lower) / 2] = true;
			}
		}
	}
	std::vector<Integer> res;
	if (lower <= 2 and 2 < upper) res.emplace_back(2);
	for (Integer i = lower | 1; i < upper; i += 2) {
		if (!notPrimeSegment[(i - lower) / 2] and (i < count*count or isPrime(i))) {
			res.emplace_back(i);
		}
	}
	return res;
}

std::vector<Integer> primes(Integer upper) {
	return primes(0, upper);
}

template<typename T>
constexpr Integer sign(T x) {
	return (T(0) < x) - (x < T(0));
}


//============================================================================//
// Geometry (this is just for utility stuff...)                               //
//============================================================================//
namespace details {
	template<typename Point>
	constexpr Integer cross(Point a, Point b) {
		return getX(a) * getY(b) - getY(a) * getX(b);
	}
	template<typename Point>
	constexpr Integer cross(Point p, Point a, Point b) {
		getX(a) -= getX(p);
		getY(a) -= getY(p);
		getX(b) -= getX(p);
		getY(b) -= getY(p);
		return cross(a, b);
	}

	template<typename Point>
	constexpr bool left(Point p) {
		return getX(p) == 0 ? getY(p) < 0 : getX(p) < 0;
	}

	template<typename Point>
	void cyclicSort(std::vector<Point>& in) {
		std::sort(in.begin(), in.end(), [](const Point& a, const Point& b){
			return left(a) != left(b) ? left(a) > left(b) : cross(a, b) > 0;
		});
	}
}


template<typename RandomIt>
constexpr bool areConvex(RandomIt first, RandomIt last) {
	std::size_t n = 0;
	for (auto it = first; it != last; it++) {
		n++;
		judgeAssert(std::abs(getX(*it)) <= 0x3FFF'FFFF, "areConvex(): coordinates too large!");
		judgeAssert(std::abs(getY(*it)) <= 0x3FFF'FFFF, "areConvex(): coordinates too large!");
	}
	if (n < 3) return false;
	bool hasArea = false;
	for (std::size_t i = 0; i < n; i++) {
		if (first[i] == first[(i+1) % n]) return false;
		if (details::cross(first[0], first[i], first[(i+1) % n]) < 0) return false;
		if (details::cross(first[i], first[(i+1) % n], first[(i+2) % n]) < 0) return false;
		hasArea |= details::cross(first[i], first[(i+1) % n], first[(i+2) % n]) != 0;
	}
	return hasArea;
}
template<typename C>
constexpr bool areConvex(const C& c) {
	return areConvex(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool areStrictlyConvex(RandomIt first, RandomIt last) {
	if (!areConvex(first, last)) return false;
	std::size_t n = std::distance(first, last);
	for (std::size_t i = 0; i < n; i++) {
		if (details::cross(first[i], first[(i+1) % n], first[(i+2) % n]) == 0) return false;
	}
	return true;
}
template<typename C>
constexpr bool areStrictlyConvex(const C& c) {
	return areStrictlyConvex(std::begin(c), std::end(c));
}

//============================================================================//
// Random                                                                     //
//============================================================================//
namespace Random {
	// You should not rely on the implementation in details!
	// Especially you should never use randomNumberGenerator on your own. there is no function in
	// c++ which uses a random engine and is not implementation defined.
	namespace details {
		constexpr Real PI = 3.141592653589793238462643383279502884_real;
		constexpr Integer PRIME_TRIALS = 4*1600;

		RandomEngine randomNumberGenerator(DEFAULT_SEED);
		static_assert(RandomEngine::max() == 0xFFFF'FFFF'FFFF'FFFF_uint, "Random Engine should produce 64bit of randomness");
		static_assert(RandomEngine::min() == 0_uint, "Random Engine should produce 64bit of randomness");

		constexpr UInteger bitMask(UInteger x) {
			static_assert(sizeof(UInteger) == 8, "bitMask requires 8byte UInteger!");
			x |= x >> 1;
			x |= x >> 2;
			x |= x >> 4;
			x |= x >> 8;
			x |= x >> 16;
			x |= x >> 32;
			return x;
		}

		Real logFac(Integer n) {
			if (n < 16) {
				Integer fac = 1;
				for (Integer i = 2; i <= n; i++) fac *= i;
				return std::log(fac);
			} else {
				// https://mathworld.wolfram.com/StirlingsSeries.html
				return 0.5_real * std::log(2.0_real * PI)
				     + (n + 0.5_real) * std::log(n + 1.0_real)
				     - (n + 1)
				     + 1.0_real / (12 * (n + 1.0_real))
				     - 1.0_real / (360 * std::pow(n + 1.0_real, 3))
				     + 1.0_real / (1260 * std::pow(n + 1.0_real, 5))
				     - 1.0_real / (1680 * std::pow(n + 1.0_real, 7));
				     //+ 1.0_real / (1188 * std::pow(n + 1.0_real, 9));
			}
		}
	}

	void seed(UInteger seed) {
		details::randomNumberGenerator.seed(seed);
	}

	//========================================================================//
	// Distributions and generators                                           //
	//========================================================================//
	bool bit() {// in {0, 1}
		return std::bitset<64>(details::randomNumberGenerator()).count() & 1;
	}

	Integer integer() {// in [-2^63, 2^63)
		return static_cast<Integer>(details::randomNumberGenerator());
	}
	Integer integer(Integer lower, Integer upper) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(lower < upper, "Random::integer(): Lower must be less than upper!");
		UInteger ul = static_cast<UInteger>(lower);
		UInteger uu = static_cast<UInteger>(upper);
		UInteger mask = details::bitMask(uu - ul - 1_uint);
		UInteger res;
		do {
			res = details::randomNumberGenerator() & mask;
		} while (res >= uu - ul);
		return static_cast<Integer>(res + ul);
	}
	Integer integer(Integer upper) {// in [0, upper)
		return integer(0, upper);
	}

	Real real() {// in [0, 1)
		while (true) {
			Real res = details::randomNumberGenerator() / 0x1.0p64_real;
			res += details::randomNumberGenerator() / 0x1.0p128_real;
			if (0.0_real <= res and res < 1.0_real) return res;
		}
	}
	Real real(Real upper) {// in [0, upper)
		judgeAssert<std::domain_error>(std::isfinite(upper), "Random::real(): Upper must be finite!");
		judgeAssert<std::invalid_argument>(upper > 0.0_real, "Random::real(): Upper must be greater than zero!");
		while (true) {
			Real res = real() * upper;
			if (0.0_real <= res and res < upper) return res;
		}
	}
	Real real(Real lower, Real upper) {// in [lower, upper)
		judgeAssert<std::domain_error>(std::isfinite(lower), "Random::real(): Lower must be finite!");
		judgeAssert<std::domain_error>(std::isfinite(upper), "Random::real(): Upper must be finite!");
		judgeAssert<std::invalid_argument>(lower < upper, "Random::real(): Lower must be less than upper!");
		while (true) {
			Real x = real();
			Real res = lower * (1.0_real - x) + upper * x;
			if (lower <= res and res < upper) return res;
		}
	}

	Real normal(Real mean, Real stddev) {// theoretically in (-inf, inf)
		judgeAssert<std::domain_error>(stddev > 0.0_real, "Random::normal(): Standard deviation must be positive!");
		Real u1 = real();
		Real u2 = real();
		Real res = std::sqrt(-2.0_real * std::log(u1)) * std::cos(2.0_real * details::PI * u2);
		return std::sqrt(stddev) * res + mean;
	}
	Real normal(Real lower, Real upper, Real mean, Real stddev) {// in [lower, upper)
		judgeAssert<std::domain_error>(!std::isnan(lower), "Random::normal(): Lower must not be NaN!");
		judgeAssert<std::domain_error>(!std::isnan(upper), "Random::normal(): Upper must not be NaN!");
		judgeAssert<std::invalid_argument>(lower < upper, "Random::normal(): Lower must be less than upper!");
		judgeAssert<std::domain_error>(stddev > 0.0_real, "Random::normal(): Standard deviation must be positive!");
		Real res;
		while (true) {
			Real u1 = real();
			Real u2 = real();
			// Box-Muller-Methode
			// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
			res = std::sqrt(-2.0_real * std::log(u1)) * std::cos(2.0_real * details::PI * u2);
			res = std::sqrt(stddev) * res + mean;
			if (lower <= res and res < upper) return res;
			res = std::sqrt(-2.0_real * std::log(u1)) * std::sin(2.0_real * details::PI * u2);
			res = std::sqrt(stddev) * res + mean;
			if (lower <= res and res < upper) return res;
		}
	}

	Real exponential(Real lambda) {// theoretically in [0, inf)
		judgeAssert<std::domain_error>(lambda > 0.0_real, "Random::lambda(): lambda must be positive!");
		return -std::log(real()) / lambda;
	}
	Real exponential(Real lower, Real upper, Real lambda) {// in [lower, upper)
		judgeAssert<std::domain_error>(std::isfinite(lower), "Random::exponential(): Lower must be finite!");
		judgeAssert<std::domain_error>(!std::isnan(upper), "Random::exponential(): Upper must not be NaN!");
		judgeAssert<std::invalid_argument>(lower < upper, "Random::exponential(): Lower must be less than upper!");
		judgeAssert<std::domain_error>(lambda > 0.0_real, "Random::exponential(): Lambda must be positive!");
		while (true) {
			Real res = lower - std::log(real()) / lambda;
			if (res < upper) return res;
		}
	}

	Integer geometric(Real p) {// theoretically in [0, inf)
		judgeAssert<std::domain_error>(0.0_real <= p and p < 1.0_real, "Random::geometric(): p must be in [0,1)!");
		return std::llround(std::floor(std::log(real()) / std::log1p(-p)));
	}
	Integer geometric(Integer lower, Integer upper, Real p) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(lower < upper, "Random::geometric(): Lower must be less than upper!");
		judgeAssert<std::domain_error>(0.0_real <= p and p < 1.0_real, "Random::geometric(): p must be in [0,1)!");
		while (true) {
			// https://en.wikipedia.org/wiki/Geometric_distribution
			// "The exponential distribution is the continuous analogue of the geometric distribution[...]"
			Integer res = lower + std::llround(std::floor(std::log(real()) / std::log1p(-p)));
			if (res < upper) return res;
		}
	}

	Integer binomial(Integer n, Real p) {// in [0, n]
		judgeAssert<std::domain_error>(n >= 0, "Random::binomial(): n must be non negative!");
		judgeAssert<std::domain_error>(0.0_real <= p and p < 1.0_real, "Random::binomial(): p must be in [0,1)!");
		bool swap = p > 0.5_real;
		p = std::min(p, 1.0_real - p);
		if (p*n <= 16.0_real) {
			// inverse sampling
			Real q = 1.0_real - p;
			Real s = p / q;
			Real a = (n + 1) * s;
			Real r = std::pow(q, n);
			Real u = real();
			Integer res = 0;
			while (u > r) {
				u -= r;
				res++;
				r *= (a / res) - s;
			}
			return swap ? n - res : res;
		} else {
			// BTRS algorithm
			// https://epub.wu.ac.at/1242/1/document.pdf
			Real q = 1.0_real - p;
			Real spq = std::sqrt(n * p * q);
			Real b = 1.15_real + 2.53_real * spq;
			Real a = -0.0873_real + 0.0248_real * b + 0.01_real * p;
			Real c = n * p + 0.5_real;
			Real vr = 0.92_real - 4.2_real / b;
			do {
				Real v, us;
				Integer res;
				do {
					v = real();
					Real u = real() - 0.5_real;
					us = 0.5_real - std::abs(u);
					res = static_cast<Integer>(std::floor((2.0_real * a / us + b) * u + c));
				} while (res < 0 or res > n);
				if (us >= 0.07_real and v <= vr) {
					return swap ? n - res : res;
				}
				Real alpha = (2.83_real + 5.1_real / b) * spq;
				Real lpq = std::log(p / q);
				Integer m = static_cast<Integer>(std::floor((n + 1) * p));
				Real h = details::logFac(m) + details::logFac(n - m);
				v += alpha / (a / (us * us) + b);
				if (v <= h - details::logFac(res) - details::logFac(n - res) + (res - m) * lpq) {
					return swap ? n - res : res;
				}
			} while (true);
		}
	}
	Integer binomial(Integer lower, Integer upper, Integer n, Real p) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(lower < upper, "Random::binomial(): n Lower must be less than upper!");
		while (true) {
			Integer res = binomial(n, p);
			if (lower <= res and res < upper) return res;
		}
	}

	Integer maximum(Integer lower, Integer upper, Integer n) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(n > 0, "Random::maximum(): n musst be positive!");
		judgeAssert<std::invalid_argument>(lower < upper, "Random::maximum(): Lower must be less than upper!");
		if (n < 5) {
			Integer res = lower;
			for (Integer i = 0; i < n; i++) res = std::max(res, integer(lower, upper));
			return res;
		} else {// such large n seem unlikely
			UInteger ul = static_cast<UInteger>(lower);
			UInteger uu = static_cast<UInteger>(upper);
			UInteger res = (uu - ul) * std::exp2(std::log2(real()) / n);
			return std::min(upper - 1, static_cast<Integer>(res + ul));
		}
	}
	Integer maximum(Integer upper, Integer n) {
		return maximum(0, upper, n);
	}

	Integer minimum(Integer lower, Integer upper, Integer n) {// in [lower, upper)
		return upper - 1 - maximum(lower, upper, n);
	}
	Integer minimum(Integer upper, Integer n) {
		return minimum(0, upper, n);
	}

	Integer prime(Integer lower, Integer upper) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(lower < upper, "Random::prime(): Lower must be less than upper!");
		Integer sampleL = lower <= 2 ? 0 : (lower / 2);
		Integer sampleU = upper / 2;
		if (sampleL < sampleU) {
			for (Integer i = 0; i < details::PRIME_TRIALS and i < 4 * (upper - lower); i++) {
				Integer res = std::max<Integer>(2, 2*integer(sampleL, sampleU) | 1);
				if (isPrime(res)) return res;
			}
		}
		judgeAssert<std::domain_error>(false, "Random::prime(): range contains no primes?");
		return -1;
	}
	Integer prime(Integer upper) {// in [0, upper)
		return prime(0, upper);
	}


	//========================================================================//
	// utility                                                                //
	//========================================================================//
	template<typename RandomIt>
	typename std::iterator_traits<RandomIt>::reference
	select(RandomIt first, RandomIt last) {
		judgeAssert<std::invalid_argument>(first < last, "Random::select(): Lower must be less than upper!");
		return first[integer(0, last - first)];
	}

	template<typename C>
	typename ::details::IsContainer<C>::value_type select(const C& c) {
		return select(std::begin(c), std::end(c));
	}

	template<typename C>
	typename C::reference select(C& c) {
		return select(std::begin(c), std::end(c));
	}

	template<typename T, std::size_t N>
	T select(const T(&c)[N]) {
		return select(std::begin(c), std::end(c));
	}

	template<typename T, std::size_t N>
	T& select(T(&c)[N]) {
		return select(std::begin(c), std::end(c));
	}

	template<typename T>
	T select(const std::pair<T, T>& t) {
		return bit() ? getX(t) : getY(t);
	}

	template<typename T>
	T select(const std::complex<T>& t) {
		return bit() ? getX(t) : getY(t);
	}

	template<typename RandomIt>
	void shuffle(RandomIt first, RandomIt last) {
		using std::swap;
		auto n = last - first;
		for (auto i = n-1; i > 0; i--) {
			swap(first[i], first[integer(0, i+1)]);
		}
	}

	template<typename C>
	void shuffle(C& c) {
		return shuffle(std::begin(c), std::end(c));
	}

	template<typename T>
	void shuffle(std::pair<T, T>& t) {
		using std::swap;
		if (bit()) swap(getX(t), getY(t));
	}

	template<typename T>
	void shuffle(std::complex<T>& t) {
		using std::swap;
		if (bit()) swap(getX(t), getY(t));
	}

	//========================================================================//
	// sequences                                                              //
	//========================================================================//
	std::vector<Integer> distinct(Integer count, Integer lower, Integer upper) {
		judgeAssert<std::invalid_argument>(count >= 0, "Random::distinct(): Count must be non negative!");
		judgeAssert<std::invalid_argument>(lower + count <= upper, "Random::distinct(): Lower must be less than upper + count!");
		std::map<Integer, Integer> used;
		std::vector<Integer> res;
		for (Integer i = 0; i < count; i++) {
			Integer x = integer(lower, upper - i);
			auto it = used.find(x);
			if (it != used.end()) res.emplace_back(it->second);
			else res.emplace_back(x);
			it = used.find(upper - i - 1);
			if (it != used.end()) used[x] = it->second;
			else used[x] = upper - i - 1;
		}
		return res;
	}
	std::vector<Integer> distinct(Integer count, Integer upper) {
		return distinct(count, 0, upper);
	}

	std::vector<Integer> perm(Integer count, Integer offset = 0) {
		return distinct(count, offset, offset+count);
	}

	std::vector<Integer> perm(const std::vector<Integer>& cycles, Integer offset = 0) {
		auto p = perm(std::accumulate(cycles.begin(), cycles.end(), 0_int));
		std::vector<Integer> res(p.size());
		Integer tmp = 0;
		for (std::size_t i = 0; i < cycles.size(); tmp += cycles[i], i++) {
			judgeAssert<std::invalid_argument>(cycles[i] > 0, "Random::perm(): Cycle lengths must be positive!");
			for (Integer j = tmp; j + 1 < tmp + cycles[i]; j++) {
				res[p[j]] = p[j + 1] + offset;
			}
			res[p[tmp + cycles[i] - 1]] = p[tmp] + offset;
		}
		return res;
	}

	std::vector<Integer> perm(std::initializer_list<Integer> cycles, Integer offset = 0) {
		return perm(std::vector<Integer>(cycles), offset);
	}

	std::vector<Integer> multiple(Integer count, Integer lower, Integer upper) {
		std::vector<Integer> res(count);
		for (Integer& x : res) x = integer(lower, upper);
		return res;
	}
	std::vector<Integer> multiple(Integer count, Integer upper) {
		return multiple(count, 0, upper);
	}

	std::vector<Integer> increasing(Integer count, Integer lower, Integer upper) {
		std::vector<Integer> res = distinct(count, lower, upper);
		std::sort(res.begin(), res.end(), std::less<Integer>());
		return res;
	}
	std::vector<Integer> increasing(Integer count, Integer upper) {
		return increasing(count, 0, upper);
	}

	std::vector<Integer> decreasing(Integer count, Integer lower, Integer upper) {
		std::vector<Integer> res = distinct(count, lower, upper);
		std::sort(res.begin(), res.end(), std::greater<Integer>());
		return res;
	}
	std::vector<Integer> decreasing(Integer count, Integer upper) {
		return decreasing(count, 0, upper);
	}

	std::vector<Integer> nonDecreasing(Integer count, Integer lower, Integer upper) {
		std::vector<Integer> res = multiple(count, lower, upper);
		std::sort(res.begin(), res.end(), std::less<Integer>());
		return res;
	}
	std::vector<Integer> nonDecreasing(Integer count, Integer upper) {
		return nonDecreasing(count, 0, upper);
	}

	std::vector<Integer> nonIncreasing(Integer count, Integer lower, Integer upper) {
		std::vector<Integer> res = multiple(count, lower, upper);
		std::sort(res.begin(), res.end(), std::greater<Integer>());
		return res;
	}
	std::vector<Integer> nonIncreasing(Integer count, Integer upper) {
		return nonIncreasing(count, 0, upper);
	}

	std::vector<Integer> partition(Integer n, Integer k, Integer min = 1) {
		judgeAssert<std::invalid_argument>(n > 0, "Random::partition(): n must be positive!");
		judgeAssert<std::invalid_argument>(k > 0, "Random::partition(): k must be positive!");
		judgeAssert<std::invalid_argument>(min <= 0 or k <= n / min, "Random::partition(): k too large!");
		n -= (min - 1) * k;
		std::vector<Integer> res = increasing(k-1, 1, n);
		res.emplace_back(n);
		for (Integer i = 0, last = 0; i < k; i++) {
			res[i] -= last;
			last += res[i];
			res[i] += min - 1;
		}
		return res;
	}

	std::string bracketSequence(Integer n, char open = '(', char close = ')') {//proper bracket sequence of length 2*n
		judgeAssert<std::invalid_argument>(0 <= n and n <= 0x7FFF'FFFF, "Random::bracketSequence(): n out of range!");
		std::string res(2 * n, open);
		for (Integer i = 0, diff = 0; i < 2 * n; i++) {
			Integer opened = (i + diff) / 2;
			if (integer((2 * n - i) * (diff + 1)) < (n - opened) * (diff + 2)) {
				diff++;
			} else {
				res[i] = close;
				diff--;
			}
		}
		return res;
	}

	//========================================================================//
	// geometry                                                               //
	//========================================================================//
	template<typename Point = std::pair<Integer, Integer>>
	std::vector<Point> convex(Integer n, Integer dim) {
		judgeAssert<std::invalid_argument>(dim <= 0x3FFF'FFFF, "Random::convex(): dim too large!");
		judgeAssert<std::invalid_argument>(dim > 0, "Random::convex(): dim must be positive!");
		judgeAssert<std::invalid_argument>(n <= 8*dim - 8, "Random::convex(): dim too small!");
		judgeAssert<std::invalid_argument>(n >= 3, "Random::convex(): n too small!");

		while (true) {
			Integer left = 1 + binomial(n - 2, 0.5);
			Integer down = 1 + binomial(n - 2, 0.5);
			auto x = partition(2 * dim - 2, left, 0);
			auto y = partition(2 * dim - 2, down, 0);
			for (auto& z : x) z = -z;
			for (auto& z : y) z = -z;
			append(x, partition(2 * dim - 2, n - left, 0));
			append(y, partition(2 * dim - 2, n - down, 0));
			auto itX = std::partition(x.begin(), x.end(), [](Integer z){return z == 0;});
			auto itY = std::partition(y.begin(), y.end(), [](Integer z){return z != 0;});
			if (std::distance(x.begin(), itX) + std::distance(itY, y.end()) > n) continue;
			shuffle(itX, x.end());
			if (itX != x.begin()) shuffle(y.begin(), itY);

			std::vector<Point> dirs(n);
			for (Integer i = 0; i < n; i++) {
				dirs[i] = {x[i], y[i]};
			}
			::details::cyclicSort(dirs);

			std::vector<Point> res = {{0, 0}};
			Integer maxX = 0;
			Integer maxY = 0;
			for (auto dir : dirs) {
				Point tmp = res.back();
				getX(tmp) += getX(dir);
				getY(tmp) += getY(dir);
				maxX = std::max(maxX, getX(tmp));
				maxY = std::max(maxY, getY(tmp));
				res.emplace_back(tmp);
			}
			res.pop_back();
			for (auto& point : res) {
				getX(point) += dim - 1 - maxX;
				getY(point) += dim - 1 - maxY;
			}
			return res;
		}
	}

} // namespace Random


//============================================================================//
// args parser                                                                //
//============================================================================//
class ParamaterBase {
	friend class Command;
	friend struct Paramater;

	std::optional<std::string_view> token;

	template<typename T>
	T parse(std::string_view s) const {
		T res = {};
		judgeAssert<std::invalid_argument>(details::parse<T>(s, res), "Command: Could not parse args");
		return res;
	}

	ParamaterBase() = default;
	explicit ParamaterBase(std::string_view token_) : token(token_) {}

public:
	std::string asString() const {
		return std::string(token.value());
	}

	std::string asString(std::string_view defaultValue) const {
		return std::string(token.value_or(defaultValue));
	}

	Integer asInteger() const {
		return parse<Integer>(token.value());
	}

	Integer asInteger(Integer defaultValue) const {
		return token ? asInteger() : defaultValue;
	}

	Real asReal() const {
		return parse<Real>(token.value());
	}

	Real asReal(Real defaultValue) const {
		return token ? asReal() : defaultValue;
	}
};

struct Paramater final : private ParamaterBase {
	using ParamaterBase::ParamaterBase;
	using ParamaterBase::asString;
	using ParamaterBase::asInteger;
	using ParamaterBase::asReal;

	bool exists() const {
		return token.has_value();
	}

	explicit operator bool() const {
		return exists();
	}
};

class Command final : private ParamaterBase {
	const std::vector<std::string>& raw;
	const Integer first, count;
	const bool found;
public:
	explicit Command(const std::vector<std::string>& raw_) : raw(raw_), first(0), count(0), found(false) {}
	explicit Command(const std::vector<std::string>& raw_, Integer first_, Integer count_)
	                 : ParamaterBase(count_ == 0 ? ParamaterBase() : ParamaterBase(raw_[first_])),
	                   raw(raw_), first(first_), count(count_), found(true) {
		judgeAssert<std::invalid_argument>(count >= 0, "Command: Invalid command in args!");
	}

	bool exists() const {
		return found;
	}

	explicit operator bool() const {
		return exists();
	}

	Integer parameterCount() const {
		return count;
	}

	Paramater operator[](Integer i) const {
		if (i >= 0 and i < count) return Paramater(raw[first + i]);
		return Paramater();
	}

	using ParamaterBase::asString;
	using ParamaterBase::asInteger;
	using ParamaterBase::asReal;

	std::vector<std::string> asStrings() const {
		std::vector<std::string> res;
		std::transform(raw.begin() + first,
		               raw.begin() + first + count,
		               std::back_inserter(res), [](const std::string& value) {
			return std::string(value);
		});
		return res;
	}

	std::vector<Integer> asIntegers() const {
		std::vector<Integer> res;
		std::transform(raw.begin() + first,
		               raw.begin() + first + count,
		               std::back_inserter(res), [this](const std::string& value) {
			return parse<Integer>(value);
		});
		return res;
	}

	std::vector<Real> asReals() const {
		std::vector<Real> res;
		std::transform(raw.begin() + first,
		               raw.begin() + first + count,
		               std::back_inserter(res), [this](const std::string& value) {
			return parse<Real>(value);
		});
		return res;
	}

};

class CommandParser final {
	std::vector<std::string> raw;
	std::map<std::string_view, std::pair<Integer, Integer>> commands;
	std::map<std::string_view, Integer> tokens;

	static bool isCommand(std::string_view s) {
		return s.size() > 2 and s.substr(0, 2) == COMMAND_PREFIX;
	}
	void addCommand(std::string_view command, Integer first, Integer count = 0) {
		judgeAssert<std::invalid_argument>(commands.count(command) == 0, "Command: Duplcated command in args!");
		commands.emplace(command, std::pair<Integer, Integer>{first, count});
	}

public:
	CommandParser() = default;
	explicit CommandParser(int argc, char** argv) {
		raw.assign(argc, {});
		std::string_view command = EMPTY_COMMAND;
		Integer first = 0;
		Integer count = 0;
		for (int i = 0; i < argc; i++) {
			raw[i] = std::string(argv[i]);
			tokens.emplace(raw[i], i+1);
			if (isCommand(raw[i])) {
				addCommand(command, first, count);
				command = raw[i];
				first = i+1;
				count = 0;
			} else {
				count++;
			}
		}
		addCommand(command, first, count);
	}
	CommandParser(CommandParser&&) = default;
	CommandParser& operator=(CommandParser&&) = default;

	CommandParser(const CommandParser&) = delete;
	CommandParser& operator=(const CommandParser&) = delete;

	std::string_view operator[](Integer t) const {
		judgeAssert<std::out_of_range>(t >= 0 and t < static_cast<Integer>(raw.size()), "Command: Index out of args!");
		return raw[t];
	}
	Command operator[](std::string_view command) const & {
		judgeAssert<std::invalid_argument>(details::isToken(command), "Command: must not contain a space!");
		auto it = commands.find(command);
		if (it == commands.end()) return Command(raw);
		return Command(raw, it->second.first, it->second.second);
	}
	Command getRaw(std::string_view command) const & {
		judgeAssert<std::invalid_argument>(details::isToken(command), "Command: must not contain a space!");
		auto it = tokens.find(command);
		if (it == tokens.end()) return Command(raw);
		return Command(raw, it->second, raw.size() - it->second);
	}
	Command getRaw() const & {
		return Command(raw, 0, raw.size());
	}
};


//============================================================================//
// Constraints                                                                //
//============================================================================//
template<typename T>
class Bounds final {
	bool hadMin, hadMax;	// was value==lower/upper at some point
	T min, max;				// range of seen values
	T lower, upper;			// bounds for value
public:
	constexpr explicit Bounds(T lower_, T upper_, T value_) :
	                          hadMin(false), hadMax(false),
	                          min(value_), max(value_),
	                          lower(lower_), upper(upper_) {
		update(lower_, upper_, value_);
	}

	void update(T lower_, T upper_, T value_) {
		if constexpr (std::is_same_v<T, Real>) {
			hadMin |= details::floatEqual(value_, lower_, DEFAULT_EPS, DEFAULT_EPS);
			hadMax |= details::floatEqual(value_, upper_, DEFAULT_EPS, DEFAULT_EPS);
		} else {
			hadMin |= value_ == lower_;
			hadMax |= value_ == upper_;
		}
		min = std::min(min, value_);
		max = std::max(max, value_);
		lower = std::min(lower, lower_);
		upper = std::max(upper, upper_);
	}

	friend std::ostream& operator<<(std::ostream& os, const Bounds<T>& bounds) {
		os << bounds.hadMin << " " << bounds.hadMax << " ";
		os << bounds.min << " " << bounds.max << " ";
		return os << bounds.lower << " " << bounds.upper;
	}

};

namespace details {
	//using typeIndex = std::type_index;
	using typeIndex = void*;

	template<typename T>
	typeIndex getTypeIndex() {
		//return std::type_index(type id(T));
		static T* uniqueTypeIndex = nullptr;
		return &uniqueTypeIndex;
	}
}

class Constraint final {
	friend class ConstraintsLogger;
	std::variant<
		std::monostate,		// uninitialized
		Bounds<Integer>,	// Integer or container bound
		Bounds<Real>		// Real bound
	> bound;
	std::optional<details::typeIndex> type;

	template<typename T, typename X = T>
	void update(T lower, T upper, T value) {
		if constexpr(std::is_integral_v<T>) {
			upper--; // for BAPCtools the range is closed but we use half open ranges!
		}
		if (!type) {
			type = details::getTypeIndex<X>();
			bound = Bounds<T>(lower, upper, value);
		}
		judgeAssert<std::logic_error>(type == details::getTypeIndex<X>(), "Constraint: type must not change!");
		std::get<Bounds<T>>(bound).update(lower, upper, value);
	}
public:
	Constraint() = default;
	Constraint(Constraint&&) = default;
	Constraint& operator=(Constraint&&) = default;

	Constraint(const Constraint&) = delete;
	Constraint& operator=(const Constraint&) = delete;

	template<typename V, typename std::enable_if_t<std::is_integral_v<V>, bool> = true>
	void log(Integer lower, Integer upper, V value) {
		update<Integer>(lower, upper, value);
	}

	template<typename V, typename std::enable_if_t<std::is_floating_point_v<V>, bool> = true>
	void log(Real lower, Real upper, V value) {
		update<Real>(lower, upper, value);
	}

	template<typename C, typename std::enable_if_t<!std::is_arithmetic_v<C>, bool> = true>
	void log(Integer lower, Integer upper, const C& container) {
		update<Integer, C>(lower, upper, static_cast<Integer>(std::size(container)));
	}
};

class ConstraintsLogger final {
	std::optional<std::string> fileName;
	std::map<std::string, std::size_t> byName;
	std::vector<std::unique_ptr<Constraint>> constraints;
public:
	ConstraintsLogger() = default;
	explicit ConstraintsLogger(std::string_view fileName_) : fileName(fileName_) {}

	ConstraintsLogger(ConstraintsLogger&&) = default;
	ConstraintsLogger& operator=(ConstraintsLogger&&) = default;

	ConstraintsLogger(const ConstraintsLogger&) = delete;
	ConstraintsLogger& operator=(const ConstraintsLogger&) = delete;

	Constraint& operator[](const std::string& name) & {
		judgeAssert<std::invalid_argument>(details::isToken(name), "Constraint: name must not contain a space!");
		auto res = byName.try_emplace(name, constraints.size());
		if (res.second) constraints.emplace_back(std::make_unique<Constraint>());
		return *(constraints[res.first->second]);
	}

	void write() const {
		if (!fileName) return;
		std::ofstream os(*fileName);
		os << std::noboolalpha;
		os << std::fixed;
		os << std::setprecision(DEFAULT_PRECISION);
		for (const auto& [name, id] : byName) {
			const Constraint& c = *(constraints[id]);
			if (c.type) {
				os << "LocationNotSupported:" << name << " " << name << " ";
				if (c.bound.index() == 1) os << std::get<1>(c.bound);
				if (c.bound.index() == 2) os << std::get<2>(c.bound);
				os << std::endl;
			}
		}
	}

	~ConstraintsLogger() noexcept {
		write();
	}
};

//============================================================================//
// custom input stream                                                        //
//============================================================================//
class InputStream final {
	std::unique_ptr<std::ifstream> managed;
	std::istream* in;
	bool spaceSensitive, caseSensitive;
	Verdict onFail;
	Real floatAbsTol;
	Real floatRelTol;

	void init() {
		if (spaceSensitive) *in >> std::noskipws;
		else *in >> std::skipws;
	}

	void checkIn() {
		judgeAssert<std::runtime_error>(in != nullptr, "InputStream: not initialized!");
	}

public:
	InputStream() = default;
	explicit InputStream(const std::filesystem::path& path,
	                     bool spaceSensitive_,
	                     bool caseSensitive_,
	                     Verdict onFail_,
	                     Real floatAbsTol_ = DEFAULT_EPS,
	                     Real floatRelTol_ = DEFAULT_EPS) :
	                     managed(std::make_unique<std::ifstream>(path)),
	                     in(managed.get()),
	                     spaceSensitive(spaceSensitive_),
	                     caseSensitive(caseSensitive_),
	                     onFail(onFail_),
	                     floatAbsTol(floatAbsTol_),
	                     floatRelTol(floatRelTol_) {
		judgeAssert<std::runtime_error>(managed->good(), "InputStream: Could not open File: " + path.string());
		init();
	}
	explicit InputStream(std::istream& in_,
	                     bool spaceSensitive_,
	                     bool caseSensitive_,
	                     Verdict onFail_,
	                     Real floatAbsTol_ = DEFAULT_EPS,
	                     Real floatRelTol_ = DEFAULT_EPS) :
	                     managed(),
	                     in(&in_),
	                     spaceSensitive(spaceSensitive_),
	                     caseSensitive(caseSensitive_),
	                     onFail(onFail_),
	                     floatAbsTol(floatAbsTol_),
	                     floatRelTol(floatRelTol_) {
		init();
	}

	InputStream(InputStream&& other) = default;
	InputStream& operator=(InputStream&& other) = default;

	InputStream(const InputStream&) = delete;
	InputStream& operator=(const InputStream&) = delete;

	void eof() {
		checkIn();
		if (!spaceSensitive) *in >> std::ws;
		if (in->peek() != std::char_traits<char>::eof()) {
			in->get();
			ValidateBase::juryOut << "Missing EOF!";
			fail();
		}
	}

	void noteof() {
		checkIn();
		if (!spaceSensitive) *in >> std::ws;
		if (in->peek() == std::char_traits<char>::eof()) {
			ValidateBase::juryOut << "Unexpected EOF!" << onFail;
		}
	}

	void space() {
		if (spaceSensitive) {
			noteof();
			if (in->get() != std::char_traits<char>::to_int_type(SPACE)) {
				ValidateBase::juryOut << "Missing space!";
				fail();
			}
		}
	}

	void newline() {
		if (spaceSensitive) {
			noteof();
			if (in->get() != std::char_traits<char>::to_int_type(NEWLINE)) {
				ValidateBase::juryOut << "Missing newline!";
				fail();
			}
		}
	}

private:
	void check(const std::string& token, const std::regex& pattern) {
		if (!std::regex_match(token, pattern)) {
			ValidateBase::juryOut << "Token \"" << token << "\" does not match pattern!";
			fail();
		}
	}

	std::function<void()> checkSeparator(char separator) {
		if (separator == SPACE) return [this](){space();};
		if (separator == NEWLINE) return [this](){newline();};
		judgeAssert<std::invalid_argument>(false, "InputStream: Separator must be ' '  or '\\n'!");
		return {};
	}

	template<typename T>
	T parse(const std::string& s) {
		T res = {};
		if (!details::parse<T>(s, res)) {
			ValidateBase::juryOut << "Could not parse token \"" << s << "\"!";
			fail();
		}
		return res;
	}

public:
	std::string string() {
		noteof();
		if (spaceSensitive and !std::isgraph(in->peek())) {
			in->get();
			ValidateBase::juryOut << "Invalid whitespace!";
			fail();
		}
		std::string res;
		*in >> res;
		if (res.empty()) {
			ValidateBase::juryOut << "Unexpected EOF!" << onFail;
		}
		if (!caseSensitive) toDefaultCase(res);
		return res;
	}

	std::string string(Integer lower, Integer upper) {
		std::string t = string();
		Integer length = static_cast<Integer>(t.size());
		if (length < lower or length >= upper) {
			ValidateBase::juryOut << "String length " << length << " out of range [" << lower << ", " << upper << ")!";
			fail();
		}
		return t;
	}

	std::string string(Integer lower, Integer upper, Constraint& constraint) {
		std::string res = string(lower, upper);
		constraint.log(lower, upper, res);
		return res;
	}

	std::string string(const std::regex& pattern) {
		std::string t = string();
		check(t, pattern);
		return t;
	}

	std::string string(const std::regex& pattern, Integer lower, Integer upper) {
		std::string t = string(lower, upper);
		check(t, pattern);
		return t;
	}

	std::string string(const std::regex& pattern, Integer lower, Integer upper, Constraint& constraint) {
		std::string res = string(pattern, lower, upper);
		constraint.log(lower, upper, res);
		return res;
	}

	template<typename... Args>
	std::vector<std::string> strings(Args... args, Integer count, char separator) {
		auto sepCall = checkSeparator(separator);
		std::vector<std::string> res(count);
		for (Integer i = 0; i < count; i++) {
			res[i] = string(args...);
			if (i + 1 < count) sepCall();
		}
		return res;
	}

	std::vector<std::string> strings(Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<>(count, separator);
	}

	std::vector<std::string> strings(Integer lower, Integer upper,
	                                 Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<Integer, Integer>(lower, upper, count, separator);
	}

	std::vector<std::string> strings(Integer lower, Integer upper, Constraint& constraint,
	                                 Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<Integer, Integer, Constraint&>(lower, upper, constraint, count, separator);
	}

	std::vector<std::string> strings(const std::regex& pattern,
	                                 Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<const std::regex&>(pattern, count, separator);
	}

	std::vector<std::string> strings(const std::regex& pattern, Integer lower, Integer upper,
	                                 Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<const std::regex&, Integer, Integer>(pattern, lower, upper, count, separator);
	}

	std::vector<std::string> strings(const std::regex& pattern, Integer lower, Integer upper, Constraint& constraint,
	                                 Integer count, char separator = DEFAULT_SEPARATOR) {
		return strings<const std::regex&, Integer, Integer, Constraint&>(pattern, lower, upper, constraint, count, separator);
	}

	Integer integer() {
		return parse<Integer>(string(INTEGER_REGEX));
	}

	Integer integer(Integer lower, Integer upper) {
		Integer res = integer();
		if (res < lower or res >= upper) {
			ValidateBase::juryOut << "Integer " << res << " out of range [" << lower << ", " << upper << ")!";
			fail();
		}
		return res;
	}

	Integer integer(Integer lower, Integer upper, Constraint& constraint) {
		Integer res = integer(lower, upper);
		constraint.log(lower, upper, res);
		return res;
	}

	template<typename... Args>
	std::vector<Integer> integers(Args... args, Integer count, char separator) {
		auto sepCall = checkSeparator(separator);
		std::vector<Integer> res(count);
		for (Integer i = 0; i < count; i++) {
			res[i] = integer(args...);
			if (i + 1 < count) sepCall();
		}
		return res;
	}

	std::vector<Integer> integers(Integer count, char separator = DEFAULT_SEPARATOR) {
		return integers<>(count, separator);
	}

	std::vector<Integer> integers(Integer lower, Integer upper,
	                              Integer count, char separator = DEFAULT_SEPARATOR) {
		return integers<Integer, Integer>(lower, upper, count, separator);
	}

	std::vector<Integer> integers(Integer lower, Integer upper, Constraint& constraint,
	                              Integer count, char separator = DEFAULT_SEPARATOR) {
		return integers<Integer, Integer, Constraint&>(lower, upper, constraint, count, separator);
	}

	// this does not allow NaN or Inf!
	// However, those should never be desired.
	Real real() {
		return parse<Real>(string(REAL_REGEX));
	}

	Real real(Real lower, Real upper) {// uses eps
		Real res = real();
		if (details::floatEqual(res, lower, floatAbsTol, floatRelTol)) return res;
		if (details::floatEqual(res, upper, floatAbsTol, floatRelTol)) return res;
		if (std::isnan(res) or !(res >= lower) or !(res < upper)) {
			ValidateBase::juryOut << "Real " << res << " out of range [" << lower << ", " << upper << ")!";
			fail();
		}
		return res;
	}

	Real real(Real lower, Real upper, Constraint& constraint) {
		Real res = real(lower, upper);
		constraint.log(lower, upper, res);
		return res;
	}

	template<typename... Args>
	std::vector<Real> reals(Args... args, Integer count, char separator) {
		auto sepCall = checkSeparator(separator);
		std::vector<Real> res(count);
		for (Integer i = 0; i < count; i++) {
			res[i] = real(args...);
			if (i + 1 < count) sepCall();
		}
		return res;
	}

	std::vector<Real> reals(Integer count, char separator = DEFAULT_SEPARATOR) {
		return reals<>(count, separator);
	}

	std::vector<Real> reals(Real lower, Real upper,
	                        Integer count, char separator = DEFAULT_SEPARATOR) {
		return reals<Real, Real>(lower, upper, count, separator);
	}

	std::vector<Real> reals(Real lower, Real upper, Constraint& constraint,
	                        Integer count, char separator = DEFAULT_SEPARATOR) {
		return reals<Real, Real, Constraint&>(lower, upper, constraint, count, separator);
	}

	Real realStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals) {// does not use eps
		std::string t = string(STRICT_REAL_REGEX);
		auto dot = t.find('.');
		Integer decimals = dot == std::string::npos ? 0 : t.size() - dot - 1;
		if (decimals < minDecimals or decimals >= maxDecimals) {
			ValidateBase::juryOut << "Real " << t << " has wrong amount of decimals!";
			fail();
			return 0;
		}
		try {
			Real res = parse<Real>(t);
			if (std::isnan(res) or !(res >= lower) or !(res < upper)) {
				ValidateBase::juryOut << "Real " << res << " out of range [" << lower << ", " << upper << ")!";
				fail();
			}
			return res;
		} catch(...) {
			ValidateBase::juryOut << "Could not parse token \"" << t << "\" as real!";
			fail();
			return 0;
		}
	}

	Real realStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint) {
		Real res = realStrict(lower, upper, minDecimals, maxDecimals);
		constraint.log(lower, upper, res);
		return res;
	}

	template<typename... Args>
	std::vector<Real> realsStrict(Args... args, Integer count, char separator) {
		auto sepCall = checkSeparator(separator);
		std::vector<Real> res(count);
		for (Integer i = 0; i < count; i++) {
			res[i] = realStrict(args...);
			if (i + 1 < count) sepCall();
		}
		return res;
	}

	std::vector<Real> realsStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals,
	                              Integer count, char separator = DEFAULT_SEPARATOR) {
		return realsStrict<Real, Real, Integer, Integer>(lower, upper, minDecimals, maxDecimals, count, separator);
	}

	std::vector<Real> realsStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint,
	                              Integer count, char separator = DEFAULT_SEPARATOR) {
		return realsStrict<Real, Real, Integer, Integer, Constraint&>(lower, upper, minDecimals, maxDecimals, constraint, count, separator);
	}

	void expectString(std::string_view expected) {
		judgeAssert<std::invalid_argument>(details::isToken(expected), "InputStream: expected must not contain a space!");
		std::string seen = string();
		if (!details::stringEqual(seen, expected, caseSensitive)) {
			ValidateBase::juryOut << "Expected \"" << expected << "\" but got \"" << seen << "\"!";
			fail();
		}
	}

	void expectInt(Integer expected) {
		Integer seen = integer();
		if (seen != expected) {
			ValidateBase::juryOut << "Expected " << expected << " but got " << seen << "!";
			fail();
		}
	}

	void expectReal(Real expected) {
		Real seen = real();
		if (details::floatEqual(seen, expected, floatAbsTol, floatRelTol)) {
			ValidateBase::juryOut << "Expected " << expected << " but got " << seen << "!";
			fail();
		}
	}
private:
	void fail() {
		//try to find input position...
		in->clear();
		auto originalPos = in->tellg();
		in->seekg(0);
		if (originalPos != std::streamoff(-1) and *in) {
			Integer line = 1;
			std::size_t l = 0, r = 0;
			std::string buffer;
			bool extend = true;
			while (*in and in->tellg() < originalPos) {
				l = r = buffer.size();
				if (std::isgraph(in->peek())) {
					std::string tmp;
					*in >> tmp;
					buffer += tmp;
				} else if (in->peek() == std::char_traits<char>::to_int_type(NEWLINE)) {
					line++;
					in->get();
					if (in->tellg() < originalPos) {
						buffer.clear();
					} else {
						buffer += ' ';
						extend = false;
					}
				} else {
					buffer += std::char_traits<char>::to_char_type(in->get());
				}
				if (*in and in->tellg() >= originalPos) {
					r = buffer.size();
				}
			}
			if (l != r) {
				ValidateBase::juryOut << " Line: " << line << ", Char: " << l << '\n';
				if (extend) {
					char tmp;
					while ((buffer.size() < 80 or buffer.size() < r + 80) and in->get(tmp) and tmp != NEWLINE) {
						buffer += tmp;
					}
				}
				if (r > 60 and l > 20) {
					Integer offset = std::min(l - 20, r - 60);
					l -= offset;
					r -= offset;
					buffer = "[...]" + buffer.substr(offset + 5);
				}
				if (buffer.size() > 80) {
					buffer = buffer.substr(0, 75);
					buffer += "[...]";
					r = std::min(r, buffer.size());
				}
				ValidateBase::juryOut << buffer << '\n';
				ValidateBase::juryOut << std::string(l, ' ') << '^' << std::string(r - l - 1, '~');
			}
		}
		ValidateBase::juryOut << onFail;
	}
};


//============================================================================//
// state guard                                                                //
//============================================================================//
namespace details {
	bool initialized(bool set = false) {
		static bool value = false;
		return std::exchange(value, value |= set);
	}

	struct InitGuard final {
		~InitGuard() {
			if (std::uncaught_exceptions() == 0) {
				judgeAssert<std::logic_error>(initialized(), "validate.h: init(argc, argv) was never called!");
			}
		}
	} initGuard;
}


//============================================================================//
// Settings                                                                   //
//============================================================================//
template<typename T>
class SettingBase {	
	template<typename U>
	friend class Setting;
	friend class SettingCaseSensitive;

	T value;

	SettingBase(T value_) : value(value_) {}

public:
	SettingBase(SettingBase<T>&& other) = delete;
	SettingBase(const SettingBase<T>&) = delete;
	SettingBase<T>& operator=(SettingBase<T>&& other) = delete;
	SettingBase<T>& operator=(const SettingBase<T>&) = delete;

	operator T() const {
		return value;
	}

	SettingBase<T>& operator=(T value_) {
		judgeAssert<std::logic_error>(!details::initialized(), "validate.h: Cannot change setting after init(argc, argv) was called!");
		value = value_;
		return *this;
	}
};

template<typename T>
class Setting final : public SettingBase<T> {
public:
	Setting(T value) : SettingBase<T>(value) {}
	using SettingBase<T>::operator T;
	using SettingBase<T>::operator=;
};

class SettingCaseSensitive final : public SettingBase<bool> {
public:
	SettingCaseSensitive(bool value) : SettingBase<bool>(value) {}
	using SettingBase<bool>::operator bool;
	using SettingBase<bool>::operator=;

	std::regex regex(std::string_view s, std::regex_constants::syntax_option_type f = std::regex_constants::ECMAScript) const {
		if (!value) f |= std::regex_constants::icase;
		return std::regex(s.data(), s.size(), f);
	}
};


//============================================================================//
// Validators and stuff                                                       //
//============================================================================//
namespace ValidateBase {
	//OutputStream juryOut(std::cout); //already defined earlier
	//OutputStream juryErr(std::cerr);
	CommandParser arguments;
	//you may change these values before calling::init() but not afterwards!
	Setting<Real> floatAbsTol(DEFAULT_EPS);
	Setting<Real> floatRelTol(DEFAULT_EPS);
	Setting<bool> spaceSensitive(false);
	SettingCaseSensitive caseSensitive(false);

	// Real r2 is considered the reference value for relative error.
	bool floatEqual(Real given,
	                Real expected,
	                Real floatAbsTol_ = floatAbsTol,
	                Real floatRelTol_ = floatRelTol) {
		return details::floatEqual(given, expected, floatAbsTol_, floatRelTol_);
	}

	bool floatLess(Real given,
	               Real expected,
	               Real floatAbsTol_ = floatAbsTol,
	               Real floatRelTol_ = floatRelTol) {
		return given <= expected or floatEqual(given, expected, floatAbsTol_, floatRelTol_);
	}

	bool floatGreater(Real given,
	                  Real expected,
	                  Real floatAbsTol_ = floatAbsTol,
	                  Real floatRelTol_ = floatRelTol) {
		return given >= expected or floatEqual(given, expected, floatAbsTol_, floatRelTol_);
	}

	constexpr bool stringEqual(std::string_view a, std::string_view b, bool caseSensitive_ = caseSensitive) {
		return details::stringEqual(a, b, caseSensitive_);
	}

	namespace details {
		void init(int argc, char** argv) {
			judgeAssert<std::logic_error>(!::details::initialized(), "validate.h: init(argc, argv) was called twice!");

			//std::ios_base::sync_with_stdio(false);
			//cin.tie(nullptr);

			arguments = CommandParser(argc, argv);
			if (auto seed = arguments[SEED_COMMAND]) Random::seed(seed.asInteger());
			// parse default flags manually, since they dont use '--' prefix
			auto eps = arguments.getRaw(FLOAT_TOLERANCE);
			floatAbsTol = eps.asReal(floatAbsTol);
			floatRelTol = eps.asReal(floatRelTol);
			floatAbsTol = arguments.getRaw(FLOAT_ABSOLUTE_TOLERANCE).asReal(floatAbsTol);
			floatRelTol = arguments.getRaw(FLOAT_RELATIVE_TOLERANCE).asReal(floatRelTol);

			if (arguments.getRaw(SPACE_SENSITIVE)) spaceSensitive = true;
			if (arguments.getRaw(CASE_SENSITIVE)) caseSensitive = true;

			::details::initialized(true);
		}
	}

} // namespace ValidateBase

namespace ConstraintsBase {
	ConstraintsLogger constraint;

	void initConstraints() {
		if (auto file = ValidateBase::arguments[CONSTRAINT_COMMAND]) {
			constraint = ConstraintsLogger(file.asString());
		}
	}

} // namespace ConstraintsBase

//called as ./validator [arguments] < inputfile
namespace InputValidator {
	using namespace ValidateBase;
	using namespace ConstraintsBase;

	InputStream testIn;

	void init(int argc, char** argv) {
		spaceSensitive = true;
		caseSensitive = true;

		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::cout);

		testIn = InputStream(std::cin, spaceSensitive, caseSensitive, WA, floatAbsTol, floatRelTol);
		initConstraints();
	}

} // namespace InputValidator

//called as ./validator input judgeanswer feedbackdir < teamoutput
namespace OutputValidator {
	using namespace ValidateBase;
	using namespace ConstraintsBase;

	InputStream testIn;
	InputStream juryAns;
	InputStream teamAns;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::filesystem::path(arguments[3]) / JUDGE_MESSAGE);

		testIn = InputStream(std::filesystem::path(arguments[1]), false, caseSensitive, FAIL);
		juryAns = InputStream(std::filesystem::path(arguments[2]), false, caseSensitive, FAIL);
		teamAns = InputStream(std::cin, spaceSensitive, caseSensitive, WA);
		initConstraints();
	}

} // namespace OutputValidator

//called as ./interactor input judgeanswer feedbackdir <> teamoutput
namespace Interactor {
	using namespace ValidateBase;

	OutputStream toTeam;
	InputStream testIn;
	InputStream fromTeam;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::filesystem::path(arguments[3]) / JUDGE_MESSAGE);
		toTeam = OutputStream(std::cout);

		testIn = InputStream(std::filesystem::path(arguments[1]), false, caseSensitive, FAIL);
		fromTeam = InputStream(std::cin, spaceSensitive, caseSensitive, WA);
	}

} // namespace Interactor

//called as ./generator [arguments]
namespace Generator {
	using namespace ValidateBase;

	OutputStream testOut;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::cerr);
		testOut = OutputStream(std::cout);
	}

} // namespace Generator

#endif
