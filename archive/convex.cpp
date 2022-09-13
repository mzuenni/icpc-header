#include "../src/validate.h"

namespace details {

	template<typename Point = std::pair<Integer, Integer>>
	std::vector<Point> convexHull(std::vector<Point> p){
		std::sort(p.begin(), p.end(), [](const Point& a, const Point& b){
			return getX(a) == getX(b) ? getY(a) < getY(b) : getX(a) < getX(b);
		});
		p.erase(std::unique(p.begin(), p.end()), p.end());
		Integer k = 0;
		std::vector<Point> h(2 * p.size());
		for (std::size_t i = 0; i < p.size(); i++) {
			while (k > 1 and cross(h[k - 2], h[k - 1], p[i]) < 0) k--;
			h[k++] = p[i];
		}
		for (Integer i = Integer(p.size()) - 2, t = k; i >= 0; i--) {
			while (k > t and cross(h[k - 2], h[k - 1], p[i]) < 0) k--;
			h[k++] = p[i];
		}
		h.resize(k);
		return h;
	}

}

namespace Random {
	template<typename Point = std::pair<Integer, Integer>>
	std::vector<Point> convexDisk(Integer n, Integer dim) {
		judgeAssert<std::invalid_argument>(dim <= 0x3FFF'FFFF, "dim too large!");
		judgeAssert<std::invalid_argument>(n * n / 1000 <= dim, "dim too small!");
		dim *= dim;

		auto sampleRing = [=](Integer r){
			while (true) {
				Real alpha = Random::real(0_real, 2_real * details::PI);
				Real radius = std::sqrt(Random::real(r - 0.71_real, dim + 0.71_real));
				Integer x = std::llround(std::sin(alpha) * radius);
				Integer y = std::llround(std::cos(alpha) * radius);
				Integer d = x * x + y * y;
				if (r <= d and d < dim) return Point{x, y};
			}
		};

		std::vector<Point> res = {{0, 0}};
		do {
			Integer r = dim;
			for (auto p : res) r = std::min(r, getX(p) * getX(p) + getY(p) * getY(p));
			for (Integer i = 0; i < n; i++) {
				res.push_back(sampleRing(r));
			}
			res = ::details::convexHull(res);
			res.pop_back();
		} while (res.size() <= n);
		res.pop_back();
		Random::shuffle(res);
		res.resize(n);
		return ::details::convexHull(res);
	}
}

int main(int argc, char **argv) {
	Generator::init(argc, argv);
	using namespace Generator;

	Integer n = arguments["--n"].asInteger(100'000);
	Integer dim = arguments["--dim"].asInteger(1'000'000'000);

	auto points = Random::convexDisk(n, dim);
	//auto points = Random::convex(n, dim); // you may also want to look at Random::convex

	testOut << points.size() << std::endl;
	testOut << join(points, '\n') << std::endl;
}
