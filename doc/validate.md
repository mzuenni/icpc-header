# validate.h
A simple library to validate team input, team output, and generate random data.


## Global Types
**`Integer = std::int64_t`**  
**`UInteger = std::uint64_t`**  
**`Real = long double`**  

> Note: The header defines the literal suffix `_int` and `_real`.

## Global Constants
**`constexpr Verdict AC(42)`**  
**`constexpr Verdict WA(43)`**  
**`constexpr Verdict PE(43)`**  
**`constexpr Verdict FAIL(1)`**  

> Note: The verdicts are only included for validators, not for generators.

**`constexpr std::string_view LETTER("a..zA..Z")`**  
**`constexpr std::string_view LOWER("a..z")`**  
**`constexpr std::string_view UPPER("A..Z")`**  
**`constexpr std::string_view VOWEL("aeiouAEIOU")`**  
**`constexpr std::string_view LOWER_VOWELS("aeiou")`**  
**`constexpr std::string_view UPPER_VOWELS("AEIOU")`**  
**`constexpr std::string_view CONSONANT("b..zB..Z")`**  
**`constexpr std::string_view LOWER_CONSONANT("b..z")`**  
**`constexpr std::string_view UPPER_CONSONANT("B..Z")`**  
**`constexpr std::string_view ALPHA_NUMERIC("0..9a..zA..Z")`**  
**`constexpr std::string_view LOWER_ALPHA_NUMERIC("0..9a..z")`**  
**`constexpr std::string_view UPPER_ALPHA_NUMERIC("0..9A..Z")`**  
**`constexpr std::string_view DIGITS("0..9")`**  


## namespace InputValidator
Use this namespace if you want to write an input validator that gets called as `./validator [arguments] < inputfile`.

#### Members
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
**`CommandParser arguments`**  
**`Setting<Real> floatAbsTol`**  
**`Setting<Real> floatRelTol`**  
**`Setting<bool> spaceSensitive`**  
**`Setting<bool> caseSensitive`**  

**`InputStream testIn`**  
**`ConstraintsLogger constraint`**  

#### Methods
**`void init(int argc, char** argv)`**  
Parses the program arguments and stores them in `arguments`.  
Further, initializes all streams and settings.  
Both `spaceSensitive` and `caseSensitive` will be set to true, and `testIn` is initialized accordingly.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatLess(Real given, Real expected)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatGreaterEqual(Real given, Real expected)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating-point numbers `given` and `expected` obey the relation `given` == `expected`, `given` <= `expected`, or `given` >= `expected`, allowing an absolute and relative error of the given tolerance.  
For relative errors, the `expected` value is used as reference value.  
If no tolerance is given, the global setting `floatAbsTol` respectively `floatRelTol` is used as fallback.

**`boolean<Integer> stringEqual(std::string_view a, std::string_view b)`**  
**`boolean<Integer> stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.  
If `caseSensitive` is not provided, the global setting `caseSensitive` is used as fallback.  
If the strings are not equal, the position of the first mismatch is returned as a witness.

## namespace AnswerValidator
Use this namespace if you want to write an answer validator that gets called as `./validator input [arguments] < ans`.

#### Members
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
**`OutputStream teamOut`**  
**`CommandParser arguments`**  
**`Setting<Real> floatAbsTol`**  
**`Setting<Real> floatRelTol`**  
**`Setting<bool> spaceSensitive`**  
**`Setting<bool> caseSensitive`**  

**`InputStream testIn`**  
**`InputStream teamAns`**  

#### Methods
**`void init(int argc, char** argv)`**  
Parses the program arguments and stores them in `arguments`.  
Further, initializes all streams and settings.  
Both `spaceSensitive` and `caseSensitive` will be set to true, and `juryAns` is initialized accordingly.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatLess(Real given, Real expected)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatGreaterEqual(Real given, Real expected)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating-point numbers `given` and `expected` obey the relation `given` == `expected`, `given` <= `expected`, or `given` >= `expected`, allowing an absolute and relative error of the given tolerance.  
For relative errors, the `expected` value is used as reference value.  
If no tolerance is given, the global setting `floatAbsTol` respectively `floatRelTol` is used as fallback.

**`boolean<Integer> stringEqual(std::string_view a, std::string_view b)`**  
**`boolean<Integer> stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.  
If `caseSensitive` is not provided, the global setting `caseSensitive` is used as fallback.  
If the strings are not equal, the position of the first mismatch is returned as a witness.

