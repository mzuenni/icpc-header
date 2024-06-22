//only gcc currently supports from_chars?
#if !defined(__GNUG__) || defined(__clang__) || defined(__INTEL_COMPILER) || __GNUG__ < 11
	#define DOUBLE_FALLBACK
#endif

namespace Settings {
	[[noreturn]] void exitVerdict(int exit_code) {
		throw exit_code;
	}
}
#include "../src/validate.h"

#if __cplusplus > 202000L
#include <source_location>
#else
namespace std {
	struct source_location {
		static constexpr std::source_location current() noexcept {return {};}
		constexpr std::uint_least32_t line() const noexcept {return 0;}
		constexpr std::uint_least32_t column() const noexcept {return 0;}
		constexpr const char* file_name() const noexcept {return "";}
		constexpr const char* function_name() const noexcept {return "";}
	};

	template <class C>
	constexpr auto ssize(const C& c) -> std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(c.size())>> {
		using R = std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(c.size())>>;
		return static_cast<R>(c.size());
	}
}
#endif

template<typename A, typename B>
constexpr bool startsWith(const A& a, const B& b) {
	if (std::size(a) < std::size(b)) return false;
	auto itA = std::begin(a);
	auto itB = std::begin(b);
	while (itB != std::end(b)) {
		if (*itA != *itB) return false;
		itA++;
		itB++;
	}
	return true;
}

template<typename A, typename B>
constexpr bool endsWith(const A& a, const B& b) {
	if (std::size(a) < std::size(b)) return false;
	auto itA = std::rbegin(a);
	auto itB = std::rbegin(b);
	while (itB != std::rend(b)) {
		if (*itA != *itB) return false;
		itA++;
		itB++;
	}
	return true;
}

void assert(bool b, const std::source_location location = std::source_location::current()) {
	if (!b) {
		std::cout << location.file_name() << "("
		          << location.line() << ":"
		          << location.column() << ") `"
		          << location.function_name() << "` failed!"
		          << std::endl;
		std::exit(1);
	}
}

template<typename F>
void assertException(const F& f, const std::source_location location = std::source_location::current()) {
	try {f();} catch (...) {}
	assert(false, location);
}

template<typename F>
void assertExit(const F& f, int expected, const std::source_location location = std::source_location::current()) {
	try {f();} catch (int exitCode) {
		assert(exitCode == expected, location);
		return;
	}
	assert(false, location);
}

template<typename F>
void assertNoException(const F& f, const std::source_location location = std::source_location::current()) {
	try {f();} catch (...) {assert(false, location);}
}
