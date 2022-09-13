//============================================================================//
// visiualizer.h                                                              //
//============================================================================//
// This header can be used to generate svg files                              //
//============================================================================//
//version 1.0.0                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <vector>
#include <cmath>
#include <optional>

using Integer = std::int64_t;
using Real = long double;
constexpr bool FLIP_Y = true;

struct Color {
	std::uint8_t r, g, b;
	constexpr explicit Color(std::uint32_t rgb = 0) :
		r((rgb >> 16) & 0xFF),
		g((rgb >> 8) & 0xFF),
		b((rgb >> 0) & 0xFF) {}

	constexpr explicit Color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) :
		r(r_), g(g_), b(b_) {}

	constexpr explicit Color(std::uint8_t gray_) :
		Color(gray_, gray_, gray_) {} 

	constexpr std::uint8_t& operator[](std::size_t i) & {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			default: throw std::out_of_range("Index out of range");
		}
	}

	constexpr const std::uint8_t& operator[](std::size_t i) const & {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			default: throw std::out_of_range("Index out of range");
		}
	}

	constexpr std::uint8_t operator[](std::size_t i) const && {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			default: throw std::out_of_range("Index out of range");
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Color& c) {
		std::ios_base::fmtflags flags(os.flags());
		os << "#" << std::hex;
		os << std::setw(2) << std::setfill('0') << static_cast<int>(c.r);
		os << std::setw(2) << std::setfill('0') << static_cast<int>(c.g);
		os << std::setw(2) << std::setfill('0') << static_cast<int>(c.b);
		os.flags(flags);
		return os;
	}
};

constexpr std::uint8_t operator "" _c(unsigned long long int value) {
	return static_cast<std::uint8_t>(value);
}

template<typename T>
constexpr std::uint8_t clamp_color(T c) = delete;

template<>
constexpr std::uint8_t clamp_color(long long int c) {
	return static_cast<std::uint8_t>(std::clamp(c, 0ll, 0xFFll));
}

template<>
constexpr std::uint8_t clamp_color(int c) {
	return static_cast<std::uint8_t>(std::clamp(c, 0, 0xFF));
}

template<>
constexpr std::uint8_t clamp_color(long double c) {
	return static_cast<std::uint8_t>(std::clamp(std::llround(c), 0ll, 0xFFll));
}

template<>
constexpr std::uint8_t clamp_color(double c) {
	return static_cast<std::uint8_t>(std::clamp(std::llround(c), 0ll, 0xFFll));
}

constexpr Color operator*(Real f, const Color& c) {
	return Color(clamp_color(c.r*f),
				 clamp_color(c.g*f),
				 clamp_color(c.b*f));
}

constexpr Color operator*(const Color& c, Real f) {
	return f*c;
}

constexpr Color& operator*=(Color& c, Real f) {
	return c=f*c;
}

constexpr Color operator+(const Color& a, const Color& b) {
	return Color(clamp_color(a.r+b.r),
				 clamp_color(a.g+b.g),
				 clamp_color(a.b+b.b));
}

constexpr Color& operator+=(Color& a, const Color& b) {
	return a=a+b;
}

constexpr Color lerp(const Color& a, const Color& b, Real f) {
	assert(0 <= f and f <= 1);
	return a*f+b*(1-f);
}

// some default colors
constexpr Color BLACK		= Color(0x00_c);
constexpr Color DARKGRAY	= Color(0x40_c);
constexpr Color GRAY		= Color(0x80_c);
constexpr Color LIGHTGRAY	= Color(0xC0_c);
constexpr Color WHITE		= Color(0xFF_c);

constexpr Color RED			= Color(0xFF,0x00,0x00);
constexpr Color GREEN		= Color(0x00,0xFF,0x00);
constexpr Color BLUE		= Color(0x00,0x00,0xFF);


constexpr Color YELLOW		= Color(0xFF,0xFF,0x00);
constexpr Color MAGENTA		= Color(0xFF,0x00,0xFF);
constexpr Color CYAN		= Color(0x00,0xFF,0xFF);