## namespace OutputValidator
Use this namespace if you want to write an output validator that gets called as `./validator input judgeanswer feedbackdir [arguments] < teamoutput`.

#### Members
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
**`OutputStream teamOut`**  
**`CommandParser arguments`**  
**`Setting<Real> floatAbsTol`**  
**`Setting<Real> floatRelTol`**  
**`Setting<bool> spaceSensitive`**  
**`Setting<bool> caseSensitive`**  

**`InputStream testIn`**  
**`InputStream juryAns`**  
**`InputStream teamAns`**  

#### Methods
**`void init(int argc, char** argv)`**  
Parses the program arguments and stores them in `arguments`.  
Further, initializes all streams and settings.  
You can set `spaceSensitive` and `caseSensitive` before this call; by default both are `false`.  
After this call, all streams will be initialized with `caseSensitive`, and `teamAns` will also be `spaceSensitive`.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatLess(Real given, Real expected)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatGreaterEqual(Real given, Real expected)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating-point numbers `given` and `expected` obey the relation `given` == `expected`, `given` <= `expected`, or `given` >= `expected`, allowing an absolute and relative error of the given tolerance.  
For relative errors, the `expected` value is used as reference value.  
If no tolerance is given, the global setting `floatAbsTol` respectively `floatRelTol` is used as fallback.

**`boolean<Integer> stringEqual(std::string_view a, std::string_view b)`**  
**`boolean<Integer> stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.  
If `caseSensitive` is not provided, the global setting `caseSensitive` is used as fallback.  
If the strings are not equal, the position of the first mismatch is returned as a witness.

## namespace Interactor
Use this namespace if you want to write an output validator for interactive problems that gets called as `./interactor input judgeanswer feedbackdir [arguments] <> team`.

#### Members
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
**`OutputStream teamOut`**  
**`CommandParser arguments`**  
**`Setting<Real> floatAbsTol`**  
**`Setting<Real> floatRelTol`**  
**`Setting<bool> spaceSensitive`**  
**`Setting<bool> caseSensitive`**  

**`OutputStream toTeam`**  
**`InputStream testIn`**  
**`InputStream fromTeam`**  

#### Methods
**`void init(int argc, char** argv)`**  
Parses the program arguments and stores them in `arguments`.  
Further, initializes all streams and settings.  
You can set `spaceSensitive` and `caseSensitive` before this call; by default both are `false`.  
After this call, all streams will be initialized with `caseSensitive`, and `fromTeam` will also be `spaceSensitive`.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatLess(Real given, Real expected)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatLess(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
**`bool floatGreaterEqual(Real given, Real expected)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatGreaterEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating-point numbers `given` and `expected` obey the relation `given` == `expected`, `given` <= `expected`, or `given` >= `expected`, allowing an absolute and relative error of the given tolerance.  
For relative errors, the `expected` value is used as reference value.  
If no tolerance is given, the global setting `floatAbsTol` respectively `floatRelTol` is used as fallback.

