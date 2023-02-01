# ICPC-Header
These headers can be used to write validators/generators and visualizers for ICPC style contests and are regularly used at the German Collegiate Programming Contest (GCPC) and the Northwestern Europe Regional Contest (NWERC).
They are intended to safely parse team ouput, strictly check sample output, and generate random numbers in a reproducible fashion independent of the compiler.
Unlike [testlib](https://github.com/MikeMirzayanov/testlib) this library is directly compatible with the [ICPC Problem Format](https://icpc.io/problem-package-format/) and can therefore be used with tools like [Domjudge](https://www.domjudge.org/), [Problemtools](https://github.com/Kattis/problemtools) or [BAPCtools](https://github.com/RagnarGrootKoerkamp/BAPCtools).

The C++ code for validators is compatible with ``C++17`` and later, and Generator and Visualizer code is compatible with ``C++20``.
Note that the later do not need to run on the judge.
The code is tested against ``g++`` and ``clang`` but should be standard conform and compile everywhere else too.

## Generell Design
This is a list of general design/usage ideas baked into these headers.
- Do not use various integer/floating point types. The header defines the types ``Integer`` and ``Real`` which should be sufficient for every integer/floating point value.
- Include one namespace of ``InputValidator``, ``OutputValidator``, ``Interactor``, or ``Generator`` depending on the type of program you are writing.
- Call the ``init(int argc, char** argv)`` method included in the namespace at the start of the main function.
- Do not use ``cin``, ``cout``, and ``cerr``.
Use the Input and Output streams defined in the namespace which have a more readable name and more functionality, i.e. you can directly print vectors etc.
- Use the namespace ``Random`` for random number generation.
Note that unlike c++ random methods, all methods in this header use half open ranges.
- Do not call ``exit()`` or ``return`` in the main method to terminate the program.
Use the verdict class instead, which you can use like this:
  - ``AC();``
  - ``return AC;``
  - ``juryOut << "This is accepted" << AC;``

## Samples
### Input Validator
```c++
#include "validate.h"

int main(int argc, char **argv) {
    InputValidator::init(argc, argv);    // initialize streams, rng and parse arguments
    using namespace InputValidator;

    std::string token;

    token = testIn.string();             // read the next token
    testIn.space();                      // ensure that tokens a separated by space
    token = testIn.string(5, 7);         // read the next token with length in [5,7)
    testIn.newline();                    // ensure newline after second token
    testIn.integer(5, 7);                // read an intger in [5,7)
    testIn.newline();
    testIn.eof();                        // read end of file

    // more parse mathods available in InputStream

    // write a message as feedback
    if (false) juryOut << "Oh no!" << WA;
    else juryOut << "Yay!" << AC;

    // this is never reached since we printed an AC/WA verdict which exits the program!
    // return 0;
}

```

### Generator
```c++
#include "validate.h"

int main(int argc, char **argv) {
    Generator::init(argc, argv);			// initialize streams, rng and parse arguments
    using namespace Generator;

    // Integer is a 64bit signed Integer
    // Real is a long double
    Integer n = arguments["--n"].asInteger(13); // parse the comandline argument "--n <x>" <x> as integer. If "--n" is not provided return 13.

    std::vector<Integer> data = Random::distinct(n, 0, 10); // generate n distinct integers in [0, 10)
    // more method available in namespace Random

    testOut << n << std::endl;
    testOut << join(data.begin(), data.end()) << std::endl; // print the numbers;
}

```

More examples can be found in [example](example/).

## Further reading
Here is a list of features you may find useful:
- The header provides many string constants like ``LOWER_VOWELS=aeiou`` which might be useful in combination with ``Random::select()``.
- The header allows printing vectors with spaces as a separate, if you want to use newlines instead you can write ``testOut << join(<vec>, '\n') << std::endl`` or ``testOut << join(<vec>, NEWLINE) << std::endl``, see the *join* method.
- Many functions to verify sequences are implemented, like ``isPerm()``, ``areNonDecreasing()`` or ``isLower(std::string)``, see more under *Utility*.
- Some math methods which occur often are implemented, like ``applyMod()``, ``powMod()`` or ``isPrime()``, see more under *Math*.
- The random namespace has many methods which can create numbers according to various distributions like ``integer()``, ``real()``, ``maximum()`` or ``prime()`` but also sequences, like ``distinct()``, ``perm()`` or ``partition()``, see under *Random*.
- The header allows parsing command line arguments with the global object ``arguments``, see *arg parser*.
- The InputStream class allows to safely and strictly parse and validate team and sample output, see *InputStream*.

The code is completely documented in [doc](doc/), but many features can be seen in action in [example](example/).

If you have ideas for more useful features, feel free to open an issue.
