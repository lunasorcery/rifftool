#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

static bool g_verbose = false;

static bool isAscii(const char* str) {
	while (*str) {
		if (*str < 0x20 || *str > 0x7e) {
			return false;
		}
		++str;
	}
	return true;
}

static void printOffset(size_t offset) {
	if (sizeof(size_t) <= 4) {
		printf("0x%08lx: ", offset);
	} else {
		printf("0x%016lx: ", offset);
	}
}

static void printIndent(int indent) {
	for (; indent; --indent) {
		printf(" ");
	}
}

static int dumpChunk(FILE* fh, int indent) {
	size_t offset = ftell(fh);
	char chunkId[5];
	fread(chunkId, 1, 4, fh);
	chunkId[4] = 0;
	if (feof(fh)) {
		return 1;
	}
	if (!isAscii(chunkId)) {
		printf("Non-ascii chunk id, probably not a valid RIFF file.\n");
		return 1;
	}
	int32_t chunkLength;
	fread(&chunkLength, 4, 1, fh);
	printOffset(offset);
	printIndent(indent);
	printf("chunk '%s'  %d bytes\n", chunkId, chunkLength);
	int32_t paddedChunkLength = (chunkLength + 1) & 0xfffffffe;
	size_t dataStart = ftell(fh);
	if (!strcmp(chunkId,"RIFF") ||
		!strcmp(chunkId,"LIST")) {
		char listId[5];
		fread(listId, 1, 4, fh);
		printOffset(dataStart);
		printIndent(indent + 1);
		printf("list '%s'\n", listId);
		while ((size_t)ftell(fh) < dataStart + paddedChunkLength) {
			if (dumpChunk(fh, indent + 2)) {
				return 1;
			}
		}
	} else {
		if (g_verbose) {
			uint8_t* buffer = malloc(paddedChunkLength);
			fread(buffer, 1, paddedChunkLength, fh);
			for (int i = 0; i < paddedChunkLength; i += 16){
				printOffset(dataStart + i);
				printIndent(indent + 1);
				for (int j = 0; j < 16; ++j) {
					if (i + j < paddedChunkLength) {
						printf("%02x ", buffer[i + j]);
					} else {
						printf("   ");
					}
				}
				printf(" |");
				for (int j = 0; j < 16; ++j) {
					if (i + j < paddedChunkLength) {
						if (buffer[i + j] >= 0x20 && buffer[i + j] <= 0x7e) {
							printf("%c", buffer[i + j]);
						} else {
							printf(".");
						}
					} else {
						printf(" ");
					}
				}
				printf("|\n");
			}
			free(buffer);
		} else {
			fseek(fh, paddedChunkLength, SEEK_CUR);
		}
	}
	return 0;
}

static void dumpRiffFromFile(const char* filepath) {
	FILE* fh = fopen(filepath, "rb");
	if (!fh) {
		printf("failed to open %s\n", filepath);
		return;
	}
	dumpChunk(fh, 0);
	fclose(fh);
}

static void parseCommandLine(int argc, char** argv) {
	while (1) {
		static struct option long_options[] = {
			{ "verbose", no_argument, 0, 'v' },
			{ 0, 0, 0, 0 }
		};
		int option_index = 0;
		int c = getopt_long(argc, argv, "v", long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
			case 'v': {
				g_verbose = true;
				break;
			}
		}
	}
}

int main(int argc, char** argv) {
	
	parseCommandLine(argc, argv);

	if (optind >= argc) {
		printf("No filepath provided.\n");
		return 1;
	}

	for (int i = optind; i < argc; ++i) {
		printf("%s: \n", argv[i]);
		dumpRiffFromFile(argv[i]);
		if (i != argc - 1) {
			printf("\n");
		}
	}
}