**`boolean<Integer> stringEqual(std::string_view a, std::string_view b)`**  
**`boolean<Integer> stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.  
If `caseSensitive` is not provided, the global setting `caseSensitive` is used as fallback.  
If the strings are not equal, the position of the first mismatch is returned as a witness.


## namespace Multipass
Additionally use this namespace if you want to write a Multipass output validator (for interactive problems).  
The namespace already provides functionality to get the ID of the current pass and save state between passes.

#### Members
**`Integer pass`**  
**`InputStream prevstate`**  
**`OutputStream nextstate`**  
**`OutputStream nextpass`**  

#### Methods
**`void NEXT()`**  
**`std::ostream& NEXT(std::ostream& os)`**  
Use this like a verdict to write into `nextpass.in` and end the current run.


## namespace Generator
Use this namespace if you want to write a generator for test cases that gets called as `./generator [arguments]`.

#### Members
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
**`CommandParser arguments`**  
**`Setting<Real> floatAbsTol`**  
**`Setting<Real> floatRelTol`**  
**`Setting<bool> spaceSensitive`**  
**`Setting<bool> caseSensitive`**  

**`OutputStream testOut`**  

#### Methods
**`void init(int argc, char** argv)`**  
Parses the program arguments and stores them in `arguments`.  
Further, initializes all streams and settings.


## class CommandParser
An instance of this class is always provided as `arguments` after calling any `init(argc, argv)` method.  
> Note: The parsed commands should start with `--`.

#### Methods
**`Command operator[](std::string_view command) const`**  
Searches for the `command` and returns it with all its parameters (until the next command starting with `--`).  
If the `command` does not exist, the list of parameters will be empty.

**`Command getRaw(std::string_view command) const`**  
Searches for the `command` and returns it with all following parameters.  
If the `command` does not exist, the list of parameters will be empty.

**`Command getRaw() const`**  
Returns all parameters.

## class Command
Gets returned by the CommandParser and corresponds to a command in `argv`.

#### Methods
**`bool exists() const`**  
**`explicit operator bool() const`**  
True if the `CommandParser` found this command.

**`Integer parameterCount() const`**  
Counts the number of parameters the command can access.  
Zero if the command was not found.

**`std::string asString() const`**  
**`std::string asString(std::string_view defaultValue) const`**  
**`Integer asInteger() const`**  
**`Integer asInteger(Integer defaultValue) const`**  
**`Real asReal() const`**  
**`Real asReal(Real defaultValue) const`**  
Parses the first token after the command.  
If the command does not exist or has no parameter, it returns `defaultValue`.  
> Note: If `defaultValue` should be returned but was not provided, an error will be thrown.

**`std::vector<std::string> asStrings() const`**  
**`std::tuple<std::string...> asStrings<N>() const`**  
**`std::vector<Integer> asIntegers() const`**  
**`std::tuple<Integer...> asIntegers<N>() const`**  
**`std::vector<Real> asReals() const`**  
**`std::tuple<Real...> asReals<N>() const`**  
Return all parameters of this command.

**`Parameter operator[](Integer i) const`**  
Returns the `i`th parameter.  
The first has index 0.

## class Parameter
Gets returned by the Command and corresponds to a parameter of a command.

#### Methods
**`bool exists() const`**  
**`explicit operator bool() const`**  
True if the parameter exists.

**`std::string asString() const`**  
**`std::string asString(std::string_view defaultValue) const`**  
**`Integer asInteger() const`**  
**`Integer asInteger(Integer defaultValue) const`**  
**`Real asReal() const`**  
**`Real asReal(Real defaultValue) const`**  
Parses the parameter.  
If the parameter does not exist, it returns `defaultValue`.  
> Note: If `defaultValue` should be returned but was not provided, an error will be thrown.

## class OutputStream
Just a wrapper for an `std::ostream` which allows changing the underlying output stream at any time.  
It also allows printing tuples and pairs.

## class InputStream
A wrapper for an `std::istream` which allows safely parsing input as tokens.  
If the input does not match the requested token, the program will terminate with an appropriate exit code (WA for team input and FAIL otherwise).  
> Note: If the stream is case-insensitive, all characters read are converted to lower case.

#### Methods
**`void eof()`**  
**`void space()`**  
**`void newline()`**  
Checks if the next char in the input is the corresponding whitespace.  
This only has an effect for space-sensitive streams, i.e., input validation.

**`void expectString(std::string_view expected)`**  
**`void expectInt(Integer expected)`**  
**`void expectReal(Real expected)`**  
Checks if the next token matches `expected`.  
If a `Real` is expected, equality is checked with absolute and relative error.  
This is mainly supposed to verify team output.

**`std::string string()`**  
**`std::string string(Integer lower, Integer upper)`**  
**`std::string string(Integer lower, Integer upper, Constraint& constraint)`**  
**`std::string string(std::regex pattern)`**  
**`std::string string(std::regex pattern, Integer lower, Integer upper)`**  
**`std::string string(std::regex pattern, Integer lower, Integer upper, Constraint& constraint)`**  
Extracts the next token.  
If `lower` or `upper` is provided, it checks that the token length is in [lower, upper).  
If a `pattern` is provided, it checks that the token matches the pattern.  
If a constraint is provided, it is updated with the length of the token.

**`Integer integer()`**  
**`Integer integer(Integer lower, Integer upper)`**  
**`Integer integer(Integer lower, Integer upper, Constraint& constraint)`**  
Extracts the next token as an `Integer`.  
If `lower` or `upper` is provided, it checks that the integer is in [lower, upper).  
If a constraint is provided, it is updated with the value of the parsed integer.

**`Real real()`**  
Extracts the next token as a `Real`.  
Use this method to parse team input as `Real`!  
> Note: This method does not allow reading `NaN` or `Inf`.

**`Real real(Real lower, Real upper)`**  
**`Real real(Real lower, Real upper, Constraint& constraint)`**  
Extracts the next token as a `Real`.  
`lower` and `upper` are used to check that the real is in [lower, upper); the check uses the global `eps` as tolerance.  
This is not supposed to be used for validators.

**`Real realStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals)`**  
**`Real realStrict(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint)`**  
`lower` and `upper` are used to check that the real is in [lower, upper); the check does **not** use any `eps` as tolerance.  
`minDecimals` and `maxDecimals` are used to check that the token was given with the number of decimal places in [minDecimals, maxDecimals).  
If a constraint is provided, it is updated with the value of the parsed integer.  
This is supposed to be used for validators only.

**`std::vector<std::string> strings(Integer count, [args])`**  
**`std::vector<std::string> strings(Integer count, [args], char separator)`**  
**`std::vector<Integer> integers(Integer count, [args])`**  
**`std::vector<Integer> integers(Integer count, [args], char separator)`**  
**`std::vector<Real> reals(Integer count, [args])`**  
**`std::vector<Real> reals(Integer count, [args], char separator)`**  
**`std::vector<Real> realsStrict(Integer count, [args])`**  
**`std::vector<Real> realsStrict(Integer count, [args], char separator)`**  
Extracts the next `count` tokens by calling `string([args])`, `integer([args])`, `real([args])`, or `realStrict([args])`.  
The tokens are expected to be separated by the char `separator`, which must be `space` or `newline`.  
If this parameter is not provided, a `space` is expected.


## namespace Random
This namespace contains functions to generate random numbers.  
> Note: Do not use C++ random functions since their output is compiler-dependent.

#### Methods
**`void seed(UInteger seed)`**  
Seeds the internal random number generator.

##### Numbers
**`bool bit()`**  
Returns a uniformly distributed bit.

**`Integer integer()`**  
**`Integer integer(Integer upper)`**  
**`Integer integer(Integer lower, Integer upper)`**  
Returns a uniformly distributed integer in [-2^63, 2^63), [0, upper), or [lower, upper).

**`Real real()`**  
**`Real real(Real upper)`**  
**`Real real(Real lower, Real upper)`**  
Returns a uniformly distributed real in [0, 1), [0, upper), or [lower, upper).

**`Integer discrete<w_1,...,w_k>()`**  
Returns an integer in [0, k), where `i` is chosen with probability `w_i / (sum over w)`.

**`Real normal(Real mean, Real stddev)`**  
**`Real normal(Real lower, Real upper, Real mean, Real stddev)`**  
**`Real exponential(Real lambda)`**  
**`Real exponential(Real lower, Real upper, Real lambda)`**  
**`Integer geometric(Real p)`**  
**`Integer geometric(Integer lower, Integer upper, Real p)`**  
**`Integer binomial(Integer n, Real p)`**  
**`Integer binomial(Integer lower, Integer upper, Integer n, Real p)`**  
Generates a number according to some distribution. The generated integer will always be in [lower, upper).

**`Integer maximum(Integer upper, Integer n)`**  
**`Integer maximum(Integer lower, Integer upper, Integer n)`**  
**`Integer minimum(Integer upper, Integer n)`**  
**`Integer minimum(Integer lower, Integer upper, Integer n)`**  
Random variable distributed like the maximum/minimum of `n` uniform integers in [0, upper) or [lower, upper).

**`Integer minmax(Integer upper, Integer n)`**  
**`Integer minmax(Integer lower, Integer upper, Integer n)`**  
If `n` > 0 this calls maximum; for `n` < 0 this calls minimum; `n` == 0 is undefined.

**`Integer prime(Integer upper)`**  
**`Integer prime(Integer lower, Integer upper)`**  
Generates a uniformly chosen prime in [0, upper) or [lower, upper).

##### Sequences
**`std::vector<Integer> distinct(Integer count, Integer upper)`**  
**`std::vector<Integer> distinct(Integer count, Integer lower, Integer upper)`**  
Generates `count` different integers in [0, upper) or [lower, upper).  
Each possible output has the same probability of being generated.

**`std::vector<Integer> perm(Integer count)`**  
**`std::vector<Integer> perm(Integer count, Integer offset)`**  
Generates a permutation of the numbers [0, count) or [offset, offset + count).  
Each possible output has the same probability of being generated.

**`std::vector<Integer> perm(std::vector<Integer> cycles)`**  
**`std::vector<Integer> perm(std::vector<Integer> cycles, Integer offset)`**  
Generates a permutation of the numbers [0, sum{cycles}) or [offset, offset + sum{cycles}).  
The cycle lengths of the generated permutations will be the values in `cycles`.  
Each possible output has the same probability of being generated.

**`std::vector<Integer> perm(Integer count, std::vector<Integer> fix)`**  
**`std::vector<Integer> perm(Integer count, std::vector<Integer> fix, Integer offset)`**  
Generates a permutation of the numbers [0, count) or [offset, offset + count) where each `i` in `fix` is a fixpoint.  
Each possible output has the same probability of being generated.

**`std::vector<Integer> multiple(Integer count, Integer lower, Integer upper)`**  
**`std::vector<Integer> multiple(Integer count, Integer upper)`**  
**`std::vector<Integer> increasing(Integer count, Integer lower, Integer upper)`**  
**`std::vector<Integer> increasing(Integer count, Integer upper)`**  
**`std::vector<Integer> decreasing(Integer count, Integer lower, Integer upper)`**  
**`std::vector<Integer> decreasing(Integer count, Integer upper)`**  
**`std::vector<Integer> nonDecreasing(Integer count, Integer lower, Integer upper)`**  
**`std::vector<Integer> nonDecreasing(Integer count, Integer upper)`**  
**`std::vector<Integer> nonIncreasing(Integer count, Integer lower, Integer upper)`**  
**`std::vector<Integer> nonIncreasing(Integer count, Integer upper)`**  
Generates `count` integers in [0, upper) or [lower, upper).  
The sequences generated by the first two methods are equal to calling `Random::integer()` `count` times.  
All other methods generate a sequence that fits the name of the generating method.  
Each possible output has the same probability of being generated.

**`std::vector<Integer> partition(Integer n, Integer k)`**  
**`std::vector<Integer> partition(Integer n, Integer k, Integer min)`**  
Uniformly generates an unsorted partition of `n` into `k` integers in [1, `n`) or [min, `n`).  
Note that with non-positive `min` the distribution is not uniform.

**`std::string string(Integer n)`**  
**`std::string string(Integer n, string_view alphabet)`**  
Uniformly generates a string of length `n` with chars from the given alphabet or a-z if no alphabet is given.

**`std::string bracketSequence(Integer n)`**  
Uniformly generates a bracket sequence of length `2n`.

**`std::vector<std::pair<Integer, Integer>> convex(Integer n, Integer dim)`**  
**`std::vector<std::pair<Integer, Integer>> convex(Integer n, Integer dimX, Integer dimY)`**  
**`std::vector<Point> convex(Integer n, Integer dim)`**  
**`std::vector<Point> convex(Integer n, Integer dimX, Integer dimY)`**  
Generates `n` points forming a convex polygon with points in (-dim, dim)<sup>2</sup> or (-dimX, dimX) × (-dimY, dimY) (the polygon will always fill the width and height).  
The points will be in counterclockwise order.  
Note that points may be collinear.

**`std::vector<std::pair<Integer, Integer>> nonCollinearPoints(Integer n, Integer dim)`**  
**`std::vector<Point> nonCollinearPoints(Integer n, Integer dim)`**  
Generates `n` points in general position with points in (-dim, dim)<sup>2</sup>.

##### Utility
**`const RandomIt::value_type& select(RandomIt first, RandomIt last)`**  
**`C::value_type select(C c)`**  
**`T select(T(c)[N])`**  
**`T select(std::pair<T, T> t)`**  
**`T select(std::complex<T> t)`**  
Uniformly selects a value from a range, complete container, initializer list, pair, or complex.

**`void shuffle(RandomIt first, RandomIt last)`**  
**`void shuffle(C& c)`**  
**`void shuffle(std::pair<T, T>& t)`**  
**`void shuffle(std::complex<T>& t)`**  
Uniformly shuffles a range, complete container, pair, or complex.

**`Integer rotate(RandomIt first, RandomIt last)`**  
**`Integer rotate(C& c)`**  
Uniformly rotates a range or complete container and returns the amount by which it was rotated left.


## Math functions
**`constexpr Integer applyMod(Integer x, Integer mod)`**  
Calculates `x` modulo `mod` in a mathematical sense, i.e., the result will be in the range [0, mod).

**`constexpr Integer mulMod(Integer lhs, Integer rhs, Integer mod)`**  
**`constexpr Integer powMod(Integer base, Integer exp, Integer mod)`**  
Calculates `(lhs * rhs) % mod` or `(lhs ^ rhs) % mod` without overflow.

**`constexpr Integer multInv(Integer n, Integer mod)`**  
Calculates the multiplicative inverse of `n` modulo `mod`.  
> Note: If the multiplicative inverse does not exist, the method returns `-1`.

**`constexpr bool isPrime(Integer n)`**  
Checks if a given number `n` is prime.

**`std::vector<Integer> primes(Integer upper)`**  
**`std::vector<Integer> primes(Integer lower, Integer upper)`**  
Returns all primes in [0, upper) or [lower, upper).

**`constexpr T sign(T x)`**  
Determines the sign of `x` as {-1, 0, 1}.

**`constexpr T dist(T a, T b)`**  
Returns the absolute difference between `a` and `b`.


## class boolean
This is a wrapper for a `bool` which may additionally hold a reason for the `bool` value in the form of some kind of witness or counterexample.

#### Members
**`bool value`**  
**`std::optional<T> reason`**  

#### Methods
**`constexpr boolean(bool value)`**  
**`constexpr boolean(bool value, T reason)`**  
Creates a new `boolean` value.

**`constexpr operator bool() const`**  
Allows implicit casting to a normal `bool`.

**`constexpr bool hasReason() const`**  
Checks if this boolean has a witness or counterexample.


## Utility functions
**`boolean<T> isPerm(C c)`**  
**`boolean<T> isPerm(RandomIt first, RandomIt last)`**  
**`boolean<T> isPerm(C c, C::value_type offset)`**  
**`boolean<T> isPerm(RandomIt first, RandomIt last, RandomIt::value_type offset)`**  
Checks if `c` respectively [first, last) is a permutation of [0, `n`) respectively [offset, offset + `n`), where `n` is the distance between `first` and `last`.  
If the input is not a permutation, a witness in the form of a duplicate element or a value outside the allowed range is returned.  
> Note: This function is only defined for integer ranges.

**`bool isPerm(C1 c1, C2 c2)`**  
**`bool isPerm(itA firstA, itA lastA, itB firstB, itB lastB)`**  
Checks if `c1` respectively [firstA, lastA) is a permutation of `c2` respectively [firstB, lastB).  
If the input is not a permutation but both ranges have the same number of elements, a witness in the form of an element from [firstA, lastA) is returned.

**`constexpr boolean<Integer> anyAdjacent(C c, BinaryPredicate p)`**  
**`constexpr boolean<Integer> noneAdjacent(C c, BinaryPredicate p)`**  
**`constexpr boolean<Integer> allAdjacent(C c, BinaryPredicate p)`**  
**`constexpr boolean<Integer> anyAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
**`constexpr boolean<Integer> noneAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
**`constexpr boolean<Integer> allAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
Checks if the predicate `p` evaluates to true for any, none, or all adjacent entries in the container `c` respectively the range [first, last).  
If the `boolean` is `false`, the length of the prefix for which it would have been true is returned as a witness.

