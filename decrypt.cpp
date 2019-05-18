#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>

extern "C" {
#include <zlib.h>
}

#include "mochi.h"

std::string __zlib_err(int32_t zerrcode) {
	switch(zerrcode) {
		case Z_MEM_ERROR:
			return "Not enough memory";
		case Z_BUF_ERROR:
			return "Not enough space in output";
		case Z_DATA_ERROR:
			return "Incomplete/corrupted data";
	}
	return "Unknown";
}

namespace mochi {

std::vector<uint8_t> decrypt(std::vector<uint8_t>& p) {
	// decrypt algo I copied from mochicrypted file... lmao
	std::vector<uint8_t> S;
	std::vector<uint8_t> dec = p;
	uint32_t i=0, j=0, k=0, u=0, v=0;
	uint32_t n = dec.size() - 32;
	for (; i < 256; i++)
		S.push_back((uint8_t) i);
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + dec[n + (i & 31)]) & 255;
		u = S[i];
		S[i] = S[j];
		S[j] = (uint8_t) u;
	}
	if(n > 0x20000)
		n = 0x20000;
	i = j = 0;
	for (; k < n; k++) {
		i = (i + 1) & 255;
		u = S[i];
		j = (j + u) & 255;
		v = S[j];
		S[i] = (uint8_t) v;
		S[j] = (uint8_t) u;
		dec[k] = (uint8_t) (dec[k] ^ S[(u + v) & 255]);
	}
	// decompress all at once
	std::vector<uint8_t> inflated(128000000);
	uint64_t infbuflen = 128000000; // 128M all at once in memory eugh
	std::printf("deflating\n");
	int32_t inflate_ret = uncompress(inflated.data(), &infbuflen, dec.data(), dec.size());
	if (inflate_ret != Z_OK)
		throw std::runtime_error("zlib uncompress() failed: " + __zlib_err(inflate_ret));
	return inflated;
}

}
