//============================================================================//
// visiualizer_png.h                                                          //
//============================================================================//
// This header can be used to generate png files                              //
//============================================================================//
//version 1.1.0                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef VISUALIZER_PNG_H
#define VISUALIZER_PNG_H

#include <algorithm>
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
	std::uint8_t r, g, b, a;
	constexpr explicit Color(std::uint32_t rgb = 0, std::uint8_t a_ = 0xFF) :
		r((rgb >> 16) & 0xFF),
		g((rgb >> 8) & 0xFF),
		b((rgb >> 0) & 0xFF),
		a(a_) {}

	constexpr explicit Color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_ = 0xFF) :
		r(r_), g(g_), b(b_), a(a_) {}

	constexpr std::uint8_t& operator[](std::size_t i) & {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			case 3: return a;
			default: throw std::out_of_range("Index out of range");
		}
	}

	constexpr const std::uint8_t& operator[](std::size_t i) const & {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			case 3: return a;
			default: throw std::out_of_range("Index out of range");
		}
	}

	constexpr std::uint8_t operator[](std::size_t i) const && {
		switch(i) {
			case 0: return r;
			case 1: return g;
			case 2: return b;
			case 3: return a;
			default: throw std::out_of_range("Index out of range");
		}
	}

	constexpr bool operator==(const Color& o) const {
		return r == o.r and g == o.g and b == o.b and a == o.a;
	}

	constexpr bool operator!=(const Color& o) const {
		return r != o.r or g != o.g or b != o.b or a != o.a;
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
	             clamp_color(c.b*f),
	             clamp_color(c.a*f));
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
	             clamp_color(a.b+b.b),
	             clamp_color(a.a+b.a));
}

constexpr Color& operator+=(Color& a, const Color& b) {
	return a=a+b;
}

constexpr Color operator-(const Color& a, const Color& b) {
	return Color(clamp_color(a.r-b.r),
	             clamp_color(a.g-b.g),
	             clamp_color(a.b-b.b),
	             clamp_color(a.a-b.a));
}

constexpr Color& operator-=(Color& a, const Color& b) {
	return a=a-b;
}

constexpr Color lerp(const Color& a, const Color& b, Real f) {
	assert(0 <= f and f <= 1);
	return a*f+b*(1-f);
}

constexpr Color blend(const Color& lower, const Color& upper) {
	if (upper.a == 0xFF) return upper;
	else if (upper.a == 0) return lower;
	int aLower = (lower.a * (0xFF - upper.a)) / 0xFF;
	int aRes = upper.a + aLower;
	return Color(clamp_color((upper.r * upper.a + lower.r * aLower) / aRes),
	             clamp_color((upper.g * upper.a + lower.g * aLower) / aRes),
	             clamp_color((upper.b * upper.a + lower.b * aLower) / aRes),
	             clamp_color(aRes));
}

enum class BlendMode{ASSIGN, BLEND, ADD, SUBTRACT};

constexpr auto ASSIGN = BlendMode::ASSIGN;
constexpr auto BLEND = BlendMode::BLEND;
constexpr auto ADD = BlendMode::ADD;
constexpr auto SUBTRACT = BlendMode::SUBTRACT;

namespace details {
	template<BlendMode M = BlendMode::ASSIGN>
	constexpr Color combine(const Color& lower, const Color& upper) {
		if constexpr (M == BlendMode::ASSIGN) return upper;
		if constexpr (M == BlendMode::BLEND) return ::blend(lower, upper);
		if constexpr (M == BlendMode::ADD) return lower + upper;
		if constexpr (M == BlendMode::SUBTRACT) return lower - upper;
	}
}

// some default colors
constexpr Color BLACK       = Color(0x000000);
constexpr Color DARKGRAY    = Color(0x404040);
constexpr Color GRAY        = Color(0x808080);
constexpr Color LIGHTGRAY   = Color(0xC0C0C0);
constexpr Color WHITE       = Color(0xFFFFFF);
constexpr Color TRANSPARENT = Color(0x000000, 0x00);

