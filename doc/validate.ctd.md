# validate.h
A simple library to validate team input, and generate random data.


## namespace ctd
This namespcase contains method to write validators in a ctd like syntax.

#### Member
**`constexpr auto SPACE`**  
**`constexpr auto NEWLINE`**  
**`constexpr auto ENDFILE`**  

#### Methods
**`constexpr auto INT()`**  
**`constexpr auto INT(Integer lower, Integer upper)`**  
**`auto INT(Integer lower, Integer upper, Constraint& constraint)`**  
**`auto INT(Integer& res)`**  
**`auto INT(Integer lower, Integer upper, Integer& res)`**  
**`auto INT(Integer lower, Integer upper, Integer& res, Constraint& constraint)`**  

**`constexpr auto REAL()`**  
**`constexpr auto REAL(Real lower, Real upper)`**  
**`constexpr auto REAL(Real lower, Real upper, Integer minDecimals, Integer maxDecimals)`**  
**`auto REAL(Real lower, Real upper, Constraint& constraint)`**  
**`auto REAL(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint)`**  
**`auto REAL(Real& res)`**  
**`auto REAL(Real lower, Real upper, Real& res)`**  
**`auto REAL(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Real& res)`**  
**`auto REAL(Real lower, Real upper, Real& res, Constraint& constraint)`**  
**`auto REAL(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Real& res, Constraint& constraint)`**  

**`constexpr auto STRING()`**  
**`constexpr auto STRING(Integer lower, Integer upper)`**  
**`auto STRING(Integer lower, Integer upper, Constraint& constraint)`**  
**`auto STRING(const std::regex& pattern)`**  
**`auto STRING(const std::regex& pattern, Integer lower, Integer upper)`**  
**`auto STRING(const std::regex& pattern, Integer lower, Integer upper, Constraint& constraint)`**  
**`auto STRING(std::string& res)`**  
**`auto STRING(Integer lower, Integer upper, std::string& res)`**  
**`auto STRING(Integer lower, Integer upper, std::string& res, Constraint& constraint)`**  
**`auto STRING(const std::regex& pattern, std::string& res)`**  
**`auto STRING(const std::regex& pattern, Integer lower, Integer upper, std::string& res)`**  
**`auto STRING(const std::regex& pattern, Integer lower, Integer upper, std::string& res, Constraint& constraint)`**  

**`constexpr auto INTS(Integer count)`**  
**`constexpr auto INTS(Integer count, char separator)`**  
**`constexpr auto INTS(Integer lower, Integer upper, Integer count)`**  
**`constexpr auto INTS(Integer lower, Integer upper, Integer count, char separator)`**  
**`auto INTS(Integer lower, Integer upper, Constraint& constraint, Integer count)`**  
**`auto INTS(Integer lower, Integer upper, Constraint& constraint, Integer count, char separator)`**  
**`auto INTS(std::vector<Integer>& res, Integer count)`**  
**`auto INTS(std::vector<Integer>& res, Integer count, char separator)`**  
**`auto INTS(Integer lower, Integer upper, std::vector<Integer>& res, Integer count)`**  
**`auto INTS(Integer lower, Integer upper, std::vector<Integer>& res, Integer count, char separator)`**  
**`auto INTS(Integer lower, Integer upper, std::vector<Integer>& res, Constraint& constraint, Integer count)`**  
**`auto INTS(Integer lower, Integer upper, std::vector<Integer>& res, Constraint& constraint, Integer count, char separator)`**  

**`constexpr auto REALS(Integer count)`**  
**`constexpr auto REALS(Integer count, char separator)`**  
**`constexpr auto REALS(Real lower, Real upper, Integer count)`**  
**`constexpr auto REALS(Real lower, Real upper, Integer count, char separator)`**  
**`constexpr auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Integer count)`**  
**`constexpr auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, Constraint& constraint, Integer count)`**  
**`auto REALS(Real lower, Real upper, Constraint& constraint, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint, Integer count)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, Constraint& constraint, Integer count, char separator)`**  
**`auto REALS(std::vector<Real>& res, Integer count)`**  
**`auto REALS(std::vector<Real>& res, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, std::vector<Real>& res, Integer count)`**  
**`auto REALS(Real lower, Real upper, std::vector<Real>& res, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, std::vector<Real>& res, Integer count)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, std::vector<Real>& res, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, std::vector<Real>& res, Constraint& constraint, Integer count)`**  
**`auto REALS(Real lower, Real upper, std::vector<Real>& res, Constraint& constraint, Integer count, char separator)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, std::vector<Real>& res, Constraint& constraint, Integer count)`**  
**`auto REALS(Real lower, Real upper, Integer minDecimals, Integer maxDecimals, std::vector<Real>& res, Constraint& constraint, Integer count, char separator)`**  

**`constexpr auto STRINGS(Integer count)`**  
**`constexpr auto STRINGS(Integer count, char separator)`**  
**`constexpr auto STRINGS(Integer lower, Integer upper, Integer count)`**  
**`constexpr auto STRINGS(Integer lower, Integer upper, Integer count, char separator)`**  
**`auto STRINGS(Integer lower, Integer upper, Constraint& constraint, Integer count)`**  
**`auto STRINGS(Integer lower, Integer upper, Constraint& constraint, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, Constraint& constraint, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, Constraint& constraint, Integer count, char separator)`**  
**`auto STRINGS(std::vector<std::string>& res, Integer count)`**  
**`auto STRINGS(std::vector<std::string>& res, Integer count, char separator)`**  
**`auto STRINGS(Integer lower, Integer upper, std::vector<std::string>& res,  Integer count)`**  
**`auto STRINGS(Integer lower, Integer upper, std::vector<std::string>& res,  Integer count, char separator)`**  
**`auto STRINGS(Integer lower, Integer upper, std::vector<std::string>& res, Constraint& constraint, Integer count)`**  
**`auto STRINGS(Integer lower, Integer upper, std::vector<std::string>& res, Constraint& constraint, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, std::vector<std::string>& res, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, std::vector<std::string>& res, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, std::vector<std::string>& res, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, std::vector<std::string>& res, Integer count, char separator)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, std::vector<std::string>& res, Constraint& constraint, Integer count)`**  
**`auto STRINGS(const std::regex& pattern, Integer lower, Integer upper, std::vector<std::string>& res, Constraint& constraint, Integer count, char separator)`**  
