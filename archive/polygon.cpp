#include "../src/validate.h"

namespace details {
	template<typename Point>
	constexpr Integer dot(Point a, Point b) {
		return getX(a) * getX(b) + getY(a) * getY(b);
	}

	template<typename Point>
	constexpr Integer dot(Point p, Point a, Point b) {
		getX(a) -= getX(p);
		getY(a) -= getY(p);
		getX(b) -= getX(p);
		getY(b) -= getY(p);
		return dot(a, b);
	}

	template<typename RandomIt>
	void findLine(RandomIt first, RandomIt last) {
		using Point = typename RandomIt::value_type;
		using std::swap;
		std::size_t n = std::distance(first, last);
		if (n <= 2) return;
		Point cutPointA;
		do {
			cutPointA = Random::select(next(first), prev(last));
		} while (cross(cutPointA, *first, *prev(last)) == 0);

		std::vector<Point> cutPointBs = {*prev(last)};
		for (auto it = next(first); it != prev(last); it++) {
			Integer cFirst = cross(cutPointA, *it, *first);
			Integer cLast = cross(cutPointA, *it, *prev(last));
			if (cFirst != 0 && sign(cFirst) * sign(cLast) <= 0) {
				cutPointBs.push_back(*it);
			}
		}
		Point cutPointB = Random::select(cutPointBs);

		auto cut = std::partition(std::next(first), std::prev(last), [=](const Point& p) {
			if (Integer c = cross(cutPointA, cutPointB, p); c != 0) {
				return sign(c) == sign(cross(cutPointA, cutPointB, *first));
			} else {
				return sign(dot(cutPointA, cutPointB, p)) == sign(dot(cutPointA, cutPointB, *first));
			}
		});
		for (auto it = cut; it != std::prev(last); it++) {
			if (*it == cutPointA) std::iter_swap(cut, it);
		}

		findLine(first, std::next(cut));
		findLine(cut, last);
	}
}

namespace Random {
	template<typename Point = std::pair<Integer, Integer>>
	Point point(Integer dim) {
		return {Random::integer(-dim + 1, dim), Random::integer(-dim + 1, dim)};
	}

	template<typename Point = std::pair<Integer, Integer>>
	std::vector<Point> polygon(Integer n, Integer dim) {
		judgeAssert<std::invalid_argument>(dim <= 0x3FFF'FFFF, "dim too large!");
		judgeAssert<std::invalid_argument>(dim > 0, "dim must be positive!");
		judgeAssert<std::invalid_argument>(n*sqrt(n) <= dim, "dim too small!");
		judgeAssert<std::invalid_argument>(n >= 3, "n too small!");
		
		std::vector<Point> points(n);
		for (auto& p : points) p = point(dim);
		Point dir = point(0x3FFF'FFFF); //sample angle instead?
		std::sort(points.begin(), points.end(), [=](const Point& a, const Point& b){
			return ::details::dot(a, dir) < ::details::dot(b, dir);
		});
		swap(points[1], points.back());
		::details::findLine(points.begin(), points.end());
		return points;
	}
}

int main(int argc, char **argv) {
	Generator::init(argc, argv);
	using namespace Generator;

	Integer n = arguments["--n"].asInteger(100'000);
	Integer dim = arguments["--dim"].asInteger(1'000'000'000);

	auto points = Random::polygon(n, dim);

	testOut << points.size() << std::endl;
	testOut << join(points, '\n') << std::endl;
}
