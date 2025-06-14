//============================================================================//
// validate.h                                                                 //
//============================================================================//
// This header can be used to safely parse team output tokenwise. We support: //
// - string tokens (get converted to lowercase)                               //
// - integer tokens in [-2^63, 2^63)                                          //
// - float tokens (relative and absolute error of 10^6 is allowed by default) //
// Tokens need to be separated by whitespace (any amount). The following      //
// command line flags allow stricter checking:                                //
// - caseSensitive: string tokens don't get converted to lowercase            //
// - space_change_sensitive: tokens need to be separated by the corect        //
//   amount of whitespaces                                                    //
// - FLOAT_{RELATIVE|ABSOLUTE}_TOLERANCE: allowed relative/absolute error     //
//                                                                            //
// This header can also be used to safely verify input files. In this case    //
// tokens are case sensitive and all whitespaces have to be checked. Also     //
// whitespaces are not interchangeable.                                       //
//                                                                            //
// This header can be used to generate random numbers in a deterministic and  //
// reproducable fashion. (The randomness is consistent across compilers and   //
// machines)                                                                  //
//============================================================================//
// version 2.6.4                                                              //
// https://github.com/mzuenni/icpc-header                                     //
//============================================================================//

#ifndef VALIDATE_H
#define VALIDATE_H

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
//#define DOUBLE_FALLBACK
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
constexpr std::string_view LETTER                       = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr std::string_view UPPER                        = LETTER.substr(0, 26);
constexpr std::string_view LOWER                        = LETTER.substr(26);
constexpr std::string_view VOWEL                        = "AEIOUaeiou";
constexpr std::string_view UPPER_VOWELS                 = VOWEL.substr(0, 5);
constexpr std::string_view LOWER_VOWELS                 = VOWEL.substr(5);
constexpr std::string_view CONSONANT                    = "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
constexpr std::string_view UPPER_CONSONANT              = CONSONANT.substr(0, 26 - 5);
constexpr std::string_view LOWER_CONSONANT              = CONSONANT.substr(26 - 5);
constexpr std::string_view ALPHA_NUMERIC                = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr std::string_view UPPER_ALPHA_NUMERIC          = ALPHA_NUMERIC.substr(0, 10 + 26);
constexpr std::string_view LOWER_ALPHA_NUMERIC          = "0123456789abcdefghijklmnopqrstuvwxyz";
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
constexpr std::string_view TEAM_MESSAGE                 = "teammessage.txt";
constexpr std::ios_base::openmode MESSAGE_MODE          = std::ios::out | std::ios::ate;
constexpr char DEFAULT_SEPARATOR                        = SPACE;
constexpr std::string_view EMPTY_COMMAND                = "";
constexpr std::string_view COMMAND_PREFIX               = "--";
constexpr std::string_view CONSTRAINT_COMMAND           = "--constraints_file";
constexpr std::string_view SEED_COMMAND                 = "--seed";
constexpr std::string_view TEXT_ELLIPSIS                = "[...]";
constexpr auto REGEX_OPTIONS                            = std::regex::nosubs | std::regex::optimize;
inline const std::regex INTEGER_REGEX("0|-?[1-9][0-9]*", REGEX_OPTIONS);
inline const std::regex REAL_REGEX("[+-]?(([0-9]*\\.[0-9]+)|([0-9]+\\.)|([0-9]+))([eE][+-]?[0-9]+)?", REGEX_OPTIONS);
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
		using iterator_type = decltype(std::begin(std::declval<std::add_lvalue_reference_t<T>>()));
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
namespace Verdicts {
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
}


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
	explicit OutputStream(const std::filesystem::path& path, std::ios_base::openmode mode) : managed(std::make_unique<std::ofstream>(path, mode)), os(managed.get()) {
		judgeAssert<std::runtime_error>(os->good(), "OutputStream(): Could not open File: " + path.string());
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

		friend std::ostream& operator<<(std::ostream& os, const TempWriter<C>& writer) = delete; //use OutputStream
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

template<typename CR>
auto asFunction(CR&& c) {
	if constexpr(std::is_rvalue_reference_v<CR&&>) {
		return [c](auto&& x){return c[std::forward<decltype(x)>(x)];};
	} else {
		return [&c](auto&& x){return c[std::forward<decltype(x)>(x)];};
	}
}

template<typename T>
auto asFunction(std::initializer_list<T> c_) {
	std::vector<T> c(c_);
	return [c](auto&& x){return c[std::forward<decltype(x)>(x)];};
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

template<typename T>
struct boolean {
	bool value;
	std::optional<T> reason;

	constexpr boolean(bool value_) : value(value_) {}
	constexpr boolean(bool value_, const T& reason_) : value(value_), reason(reason_) {}

	constexpr operator bool() const {
		return value;
	}

	constexpr bool hasReason() const {
		return reason.has_value();
	}
};

// the lambda capture makes it harder to cal this with anything else then a named variable
#define allOf(x) [&x](){return std::begin(x);}(), std::end(x)

//============================================================================//
// Utility                                                                    //
//============================================================================//
// for sequences
template<typename RandomIt,
         typename = std::enable_if_t<std::is_integral_v<typename std::iterator_traits<RandomIt>::value_type>>>
auto isPerm(RandomIt first, RandomIt last, typename std::iterator_traits<RandomIt>::value_type offset = {}) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	auto count = std::distance(first, last);
	std::vector<bool> seen(count, false);
	for (; first != last; first++) {
		const T& x = *first;
		if (x < offset or x - offset >= count or seen[x - offset]) {
			return boolean<T>(false, x);
		}
		seen[x - offset] = true;
	}
	return boolean<T>(true);
}
template<typename C, typename std::enable_if_t<std::is_integral_v<typename details::IsContainer<C>::value_type>, bool> = true>
auto isPerm(const C& c, typename details::IsContainer<C>::value_type offset = {}) {
	return isPerm(std::begin(c), std::end(c), offset);
}

template<typename itA, typename itB>
auto isPerm(itA firstA, itA lastA, itB firstB, itB lastB) {
	using T = typename std::iterator_traits<itA>::value_type;
	std::vector<typename std::iterator_traits<itA>::value_type> a(firstA, lastA);
	std::vector<typename std::iterator_traits<itB>::value_type> b(firstB, lastB);
	if (a.size() != b.size()) return boolean<T>(false);
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());
	for (std::size_t i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) return boolean<T>(false, a[i]);
	}
	return boolean<T>(true);
}
template<typename C1,
         typename C2,
         typename = std::enable_if_t<details::IsContainer<C1>{}>,
         typename = std::enable_if_t<details::IsContainer<C2>{}>>