template<typename K, typename V>
std::string attribute(K key, V value) {
	std::ostringstream tmp;
	tmp << key << "=\"" << value << "\" ";
	return tmp.str();
}

template<typename K, typename V>
std::string attribute(K key, std::optional<V> value) {
	std::ostringstream tmp;
	if (value) tmp << key << "=\"" << *value << "\" ";
	return tmp.str();
}

struct Fill {
	Color color;
	constexpr Fill(const Color& color_) : color(color_) {}

	friend std::ostream& operator<<(std::ostream& os, const Fill& fill) {
		return os << attribute("fill", fill.color);
	}
};

enum class Linecap {
	BUTT,
	ROUND,
	SQUARE,
};

std::ostream& operator<<(std::ostream& os, const Linecap& linecap) {
	switch (linecap) {
		case Linecap::BUTT: return os << "butt";
		case Linecap::ROUND: return os << "round";
		case Linecap::SQUARE: return os << "square";
		default: return os;
	}
}

enum class Linejoin {
	ROUND,
	BEVEL,
	MITER,
};

std::ostream& operator<<(std::ostream& os, const Linejoin& linejoin) {
	switch (linejoin) {
		case Linejoin::ROUND: return os << "round";
		case Linejoin::BEVEL: return os << "bevel";
		case Linejoin::MITER: return os << "miter";
		default: return os;
	}
}

struct Stroke {
	Color color;
	Real width;
	std::optional<Linecap> linecap;
	std::optional<Linejoin> linejoin;
	bool scale;
	constexpr explicit Stroke(const Color& color_, Real width_, bool scale_ = true) :
		color(color_), width(width_), scale(scale_) {}

	friend std::ostream& operator<<(std::ostream& os, const Stroke& stroke) {
		os << attribute("stroke", stroke.color);
		os << attribute("stroke-width", stroke.width);
		os << attribute("stroke-linecap", stroke.linecap);
		os << attribute("stroke-linejoin", stroke.linejoin);
		if (!stroke.scale) os << attribute("vector-effect", "non-scaling-stroke");
		return os;
	}
};

struct Dimension {
	Real width, height;
	constexpr Dimension(Real width_, Real height_) :
		width(width_), height(height_) {}

	constexpr Dimension operator+(const Dimension& d) const {
		return Dimension(width + d.width, height + d.height);
	}
	constexpr Dimension operator-(const Dimension& d) const {
		return Dimension(width - d.width, height - d.height);
	}

	constexpr Dimension& operator+=(const Dimension& d) {
		width += d.width;
		height += d.height;
		return *this;
	}
	constexpr Dimension& operator-=(const Dimension& d) {
		width -= d.width;
		height -= d.height;
		return *this;
	}
};

struct Point {
	Real x, y;
	constexpr Point(Real x_, Real y_) :
		x(x_), y(y_) {}

	constexpr Point operator+(const Dimension& d) const {
		return Point(x + d.width, y + d.height);
	}
	constexpr Point operator-(const Dimension& d) const {
		return Point(x - d.width, y - d.height);
	}

	constexpr Point& operator+=(const Dimension& d) {
		x += d.width;
		y += d.height;
		return *this;
	}
	constexpr Point& operator-=(const Dimension& d) {
		x -= d.width;
		y -= d.height;
		return *this;
	}
};

constexpr Real flipY(Real y, Real height = 0) {
	if constexpr (FLIP_Y) {
		return -(y + height);
	} else {
		return y;
	}
}

class SVG final : public std::ostream {
	std::optional<std::string> fileName;
	std::ostringstream out;
public:
	Point origin = Point(0.0, 0.0);
	Dimension dimension = Dimension(0.0, 0.0);

	explicit SVG() {
		init(out.rdbuf());
	}

	explicit SVG(const std::string& fileName_) : fileName(fileName_) {
		init(out.rdbuf());
	}

	SVG(const SVG&) = default;
	SVG(SVG&&) = default;
	SVG& operator=(const SVG&) = default;
	SVG& operator=(SVG&&) = default;

