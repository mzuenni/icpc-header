# validate.h
The `validate.h` headers has some options that can be set at compiletime and some that can be set at runtime to customize behaviour without the need to modify the header.

## Compiletime
There is a namespace `Settings` which contains some options which will be visible inside the global namespace.
You can "override" them in the namespace `Settings` as follows.
> Note that all settings have to be set before the include of `validate.h`!

#### Settings
**`RandomEngine = std::mt19937_64;`**  
**`constexpr Integer LARGE = 0x3FFF'FFFF'FFFF'FFFF;`**  
**`constexpr bool DEFAULT_CASE_LOWER = true;`**  
**`constexpr int DEFAULT_PRECISION = 6;`**  
**`constexpr Real DEFAULT_EPS = 1e-6_real;`**  
**`void exitVerdict(int exit_code) = std::exit(exit_code);`**  

#### Example
```
// open the namespace to override settins
namespace Settings {

	// override the default output precision for floating point values
	constexpr int DEFAULT_PRECISION = 4;

	// use a different random number engine internally
	using RandomEngine = AES_RNG;

	// override the default exit method
	// this may be useful for validators with multiple threads
	[[noreturn]] void exitVerdict(int exit_code) {
		std::quick_exit(exit_code);
	}
}
// after we set the settings we can include validate.h
#include "validate.h"
```

## Runtime
The namespaces `InputValidator`, `OutputValidator`, `Interactor` and `Generator` contain shared global constants of type `Setting<T>`.
These settings can be adjusted before the call to `init(argc, argv)`.
> Note that the setting encoded in `argv` have priority and will override any manual setting assigned before!
The `caseSensitive` setting has an additional funtion `regex()` to create a (depneding on the value of the setting) case (in)sensitive regex.

#### Settings
**`Setting<Real> floatAbsTol(DEFAULT_EPS);`**  
**`Setting<Real> floatRelTol(DEFAULT_EPS);`**  
**`Setting<bool> spaceSensitive(false);`**  
**`Setting<bool> caseSensitive(false);`**  

#### Example
```
int main(int argc, char **argv) {
	// require less precision
	floatAbsTol = 1e-5_real;
	floatRelTol = 1e-5_real;

	// parse team input case sensitive
	caseSensitive = true;

	// parse argv
	InputValidator::init(argc, argv);
}
```
