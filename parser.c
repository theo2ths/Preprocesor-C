#include "parser.h"

static char *replace(char *s, char *t, char *u)
{
	char *new_line = NULL;
	char *aux_s = s;
	char *p = NULL;
	int get_c = 0;
	int len = 0;
	int i = 0;
	int j = 0;
	int mystr;

	while (strstr(aux_s, t) != NULL) {
		p = strstr(aux_s, t);
		if (!(p - s > 0 && *(p - 1) == '\"'))
			++get_c;
		aux_s = p + strlen(t);
	}
	len = strlen(s) - strlen(t) * get_c + strlen(u) * get_c;
	new_line = (char *)malloc(len + 1);
	if (new_line == NULL)
		exit(12);
	new_line[len] = '\0';
	aux_s = s;
	mystr = strlen(s);
	for (i = 0; i < mystr; ++i) {
		p = strstr(s + i, t);
		if (p != NULL && (p == s + i) &&
			!(p - s > 0 && *(p - 1) == '\"')) {
			memcpy(new_line + j, u, strlen(u));
			j += strlen(u);
			i += strlen(t) - 1;
		} else {
			new_line[j] = s[i];
			++j;
		}
	}
	return new_line;
}

static char *get_line(char *s, struct hashTable *h)
{
	char *new_line = NULL;
	char *aux_line = NULL;
	char *val = NULL;
	char *token;

	new_line = (char *)malloc(strlen(s) + 1);
	if (new_line == NULL)
		exit(12);
	memcpy(new_line, s, strlen(s));
	new_line[strlen(s)] = '\0';
	token = strtok(s, sep);
	while (token != NULL) {
		val = get_hash_table(h, token);
		if (val != NULL) {
			aux_line = new_line;
			new_line = replace(new_line, token, val);
			free(val);
			free(aux_line);
		}
		token = strtok(NULL, sep);
	}
	return new_line;
}

static char *get_full_dir(char *path, char *name)
{
	int len = strlen(path) + strlen(name);
	int i = 0;
	char *f = NULL;
	int mystr;

	f = (char *)malloc(len + 2);
	if (f == NULL)
		exit(12);
	mystr = strlen(path);
	for (i = 0; i < mystr; ++i)
		f[i] = path[i];
	f[strlen(path)] = '/';
	mystr = strlen(name);
	for (i = 0; i < mystr; ++i)
		f[strlen(path) + 1 + i] = name[i];
	f[len + 1] = '\0';
	return f;
}

static char *get_current_dir(char *l)
{
	int i;
	static const char p[] = "_test/inputs/";
	char *r = NULL;
	int mystr;

	r = (char *)malloc(strlen(l) + strlen(p) + 1);
	if (r == NULL)
		exit(12);
	r[strlen(l) + strlen(p)] = '\0';
	mystr = strlen(p);
	for (i = 0; i < mystr; ++i)
		r[i] = p[i];
	mystr = strlen(l);
	for (i = 0; i < mystr; ++i)
		r[i + strlen(p)] = l[i];
	return r;
}

static void refactor_digit(char *key, int *if_true, int *this_if_true,
		struct hashTable *h)
{
	char *aux_key = NULL;

	if ('1' <= key[0] && key[0] <= '9') {
		*if_true = 1;
		*this_if_true = 1;
	} else {
		aux_key = get_hash_table(h, key);
		if (aux_key == NULL || aux_key[0] == '0') {
			*if_true = 0;
			*this_if_true = 0;
		} else {
			*if_true = 1;
			*this_if_true = 1;
		}
		if (aux_key)
			free(aux_key);
	}
}

