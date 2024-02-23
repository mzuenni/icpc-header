// This generator generates a permutation for which javas Arrays.sort takes quadratic time.
// I would recomment not use this generator for ICPC style contests!
// code from https://codeforces.com/profile/dalex

#include "../src/validate.h"

constexpr Integer NO_VALUE = -1;
constexpr Integer INSERTION_SORT_THRESHOLD = 47;

struct HackedSort {
	std::vector<Integer> data;

	HackedSort(Integer n, Integer mi = 1) : data(n, NO_VALUE), p(n), MIN_VALUE(mi), MAX_VALUE(mi + n) {
		for (Integer i = 0; i < n; i++) p[i] = i;
		hackedSort(0, n-1, true);
		std::vector<Integer> tmp(n);
		for (Integer i = 0; i < n; i++) {
			tmp[p[i]] = data[i];
		}
		data = std::move(tmp);
	}

private:
	std::vector<Integer> p;
	Integer MIN_VALUE, MAX_VALUE;

	bool myLess(Integer a, Integer b) {
		if (a != NO_VALUE && b != NO_VALUE) return a < b;
		if (a == NO_VALUE) return b > MAX_VALUE;
		if (b == NO_VALUE) return a < MIN_VALUE;
		return false;
	}

	bool myGreater(Integer a, Integer b) {
		if (a != NO_VALUE && b != NO_VALUE) return a > b;
		if (a == NO_VALUE) return b < MIN_VALUE;
		if (b == NO_VALUE) return a > MAX_VALUE;
		return false;
	}

	void hackedSort(Integer left, Integer right, bool leftmost) {
		Integer length = right - left + 1;
		if (length < INSERTION_SORT_THRESHOLD) {
			for (Integer i = right; i >= left; i--) {
				if (data[i] == NO_VALUE) data[i] = MIN_VALUE++;
			}
			std::sort(data.begin()+left, data.begin()+right+1);
			Random::shuffle(p.begin()+left, p.begin()+right+1);
			return;
		}
		Integer seventh = (length >> 3) + (length >> 6) + 1;
		std::array<Integer, 5> e;
		e[2] = (left + right) >> 1;
		e[1] = e[2] - seventh;
		e[0] = e[1] - seventh;
		e[3] = e[2] + seventh;
		e[4] = e[3] + seventh;
		
		if (data[e[4]] == NO_VALUE) data[e[4]] = MIN_VALUE++;
		if (data[e[3]] == NO_VALUE) data[e[3]] = MIN_VALUE++;
		
		if (data[e[0]] == NO_VALUE) data[e[0]] = MAX_VALUE--;
		if (data[e[1]] == NO_VALUE) data[e[1]] = MAX_VALUE--;

		for (Integer i = 1; i <= 4; i++) {
			for (Integer j = i; j > 0 && myLess(data[e[j]], data[e[j - 1]]); j--) {
				std::swap(data[e[j]], data[e[j-1]]);
				std::swap(p[e[j]], p[e[j-1]]);
			}
		}

		Integer less  = left;
		Integer great = right;

		Integer pivot1 = data[e[1]];
		Integer pivot2 = data[e[3]];
		Integer ppivot1 = p[e[1]];
		Integer ppivot2 = p[e[3]];

		data[e[1]] = data[left];
		data[e[3]] = data[right];
		p[e[1]] = p[left];
		p[e[3]] = p[right];

		while (myLess(data[++less], pivot1)) {};
		while (myGreater(data[--great], pivot2)) {};

		for (Integer k = less - 1; ++k <= great; ) {
			if (myLess(data[k], pivot1)) {
				std::swap(data[k], data[less]);
				std::swap(p[k], p[less]);
				++less;
			} else if (myGreater(data[k], pivot2)) {
				while (myGreater(data[great], pivot2)) {
					if (great-- == k) goto outerA;
				}
				std::swap(data[k], data[great]);
				std::swap(p[k], p[great]);
				if (myLess(data[k], pivot1)) {
					std::swap(data[k], data[less]);
					std::swap(p[k], p[less]);
					++less;
				}
				--great;
			}
		}
		outerA:

		data[left] = data[less - 1];
		p[left] = p[less - 1];
		data[less - 1] = pivot1;
		p[less - 1] = ppivot1;
		data[right] = data[great + 1];
		p[right] = p[great + 1];
		data[great + 1] = pivot2;
		p[great + 1] = ppivot2;
		
		hackedSort(left, less - 2, leftmost);
		hackedSort(great + 2, right, false);

		if (less < e[0] && e[4] < great) {
			for (Integer k = less - 1; ++k <= great;) {
				if (data[k] == pivot1) {
					std::swap(data[k], data[less]);
					std::swap(p[k], p[less]);
					++less;
				} else if (data[k] == pivot2) {
					while (data[great] == pivot2) {
						if (great-- == k) goto outerB;
					}
					std::swap(data[k], data[great]);
					std::swap(p[k], p[great]);
					if (data[k] == pivot1) {
						std::swap(data[k], data[less]);
						std::swap(p[k], p[less]);
						++less;
					}
					--great;
				}
			}
		}
		outerB:
		hackedSort(less, great, false);
	}

};

int main(int argc, char **argv) {
	Generator::init(argc, argv);
	using namespace Generator;

	Integer n = arguments["--n"].asInteger(100'000);
	Integer min = arguments["--min"].asInteger(1);

	HackedSort hs(n, min);
	testOut << n << std::endl;
	testOut << join(hs.data) << std::endl;
}