**`constexpr boolean<Integer> areIncreasing(C c)`**  
**`constexpr boolean<Integer> areNonDecreasing(C c)`**  
**`constexpr boolean<Integer> areDecreasing(C c)`**  
**`constexpr boolean<Integer> areNonIncreasing(C c)`**  
**`constexpr boolean<Integer> areIncreasing(RandomIt first, RandomIt last)`**  
**`constexpr boolean<Integer> areNonDecreasing(RandomIt first, RandomIt last)`**  
**`constexpr boolean<Integer> areDecreasing(RandomIt first, RandomIt last)`**  
**`constexpr boolean<Integer> areNonIncreasing(RandomIt first, RandomIt last)`**  
Checks if the container `c` respectively the range [first, last) is ordered according to the name of this function.  
If the `boolean` is `false`, the length of the prefix for which it would have been true is returned as a witness.


**`constexpr boolean<T> areDistinct(C c)`**  
**`constexpr boolean<T> areDistinct(RandomIt first, RandomIt last)`**  
Checks if the container `c` respectively the range [first, last) contains only distinct elements.  
If there is a duplicate element, it is returned as a witness.

**`constexpr auto join(C c)`**  
**`constexpr auto join(C c, char separator)`**  
**`constexpr auto join(T first, T last)`**  
**`constexpr auto join(T first, T last, char separator)`**  
**`constexpr auto join(std::pair in)`**  
**`constexpr auto join(std::pair in, char separator)`**  
**`constexpr auto join(std::tuple in)`**  
**`constexpr auto join(std::tuple in, char separator)`**  
**`auto join({brace initializer list} in)`**  
**`auto join({brace initializer list} in, char separator)`**  
**`constexpr auto join(char[] cstring)`**  
**`constexpr auto join(char[] cstring, char separator)`**  
Generates an object that can be printed with an `std::ostream`.  
All elements in `c`, `in`, [first, last), or all but the last element in `cstring` will be printed and separated by `separator`.  
Note that for a `cstring` it is expected that the last value is the null character `\0`.  
If `separator` is not specified, a space is used.  
If the container is given as an rvalue, the generated object stores all values; otherwise, it just stores a reference.

