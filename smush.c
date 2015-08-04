#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#include "argparse.h"

static const char *const usages[] = {
	"smush [output] [img1,sysid]... [[--] args]",
	NULL,
};

// Info from wiki.osdev.org/Partition_Table
struct mbr_table {
	uint8_t bfl;			// Boot bit flag
	uint8_t shd;			// Starting head
	unsigned int sct : 6;	// Starting sector
	unsigned int cyl : 10;	// Starting cylinder
	uint8_t sid;			// System ID
	uint8_t ehd;			// Ending head
	unsigned int esc : 6;	// Ending sector
	unsigned int ecy : 10;	// Ending cylinder
	uint32_t ss;			// Starting sector LBA
	uint32_t ts;			// Total # of sectors
} __attribute__((packed));
typedef struct mbr_table mbr_table_t;

int main(int argc, const char **argv) {
	FILE *output;
	uint8_t first = 1;
	char *outputname;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_STRING('o', "outfile", &outputname, "output file")
	};
	struct argparse argparse;
	argparse_init(&argparse, options, usages, 0);
	argparse_describe(&argparse, "\nSmush is a utility for combining unorthodox disk images into one.", "");
	argc = argparse_parse(&argparse, argc, argv);

	if (!(output = fopen(outputname, "wb"))) {
		fprintf(stderr, "smush: fatal error: could not open output file `%s'!\n", outputname);
		exit(ENOENT);
	}
	if (argc > 4) {
		fprintf(stderr, "smush: fatal error: MBR does not support more than 4 partitions!\n");
		exit(E2BIG);
	}
	for (;argc != 0; argc--,argv++) {
		FILE *new = fopen(*argv, "rb");
		printf("Smushing file `%s' into `%s'\n", *argv, outputname);
		if (!first)
			fseek(new, 512, SEEK_SET);
		int c;
		while ((c = getc(new)) != EOF)
			putc(c, output);
		fclose(new);
	}
	// Now add the new partiton table to the output
	mbr_table_t buf[4];
	fseek(output, 445, SEEK_SET);
	fclose(output);
}