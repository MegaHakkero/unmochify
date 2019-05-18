#include <cstdint>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <vector>

extern "C" {
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
}

#include "mochi.h"

int32_t main(int32_t argc, char **argv) {
	if (argc < 3) {
		std::printf("usage: %s <mochicrypted file> <output>\n", argv[0]);
		return 1;
	}
	int32_t f = open(argv[1], O_RDONLY | O_NOCTTY);
	if (f < 0) {
		std::printf("open(input, O_RDONLY) failed: %s\n", std::strerror(errno));
		return 1;
	}
	int32_t w = open(argv[2], O_WRONLY | O_CREAT | O_NOCTTY);
	if (w < 0) {
		std::printf("open(output, O_WRONLY) failed: %s\n", std::strerror(errno));
		return 1;
	}
	struct stat s;
	fstat(f, &s);
	std::vector<uint8_t> mochi_data(s.st_size);
	read(f, mochi_data.data(), s.st_size);
	std::vector<uint8_t> decrypted = mochi::decrypt(mochi_data);
	write(w, decrypted.data(), decrypted.size());
	close(f);
	close(w);
	return 0;
}