	void write() const {
		if (!fileName) return;
		std::ofstream os(*fileName);
		os << "<?xml " << attribute("version", "1.0") << attribute("encoding", "UTF8")<< "?>" << std::endl;
		os << "<svg " << attribute("xmlns", "http://www.w3.org/2000/svg")
					  << attribute("version", "1.0")
					  << attribute("baseProfile", "full")
					  << attribute("viewBox", std::to_string(origin.x) + " " +
					  						  std::to_string(flipY(origin.y, dimension.height)) + " " +
					  						  std::to_string(dimension.width) + " " +
					  						  std::to_string(dimension.height)) << ">" << std::endl;
		os << out.str();
		os << "</svg>";
	}

	void includeInBB(const Point& p) {
		if (p.x < origin.x) {
			dimension.width += origin.x - p.x;
			origin.x = p.x;
		}
		if (p.x > origin.x + dimension.width) {
			dimension.width = p.x - origin.x;
		}
		if (p.y < origin.y) {
			dimension.height += origin.y - p.y;
			origin.y = p.y;
		}
		if (p.y > origin.y + dimension.height) {
			dimension.height = p.y - origin.y;
		}
	}

	~SVG() {
		write();
	}

};

struct Shape {
	std::optional<Stroke> stroke;
	std::optional<Fill> fill;
	//std::vector<std::pair<std::string, std::string>> raw;
	protected:
	constexpr Shape() {}

	friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
		if (shape.stroke) os << shape.stroke.value();
		else os << attribute("stroke", "none");
		if (shape.fill) os << shape.fill.value();
		else os << attribute("fill", "none");
		//for (const auto e : shape.raw) os << attribute(e.first, e.second);
		return os;
	}
};

struct Rectangle final : public Shape {
	Point origin;
	Dimension dimension;
	constexpr explicit Rectangle(Point origin_, Dimension dimension_) :
		origin(origin_), dimension(dimension_) {}
	constexpr explicit Rectangle(Point origin_, Real width_, Real height_) :
		Rectangle(origin_, {width_, height_}) {}
	constexpr explicit Rectangle(Real x_, Real y_, Dimension dimension_) :
		Rectangle({x_, y_}, dimension_) {}
	constexpr explicit Rectangle(Real x_, Real y_, Real width_, Real height_) :
		Rectangle({x_, y_}, {width_, height_}) {}

	friend std::ostream& operator<<(std::ostream& os, const Rectangle& rectangle) {
		os << "<rect ";
		os << attribute("x", rectangle.origin.x);
		os << attribute("y", flipY(rectangle.origin.y, rectangle.dimension.height));
		os << attribute("width", rectangle.dimension.width);
		os << attribute("height", rectangle.dimension.height);
		os << static_cast<const Shape&>(rectangle);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Rectangle& rectangle) {
		static_cast<std::ostream&>(os) << rectangle;
		os.includeInBB(rectangle.origin);
		os.includeInBB(rectangle.origin + rectangle.dimension);
		return os;
	}
};

struct Circle final : public Shape {
	Point origin;
	Real r;
	constexpr explicit Circle(Point origin_, Real r_) :
		origin(origin_), r(r_) {}
	constexpr explicit Circle(Real x_, Real y_, Real r_) :
		Circle({x_, y_}, r_) {}

	friend std::ostream& operator<<(std::ostream& os, const Circle& circle) {
		os << "<circle ";
		os << attribute("r", circle.r);
		os << attribute("cx", circle.origin.x);
		os << attribute("cy", flipY(circle.origin.y));
		os << static_cast<const Shape&>(circle);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Circle& circle) {
		static_cast<std::ostream&>(os) << circle;
		os.includeInBB(circle.origin + Dimension(circle.r, circle.r));
		os.includeInBB(circle.origin + Dimension(-circle.r, -circle.r));
		return os;
	}
};

