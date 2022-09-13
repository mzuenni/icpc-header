// This generator generates a permutation for which javas Arrays.sort takes quadratic time.
// I would recomment not use this generator for ICPC style contests!
// code from https://codeforces.com/profile/dalex

#include "../src/validate.h"
using namespace std;

constexpr Integer NO_VALUE = -1;
constexpr Integer INSERTION_SORT_THRESHOLD = 47;

struct HackedSort {
	vector<Integer> data;
private:
	vector<Integer> p;
	Integer MIN_VALUE, MAX_VALUE;

	bool myLess(Integer a, Integer b) {
		if (a != NO_VALUE && b != NO_VALUE) {
			return a < b;
		}
		if (a == NO_VALUE) {
			return b > MAX_VALUE;
		}
		if (b == NO_VALUE) {
			return a < MIN_VALUE;
		}
		return false;
	}

	bool myGreater(Integer a, Integer b) {
		if (a != NO_VALUE && b != NO_VALUE) {
			return a > b;
		}
		if (a == NO_VALUE) {
			return b < MIN_VALUE;
		}
		if (b == NO_VALUE) {
			return a > MAX_VALUE;
		}
		return false;
	}

	void hackedSort(Integer left, Integer right, bool leftmost) {
		Integer length = right - left + 1;
		if (length < INSERTION_SORT_THRESHOLD) {
			for (Integer i = right; i >= left; i--) {
				if (data[i] == NO_VALUE) data[i] = MIN_VALUE++;
			}
			Random::shuffle(data.begin()+left, data.begin()+right+1);
			if (leftmost) {
				for (Integer i = left, j = i; i < right; j = ++i) {
					Integer ai = data[i + 1];
					Integer pi = p[i + 1];
					while (ai < data[j]) {
						data[j + 1] = data[j];
						p[j + 1] = p[j];
						if (j-- == left) {
							break;
						}
					}
					data[j + 1] = ai;
					p[j + 1] = pi;
				}
			} else {
				do {
					if (left >= right) {
						return;
					}
				} while (data[++left] >= data[left - 1]);
				for (Integer k = left; ++left <= right; k = ++left) {
					Integer a1 = data[k], a2 = data[left];
					Integer p1 = p[k], p2 = p[left];

					if (a1 < a2) {
						a2 = a1; a1 = data[left];
						p2 = p1; p1 = p[left];
					}
					while (a1 < data[--k]) {
						data[k + 2] = data[k];
						p[k + 2] = p[k];
					}
					++k;
					data[k + 1] = a1;
					p[k + 1] = p1;

					while (a2 < data[--k]) {
						data[k + 1] = data[k];
						p[k + 1] = p[k];
					}
					data[k + 1] = a2;
					p[k + 1] = p2;
				}
				Integer last = data[right];
				Integer plast = p[right];

				while (last < data[--right]) {
					data[right + 1] = data[right];
					p[right + 1] = p[right];
				}
				data[right + 1] = last;
				p[right + 1] = plast;
			}
			return;
		}
		Integer seventh = (length >> 3) + (length >> 6) + 1;
		Integer e3 = (left + right) >> 1;
		Integer e2 = e3 - seventh;
		Integer e1 = e2 - seventh;
		Integer e4 = e3 + seventh;
		Integer e5 = e4 + seventh;
		
		if (data[e5] == NO_VALUE) data[e5] = MIN_VALUE++;
		if (data[e4] == NO_VALUE) data[e4] = MIN_VALUE++;
		
		if (data[e1] == NO_VALUE) data[e1] = MAX_VALUE--;
		if (data[e2] == NO_VALUE) data[e2] = MAX_VALUE--;
		if (myLess(data[e2], data[e1])) { Integer t = data[e2]; data[e2] = data[e1]; data[e1] = t;
								  Integer s = p[e2]; p[e2] = p[e1]; p[e1] = s; }

		if (myLess(data[e3], data[e2])) { Integer t = data[e3]; data[e3] = data[e2]; data[e2] = t;
								  Integer s = p[e3]; p[e3] = p[e2]; p[e2] = s;
			if (myLess(t, data[e1])) { data[e2] = data[e1]; data[e1] = t;
								  p[e2] = p[e1]; p[e1] = s; }
		}
		if (myLess(data[e4], data[e3])) { Integer t = data[e4]; data[e4] = data[e3]; data[e3] = t;
								  Integer s = p[e4]; p[e4] = p[e3]; p[e3] = s;
			if (myLess(t, data[e2])) { data[e3] = data[e2]; data[e2] = t;
								  p[e3] = p[e2]; p[e2] = s;
				if (myLess(t, data[e1])) { data[e2] = data[e1]; data[e1] = t;
									  p[e2] = p[e1]; p[e1] = s; }
			}
		}
		if (myLess(data[e5], data[e4])) { Integer t = data[e5]; data[e5] = data[e4]; data[e4] = t;
								  Integer s = p[e5]; p[e5] = p[e4]; p[e4] = s;
			if (myLess(t, data[e3])) { data[e4] = data[e3]; data[e3] = t;
								  p[e4] = p[e3]; p[e3] = s;
				if (myLess(t, data[e2])) { data[e3] = data[e2]; data[e2] = t;
									  p[e3] = p[e2]; p[e2] = s;
					if (myLess(t, data[e1])) { data[e2] = data[e1]; data[e1] = t;
										  p[e2] = p[e1]; p[e1] = s; }
				}
			}
		}
		Integer less  = left;
		Integer great = right;

		Integer pivot1 = data[e2];
		Integer pivot2 = data[e4];
		Integer ppivot1 = p[e2];
		Integer ppivot2 = p[e4];

		data[e2] = data[left];
		data[e4] = data[right];
		p[e2] = p[left];
		p[e4] = p[right];

		while (myLess(data[++less], pivot1)) {};
		while (myGreater(data[--great], pivot2)) {};

		for (Integer k = less - 1; ++k <= great; ) {
			Integer ak = data[k];
			Integer pk = p[k];
			if (myLess(ak, pivot1)) {
				data[k] = data[less];
				p[k] = p[less];

				data[less] = ak;
				p[less] = pk;
				++less;
			} else if (myGreater(ak, pivot2)) {
				while (myGreater(data[great], pivot2)) {
					if (great-- == k) {
						goto outerA;
					}
				}
				if (myLess(data[great], pivot1)) {
					data[k] = data[less];
					p[k] = p[less];
					data[less] = data[great];
					p[less] = p[great];
					++less;
				} else {
					data[k] = data[great];
					p[k] = p[great];
				}
				data[great] = ak;
				p[great] = pk;
				--great;
			}
		}
		outerA:

		data[left]  = data[less  - 1]; data[less  - 1] = pivot1;
		data[right] = data[great + 1]; data[great + 1] = pivot2;
		p[left]  = p[less  - 1]; p[less  - 1] = ppivot1;
		p[right] = p[great + 1]; p[great + 1] = ppivot2;
		
		hackedSort(left, less - 2, leftmost);
		hackedSort(great + 2, right, false);

		if (less < e1 && e5 < great) {
			for (Integer k = less - 1; ++k <= great; ) {
				Integer ak = data[k];
				Integer pk = p[k];
				if (ak == pivot1) {
					data[k] = data[less];
					p[k] = p[less];
					data[less] = ak;
					p[less] = pk;
					++less;
				} else if (ak == pivot2) {
					while (data[great] == pivot2) {
						if (great-- == k) {
							goto outerB;
						}
					}
					if (data[great] == pivot1) {
						data[k] = data[less];
						p[k] = p[less];
						data[less] = pivot1;
						p[less] = ppivot1;
						++less;
					} else {
						data[k] = data[great];
						p[k] = p[great];
					}
					data[great] = ak;
					p[great] = pk;
					--great;
				}
			}
		}
		outerB:
		hackedSort(less, great, false);
	}

	void applyPermutation(vector<Integer>& a, vector<Integer>& pos) {
		Integer n = a.size();
		vector<Integer> tmp(n);
		for (Integer i = 0; i < n; i++) {
			tmp[pos[i]] = data[i];
		}
		for (Integer i = 0; i < n; i++) {
			data[i] = tmp[i];
			pos[i] = i;
		}
	}

public:
	void run(Integer n, Integer mi = 1) {
		data.assign(n, NO_VALUE);
		p.assign(n, 0);
		for (Integer i = 0; i < n; i++) p[i] = i;
		MIN_VALUE = mi;
		MAX_VALUE = mi + n;
		hackedSort(0, n-1, true);
		applyPermutation(data, p);
	}
};

int main(int argc, char **argv) {
	Generator::init(argc, argv);
	using namespace Generator;

	Integer n = arguments["--n"].asInteger(100'000);
	Integer min = arguments["--min"].asInteger(1);

	HackedSort hs;
	hs.run(n);
	testOut << n << endl;
	testOut << join(hs.data) << endl;
}
