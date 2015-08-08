#include "dbfs.h"
#include "argparse.h"

static const char *const usages[] = {
	"dbfs [action] [args] [[--] options]",
	NULL,
};

int main(int argc, const char **argv) {
	char *outputname;
	uint32_t sector_size = 512;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_STRING('o', "outfile", &outputname, "output file"),
		OPT_INTEGER('s', "sector-size", &sector_size, "disk sector size in bytes (defaults to 512)"),
		OPT_END()
	};
	struct argparse argparse;
	argparse_init(&argparse, options, usages, 0);
	argparse_describe(&argparse, "\ndbfs is a tool for dealing with DBFS (Delta Boot Filesystem) disks. \
\nActions:\n    smush [img1,sysid]...     combine multiple images (not necessarily DBFS)","");
	argc = argparse_parse(&argparse, argc, argv);

	if (argc == 0) {
		help();
		exit(EPERM);
	}

	if (!strcmp(*argv, "smush"))
		argv++, smush(argc, argv, sector_size, outputname);
	else
		throwf(EINVAL, "Inalid arg `%s'!", *argv);

	return 0;
}