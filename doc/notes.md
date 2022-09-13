# Notes
Some generel notes:
- Initialice the library by calling `InputValidator::init(argc, argv)` etc.
- Use the types `Integer` and `Real` for integer/floating point values
- Never use c++ random facilities, use the methods inside the namespace `random`
- Any method that takes a range (of numbers or iterators) take an half open range [a, b)
- Use `isLower` etc. instead of the `std::islower` functions adopted from c
- You can print containers with the help of the `join` function
- You can exit the program with a verdict by printing it (`operator<<`) to an output stream
- Use the `arguments` object from `InputValidator` etc. to access `argv` parameters