constexpr Color RED         = Color(0xFF0000);
constexpr Color GREEN       = Color(0x00FF00);
constexpr Color BLUE        = Color(0x0000FF);

constexpr Color YELLOW      = Color(0xFFFF00);
constexpr Color MAGENTA     = Color(0xFF00FF);
constexpr Color CYAN        = Color(0x00FFFF);

namespace details {
	constexpr uint32_t CRC32[] = {
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c, 
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
	};
	constexpr uint8_t MAGICK[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; //PNG header
	constexpr uint8_t SETTINGS[] = {
		0x08, // bits per color
		0x06, // alpha | true color
		0x00, // default compression
		0x00, // no filter
		0x00, // no interlace
	};

	class PNGWriter final {
		std::ofstream os;
		uint32_t a = 0, b = 0, c = 0; // ADLER-a, ADLER-b, CRC 

		void writeU8(uint8_t x){
			os << x;
			c ^= x;
			c = (c >> 4) ^ CRC32[c & 15];
			c = (c >> 4) ^ CRC32[c & 15]; 
		}
		void writeU16L(uint32_t x) {
			writeU8((x >> 0) & 0xFF);
			writeU8((x >> 8) & 0xFF);
		}
		void writeU32(uint32_t x) {
			writeU8((x >> 24) & 0xFF);
			writeU8((x >> 16) & 0xFF);
			writeU8((x >> 8) & 0xFF);
			writeU8((x >> 0) & 0xFF);
		}
		void writeU8Adler(uint8_t x){
			writeU8(x);
			a = (a + x) % 65521;
			b = (b + a) % 65521;
		}
		void writeMagick() {for (uint8_t x : MAGICK) writeU8(x);}
		void writeSettings() {for (uint8_t x : SETTINGS) writeU8(x);}
		void beginChunk(std::string_view s, uint32_t l) {
			writeU32(l);
			c = ~0u;
			for (char x : s) writeU8(x);
		}
		void endChunk() {writeU32(~c);}
	public:
		explicit PNGWriter(const std::string& name): os(name) {}

		PNGWriter(const PNGWriter&) = delete;
		PNGWriter(PNGWriter&&) = delete;
		PNGWriter& operator=(const PNGWriter&) = delete;
		PNGWriter& operator=(PNGWriter&&) = delete;

		void write(const std::vector<std::vector<Color>>& colors) {
			assert(!colors.empty());
			a = 1;
			b = 0;
			uint32_t p = 4 * colors[0].size() + 1;
			writeMagick();
			
			beginChunk("IHDR", 13);
			writeU32(colors[0].size()); //width
			writeU32(colors.size());	//height
			writeSettings();
			endChunk();
			
			beginChunk("IDAT", 2 + colors.size() * (p + 5) + 4);
			writeU8('\x78');
			writeU8('\x1');
			for (size_t y = 0; y < colors.size(); y++) {
				writeU8(y + 1 == colors.size());
				writeU16L(p);
				writeU16L(~p);
				writeU8Adler(0);
				if constexpr (FLIP_Y) {					
					for (Color x : colors[colors.size() - 1 - y]) {
						writeU8Adler(x.r);
						writeU8Adler(x.g);
						writeU8Adler(x.b);
						writeU8Adler(x.a);
					}
				} else {
					for (Color x : colors[y]) {
						writeU8Adler(x.r);
						writeU8Adler(x.g);
						writeU8Adler(x.b);
						writeU8Adler(x.a);
					}
				}
			}
			writeU32((b << 16) | a);
			endChunk();

			beginChunk("IEND", 0);
			endChunk();
		}
	};

	constexpr std::pair<Integer, Integer> DIRECTIONS[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
}

struct Point {
	Integer x, y;
	constexpr Point(Integer x_, Integer y_) :
		x(x_), y(y_) {}

	constexpr bool operator==(const Point& o) const {
		return x == o.x and y == o.y;
	}

	constexpr bool operator!=(const Point& o) const {
		return x != o.x or y != o.y;
	}
};

class PNG final {
	std::optional<std::string> fileName;
	std::vector<std::vector<Color>> colors;
public:

	explicit PNG() {}
	explicit PNG(const std::string& fileName_) : fileName(fileName_) {}

	PNG(const PNG&) = default;
	PNG(PNG&&) = default;
	PNG& operator=(const PNG&) = default;
	PNG& operator=(PNG&&) = default;

	void clear(Integer w, Integer h, Color fill = TRANSPARENT) {
		//4*w+1 must fit in an 16bit unsigned integer!
		assert(w <= 16383);
		colors.assign(h, std::vector<Color>(w, fill));
	}

	Color& operator()(Integer x, Integer y) {
		return colors[y][x];
	}

	Color operator()(Integer x, Integer y) const {
		return colors[y][x];
	}

	Integer width() const {
		return colors.empty() ? 0 : colors[0].size();
	}

	Integer height() const {
		return colors.size();
	}

	template<BlendMode M = BlendMode::ASSIGN>
	void fillRectangle(const Point& a, const Point& b, const Color border, const Color& fill) {
		for (Integer y = std::min(a.y, b.y); y <= std::max(a.y, b.y); y++) {
			for (Integer x = std::min(a.x, b.x); x <= std::max(a.x, b.x); x++) {
				if (y = a.y or y == b.y or x == a.x or x == b.x) {
					colors[y][x] = details::combine<M>(colors[y][x], border);
				} else {
					colors[y][x] = details::combine<M>(colors[y][x], fill);
				}
			}
		}
	}

	template<BlendMode M = BlendMode::ASSIGN>
	void fillRectangle(const Point& a, const Point& b, const Color& fill) {
		fillRectangle<M>(a, b, fill, fill);
	}

	template<BlendMode M = BlendMode::ASSIGN>
	void floodFill(const Point& a, const Color& fill) {
		std::vector<Point> todo = {a};
		Color old = colors[a.y][a.x];
		colors[a.y][a.x] = details::combine<M>(colors[a.y][a.x], fill);
		if (old == colors[a.y][a.x]) return;
		while (!todo.empty()) {
			Point p = todo.back();
			todo.pop_back();
			for (auto [dx, dy] : details::DIRECTIONS) {
				if (p.y+dy < 0 or p.y+dy >= height()) continue;
				if (p.x+dx < 0 or p.x+dx >= width()) continue;
				if (colors[p.y+dy][p.x+dx] != old) continue;
				colors[p.y+dy][p.x+dx] = details::combine<M>(colors[p.y+dy][p.x+dx], fill);
				todo.emplace_back(p.x+dx,p.y+dy);
			}
		}
	}

	template<BlendMode M = BlendMode::ASSIGN>
	void drawLine(const Point& a, const Point& b, const Color& fill) {
		// Bresenham Algorithm
		Integer dx = std::abs(b.x - a.x);
		Integer sx = (a.x - b.x < 0) - (a.x - b.x > 0);
		Integer dy = -std::abs(b.y - a.y);
		Integer sy = (a.y - b.y < 0) - (a.y - b.y > 0);
		Integer error = dx + dy;
		Point c = a;
		colors[c.y][c.x] = details::combine<M>(colors[c.y][c.x], fill);
		while (c != b) {
			Integer e2 = 2 * error;
			if (e2 >= dy) {
				error += dy;
				c.x += sx;
			}
			if (e2 <= dx) {
				error += dx;
				c.y += sy;
			}
			colors[c.y][c.x] = details::combine<M>(colors[c.y][c.x], fill);
		}
	}

	void write() const {
		if (!fileName or width() == 0) return;
		details::PNGWriter png(*fileName);
		png.write(colors);
	}

	~PNG() {
		write();
	}

};

namespace Visualizer {

	PNG image;
	std::ifstream testIn;
	std::ifstream testAns;

	void init(int argc, char** argv) {
		assert(argc > 2);
		image = PNG("testcase.png");
		testIn.open(argv[1]);
		testAns.open(argv[2]);
	}

} // namespace Visualizer

#endif
