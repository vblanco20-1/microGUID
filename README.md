# microGUID
Tiny single-header GUID implementation for C++

Implements standard compliant type 4 GUID (random one)
Uses xorshiro for random generation, and seeds from std::random_device.


## Usage


```cpp
//use this define exactly in one source file
#define MICROGUID_IMPLEMENTATION
#include "microGUID.h"

void generate_guid(){	
	guid::GUIDGenerator generator;
	//seed the generator
	generator.init();

	//build a optimal binary guid. 2 int64s of state
	guid::BinaryGUID binary = gen.make_binary_guid();
	//create a text based guid. stores null-terminated string inside
	guid::TextGUID text = guid::convert_to_text(binary);
}

```