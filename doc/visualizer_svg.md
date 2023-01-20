# visiualizer_svg.h
A simple library to draw svg images.


## Constants and Types
**`Integer = std::int64_t`**  
**`Real = long double`**  

**`Color BLACK(0x00)`**  
**`Color DARKGRAY(0x40)`**  
**`Color GRAY(0x80)`**  
**`Color LIGHTGRAY(0xC0)`**  
**`Color WHITE(0xFF)`**  

**`Color RED(0xFF,0x00,0x00)`**  
**`Color GREEN(0x00,0xFF,0x00)`**  
**`Color BLUE(0x00,0x00,0xFF)`**  

**`Color YELLOW(0xFF,0xFF,0x00)`**  
**`Color MAGENTA(0xFF,0x00,0xFF)`**  
**`Color CYAN(0x00,0xFF,0xFF)`**  


## namespace Visualizer
#### Member
**`SVG image`**  
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

#### Methods
**`constexpr Color()`**  
**`constexpr Color(std::uint32_t rgb)`**  
**`constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)`**  
**`constexpr Color(std::uint8_t gray)`**  
Constructor, creates a color with 8 bit red, green and blue parts.
If a single `std::uint32_t` is given only the least significant 24 bits are used.
If a single `std::uint8_t` is given all parts are the same resulting in a gray color.

**`constexpr std::uint8_t& operator[](std::size_t i)`** 
**`constexpr std::uint8_t operator[](std::size_t i) const`**  
Gets the red(`i=0`), green(`i=1`) or blue(`i=2`) part of the color.

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


## class SVG
> All object can be written to the image with the shift output operator `<<`.
> The bounding box of the image will automatically increased to contail the given object.
#### Member
**`Point origin`**  
**`Dimension dimension`**  

#### Methods
**`SVG()`**  
**`SVG(std::string fileName)`**  
The constructor.
Optinally takes a file name to which the image will be written automatically when destructed.

**`includeInBB(Point x)`**  
Change the bounding box such that the given point `x` is included.
The origin `(0, 0)` is always included.


## class Rectangle
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`Point origin`**  
**`Dimension dimension`**  

#### Methods
**`constexpr Rectangle(Point o, Dimension d)`**  
**`constexpr Rectangle(Point o, Real w, Real h)`**  
**`constexpr Rectangle(Real x, Real y, Dimension d)`**  
**`constexpr Rectangle(Real x, Real y, Real w, Real h)`**  
The constructor.
Creates a rectange with given `origin` an `width`, which can be given as `Point`, `Dimension` or `Reals`.


## class Circle
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`Point origin`**  
**`Real r`**  

#### Methods
**`constexpr Circle(Point o, Real r)`**  
**`constexpr Circle(Real x, Real y, Real r)`**  
The constructor.
Creates a circle with given `origin` an `radius`, which can be given as `Point` or `Reals`.


## class Ellipse
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`Point origin`**  
**`Real rx`**  
**`Real ry`**  

#### Methods
**`constexpr Ellipse(Point o, Real rx, Real ry)`**  
**`constexpr Ellipse(Real x, Real y, Real rx, Real ry)`**  
The constructor.
Creates a ellipse with given `origin` an `radii`, which can be given as `Point` or `Reals`.


## class Line
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`Point p1`**  
**`Point p2`**  

#### Methods
**`constexpr Line()`**  
**`constexpr Line(Point p1, Point p2)`**  
**`constexpr Line(Real x1, Real y1, Real x2, Real y2)`**  
The constructor.
Creates a linesegment with given start and end points, which can be given as `Point` or `Reals`.


## class Polygon
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`std::vector<Point> points`**  

#### Methods
**`Polygon()`**  
**`Polygon(std::initializer_list<Point> points)`**  
**`Polygon(std::vector<Point> points)`**  
The constructor.
Creates a polygon from points.


## class Polyline
#### Member
**`std::optional<Stroke> stroke`**  
**`std::optional<Fill> fill`**  
**`std::vector<Point> points`**  

#### Methods
**`Polyline()`**  
**`Polyline(std::initializer_list<Point> points)`**  
**`Polyline(std::vector<Point> points)`**  
The constructor.
Creates a polyline from points.


## class Comment
#### Member
**`std::string comment`**  

#### Methods
**`Comment(td::string comment)`**  
The constructor.
Creates a comment from given `std::string`.


## class Fill
#### Member
**`Color color`**  

#### Methods
**`constexpr Fill(Color c)`**  
The constructor.
Takes a single color. 


## enum Linecap
**`BUTT`**  
**`ROUND`**  
**`SQUARE`**  


## enum Linecap
**`ROUND`**  
**`BEVEL`**  
**`MITER`**  


## class Stroke
#### Member
**`Color color`**  
**`Real width`**  
**`std::optional<Linecap> linecap`**  
**`std::optional<Linejoin> linejoin`**  
**`bool scale`**  

#### Methods
**`constexpr Stroke(Color c, Real width)`**  
**`constexpr Stroke(Color c, Real width, bool scale)`**  
The constructor.
Takes a color, a real as `width` and an optional bool `scale`.
If scale is `false`, the vector effect `non-scaling-stroke` is applied.


## class Dimension
#### Member
**`Real width`**  
**`Real height`**  

#### Methods
**`constexpr Dimension(Real width, Real height)`**  
The constructor.
Takes two reals as `width` and `height`.

**`constexpr Dimension operator+(Dimension x) const`**  
**`constexpr Dimension operator-(Dimension x) const`**  
**`constexpr Dimension& operator+=(Dimension x)`**  
**`constexpr Dimension& operator-=(Dimension x)`**  
Adds or subtracts another dimension.

## class Point
#### Member
**`Real x`**  
**`Real y`**  

#### Methods
**`constexpr Point(Real x, Real y)`**  
The constructor.
Takes two reals as `x` and `y` coordinate.

**`constexpr Point operator+(Dimension x) const`**  
**`constexpr Point operator-(Dimension x) const`**  
**`constexpr Point& operator+=(Dimension x)`**  
**`constexpr Point& operator-=(Dimension x)`**  
Moves the point by a given dimension.