**`constexpr bool isLower(char c)`**  
**`constexpr bool isUpper(char c)`**  
**`constexpr bool isLetter(char c)`**  
**`constexpr bool isDigit(char c)`**  
**`constexpr bool isVowel(char c)`**  
**`constexpr bool isConsonant(char c)`**  
Checks if a given char belongs to the alphabet that fits the name of the function.

**`constexpr boolean<char> isLower(std::string_view s)`**  
**`constexpr boolean<char> isUpper(std::string_view s)`**  
**`constexpr boolean<char> isLetter(std::string_view s)`**  
**`constexpr boolean<char> isDigit(std::string_view s)`**  
**`constexpr boolean<char> isVowel(std::string_view s)`**  
**`constexpr boolean<char> isConsonant(std::string_view s)`**  
Checks if all chars of a given string belong to the alphabet that fits the name of the function.  
If not, an invalid char from the string is given as a witness.

**`constexpr char toLower(char c)`**  
**`constexpr char toUpper(char c)`**  
**`constexpr char toDefaultCase(char c)`**  
**`void toLower(std::string& s)`**  
**`void toUpper(std::string& s)`**  
**`void toDefaultCase(std::string& s)`**  
Converts a `char` or `std::string` to the given case.  
> Note: The default case is lower case.

**`std::vector<Integer> thueMorse(Integer upper)`**  
**`std::vector<Integer> thueMorse(Integer lower, Integer upper)`**  
Generates the Thue-Morse sequence [0, upper) respectively [lower, upper).