auto isPerm(const C1& c1, const C2& c2) {
	return isPerm(std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
}

template<typename RandomIt, typename BinaryPredicate>
constexpr boolean<Integer> anyAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	if (first != last) {
		for (Integer i = 1; std::next(first) != last; first++, i++) {
			if (p(*first, *std::next(first))) {
				return boolean<Integer>(true, i);
			}
		}
	}
	return boolean<Integer>(false);
}
template<typename C, typename BinaryPredicate>
constexpr boolean<Integer> anyAdjacent(const C& c, BinaryPredicate p) {
	return anyAdjacent(std::begin(c), std::end(c), p);
}

template<typename RandomIt, typename BinaryPredicate>
constexpr boolean<Integer> noneAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	auto res = anyAdjacent(first, last, p);
	res.value = !res.value;
	return res;
}
template<typename C, typename BinaryPredicate>
constexpr boolean<Integer> noneAdjacent(const C& c, BinaryPredicate p) {
	return noneAdjacent(std::begin(c), std::end(c), p);
}

template<typename RandomIt, typename BinaryPredicate>
constexpr boolean<Integer> allAdjacent(RandomIt first, RandomIt last, BinaryPredicate p) {
	return noneAdjacent(first, last, std::not_fn(p));
}
template<typename C, typename BinaryPredicate>
constexpr boolean<Integer> allAdjacent(const C& c, BinaryPredicate p) {
	return noneAdjacent(std::begin(c), std::end(c), p);
}

