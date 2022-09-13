# validate.h
A simple library to validate team input, and generate random data.


## Global Constants
**`Integer = std::int64_t`**  
**`UInteger = std::uint64_t`**  
**`Real = long double`**  

> Note: The header defines the literal `_int` and `_real`.

**`constexpr Verdict AC(42)`**  
**`constexpr Verdict WA(43)`**  
**`constexpr Verdict PE(43)`**  
**`constexpr Verdict FAIL(1)`**  

**`constexpr std::string_view LETTER("a..zA..Z")`**  
**`constexpr std::string_view LOWER("a..z")`**  
**`constexpr std::string_view UPPER("A..Z")`**  
**`constexpr std::string_view VOWEL("aeiouAEIOU")`**  
**`constexpr std::string_view LOWER_VOWELS("aeiou")`**  
**`constexpr std::string_view UPPER_VOWELS("AEIOU")`**  
**`constexpr std::string_view CONSONANT("b..zB..z")`**  
**`constexpr std::string_view LOWER_CONSONANT("b..z")`**  
**`constexpr std::string_view UPPER_CONSONANT("B..Z")`**  
**`constexpr std::string_view ALPHA_NUMERIC("0..9a..zA..Z")`**  
**`constexpr std::string_view LOWER_ALPHA_NUMERIC("0..9a..z")`**  
**`constexpr std::string_view UPPER_ALPHA_NUMERIC("0..9A..Z")`**  
**`constexpr std::string_view DIGITS("0..9")`**  


## namespace InputValidator
Use this namespace if you want to write an input validator which gets called as `./validator [arguments] < inputfile`.

#### Member
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
Both `spaceSensitive` and `caseSensitive` will be set to true and the `testIn` is initialized accordingly.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating point numbers are equal with absolute and relative error less than some tolerance.
For relative errors the `expected` value is used as refference value.
If no tolerance is given the global setting `floatAbsTol` respectively `floatRelTol` get used as fallback.

