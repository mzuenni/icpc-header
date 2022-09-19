//============================================================================//
// rng.h                                                                      //
//============================================================================//
// Replacement for Mersenne Twister                                           //
// This generator should be faster and generate better random numbers         //
//============================================================================//
//version 1.0.0                                                               //
//https://github.com/mzuenni/icpc-header                                      //
//============================================================================//

#ifndef RNG_H
#define RNG_H

#include <array>
#include <cstdint>

struct Xoshiro256ss {
	using result_type = std::uint64_t;
	static constexpr result_type min() {return 0;}
	static constexpr result_type max() {return 0xFFFF'FFFF'FFFF'FFFFull;}

private:
	std::array<result_type, 4> state = {//pi
		0x3243F6a8885a308d,
		0x313198a2e0370734,
		0x4a4093822299f31d,
		0x0082efa98ec4e6c8,
	};

	static inline result_type rotateLeft(const result_type x, int k) {
		return (x << k) | (x >> (64 - k));
	}
public:
	explicit Xoshiro256ss(const std::array<result_type, 4>& seed) : state(seed) {}
	explicit Xoshiro256ss(result_type seed) {
		state[0] ^= seed;
		state[1] ^= seed;
		state[2] ^= seed;
		state[3] ^= seed;
		// even if almost all bits would be zero the engine should be
		// be seeded well after 2^128 state transitions
		jump();
	}

	result_type operator()() {
		const result_type result = rotateLeft(state[1] * 5, 7) * 9;
		const result_type tmp = state[1] << 17;
		state[2] ^= state[0];
		state[3] ^= state[1];
		state[1] ^= state[2];
		state[0] ^= state[3];
		state[2] ^= tmp;
		state[3] = rotateLeft(state[3], 45);
		return result;
	}

	void jump() {//2^128 calls to operator()
		static const std::array<result_type, 4> jumps = {
			0x180ec6d33cfd0aba,
			0xd5a61266f0c9392c,
			0xa9582618e03fc9aa,
			0x39abdc4529b1661c,
		};

		std::array<result_type, 4> newState = {0, 0, 0, 0};
		for(std::size_t i = 0; i < jumps.size(); i++) {
			for(int b = 0; b < 64; b++) {
				if (jumps[i] & static_cast<result_type>(1) << b) {
					newState[0] ^= state[0];
					newState[1] ^= state[1];
					newState[2] ^= state[2];
					newState[3] ^= state[3];
				}
				this->operator()();
			}
		}
		state = std::move(newState);
	}

};

#endif