**`std::vector<Integer> range(Integer to)`**  
**`std::vector<Integer> range(Integer from, Integer to)`**  
**`std::vector<Integer> range(Integer from, Integer to, Integer step)`**  
Generates the integers `res[i] = from + i * step` for all non-negative `i` where `res[i] < to` (or `res[i] > to` if `step < 0`).  

**`std::vector<Integer> range(Integer to)`**  
**`std::vector<Integer> range(Integer from, Integer to)`**  
**`std::vector<Integer> range(Integer from, Integer to, Integer step)`**  
Generates the integers `res[i] = from + i * step` for all non-negative `i` where `res[i] < to` (or `res[i] > to` if `step < 0`).  

**`boolean<Integer> isInteger(std::string s)`**  
Checks if the given string can be parsed as `Integer`. If yes, that integer is also provided.

**`boolean<Real> isReal(std::string s)`**  
Checks if the given string can be parsed as `Real`. If yes, that real is also provided.

## Geometry functions
**`constexpr bool isConvex(RandomIt first, RandomIt last)`**  
**`constexpr bool isConvex(const C& c)`**  
Checks if the points form a counterclockwise oriented convex polygon.

**`constexpr bool isStrictlyConvex(RandomIt first, RandomIt last)`**  
**`constexpr bool isStrictlyConvex(const C& c)`**  
Checks if the points form a counterclockwise oriented convex polygon without three collinear points.

**`bool isSimple(RandomIt first, RandomIt last)`**  
**`bool isSimple(const C& c)`**  
Checks if the points form a simple polygon.

**`constexpr bool isCCW(RandomIt first, RandomIt last)`**  
**`constexpr bool isCCW(const C& c)`**  
Checks if the points are oriented counterclockwise (this is only well defined for simple polygons).


## class ConstraintsLogger 
An instance of this class is provided for input and output validators as `constraint` after calling `init(argc, argv)`.

#### Methods
**`Constraint& operator[](std::string name)`**  
Returns a `constraint` which can be given to an input stream to automatically log information about the parsed input.  
See [BAPCtools](https://github.com/RagnarGrootKoerkamp/BAPCtools/blob/master/doc/implementation_notes.md#constraints-checking).