struct Ellipse final : public Shape {
	Point origin;
	Real rx, ry;
	constexpr explicit Ellipse(Point origin_, Real rx_, Real ry_) :
		origin(origin_), rx(rx_), ry(ry_) {}
	constexpr explicit Ellipse(Real x_, Real y_, Real rx_, Real ry_) :
		Ellipse({x_, y_}, rx_, ry_) {}

	friend std::ostream& operator<<(std::ostream& os, const Ellipse& ellipse) {
		os << "<ellipse ";
		os << attribute("rx", ellipse.rx);
		os << attribute("ry", ellipse.ry);
		os << attribute("cx", ellipse.origin.x);
		os << attribute("cy", flipY(ellipse.origin.y));
		os << static_cast<const Shape&>(ellipse);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Ellipse& ellipse) {
		static_cast<std::ostream&>(os) << ellipse;
		os.includeInBB(ellipse.origin + Dimension(ellipse.rx, ellipse.ry));
		os.includeInBB(ellipse.origin + Dimension(-ellipse.rx, -ellipse.ry));
		return os;
	}
};

struct Polygon final : public Shape {
	std::vector<Point> points;
	explicit Polygon(std::initializer_list<Point> points_ = {}) :
		points(points_) {}
	explicit Polygon(std::vector<Point> points_) :
		points(points_) {}

	friend std::ostream& operator<<(std::ostream& os, const Polygon& polygon) {
		os << "<polygon ";
		os << "points=\"";
		for (const Point& p : polygon.points) os << p.x << "," << flipY(p.y) << " ";
		os << "\" ";
		os << static_cast<const Shape&>(polygon);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Polygon& polygon) {
		static_cast<std::ostream&>(os) << polygon;
		for (const Point& p : polygon.points) os.includeInBB(p);
		return os;
	}
};

struct Line final : public Shape {
	Point p1, p2;
	constexpr explicit Line(Point p1_, Point p2_) :
		p1(p1_), p2(p2_) {}
	constexpr explicit Line(Real x1_, Real y1_, Real x2_, Real y2_) :
		p1({x1_, y1_}), p2({x2_, y2_}) {}

	friend std::ostream& operator<<(std::ostream& os, const Line& line) {
		os << "<line ";
		os << attribute("x1", line.p1.x);
		os << attribute("y1", flipY(line.p1.y));
		os << attribute("x2", line.p2.x);
		os << attribute("y2", flipY(line.p2.y));
		os << static_cast<const Shape&>(line);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Line& line) {
		static_cast<std::ostream&>(os) << line;
		os.includeInBB(line.p1);
		os.includeInBB(line.p2);
		return os;
	}
};

struct Polyline final : public Shape {
	std::vector<Point> points;
	explicit Polyline(std::initializer_list<Point> points_ = {}) :
		points(points_) {}
	explicit Polyline(std::vector<Point> points_) :
		points(points_) {}

	friend std::ostream& operator<<(std::ostream& os, const Polyline& polyline) {
		os << "<polyline ";
		os << "points=\"";
		for (const Point& p : polyline.points) os << p.x << "," << flipY(p.y) << " ";
		os << "\" ";
		os << static_cast<const Shape&>(polyline);
		os << "/>";
		return os;
	}

	friend SVG& operator<<(SVG& os, const Polyline& polyline) {
		static_cast<std::ostream&>(os) << polyline;
		for (const Point& p : polyline.points) os.includeInBB(p);
		return os;
	}
};

struct Comment final {
	std::string comment;
	explicit Comment(const std::string& comment_) : comment(comment_) {}

	friend std::ostream& operator<<(std::ostream& os, const Comment& comment) {
		return os << "<!-- " << comment.comment << " -->";
	}
};

namespace Visualizer {

	SVG image;
	std::ifstream testIn;
	std::ifstream testAns;

	void init(int argc, char** argv) {
		assert(argc > 1);
		std::string name = argv[1];
		image = SVG(name + ".svg");
		testIn.open(name + ".in");
		testAns.open(name + ".ans");
	}

} // namespace Visualizer

#endif