template<typename RandomIt>
constexpr boolean<Integer> areIncreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::less<T>());
}
template<typename C>
constexpr boolean<Integer> areIncreasing(const C& c) {
	return areIncreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr boolean<Integer> areNonDecreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::less_equal<T>());
}
template<typename C>
constexpr boolean<Integer> areNonDecreasing(const C& c) {
	return areNonDecreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr boolean<Integer> areDecreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::greater<T>());
}
template<typename C>
constexpr boolean<Integer> areDecreasing(const C& c) {
	return areDecreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr boolean<Integer> areNonIncreasing(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	return allAdjacent(first, last, std::greater_equal<T>());
}
template<typename C>
constexpr boolean<Integer> areNonIncreasing(const C& c) {
	return areNonIncreasing(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr auto areDistinct(RandomIt first, RandomIt last) {
	using T = typename std::iterator_traits<RandomIt>::value_type;
	std::vector<T> tmp(first, last);
	std::sort(tmp.begin(), tmp.end());
	auto [b, v] = anyAdjacent(tmp, std::equal_to<T>());
	if (v) return boolean<T>(!b, tmp[*v]);
	return boolean<T>(!b);
}
template<typename C>
constexpr auto areDistinct(const C& c) {
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

constexpr boolean<char> isUpper(std::string_view s) {
	for (char c : s) if (!isUpper(c)) return boolean<char>(false, c);
	return boolean<char>(true);
}

constexpr boolean<char> isLetter(std::string_view s) {
	for (char c : s) if (!isLetter(c)) return boolean<char>(false, c);
	return boolean<char>(true);
}

constexpr boolean<char> isDigit(std::string_view s) {
	for (char c : s) if (!isDigit(c)) return boolean<char>(false, c);
	return boolean<char>(true);
}

constexpr boolean<char> isVowel(std::string_view s) {
	for (char c : s) if (!isVowel(c)) return boolean<char>(false, c);
	return boolean<char>(true);
}

constexpr boolean<char> isConsonant(std::string_view s) {
	for (char c : s) if (!isConsonant(c)) return boolean<char>(false, c);
	return boolean<char>(true);
}

std::vector<Integer> thueMorse(Integer lower, Integer upper) {
	judgeAssert<std::invalid_argument>(lower < upper, "thueMorse(): Lower must be less than upper!");
	judgeAssert<std::invalid_argument>(lower >= 0, "thueMorse(): Lower must be non negative!");
	std::vector<Integer> res(upper - lower);
	for (std::size_t i = 0; i < res.size(); i++) {
		res[i] = std::bitset<std::numeric_limits<UInteger>::digits>(i + static_cast<UInteger>(lower)).count() % 2;
	}
	return res;
}

std::vector<Integer> thueMorse(Integer upper) {
	return thueMorse(0, upper);
}

std::vector<Integer> range(Integer from, Integer to, Integer step = 1) {
	judgeAssert<std::domain_error>(step != 0, "range(): step must not be zero!");
	if (step > 0 and to <= from) return {};
	if (step < 0 and from <= to) return {};
	UInteger n = static_cast<UInteger>(to) - static_cast<UInteger>(from);
	if (step < 0) n = 0 - n;
	std::vector<Integer> res(1 + (n - 1) / std::abs(step), from);
	for (std::size_t i = 1; i < res.size(); i++) {
		res[i] = res[i - 1] + step;
	}
	return res;
}

std::vector<Integer> range(Integer upper) {
	return range(0, upper);
}

// allow using std::pair and std::complex similiar
// (may be useful for geometric problem)

// for std::pair, std::tuple, std::array, ...
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getX(T& point) {
	return std::get<0>(point);
}
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getY(T& point) {
	return std::get<1>(point);
}
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getZ(T& point) {
	return std::get<2>(point);
}
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto getX(const T& point) {
	return std::get<0>(point);
}
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto getY(const T& point) {
	return std::get<1>(point);
}
template<typename T, typename std::enable_if_t<details::IsTupleLike<T>{}, bool> = true>
constexpr auto getZ(const T& point) {
	return std::get<2>(point);
}

// for std::complex
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

// fallback
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getX(T& point) {
	return point.x;
}
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getY(T& point) {
	return point.y;
}
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto& getZ(T& point) {
	return point.z;
}
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto getX(const T& point) {
	return point.x;
}
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto getY(const T& point) {
	return point.y;
}
template<typename T, typename std::enable_if_t<!details::IsTupleLike<T>{}, bool> = true>
constexpr auto getZ(const T& point) {
	return point.z;
}

// convert std::complex and std::pair
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

	constexpr boolean<std::size_t> stringEqual(std::string_view a, std::string_view b, bool caseSensitive) {
		std::size_t i = 0;
		for (; i < a.size() and i < b.size(); i++) {
			char aa = a[i];
			char bb = b[i];
			if (!caseSensitive) {
				aa = toDefaultCase(aa);
				bb = toDefaultCase(bb);
			}
			if (aa != bb) {
				return boolean<std::size_t>(false, i);
			}
		}
		if (a.size() != b.size()) {
			return boolean<std::size_t>(false, i);
		} else {
			return boolean<std::size_t>(true);
		}
	}

	constexpr bool isToken(std::string_view a) {
		if (a.empty()) return false;
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
		const char* begin = s.data();
		const char* end = s.data() + s.size();
		if (!s.empty() && s[0] == '+') begin++;
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
	template<>
	bool parse(std::string_view s, std::string& res) {
		res = s;
		return true;
	}

}

boolean<Integer> isInteger(const std::string& s) {
	if (!std::regex_match(s, INTEGER_REGEX)) return boolean<Integer>(false);
	Integer value = 0;
	if (!details::parse<Integer>(s, value)) return boolean<Integer>(false);
	return boolean<Integer>(true, value);
}

boolean<Real> isReal(const std::string& s) {
	if (!std::regex_match(s, REAL_REGEX)) return boolean<Real>(false);
	Real value = 0;
	if (!details::parse<Real>(s, value)) return boolean<Real>(false);
	return boolean<Real>(true, value);
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
	#ifdef __SIZEOF_INT128__
		return static_cast<UInteger>((static_cast<__uint128_t>(lhs) *
		                              static_cast<__uint128_t>(rhs)) % mod);
	#else
		UInteger res = 0;
		while (rhs > 0) {
			if (rhs & 1) res = (lhs + res) % mod;
			lhs = (lhs + lhs) % mod;
			rhs /= 2;
		}
		return res;
	#endif
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
	UInteger un = static_cast<UInteger>(n);
	static_assert(details::TRIAL_PRIMES.size() > 0);
	if (details::powMod(details::TRIAL_PRIMES.back() + 1, un - 1, un) != 1) {
		return false;
	}
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
	Integer count = std::min<Integer>(0x3FFF'FFFF, upper - lower);
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
constexpr T sign(T x) {
	return (T(0) < x) - (x < T(0));
}

template<typename T>
constexpr T dist(T a, T b) {
	return a > b ? a - b : b - a;
}


//============================================================================//
// Geometry (this is just for utility stuff...)                               //
//============================================================================//
template<typename Point>
constexpr auto dot(const Point& a, const Point& b) {
	return getX(a) * getX(b) + getY(a) * getY(b);
}

template<typename Point>
constexpr auto cross(const Point& a, const Point& b) {
	return getX(a) * getY(b) - getY(a) * getX(b);
}
template<typename Point>
constexpr auto cross(const Point& p, Point a, Point b) {
	getX(a) -= getX(p);
	getY(a) -= getY(p);
	getX(b) -= getX(p);
	getY(b) -= getY(p);
	return cross(a, b);
}

namespace details {
	template<typename Point>
	constexpr bool left(const Point& p) {
		return getX(p) == 0 ? getY(p) < 0 : getX(p) < 0;
	}

	template<typename Point>
	void cyclicSort(std::vector<Point>& in) {
		std::sort(in.begin(), in.end(), [](const Point& a, const Point& b){
			return left(a) != left(b) ? left(a) > left(b) : cross(a, b) > 0;
		});
	}

	template<typename RandomIt>
	constexpr std::size_t assertBounds(RandomIt first, RandomIt last) {
		std::size_t n = 0;
		for (auto it = first; it != last; it++, n++) {
			judgeAssert(std::abs(getX(*it)) <= 0x3FFF'FFFF, "assertBounds(): coordinates too large!");
			judgeAssert(std::abs(getY(*it)) <= 0x3FFF'FFFF, "assertBounds(): coordinates too large!");
		}
		return n;
	}
}

template<typename RandomIt>
constexpr bool isConvex(RandomIt first, RandomIt last) {
	std::size_t n = details::assertBounds(first, last);
	if (n < 3) return false;
	bool hasArea = false;
	Real winding = 0.0_real;
	auto dir = [&](std::size_t i){
		auto a = first[i % n];
		auto b = first[(i+1) % n];
		std::complex<Real> d(getX(b) - getX(a), getY(b) - getY(a));
		if (a == b) return d;
		return d / std::abs(d);
	};
	for (std::size_t i = 0; i < n; i++) {
		if (first[i] == first[(i+1) % n]) return false;
		Integer ccw = cross(first[i], first[(i+1) % n], first[(i+2) % n]);
		hasArea |= ccw > 0;
		if (ccw < 0) return false;
		winding += std::acos(std::clamp(dot(dir(i), dir(i+1)), -1.0_real, 1.0_real));
	}
	judgeAssert(!std::isnan(winding), "isConvex(): float error while calculating the winding number");
	return hasArea && winding < 9;
}
template<typename C>
constexpr bool isConvex(const C& c) {
	return isConvex(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool isStrictlyConvex(RandomIt first, RandomIt last) {
	if (!isConvex(first, last)) return false;
	std::size_t n = std::distance(first, last);
	for (std::size_t i = 2; i < n; i++) {
		if (cross(first[i-2], first[i-1], first[i]) == 0) return false;
	}
	if (cross(last[-2], last[-1], first[0]) == 0) return false;
	if (cross(last[-1], first[0], first[1]) == 0) return false;
	return true;
}
template<typename C>
constexpr bool isStrictlyConvex(const C& c) {
	return isStrictlyConvex(std::begin(c), std::end(c));
}

template<typename RandomIt>
bool isSimple(RandomIt first, RandomIt last) {
	using Point = typename std::iterator_traits<RandomIt>::value_type;
	struct Segment {
		Point l, r;
		std::size_t id;

		Segment(Point l_, Point r_, std::size_t id_) : l(l_), r(r_), id(id_) {}

		bool operator<(const Segment& other) const {
			if (getX(l) > getX(other.l)) return !(other < *this);
			Integer s = cross(l, r, other.l);
			if (s != 0) return s > 0;
			s = cross(l, r, other.r) > 0;
			if (s != 0) return s > 0;
			return getY(l) < getY(other.l);
		}

		bool intersect(const Segment& other, std::size_t n) const {
			if (dist(id, other.id) == 1 or dist(id, other.id) == n-1) return false;
			// no collinear special case needed!
			return sign(cross(l, r, other.l)) * cross(l, r, other.r) <= 0 and
			       sign(cross(other.l, other.r, l)) * cross(other.l, other.r, r) <= 0;
		}
	};

	std::size_t n = details::assertBounds(first, last);
	if (n < 3) return false;

	std::vector<std::tuple<Integer, Integer, bool, bool, std::size_t>> events;
	for (std::size_t i = 0; i < n; i++) {
		bool swap = false;
		Point a = first[i];
		Point b = first[(i+1) % n];
		if (getX(a) > getX(b)) swap = true;
		if (getX(a) == getX(b) and getY(a) > getY(b)) swap = true;
		if (swap) std::swap(a, b);
		events.emplace_back(getX(a), getY(a), false, swap, i);
		events.emplace_back(getX(b), getY(b), true, swap, i);
	}
	std::sort(events.begin(), events.end());

	std::set<Segment> sweepline;
	std::vector<typename std::set<Segment>::iterator> where(n);
	for (auto [x, y, remove, swap, i] : events) {
		if (remove) {
			auto it = sweepline.erase(where[i]);
			if (it != sweepline.begin() and it != sweepline.end() and it->intersect(*std::prev(it), n)) {
				return false;
			}
		} else {
			Point a = first[i];
			Point b = first[(i+1) % n];
			if (swap) std::swap(a, b);
			auto [it, inserted] = sweepline.emplace(a, b, i);
			if (!inserted) return false;
			if (it != sweepline.begin() and it->intersect(*prev(it), n)) return false;
			if (next(it) != sweepline.end() and it->intersect(*next(it), n)) return false;
			where[i] = it;
		}
	}
	return true;
}
template<typename C>
bool isSimple(const C& c) {
	return isSimple(std::begin(c), std::end(c));
}

template<typename RandomIt>
constexpr bool isCCW(RandomIt first, RandomIt last) {
	std::size_t n = details::assertBounds(first, last);
	if (n < 2) return false;
	Integer area = cross(first[n-1], first[0]); // can't overflow
	for (std::size_t i = 1; i < n; i++) area += cross(first[i-1], first[i]);
	return area > 0;
}
template<typename C>
constexpr bool isCCW(const C& c) {
	return isCCW(std::begin(c), std::end(c));
}


//============================================================================//
// Random                                                                     //
//============================================================================//
namespace Random {
	// You should not rely on the implementation in details!
	// Especially you should never use randomNumberGenerator on your own. There is no function in
	// c++ which uses a random engine and is not implementation defined.
	namespace details {
		constexpr Real PI = 3.141592653589793238462643383279502884_real;
		constexpr Integer PRIME_TRIALS = 4*1600;

		RandomEngine randomNumberGenerator(DEFAULT_SEED);
		static_assert(RandomEngine::max() == 0xFFFF'FFFF'FFFF'FFFF_uint, "Random Engine should produce 64bit of randomness");
		static_assert(RandomEngine::min() == 0_uint, "Random Engine should produce 64bit of randomness");

		template<Integer... Is>
		constexpr std::array<Integer, sizeof...(Is)> prefixSum() {
			std::array<Integer, sizeof...(Is)> res{Is...};
			//std::partial_sum(res.begin(), res.end(), res.begin());
			Integer sum = 0;
			for (Integer& x : res) sum = x += sum;
			return res;
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
		//https://lemire.me/blog/2019/06/06/nearly-divisionless-random-integer-generation-on-various-systems/
		UInteger s = uu - ul;
		UInteger x = Random::details::randomNumberGenerator();
		if (x * s < s) {
			UInteger t = (0 - s) % s;
			while (x * s < t) x = Random::details::randomNumberGenerator();
		}
	#ifdef __SIZEOF_INT128__
		return static_cast<Integer>(((static_cast<__uint128_t>(x) *
		                              static_cast<__uint128_t>(s)) >> 64) + ul);
	#else
		//x * s >> 64
		//https://github.com/catid/fp61/blob/2eddbeaa19f3b838a833b1a2ba256d32aa9bfaa5/fp61.h#L71
		UInteger x0 = static_cast<uint32_t>(x);
		UInteger x1 = x >> 32;
		UInteger y0 = static_cast<uint32_t>(s);
		UInteger y1 = s >> 32;
		UInteger p11 = x1 * y1;
		UInteger p01 = x0 * y1;
		UInteger p10 = x1 * y0;
		UInteger p00 = x0 * y0;
		UInteger mid = p10 + (p00 >> 32) + static_cast<uint32_t>(p01);
		UInteger res = p11 + (mid >> 32) + (p01 >> 32);
		return static_cast<Integer>(res + ul);
	#endif
	}
	Integer integer(Integer upper) {// in [0, upper)
		return integer(0, upper);
	}

	Real real() {// in [0, 1)
		UInteger mantissa = details::randomNumberGenerator() | (1ull << 63);
		int exponent = -64;
		UInteger x;
		do {
			x = details::randomNumberGenerator();
			//count trailing (or leading) zeroes in x
			exponent -= static_cast<int>(std::bitset<64>(~x & (x-1)).count());
		} while (x == 0);
		return std::ldexp(mantissa, exponent);
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

	template<Integer I, Integer... Is>
	Integer discrete() {
		static_assert(((I >= 0) and ... and (Is >= 0)));
		static constexpr std::array<Integer, 1 + sizeof...(Is)> is = details::prefixSum<I, Is...>();
		return std::distance(is.begin(), std::upper_bound(is.begin(), is.end(), Random::integer(is.back())));
	}

	Real normal(Real mean, Real stddev) {// theoretically in (-inf, inf)
		judgeAssert<std::domain_error>(stddev >= 0.0_real, "Random::normal(): Standard deviation must be non negative!");
		Real u1 = real();
		Real u2 = real();
		Real res = std::sqrt(-2.0_real * std::log(u1)) * std::cos(2.0_real * details::PI * u2);
		return std::sqrt(stddev) * res + mean;
	}
	Real normal(Real lower, Real upper, Real mean, Real stddev) {// in [lower, upper)
		judgeAssert<std::domain_error>(!std::isnan(lower), "Random::normal(): Lower must not be NaN!");
		judgeAssert<std::domain_error>(!std::isnan(upper), "Random::normal(): Upper must not be NaN!");
		judgeAssert<std::invalid_argument>(lower < upper, "Random::normal(): Lower must be less than upper!");
		judgeAssert<std::domain_error>(stddev >= 0.0_real, "Random::normal(): Standard deviation must be non negative!");
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
		judgeAssert<std::domain_error>(0.0_real <= p and p <= 1.0_real, "Random::binomial(): p must be in [0,1)!");
		bool swap = p > 0.5_real;
		p = std::min(p, 1.0_real - p);
		if (p*n < 10.0_real) {
			// BG: Geometric method
			// https://dl.acm.org/doi/pdf/10.1145/42372.42381
			Integer res = 0;
			Integer y = 0;
			Real lg = std::log1p(-p);
			if (lg >= 0) return swap ? n : 0;
			do {
				y += std::llround(std::floor(std::log(real()) / lg)) + 1;
				if (y > n) return swap ? n - res : res;
				res++;
			} while (true);
		} else {
			// BTRS algorithm
			// https://epub.wu.ac.at/1242/1/document.pdf
			// note that the original paper has an error
			// the break condition at the end has to be log(v) < h[...]
			Real q = 1.0_real - p;
			Real spq = std::sqrt(n * p * q);
			Real b = 1.15_real + 2.53_real * spq;
			Real a = -0.0873_real + 0.0248_real * b + 0.01_real * p;
			Real c = n * p + 0.5_real;
			Real vr = 0.92_real - 4.2_real / b;

			bool initialized = false;
			Real alpha, lpq, m, h;
			do {
				Real u = real() - 0.5_real;
				Real us = 0.5_real - std::abs(u);
				Integer res = std::llround(std::floor((2.0_real * a / us + b) * u + c));
				if (res < 0 or res > n) continue;

				Real v = real();
				if (us >= 0.07_real and v <= vr) {
					return swap ? n - res : res;
				}

				if (!initialized) {
					alpha = (2.83_real + 5.1_real / b) * spq;
					lpq = std::log(p / q);
					m = std::round(std::floor((n + 1) * p));
					h = std::lgamma(m + 1) + std::lgamma(n - m + 1);
					initialized = true;
				}
				v *= alpha / (a / (us * us) + b);
				if (std::log(v) <= h - std::lgamma(res + 1) - std::lgamma(n - res + 1) + (res - m) * lpq) {
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
		} else {
			UInteger ul = static_cast<UInteger>(lower);
			UInteger uu = static_cast<UInteger>(upper);
			UInteger res = static_cast<UInteger>((uu - ul) * std::exp2(std::log2(real()) / n));
			return std::min(upper - 1, static_cast<Integer>(res + ul));
		}
	}
	Integer maximum(Integer upper, Integer n) {
		return maximum(0, upper, n);
	}

	Integer minimum(Integer lower, Integer upper, Integer n) {// in [lower, upper)
		return upper - 1 - maximum(0, upper - lower, n);
	}
	Integer minimum(Integer upper, Integer n) {
		return minimum(0, upper, n);
	}

	Integer minmax(Integer lower, Integer upper, Integer n) {// in [lower, upper)
		judgeAssert<std::invalid_argument>(n != 0, "Random::minmax(): n musst not be zero!");
		if (n > 0) {
			return maximum(lower, upper, n);
		} else {
			return minimum(lower, upper, -n);
		}
	}
	Integer minmax(Integer upper, Integer n) {
		return minmax(0, upper, n);
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
	typename std::iterator_traits<typename C::iterator>::reference select(C& c) {
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

	template<typename RandomIt>
	Integer rotate(RandomIt first, RandomIt last) {
		Integer rotation = integer(0, last - first);
		std::rotate(first, first + rotation, last);
		return rotation;
	}

	template<typename C>
	Integer rotate(C& c) {
		return rotate(std::begin(c), std::end(c));
	}

	//========================================================================//
	// sequences                                                              //
	//========================================================================//
	std::vector<Integer> distinct(Integer count, Integer lower, Integer upper) {
		judgeAssert<std::invalid_argument>(count >= 0, "Random::distinct(): count must be non negative!");
		judgeAssert<std::invalid_argument>(lower + count <= upper, "Random::distinct(): upper - lower must be at least count!");
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
		judgeAssert<std::invalid_argument>(count >= 0, "Random::perm(): count must be non negative!");
		std::vector<Integer> res = range(offset, offset + count);
		shuffle(res);
		return res;
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

	std::vector<Integer> perm(Integer count, const std::vector<Integer>& fix, Integer offset = 0) {
		judgeAssert<std::invalid_argument>(count >= 0, "Random::perm(): count must be non negative!");
		std::vector<Integer> tmp = range(count);
		std::vector<Integer> res(count, -1);
		for (Integer x : fix) {
			judgeAssert<std::invalid_argument>(offset <= x and x < offset + count, "Random::perm(): fix point outside of permutation!");
			std::swap(tmp[x - offset], res[x - offset]);
		}
		shuffle(tmp);
		std::size_t i = 0;
		for (Integer& x : res) {
			for (; x < 0; i++) x = tmp[i];
			x += offset;
		}
		return res;
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

	std::string string(Integer n, std::string_view alphabet = LOWER) {
		judgeAssert<std::invalid_argument>(0 <= n and n <= 0xFFFF'FFFF, "Random::string(): n out of range!");
		judgeAssert<std::invalid_argument>(!alphabet.empty(), "Random::string(): alphabet must not be empty!");
		std::string res(n, '*');
		for (char& c : res) c = select(alphabet);
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
			for (std::size_t i = 0; i < dirs.size(); i++) {
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

	template<typename Point = std::pair<Integer, Integer>>
	std::vector<Point> nonCollinearPoints(Integer n, Integer dim) {
		judgeAssert<std::invalid_argument>(dim <= 0x1FFF'FFFF, "Random::nonCollinearPoints(): dim too large!");
		judgeAssert<std::invalid_argument>(n >= 0, "Random::nonCollinearPoints(): dim must be non negative!");
		judgeAssert<std::invalid_argument>(dim > n, "Random::nonCollinearPoints(): dim too small!");
		Integer p = prime(dim - 1, 2*dim + 2);
		Integer rotA = 0;
		Integer rotB = 0;
		while (rotA == 0 and rotB == 0) {
			rotA = integer(0, p);
			rotB = integer(0, p);
		}
		std::array<Integer, 3> abc = {
			integer(1, p),
			integer(0, p),
			integer(0, p),
		};
		Integer dx = integer(-dim + 1, dim - p);
		Integer dy = integer(-dim + 1, dim - p);

		auto xs = distinct(n, p);
		std::vector<Point> res;
		for (auto tmpX : xs) {
			Integer tmpY = 0;
			for (Integer add : abc) {
				tmpY *= tmpX;
				tmpY += add;
				tmpY %= p;
			}

			Integer x = applyMod(tmpX * rotA - tmpY * rotB, p);
			Integer y = applyMod(tmpX * rotB + tmpY * rotA, p);

			res.emplace_back(x + dx, y + dy);
		}
		return res;
	}

} // namespace Random


//============================================================================//
// args parser                                                                //
//============================================================================//
class ParameterBase {
	friend class Command;
	friend struct Parameter;

	std::optional<std::string_view> token;

	template<typename T>
	T parse(std::string_view s) const {
		T res = {};
		judgeAssert<std::invalid_argument>(details::parse<T>(s, res), "Command: Could not parse args");
		return res;
	}

	ParameterBase() = default;
	explicit ParameterBase(std::string_view token_) : token(token_) {}

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

struct Parameter final : private ParameterBase {
	Parameter() = default;
	explicit Parameter(std::string_view token) : ParameterBase(token) {}

	using ParameterBase::asString;
	using ParameterBase::asInteger;
	using ParameterBase::asReal;

	bool exists() const {
		return token.has_value();
	}

	explicit operator bool() const {
		return exists();
	}
};

class Command final : private ParameterBase {
	const std::vector<std::string>& raw;
	const Integer first, count;
	const bool found;

	template<typename T, std::size_t... IS >
	auto asTuple(std::index_sequence<IS...> /**/) const {
		return std::make_tuple(parse<T>(raw[first + IS])...);
	}

	template<typename T, Integer N>
	auto as() const {
		if constexpr (N < 0) {
			std::vector<T> res;
			std::transform(raw.begin() + first,
			               raw.begin() + first + count,
			               std::back_inserter(res), [this](const std::string& value) {
				return parse<T>(value);
			});
			return res;
		} else {
			judgeAssert<std::invalid_argument>(N <= count, "Command: Could not parse args (too few args)");
			return asTuple<T>(std::make_index_sequence<static_cast<UInteger>(N)>{});
		}
	}
public:
	explicit Command(const std::vector<std::string>& raw_) : raw(raw_), first(0), count(0), found(false) {}
	explicit Command(const std::vector<std::string>& raw_, Integer first_, Integer count_)
	                 : ParameterBase(count_ == 0 ? ParameterBase() : ParameterBase(raw_[first_])),
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

	Parameter operator[](Integer i) const {
		if (i >= 0 and i < count) return Parameter(raw[first + i]);
		return Parameter();
	}

	using ParameterBase::asString;
	using ParameterBase::asInteger;
	using ParameterBase::asReal;

	template<Integer N = -1>
	auto asStrings() const {
		return as<std::string, N>();
	}

	template<Integer N = -1>
	auto asIntegers() const {
		return as<Integer, N>();
	}

	template<Integer N = -1>
	auto asReals() const {
		return as<Real, N>();
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
		judgeAssert<std::invalid_argument>(commands.count(command) == 0, "CommandParser: Duplicated command in args!");
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
		judgeAssert<std::out_of_range>(t >= 0 and t < static_cast<Integer>(raw.size()), "CommandParser: Index out of args!");
		return raw[t];
	}
	Command operator[](std::string_view command) const & {
		judgeAssert<std::invalid_argument>(details::isToken(command), "CommandParser: command must not contain a space!");
		auto it = commands.find(command);
		if (it == commands.end()) return Command(raw);
		return Command(raw, it->second.first, it->second.second);
	}
	Command getRaw(std::string_view command) const & {
		judgeAssert<std::invalid_argument>(details::isToken(command), "CommandParser: command must not contain a space!");
		auto it = tokens.find(command);
		if (it == tokens.end()) return Command(raw);
		return Command(raw, it->second, raw.size() - it->second);
	}
	Command getRaw() const & {
		return Command(raw, 0, raw.size());
	}
};


//============================================================================//
// Constants                                                                  //
//============================================================================//
Parameter parseConstant(std::string_view s) {
	if (s.size() >= 4 and 
		s.substr(0, 2) == "{{" and 
		s.substr(s.size() - 2) == "}}") {
		return Parameter();
	}
	return Parameter(s);
}

#define constant(key) parseConstant(#key)


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
		std::vector<std::string_view> names(byName.size());
		for (const auto& [name, id] : byName) names[id] = name;
		for (std::size_t i = 0; i < names.size(); i++) {
			const Constraint& c = *(constraints[i]);
			if (c.type) {
				os << "LocationNotSupported:" << names[i] << " " << names[i] << " ";
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
	OutputStream* out;
	Verdicts::Verdict onFail;
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
	                     OutputStream& out_,
	                     Verdicts::Verdict onFail_,
	                     Real floatAbsTol_ = DEFAULT_EPS,
	                     Real floatRelTol_ = DEFAULT_EPS) :
	                     managed(std::make_unique<std::ifstream>(path)),
	                     in(managed.get()),
	                     spaceSensitive(spaceSensitive_),
	                     caseSensitive(caseSensitive_),
	                     out(&out_),
	                     onFail(onFail_),
	                     floatAbsTol(floatAbsTol_),
	                     floatRelTol(floatRelTol_) {
		judgeAssert<std::runtime_error>(managed->good(), "InputStream: Could not open File: " + path.string());
		init();
	}
	explicit InputStream(std::istream& in_,
	                     bool spaceSensitive_,
	                     bool caseSensitive_,
	                     OutputStream& out_,
	                     Verdicts::Verdict onFail_,
	                     Real floatAbsTol_ = DEFAULT_EPS,
	                     Real floatRelTol_ = DEFAULT_EPS) :
	                     managed(),
	                     in(&in_),
	                     spaceSensitive(spaceSensitive_),
	                     caseSensitive(caseSensitive_),
	                     out(&out_),
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
			*out << "Missing EOF!";
			fail();
		}
	}

	void noteof() {
		checkIn();
		if (!spaceSensitive) *in >> std::ws;
		if (in->peek() == std::char_traits<char>::eof()) {
			*out << "Unexpected EOF!" << onFail;
		}
	}

	void space() {
		if (spaceSensitive) {
			noteof();
			if (in->get() != std::char_traits<char>::to_int_type(SPACE)) {
				*out << "Missing space!";
				fail();
			}
		}
	}

	void newline() {
		if (spaceSensitive) {
			noteof();
			if (in->get() != std::char_traits<char>::to_int_type(NEWLINE)) {
				*out << "Missing newline!";
				fail();
			}
		}
	}

private:
	void check(const std::string& token, const std::regex& pattern) {
		if (!std::regex_match(token, pattern)) {
			*out << "Token \"" << token << "\" does not match pattern!";
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
			*out << "Could not parse token \"" << s << "\"!";
			fail();
		}
		return res;
	}

public:
	std::string string() {
		noteof();
		if (spaceSensitive and !std::isgraph(in->peek())) {
			in->get();
			*out << "Invalid whitespace!";
			fail();
		}
		std::string res;
		*in >> res;
		if (res.empty()) {
			*out << "Unexpected EOF!" << onFail;
		}
		if (!caseSensitive) toDefaultCase(res);
		return res;
	}

	std::string string(Integer lower, Integer upper) {
		std::string t = string();
		Integer length = static_cast<Integer>(t.size());
		if (length < lower or length >= upper) {
			*out << "String length " << length << " out of range [" << lower << ", " << upper << ")!";
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
		for (std::size_t i = 0; i < res.size(); i++) {
			res[i] = string(args...);
			if (i + 1 < res.size()) sepCall();
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
			*out << "Integer " << res << " out of range [" << lower << ", " << upper << ")!";
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
		for (std::size_t i = 0; i < res.size(); i++) {
			res[i] = integer(args...);
			if (i + 1 < res.size()) sepCall();
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
			*out << "Real " << res << " out of range [" << lower << ", " << upper << ")!";
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
		for (std::size_t i = 0; i < res.size(); i++) {
			res[i] = real(args...);
			if (i + 1 < res.size()) sepCall();
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
			*out << "Real " << t << " has wrong amount of decimals!";
			fail();
			return 0;
		}
		try {
			Real res = parse<Real>(t);
			if (std::isnan(res) or !(res >= lower) or !(res < upper)) {
				*out << "Real " << res << " out of range [" << lower << ", " << upper << ")!";
				fail();
			}
			return res;
		} catch(...) {
			*out << "Could not parse token \"" << t << "\" as real!";
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
		for (std::size_t i = 0; i < res.size(); i++) {
			res[i] = realStrict(args...);
			if (i + 1 < res.size()) sepCall();
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
		auto [eq, pos] = details::stringEqual(seen, expected, caseSensitive);
		if (!eq) {
			auto format = [pos=pos,out=out](std::string_view s){
				Integer PREFIX = 10;
				Integer WINDOW = 5;
				if (s.size() <= PREFIX + WINDOW + TEXT_ELLIPSIS.size() * 2) {
					*out << s;
				} else if (*pos <= PREFIX + TEXT_ELLIPSIS.size() + WINDOW / 2 or *pos >= s.size()) {
					*out << s.substr(0, PREFIX + TEXT_ELLIPSIS.size() + WINDOW) << TEXT_ELLIPSIS;
				} else if (*pos + TEXT_ELLIPSIS.size() + WINDOW / 2 > s.size()) {
					*out << s.substr(0, PREFIX) << TEXT_ELLIPSIS << s.substr(*pos - WINDOW / 2);
				} else {
					*out << s.substr(0, PREFIX) << TEXT_ELLIPSIS << s.substr(*pos - WINDOW / 2, WINDOW) << TEXT_ELLIPSIS;
				}
			};
			*out << "Expected \"";
			format(expected);
			*out << "\" but got \"";
			format(seen);
			*out << "\"!";
			if (pos and *pos > 5) {
				*out << " (different at position: " << *pos+1 << ")";
			}
			fail();
		}
	}

	void expectInt(Integer expected) {
		Integer seen = integer();
		if (seen != expected) {
			*out << "Expected " << expected << " but got " << seen << "!";
			fail();
		}
	}

	void expectReal(Real expected) {
		Real seen = real();
		if (details::floatEqual(seen, expected, floatAbsTol, floatRelTol)) {
			*out << "Expected " << expected << " but got " << seen << "!";
			if (std::isfinite(seen) and std::isfinite(expected)) {
				Real absDiff = std::abs(seen-expected);
				Real relDiff = std::abs((seen-expected)/expected);
				*out << " (abs: " << absDiff << ", rel: " << relDiff << ")";
			}
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
				*out << " Line: " << line << ", Char: " << l << '\n';
				if (extend) {
					char tmp;
					while ((buffer.size() < 80 or buffer.size() < r + 80) and in->get(tmp) and tmp != NEWLINE) {
						buffer += tmp;
					}
				}
				if (r > 60 and l > 20) {
					std::size_t offset = std::min(l - 20, r - 60);
					l -= offset;
					r -= offset;
					buffer = std::string(TEXT_ELLIPSIS) + buffer.substr(offset + TEXT_ELLIPSIS.size());
				}
				if (buffer.size() > 80) {
					buffer = buffer.substr(0, 80 - TEXT_ELLIPSIS.size());
					buffer += TEXT_ELLIPSIS;
					r = std::min(r, buffer.size());
				}
				*out << buffer << '\n';
				*out << std::string(l, ' ') << '^' << std::string(r - l - 1, '~');
			}
		}
		*out << onFail;
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
	Setting(T value_) : SettingBase<T>(value_) {}
	using SettingBase<T>::operator T;
	using SettingBase<T>::operator=;
};

class SettingCaseSensitive final : public SettingBase<bool> {
public:
	SettingCaseSensitive(bool value_) : SettingBase<bool>(value_) {}
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

	constexpr boolean<std::size_t> stringEqual(std::string_view a, std::string_view b, bool caseSensitive_ = caseSensitive) {
		return details::stringEqual(a, b, caseSensitive_);
	}

	namespace details {
		void init(int argc, char** argv) {
			judgeAssert<std::logic_error>(!::details::initialized(), "validate.h: init(argc, argv) was called twice!");

			//std::ios_base::sync_with_stdio(false);
			//cin.tie(nullptr);

			arguments = CommandParser(argc, argv);
			if (auto seed = arguments[SEED_COMMAND]) Random::seed(static_cast<UInteger>(seed.asInteger()));
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
	using namespace Verdicts;

	InputStream testIn;

	void init(int argc, char** argv) {
		spaceSensitive = true;
		caseSensitive = true;

		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::cout);

		testIn = InputStream(std::cin, spaceSensitive, caseSensitive, juryOut, Verdicts::WA, floatAbsTol, floatRelTol);
		initConstraints();
	}

} // namespace InputValidator

//called as ./validator input [arguments] < ansfile
namespace AnswerValidator {
	using namespace ValidateBase;
	using namespace ConstraintsBase;
	using namespace Verdicts;

	InputStream testIn;
	InputStream ans;

	void init(int argc, char** argv) {
		spaceSensitive = true;
		caseSensitive = true;

		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::cout);

		testIn = InputStream(std::filesystem::path(arguments[1]), false, caseSensitive, juryOut, Verdicts::FAIL);
		ans = InputStream(std::cin, spaceSensitive, caseSensitive, juryOut, Verdicts::WA);
		initConstraints();
	}

} // namespace AnswerValidator

//called as ./validator input judgeanswer feedbackdir [arguments] < teamoutput
namespace OutputValidator {
	using namespace ValidateBase;
	using namespace ConstraintsBase;
	using namespace Verdicts;

	InputStream testIn;
	InputStream juryAns;
	InputStream teamAns;
	OutputStream teamOut;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::filesystem::path(arguments[3]) / JUDGE_MESSAGE, MESSAGE_MODE);
		teamOut = OutputStream(std::filesystem::path(arguments[3]) / TEAM_MESSAGE, MESSAGE_MODE);

		testIn = InputStream(std::filesystem::path(arguments[1]), false, caseSensitive, juryOut, Verdicts::FAIL);
		juryAns = InputStream(std::filesystem::path(arguments[2]), false, caseSensitive, juryOut, Verdicts::FAIL);
		teamAns = InputStream(std::cin, spaceSensitive, caseSensitive, juryOut, Verdicts::WA);
		initConstraints();
	}

} // namespace OutputValidator

//called as ./interactor input judgeanswer feedbackdir <> teamoutput
namespace Interactor {
	using namespace ValidateBase;
	using namespace Verdicts;

	OutputStream toTeam;
	InputStream testIn;
	InputStream fromTeam;
	OutputStream teamOut;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::filesystem::path(arguments[3]) / JUDGE_MESSAGE, MESSAGE_MODE);
		teamOut = OutputStream(std::filesystem::path(arguments[3]) / TEAM_MESSAGE, MESSAGE_MODE);
		toTeam = OutputStream(std::cout);

		testIn = InputStream(std::filesystem::path(arguments[1]), false, caseSensitive, juryOut, Verdicts::FAIL);
		fromTeam = InputStream(std::cin, spaceSensitive, caseSensitive, juryOut, Verdicts::WA);
	}

} // namespace Interactor

//for called see OutputValidator or Interactor respectively
namespace Multipass {
	using namespace ValidateBase;

	namespace details {
		std::ostringstream nextpassBuffer;
	}
	Integer pass;
	InputStream prevstate;
	OutputStream nextstate;
	OutputStream nextpass;

	void init() {
		judgeAssert<std::logic_error>(::details::initialized(), "validate.h: Multipass::init() was called before init(argc, argv)!");

		auto path = std::filesystem::path(arguments[3]) / ".pass";
		std::string nextfile = ".state0";
		std::string prevfile = ".state1";
		if (std::filesystem::exists(path)) {
			std::ifstream in(path);
			in >> pass;
			pass++;
			if ((pass & 1) != 0) {
				std::swap(nextfile, prevfile);
			}
			prevstate = InputStream(std::filesystem::path(arguments[3]) / prevfile, false, true, juryOut, Verdicts::FAIL);
		} else {
			pass = 0;
		}
		std::filesystem::remove(std::filesystem::path(arguments[3]) / nextfile);
		nextstate = OutputStream(std::filesystem::path(arguments[3]) / nextfile, std::ios::out);
		nextpass = OutputStream(details::nextpassBuffer);
		std::ofstream out(path);
		out << pass;
	}

	[[noreturn]] void NEXT() {
		{
			std::ofstream file(std::filesystem::path(arguments[3]) / "nextpass.in");
			judgeAssert<std::runtime_error>(file.good(), "NEXT(): Could not open file: nextpass.in");
			file << details::nextpassBuffer.str();
		}
		exitVerdict(Verdicts::AC);
	}
	[[noreturn]] std::ostream& NEXT(std::ostream& os) {
		os << std::endl;
		NEXT();
	}

} // namespace Multipass

//called as ./generator [arguments]
namespace Generator {
	using namespace ValidateBase;
	using Verdicts::FAIL;

	OutputStream testOut;

	void init(int argc, char** argv) {
		ValidateBase::details::init(argc, argv);
		juryOut = OutputStream(std::cerr);
		testOut = OutputStream(std::cout);
	}

} // namespace Generator

#endif
