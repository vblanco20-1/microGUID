#pragma once
#include "stdint.h"

namespace guid {

	struct BinaryGUID {
		uint8_t bytes[16];

	};

	struct TextGUID {
		char letters[37];
	};

	TextGUID convert_to_text(const BinaryGUID& binGuid);

	class GUIDGenerator {

		class Xorshiro {
		public:

			void seed(uint64_t seed[4]) {
				for (int i = 0; i < 4; i++) {
					s[i] = seed[i];
				}
			}
			uint64_t next(void) {
				const uint64_t result = rotl(s[1] * 5, 7) * 9;

				const uint64_t t = s[1] << 17;

				s[2] ^= s[0];
				s[3] ^= s[1];
				s[1] ^= s[2];
				s[0] ^= s[3];

				s[2] ^= t;

				s[3] = rotl(s[3], 45);

				return result;
			}
		private:
			static inline uint64_t rotl(const uint64_t x, int k) {
				return (x << k) | (x >> (64 - k));
			}

			uint64_t s[4];
		};

	public:
		void init();

		BinaryGUID make_binary_guid();

		
	private:

		Xorshiro rng;
	};
}

#ifdef MICROGUID_IMPLEMENTATION

#include <random>

namespace guid {
	char byte_to_hex(uint8_t byte)
	{
		uint8_t low_nibble = byte & 0x0f;

		if (low_nibble >= 10)
		{
			low_nibble -= 10;
			return 'A' + low_nibble;
		}
		else {
			return '0' + low_nibble;
		}
	}

	guid::TextGUID guid::convert_to_text(const BinaryGUID& binGuid)
	{
		TextGUID newguid;

		//123e4567-e89b-42d3-a456-426614174000
		int l = 0;
		int i = 0;
		for (i; i < 4; i++) {

			newguid.letters[l] = byte_to_hex(binGuid.bytes[i] >> 4);
			newguid.letters[l + 1] = byte_to_hex(binGuid.bytes[i]);

			l += 2;
		}

		newguid.letters[l] = '-';
		l++;
		for (i; i < 6; i++) {

			newguid.letters[l] = byte_to_hex(binGuid.bytes[i] >> 4);
			newguid.letters[l + 1] = byte_to_hex(binGuid.bytes[i]);

			l += 2;
		}
		newguid.letters[l] = '-';
		l++;
		for (i; i < 8; i++) {

			newguid.letters[l] = byte_to_hex(binGuid.bytes[i] >> 4);
			newguid.letters[l + 1] = byte_to_hex(binGuid.bytes[i]);

			l += 2;
		}
		newguid.letters[l] = '-';
		l++;
		for (i; i < 10; i++) {

			newguid.letters[l] = byte_to_hex(binGuid.bytes[i] >> 4);
			newguid.letters[l + 1] = byte_to_hex(binGuid.bytes[i]);

			l += 2;
		}
		newguid.letters[l] = '-';
		l++;
		for (i; i < 16; i++) {

			newguid.letters[l] = byte_to_hex(binGuid.bytes[i] >> 4);
			newguid.letters[l + 1] = byte_to_hex(binGuid.bytes[i]);

			l += 2;
		}
		//null terminate
		newguid.letters[l] = 0;
		return newguid;
	}

	void GUIDGenerator::init()
	{
		std::random_device generator;

		uint64_t seed[4];
		for (int i = 0; i < 4; i++) {
			uint64_t n = generator(); //first 32 bits
			n = (n << 32) | generator(); //second 32 bits

			seed[i] = n;
		}
		rng.seed(seed);
	}

	BinaryGUID GUIDGenerator::make_binary_guid()
	{
		uint64_t rng_bytes[2];

		rng_bytes[0] = rng.next();
		rng_bytes[1] = rng.next();

		BinaryGUID new_guid;

		memcpy(&new_guid, rng_bytes, sizeof(BinaryGUID));

		//clear the 4th most significant bytes
		new_guid.bytes[6] &= 0xf;
		//set 4 most significant bytes to type 4
		new_guid.bytes[6] |= 4 << 4;

		//clear the 2th most significant bytes
		new_guid.bytes[8] &= 0x3f;
		//set 2 most significant bytes to 2
		new_guid.bytes[8] |= 2 << 6;

		return new_guid;
	}	
}

#endif MICROGUID_IMPLEMENTATION
 