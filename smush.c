#include "dbfs.h"
#include "argparse.h"

mbr_table_t *mkpart(mbr_table_t *c, uint8_t bootable, uint8_t start_head, 
			uint8_t start_sect, uint8_t sc, uint8_t sysid, uint32_t size, uint32_t sector_size)
{
	c->boot_flag      = bootable ? 0x80 : 0x00;
	c->start_head     = start_head;
	c->start_sect     = start_head;
	c->start_cyl      = sc;
	c->sysid          = sysid;

	if (sector_size != 0)
		c->ending_sect    = size / sector_size;
	c->total_sect         = c->ending_sect - c->start_sect;

	// DEBUG
	c->starting_sect  = 0;
	c->ending_cyl     = 100;
	c->ending_head    = 45;
	return c;
}

void smush(uint8_t argc, const char **argv, uint32_t sector_size, char *outputname) {
	FILE *output;
	uint8_t first = 1;

	uint32_t sizes[4];					// Buffer for file sizes
	memset(sizes, 0, sizeof(sizes));

	if (argc > 4)
		throw(E2BIG, "MBR does not support more than 4 partitions!");

	for (;argc != 1; argc--,argv++) {
		FILE *new;
		if (!(new = fopen(*argv, "rb")))
			throwf(EIO, "could not open input file `%s'!", *argv);
		if (first && !(output = fopen(outputname, "wb")))
			throwf(EIO, "could not open output file `%s'!", outputname);
		printf("Smushing file `%s' into `%s'\n", *argv, outputname);

		if (!first)							// If not the first time, truncate first 512 bytes
			fseek(new, 512, SEEK_SET);
		first = 0;

		int byte_of_input;
		while ((byte_of_input = getc(new)) != EOF)
			putc(byte_of_input, output);
		fclose(new);
	}

	// Now add the new partiton table to the output
	mbr_table_t buf[4];
	memset(buf, 0, sizeof(buf));
	fseek(output, 445, SEEK_SET);
	fwrite(mkpart(&buf[0], 1, 0, 0, 0, 8, 90, sector_size), sizeof(mbr_table_t), 1, output);
	fclose(output);
}