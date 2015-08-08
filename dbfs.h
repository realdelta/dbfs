#ifndef _DBFS_
#define _DBFS_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Info from wiki.osdev.org/Partition_Table
struct mbr_table {
	uint8_t boot_flag;				// Boot bit flag
	uint8_t start_head;				// Starting head
	unsigned int start_sect  : 6;	// Starting sector
	unsigned int start_cyl   : 10;	// Starting cylinder
	uint8_t sysid;					// System ID
	uint8_t ending_head;			// Ending head
	unsigned int ending_sect : 6;	// Ending sector
	unsigned int ending_cyl  : 10;	// Ending cylinder
	uint32_t starting_sect;			// Starting sector LBA
	uint32_t total_sect;			// Total # of sectors
} __attribute__((packed));
typedef struct mbr_table mbr_table_t;

mbr_table_t *mkpart(mbr_table_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint32_t);
void smush(uint8_t, const char**, uint32_t, char*);

#define throw(ERRNO, MESSAGE) 		fputs(MESSAGE, stderr), putc('\n', stderr), exit(ERRNO)
#define throwf(ERRNO, MESSAGE, ...) fprintf(stderr, "smush: fatal error: " MESSAGE, __VA_ARGS__), putc('\n', stderr), exit(ERRNO)

#define help()						argparse_usage(&argparse);

#endif