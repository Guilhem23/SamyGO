
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int i;

	for (;;) {
		if (argc == 1) {
			if (fputs("y\n", stdout) == EOF)
				exit(1);
		} else {
			for (i = 1; i < argc; i++)
				if (fputs(argv[i], stdout) == EOF || fputc(i == argc - 1 ? '\n' : ' ', stdout) == EOF)
					exit(1);
		}
	}

	return 0;
}