**`bool stringEqual(std::string_view a, std::string_view b)`**  
**`bool stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.
If `caseSensitive` is not provided the global setting `caseSensitive` gets used as fallback.


## namespace OutputValidator
Use this namespace if you want to write an output validator which gets called as `./validator input judgeanswer feedbackdir [arguments < teamoutput`.

#### Member
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
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
You can set `spaceSensitive` and `caseSensitive` before this call, by default both are `false`.
After this call all streams will be initialized with `caseSensitive` and the `teamAns` will also be `spaceSensitive`.

**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating point numbers are equal with absolute and relative error less than some tolerance.
For relative errors the `expected` value is used as refference value.
If no tolerance is given the global setting `floatAbsTol` respectively `floatRelTol` get used as fallback.

**`bool stringEqual(std::string_view a, std::string_view b)`**  
**`bool stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.
If `caseSensitive` is not provided the global setting `caseSensitive` gets used as fallback.


## namespace Interactor
Use this namespace if you want to write an output validator for interactive problems which gets called as `./interactor input judgeanswer feedbackdir [arguments] <> team`.

#### Member
**`OutputStream juryOut`**  
**`OutputStream juryErr`**  
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
You can set `spaceSensitive` and `caseSensitive` before this call, by default both are `false`.
After this call all streams will be initialized with `caseSensitive` and the `fromTeam` will also be `spaceSensitive`.


**`bool floatEqual(Real given, Real expected)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol)`**  
**`bool floatEqual(Real given, Real expected, Real floatAbsTol, Real floatRelTol)`**  
Checks if two floating point numbers are equal with absolute and relative error less than some tolerance.
For relative errors the `expected` value is used as refference value.
If no tolerance is given the global setting `floatAbsTol` respectively `floatRelTol` get used as fallback.

**`bool stringEqual(std::string_view a, std::string_view b)`**  
**`bool stringEqual(std::string_view a, std::string_view b, bool caseSensitive)`**  
Checks if two strings are equal but may ignore the case of letters if `caseSensitive` is false.
If `caseSensitive` is not provided the global setting `caseSensitive` gets used as fallback.


## namespace Generator
Use this namespace if you want to write an generator for testcases which gets called as `./generator [arguments]`.

#### Member
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
> Note: the parsed commands should start with `--`.

#### Methods
**`Command operator[](std::string_view command) const`**  
Searches for the `command` and returns it will all its parameters (until the next command starting with `--`).
If the `command` does not exists the list of parameters will be empty.

**`Command getRaw(std::string_view command) const`**  
Searches for the `command` and returns it will all following parameters.
If the `command` does not exists the list of parameters will be empty.

**`Command getRaw() const`**  
Returns alls parameters.

## class Command
Gets returned by the CommandParser and corresponds to an command in `argv`.
#### Methods
**`bool exists() const`**  
**`explicit operator bool() const`**  
Will be true if the commandParser found this command.

**`Integer parameterCount() const`**  
Counts the number of parameters the command can access.
Will be zero if the command was not found.

**`std::string asString() const`**  
**`std::string asString(std::string_view defaultValue) const`**  
**`Integer asInteger() const`**  
**`Integer asInteger(Integer defaultValue) const`**  
**`Real asReal() const`**  
**`Real asReal(Real defaultValue) const`**  
Parses the first token after the command.
If the command does not exists or has no parameter it will instead return the `defaultValue`.
>Note: If `defaultValue` should be returned but was not provided an error will be thrown.

**`std::vector<std::string> asStrings() const`**  
**`std::vector<Integer> asIntegers() const`**  
**`std::vector<Real> asReals() const`**  
Return all parameters of this command.

**`Paramater operator[](Integer i) const`**  
Return the `i`th parameter.
The first has index 0.

## class Paramater
Gets returned by the Command and corresponds to an parameter of an command.
#### Methods
**`bool exists() const`**  
**`explicit operator bool() const`**  
Will be true if the parameter exists.

**`std::string asString() const`**  
**`std::string asString(std::string_view defaultValue) const`**  
**`Integer asInteger() const`**  
**`Integer asInteger(Integer defaultValue) const`**  
**`Real asReal() const`**  
**`Real asReal(Real defaultValue) const`**  
Parses the parameter.
If the parameter does not exists it will instead return the `defaultValue`.
>Note: If `defaultValue` should be returned but was not provided an error will be thrown.


## class OutputStream
This is just a wrapper for an `std::ostream` which allows changing the underlying output stream at any time.


## class InputStream
This is a wrapper for an `std::istream` which allows safely parsing input as tokens.
If the input does not match the requested token the program will terminate with an appropriate exit code (WA for team input and FAIL else).
> Note: If the stream is case insensitive all characters read are converted to lower case.

#### Methods
**`void eof()`**  
**`void space()`**  
**`void newline()`**  
Checks if the next char in the input is the corresponding whitespace.
This only hase an effect for space sensitive streams i.e. input validation.

**`void expectString(std::string_view expected)`**  
**`void expectInt(Integer expected)`**  
**`void expectReal(Real expected)`**  
Checks if the next token matches `expected`.
If a `Real` is expected equallity is checked wit absolute and relative error.
This is mainly supposed to verify team output.

**`std::string string()`**  
**`std::string string(Integer lower, Integer upper)`**  
**`std::string string(Integer lower, Integer upper, Constraint& constraint)`**  
**`std::string string(std::regex pattern)`**  
**`std::string string(std::regex pattern, Integer lower, Integer upper)`**  
**`std::string string(std::regex pattern, Integer lower, Integer upper, Constraint& constraint)`**  
Will extract the next token.
If `lower` or `upper` is provided it will be checked that the token length is in [lower, upper).
If a `pattern` is provided it will be checked that the token matches the pattern.
If an constraint is provided it will be updated with the length of the token.

**`Integer integer()`**  
**`Integer integer(Integer lower, Integer upper)`**  
**`Integer integer(Integer lower, Integer upper, Constraint& constraint)`**  
Will extract the next token as an `Integer`.
If `lower` or `upper` is provided it will be checked that the integer is in [lower, upper).
If an constraint is provided it will be updated with the value of the parsed integer.

// this does not allow NaN or Inf!
**`Real real()`**  
Will extract the next token as a `Real`.
Use this method to parse team input as `Real`!.
> Note: this method does not allow to read `NaN` or `Inf`.

**`Real real(Real lower, Real upper)`**  
**`Real real(Real lower, Real upper, Constraint& constraint)`**  
**`Real real(Real lower, Real upper, Integer minDecimals, Integer maxDecimals)`**  
**`Real real(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint)`**  
Will extract the next token as a `Real`.
This is supposed to be yoused for validators only, and the last two methods are preferred.
If `lower` or `upper` is provided it will be checked that the real is in [lower, upper), the check does **not** use any `eps` as tolerance.
If `minDecimals` or `maxDecimals` is provided it will be checked that the token was given with the number of decimal places in [minDecimals, maxDecimals).
If an constraint is provided it will be updated with the value of the parsed integer.

**`std::vector<std::string> strings([args], Integer count)`**  
**`std::vector<std::string> strings([args], Integer count, char separator)`**  
**`std::vector<Integer> integers([args], Integer count)`**  
**`std::vector<Integer> integers([args], Integer count, char separator)`**  
**`std::vector<Real> reals([args], Integer count)`**  
**`std::vector<Real> reals([args], Integer count, char separator)`**  
Will extract the next `count` tokens by calling `string([args])`, `integer([args])` or `real([args])`.
The tokens are expected to be separated by the char `separator` which must be `space` or `newline`.
If this parameter is not provided a `space` is expected.


## namespace Random
This namespace contains functions to generate random numbers.
> Note: do not use c++ random functions since there output is compiler dependent.

#### Methods
**`void seed(UInteger seed)`**  
Seeds the internal random number generator.

##### Numbers
**`bool bit()`**  
Returns a uniformally distributed bit.

**`Integer integer()`**  
**`Integer integer(Integer upper)`**  
**`Integer integer(Integer lower, Integer upper)`**  
Returns a uniformally distributed integer in [-2^63, 2^63), [0, upper) or [lower, upper).

**`Real real()`**  
**`Real real(Real upper)`**  
**`Real real(Real lower, Real upper)`**  
Returns a uniformally distributed real in [0, 1), [0, upper) or [lower, upper).

**`Real normal(Real mean, Real stddev)`**  
**`Real normal(Real lower, Real upper, Real mean, Real stddev)`**  
**`Real exponential(Real lambda)`**  
**`Real exponential(Real lower, Real upper, Real lambda)`**  
**`Integer geometric(Real p)`**  
**`Integer geometric(Integer lower, Integer upper, Real p)`**  
**`Integer binomial(Integer n, Real p)`**   
**`Integer binomial(Integer lower, Integer upper, Integer n, Real p)`**   
Generates a number according to some distribution. The generated integer will always be in [lower, upper).

**`Integer prime(Integer upper)`**  
**`Integer prime(Integer lower, Integer upper)`** 
Generates a unirformally choosen prime in the [0, upper) or [lower, upper).

##### Seqeunces
**`std::vector<Integer> distinct(Integer count, Integer upper)`**  
**`std::vector<Integer> distinct(Integer count, Integer lower, Integer upper)`**  
Generates `count` different integers in [0, upper) respectively [lower, upper).
Each possible output has the same probability to get generated.

**`std::vector<Integer> perm(Integer count)`**  
**`std::vector<Integer> perm(Integer count, Integer offset)`**  
Generates a permutation of the numbers [0, count) respectively [offset, offset+count).
Each possible output has the same probability to get generated.

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
Generates `count` different integers in [0, upper) respectively [lower, upper).
The sequences generated by the first two methods is equal to calling `Random::integer()` `count` times.
All other methods will generate a sequence which fits the name of the generating mehtod.
Each possible output has the same probability to get generated.

**`std::vector<Integer> partition(Integer n, Integer k)`**  
**`std::vector<Integer> partition(Integer n, Integer k, Integer min)`**  
Uniformally generates an unsorted partition of `n` into `k` integers in [1, n) or [min, n).
Note that with non positive `min` the distribution is not uniform.

**`std::string bracketSequence(Integer n)`**  
Uniformally generates a bracket sequence of length 2n.

**`std::vector<std::pair<Integer, Integer>> convex(Integer n, Integer dim)`**  
**`std::vector<Point> convex(Integer n, Integer dim)`**  
Generates `n` points forming a convex polygon with points in (-dim,dim)<sup>2</sup> (ther polygon will always fill the width and height).
The points will be in counter clockwise order.
Note that points may be collinear.

##### utility
**`const RandomIt::value_type& select(RandomIt first, RandomIt last)`**  
**`C::value_type select(C c)`**  
**`T select(T(c)[N])`**  
**`T select(std::pair<T, T> t)`**  
**`T select(std::complex<T> t)`**  
Uniformally select a value from a range, complete container, initializer list, pair or complex.

**`void shuffle(RandomIt first, RandomIt last)`**  
**`void shuffle(C& c)`**  
**`void shuffle(std::pair<T, T>& t)`**  
**`void shuffle(std::complex<T>& t)`**  
Uniformally shuffle a range, complete container, pair or complex.


## Math functions
**`constexpr Integer applyMod(Integer x, Integer mod)`**  
Calculates `x` modulo `mod` in a mathematical sense i.e. the result will be in the range [0, mod).

**`constexpr Integer mulMod(Integer lhs, Integer rhs, Integer mod)`**  
**`constexpr Integer powMod(Integer base, Integer exp, Integer mod)`** 
Calculate `(lhs * rhs) % mod` or `(lhs ^ rhs) % mod` without overflow.

**`constexpr Integer multInv(Integer n, Integer mod)`**  
Calculates the multiplicative inverse of `n` modulo `mod`.
> Note: if the multiplicative inverse does not exists the method will return `-1`.

**`constexpr bool isPrime(Integer n)`**  
Checks if a given number `n` is prime.

**`std::vector<Integer> primes(Integer upper)`**  
**`std::vector<Integer> primes(Integer lower, Integer upper)`**  
Returns all primes in [0, upper) or [lower, upper).

**`constexpr Integer sign(Integer x)`**  
Determines the sign of `x` as {-1,0,1}.


## Utility functions
**`bool isPerm(C c)`**  
**`bool isPerm(RandomIt first, RandomIt last)`**  
**`bool isPerm(C c, C::value_type offset)`**  
**`bool isPerm(RandomIt first, RandomIt last, RandomIt::value_type offset)`**  
Checks if `c` respectively [first, last) is an permutation of [0, n) respectively [offset, offset+n), where n is the distance between first and last.
> Note: this function is only defined for integer ranges.

**`bool isPerm(C1 c1, C2, c2)`**  
**`bool isPerm(itA firstA, itA lastA, itB firstB, itB lastB)`**  
Checks if `c1` respectively [firstA, lastA) is an permutation of `c2` respectively [firstB, lastB)

**`constexpr bool anyAdjacent(C c, BinaryPredicate p)`**  
**`constexpr bool anyAdjacent(C c, BinaryPredicate p)`**  
**`constexpr bool noneAdjacent(C c, BinaryPredicate p)`**  
**`constexpr bool allAdjacent(C c, BinaryPredicate p)`**  
**`constexpr bool anyAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
**`constexpr bool noneAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
**`constexpr bool allAdjacent(RandomIt first, RandomIt last, BinaryPredicate p)`**  
Checks if the predicate `p` evaluates to true for any, none or all adjacent entries in the container `c` respectively the range [first, last).

**`constexpr bool areIncreasing(C c)`**  
**`constexpr bool areNonDecreasing(C c)`**  
**`constexpr bool areDecreasing(C c)`**  
**`constexpr bool areNonIncreasing(C c)`**  
**`constexpr bool areDistinct(C c)`**  
**`constexpr bool areIncreasing(RandomIt first, RandomIt last)`**  
**`constexpr bool areNonDecreasing(RandomIt first, RandomIt last)`**  
**`constexpr bool areDecreasing(RandomIt first, RandomIt last)`**  
**`constexpr bool areNonIncreasing(RandomIt first, RandomIt last)`**  
**`constexpr bool areDistinct(RandomIt first, RandomIt last)`**  
Checks if the container `c` respectively the range [first, last) is ordered according to the name of this function.

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
Generates an object which can be printed with an `std::ostream`.
All elements in `c`, `in`, [first, last) or all but the last element in `cstring` will be printed and separated by `separator`.
Note that a for a `cstring` it is expected that the last value is the null character `\0`.
If `separator` is not specified a `space` is used.
If the container is given as rvalue the generated object stores all values, else it just stores an reference.

**`constexpr bool isLower(char c)`**  
**`constexpr bool isUpper(char c)`**  
**`constexpr bool isLetter(char c)`**  
**`constexpr bool isDigit(char c)`**  
**`constexpr bool isVowel(char c)`**  
**`constexpr bool isConsonant(char c)`**  
**`constexpr bool isLower(std::string_view s)`**  
**`constexpr bool isUpper(std::string_view s)`**  
**`constexpr bool isLetter(std::string_view s)`**  
**`constexpr bool isDigit(std::string_view s)`**  
**`constexpr bool isVowel(std::string_view s)`**  
**`constexpr bool isConsonant(std::string_view s)`**  
Checks if a given char (all chars) belong to the alphabet fitting the name of the function.

**`constexpr char toLower(char c)`**  
**`constexpr char toUpper(char c)`**  
**`constexpr char toDefaultCase(char c)`**  
**`void toLower(std::string& s)`**  
**`void toUpper(std::string& s)`**  
**`void toDefaultCase(std::string& s)`**  
Converts a `char` or `std::string` to the given case.
> Note: defaultCase is lowerCase.

## class ConstraintsLogger 
An instance of this class is provided for input validators as `constraint` after calling `init(argc, argv).
#### Methods
**`Constraint& operator[](const std::string& name)`**  
Returns a `constraint` which can be given to an input stream to automatically log informations about the parsed input.
See [BAPCtools](https://github.com/RagnarGrootKoerkamp/BAPCtools/blob/master/doc/implementation_notes.md#constraints-checking).
