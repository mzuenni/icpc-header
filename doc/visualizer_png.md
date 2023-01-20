# visiualizer_png.h
A simple library to draw png images.


## Constants and Types
**`Integer = std::int64_t`**  
**`Real = long double`**  

**`Color BLACK(0x000000)`**  
**`Color DARKGRAY(0x404040)`**  
**`Color GRAY(0x808080)`**  
**`Color LIGHTGRAY(0xC0C0C0)`**  
**`Color WHITE(0xFFFFFF)`**  
**`Color TRANSPARENT(0x000000, 0x00)`**  

**`Color RED(0xFF0000)`**  
**`Color GREEN(0x00FF00)`**  
**`Color BLUE(0x0000FF)`**  

**`Color YELLOW(0xFFFF00)`**  
**`Color MAGENTA(0xFF00FF)`**  
**`Color CYAN(0x00FFFF)`**  


## namespace Visualizer
#### Member
**`PNG image`**  
**`std::ifstream testIn`**  
**`std::ifstream testAns`**  

#### Methods
**`init(int argc, char** argv)`**  
Initializes the library by parsing `argv` and initializing:
- 'Visualizer::image'
- 'Visualizer::testIn'
- 'Visualizer::testAns'


## class Color
#### Member
**`std::uint8_t r`**  
**`std::uint8_t g`**  
**`std::uint8_t b`**  
**`std::uint8_t a`**  

#### Methods
**`constexpr Color()`**  
**`constexpr Color(std::uint32_t rgb)`**  
**`constexpr Color(std::uint32_t rgb, std::uint8_t a)`**  
**`constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)`**  
**`constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)`**  
Constructor, creates a color with 8 bit red, green and blue parts. If no alpha value
is provided it is set to 0xFF.
If a single `std::uint32_t` is given only the least significant 24 bits are used.
If a single `std::uint8_t` is given all parts are the same resulting in a gray color.

**`constexpr std::uint8_t& operator[](std::size_t i)`** 
**`constexpr std::uint8_t operator[](std::size_t i) const`**  
Gets the red(`i=0`), green(`i=1`), blue(`i=2`) or alpha(`i=3`) part of the color.

**`constexpr Color operator*(Real f) const`**  
Multiplies each color part with `f` i.e. fades the color to black.

**`constexpr Color operator+(Color x) const`**  
**`constexpr Color& operator+=(Color x)`**  
Adds each color part separately and clamps the result.

**`constexpr Color operator-(Color x) const`**  
**`constexpr Color& operator-=(Color x)`**  
Subtracts each color part separately and clamps the result.

**`constexpr bool operator==(Color x)`**  
**`constexpr bool operator!=(Color x)`**  
Checks if two points are equal or not.


#### Related (non class) Methods
**`constexpr Color lerp(Color a, Color b, Real f)`**  
Interpolates the two colors `a` and `b`.


## class PNG
> NOTE: It is required that any pixel is inside the image.
> No operation checks if this is the case.
#### Methods
**`PNG()`**  
**`PNG(std::string fileName)`**  
The constructor.
Optinally takes a file name to which the image will be written automatically when destructed.

**`clear(Integer w, Integer h)`**  
**`clear(Integer w, Integer h, Color fill)`**  
Resizes and clears the image data.
If no fill color is provided the image will be transparent.

**`Color& operator()(Integer x, Integer y)`** 
**`Color operator()(Integer x, Integer y) const`**  
Accesses the pixel at (x, y).

**`Integer width() const`**  
**`Integer height() const`**  
Getter for the width/height of the image.


**`void fillRectangle(const Point& a, const Point& b, const Color& fill)`**  
**`void fillRectangle(const Point& a, const Point& b, const Color border, const Color& fill)`**  
**`void fillRectangle<BlendMode>(const Point& a, const Point& b, const Color& fill)`**  
**`void fillRectangle<BlendMode>(const Point& a, const Point& b, const Color border, const Color& fill)`**  
Fills the rectangle where `a` and `b` are at opposite corners.
Both `a` and `b` will be on the border.
If no extra color for the border is provided it will be filled with the same color as the content.
If no blend mode is proived pixels will be overwritten i.e. BlendMode::ASSIGN is used.

**`void floodFill(const Point& a, const Color& fill)`**  
**`void floodFill<BlendMode>(const Point& a, const Color& fill)`**  
Changes the color at position `a` and also all pixel connected to `a` with the same color as pixel `a`.
Two pixels are neighboured if they are next to each other i.e. share a side.
If no blend mode is proived pixels will be overwritten i.e. BlendMode::ASSIGN is used.

**`void drawLine(const Point& a, const Point& b, const Color& fill)`**  
**`void drawLine<BlendMode>(const Point& a, const Point& b, const Color& fill)`**  
Draws a line from `a` to `b`.
If no blend mode is proived pixels will be overwritten i.e. BlendMode::ASSIGN is used.


## class Point
#### Member
**`Integer x`**  
**`Integer y`**  

#### Methods
**`constexpr Point(Integer x, Integer y)`**  
The constructor.
Takes two reals as `x` and `y` coordinate.

**`constexpr bool operator==(Point x)`**  
**`constexpr bool operator!=(Point x)`**  
Checks if two points are equal or not.
