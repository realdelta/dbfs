#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
	FILE *output;
	uint8_t first = 1;
	if (!(output = fopen(*(++argv), "wb"))) {
		fprintf(stderr, "smush: fatal error: could not open output file `%s'\n", *argv);
		exit(ENOENT);
	}
	const char *outname = *argv;
	for (; argc > 2; argc--) {
		FILE *new;
		if (!(new = fopen(*(++argv), "rb"))) {
			fprintf(stderr, "smush: fatal error: could not open input file `%s'\n", *argv);
			exit(ENOENT);
		}
		printf("Smushing image `%s' into `%s'\n", *argv, outname);
		if (!first)	// Check if not first run so we only use the MBR from disk 1
			fseek(new, 512, SEEK_SET);
		first = 0;
		int c;
		while((c = getc(new)) != EOF)
			fputc(c, output);
		fclose(new);
	}
	fclose(output);
}