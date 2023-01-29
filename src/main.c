#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <sys/time.h>

#include "generate.h"
#include "grid.h"
#include "out_ascii.h"
#include "path.h"

#ifndef GIT_VERSION
#	define GIT_VERSION "<unknown>"
#endif /* #ifndef GIT_VERSION */

#define MAXIMUM_DIMENSION 1000000000

enum generate {
	dfs,
	kruskal,
};

enum output {
	simple,
	compact,
	pretty,
	null,
};

static enum generate generate_method = dfs;
static enum output output_method = compact;
static bool output_method_override = false;
static uint64_t seed = 0;
static uint32_t width = 19;
static uint32_t height = 10;
static int verbose_flag;
static int with_path_flag;

static struct option long_options[] = {
	{"dimensions", required_argument, 0,               'd'},
	{"generate",   required_argument, 0,               'g'},
	{"width",      required_argument, 0,               'w'},
	{"height",     required_argument, 0,               'h'},
	{"output",     required_argument, 0,               'o'},
	{"with-path",  no_argument,       &with_path_flag, 'p'},
	{"seed",       required_argument, 0,               's'},
	{"verbose",    no_argument,       &verbose_flag,     1},
	{"version",    no_argument,       0,               'V'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
	mz_get_entropy((void*) &seed, 8);

	int option_index = 0, c;
	char *end;
	long n;
	do {
		int curopt = optind;
		c = getopt_long(argc, argv, "d:g:w:h:o:ps:vV", long_options, &option_index);
		switch (c) {
		case 'g':
			if (strcmp("dfs", optarg) == 0) {
				generate_method = dfs;
			} else if (strcmp("kruskal", optarg) == 0) {
				generate_method = kruskal;
			} else {
				fprintf(stderr, "invalid generate method, choose from \"dfs\", and \"kruskal\"\n");
				return 1;
			}
			break;
		case 'd':
		case 'w':
		case 'h':
			errno = 0;
			n = strtol(optarg, &end, 10);
			if (n < 1 || n > MAXIMUM_DIMENSION || errno == ERANGE || *end != '\0') {
				fprintf(stderr, "invalid number specified for %s must be 0 < width <= %d\n", argv[curopt], MAXIMUM_DIMENSION);
				return 1;
			}
			switch (c) {
				case 'd': width = n; height = n; break;
				case 'w': width = n; break;
				case 'h': height = n; break;
			}
			break;
		case 'o':
			output_method_override = true;
			if (strcmp("simple", optarg) == 0) {
				output_method = simple;
			} else if (strcmp("compact", optarg) == 0) {
				output_method = compact;
			} else if (strcmp("pretty", optarg) == 0) {
				output_method = pretty;
			} else if (strcmp("null", optarg) == 0) {
				output_method = null;
			} else {
				fprintf(stderr, "invalid output method, choose from \"simple\", \"compact\", and \"pretty\"\n");
				return 1;
			}
			break;
		case 'p':
			with_path_flag = 1;
			break;
		case 's':
			seed = strtoull(optarg, &end, 16);
			if (errno == ERANGE || *end != '\0') {
				fprintf(stderr, "invalid number specified for %s, enter a base16 string at most 16 characters long\n", argv[curopt]);
				return 1;
			}
			break;
		case 'v':
			verbose_flag = 1;
			break;
		case 'V':
			printf("uhmazeing %s\n", GIT_VERSION);
			return 0;
		case '?':
			fprintf(stderr, "error\n");
			return 1;
		}
	} while (c != -1);

	if (with_path_flag && !output_method_override) {
		output_method = pretty;
	}

	if (with_path_flag && output_method_override && output_method == compact) {
		fprintf(stderr, "warning: output method \"compact\" will not display the path, to see the path choose \"simple\" or \"pretty\" as output method\n");
	}

	if (verbose_flag) {
		fprintf(stderr, "seed = %llx\n", seed);
	}

	struct mz_rnd_t rnd;
	mz_rnd_init(&rnd, seed);

	struct mz_grid_t *g = mz_grid_create(height, width);

	// Generate
	switch (generate_method) {
	case dfs:
		if (verbose_flag) {
			fprintf(stderr, "generate = dfs\n");
		}
		mz_dfs(&rnd, g);
		break;
	case kruskal:
		if (verbose_flag) {
			fprintf(stderr, "generate = kruskal\n");
		}
		mz_kruskal(&rnd, g);
		break;
	}

	// Solve
	struct mz_path_t *path = 0;
	if (with_path_flag) {
		path = mz_dial(g);
		if (verbose_flag) {
			fprintf(stderr, "path_length = %u\n", mz_path_length(path));
		}
	}

	// Print
	switch (output_method) {
	case simple:
		if (verbose_flag) {
			fprintf(stderr, "print = simple\n");
		}
		mz_out_ascii_simple(g, path);
		break;
	case compact:
		if (verbose_flag) {
			fprintf(stderr, "print = compact\n");
		}
		mz_out_ascii_boxchars_compact(g);
		break;
	case pretty:
		if (verbose_flag) {
			fprintf(stderr, "print = pretty\n");
		}
		mz_out_ascii_boxchars(g, path);
		break;
	case null: /* no-op */ break;
	}

	/* No cleanup, OS will do it for us */
	return 0;
}