void print_include(FILE *in, FILE *out, int num_includes, char **includes,
			struct hashTable *h)
{
	char line[300];
	char *token = NULL;
	char *key = NULL;
	char *val = NULL;
	int if_true = 1;
	int this_if_true = 1;
	char *new_line = NULL;
	int i = 0;
	FILE *include_in = NULL;
	char *n = NULL;
	char *c_dir = NULL;
read_line:
	while (fgets(line, 300, in)) {
		if (strcmp(line, "\n") == 0)
			continue;
		if (line[0] == '#') {
			token = strtok(line, " \t\n");
			if (strcmp(token, "#define") == 0) {
				if (this_if_true == 0)
					continue;
				key = strtok(NULL, " ");
				val = strtok(NULL, "\n");
				if (val == NULL)
					val = (char *)"";
				val = get_line(val, h);
				insert_hash_table(h, key, val);
				free(val);
			} else if (strcmp(token, "#if") == 0) {
				if (if_true == 0)
					continue;
				this_if_true = 0;
				key = strtok(NULL, "\n");
				if (key[0] == '0') {
					this_if_true = 0;
					if_true = 0;
				} else
					refactor_digit(key, &if_true,
					&this_if_true, h);
			} else if (strcmp(token, "#else") == 0) {
				if_true = 1;
				this_if_true = 1 - this_if_true;
			} else if (strcmp(token, "#elif") == 0) {
				if (if_true)
					this_if_true = 0;
				this_if_true = 0;
				key = strtok(NULL, "\n");
				if (key[0] == '0')
					this_if_true = 0;
				else
					refactor_digit(key, &if_true,
					&this_if_true, h);
			} else if (strcmp(token, "#endif") == 0) {
				this_if_true = 1;
				if_true = 1;
				} else if (strcmp(token, "#ifdef") == 0) {
					if (this_if_true == 0)
						continue;
					key = strtok(NULL, "\n");
					val = get_hash_table(h, key);
					this_if_true = val != NULL;
					if_true = val != NULL;
					if (val)
						free(val);
				} else if (strcmp(token, "#ifndef") == 0) {
					if (this_if_true == 0)
						continue;
					key = strtok(NULL, "\n");
					val = get_hash_table(h, key);
					this_if_true = val == NULL;
					if_true = val == NULL;
					if (val)
						free(val);
			} else if (strcmp(token, "#undef") == 0) {
				if (this_if_true == 0)
					continue;
				key = strtok(NULL, "\n");
				erase_hash_table(h, key);
			} else if (strcmp(token, "#include") == 0) {
				if (this_if_true == 0)
					continue;
				key = strtok(NULL, "\n\" ");
				c_dir = get_current_dir(key);
				include_in = fopen(c_dir, "r");
				if (c_dir)
					free(c_dir);
				if (include_in) {
					print_include(include_in, out,
					  num_includes, includes, h);
					continue;
				}
				for (i = 0; i < num_includes; ++i) {
					n = get_full_dir(includes[i], key);
					include_in = fopen(n, "r");
					if (include_in) {
						print_include(include_in, out,
						    num_includes, includes, h);
						free(n);
						goto read_line;
					}
					free(n);
				}
				exit(12);
			}
		} else if (this_if_true) {
			new_line = get_line(line, h);
			fprintf(out, "%s", new_line);
			free(new_line);
		}
	}
}

static void get_define(char *key, struct hashTable *h)
{
	char *def = strtok(key, " =");
	char *val = strtok(NULL, " =");

	if (val == NULL)
		val = (char *)"";
	insert_hash_table(h, def, val);
}

void command_line_parrser(int argc, char **argv, char ***includes,
		char **source, char **dest, struct hashTable *h,
		int *includes_count)
{
	int num_i = 0;
	int i = 0;
	int last_d = 0;
	int last_o = 0;
	int last_i = 0;
	int num_includes = 0;

	for (i = 0; i < argc; ++i) {
		if (strlen(argv[i]) >= 2 && argv[i][0] == '-') {
			if (argv[i][1] == 'I')
				num_i++;
		}
	}
	*includes_count = num_i;
	if (num_i) {
		*includes = (char **)malloc(num_i * sizeof(char *));
			if (*includes == NULL)
				exit(12);
	}
	for (i = 1; i < argc; ++i) {
		if (last_d == 1) {
			get_define(argv[i], h);
			last_d = 0;
			last_i = 0;
			last_o = 0;
		} else if (last_i == 1) {
			(*includes)[num_includes++] = argv[i];
			last_d = 0;
			last_i = 0;
			last_o = 0;
		} else if (last_o == 1) {
			*dest = argv[i];
			last_d = 0;
			last_i = 0;
			last_o = 0;
		} else if (strlen(argv[i]) >= 2 && argv[i][0] == '-') {
			if (argv[i][1] == 'D') {
				if (strlen(argv[i]) == 2)
					last_d = 1;
				if (strlen(argv[i]) > 2)
					get_define(argv[i] + 2, h);
				continue;
			} else if (argv[i][1] == 'I') {
				if (strlen(argv[i]) == 2)
					last_i = 1;
				if (strlen(argv[i]) > 2)
					(*includes)[num_includes++] = argv[i] +
					2;
				continue;
			} else if (argv[i][1] == 'o') {
				if (strlen(argv[i]) == 2)
					last_o = 1;
				if (strlen(argv[i]) > 2)
					*dest = argv[i] + 2;
				continue;
				}
		} else {
			if (i == argc - 1 && *source != NULL)
				*dest = argv[i];
			else
				*source = argv[i];
		}
	}
}
