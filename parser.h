#include "hashmap.h"

static const char sep[] = "\t[]{}<>=+-*/%!&|^.,:;()\\. ";

void print_include(FILE *in, FILE *out, int num_includes, char **includes,
			struct hashTable *h);
void command_line_parrser(int argc, char **argv, char ***includes,
		char **source, char **dest, struct hashTable *h,
		int *includes_count);
