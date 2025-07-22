#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


typedef struct {
    bool is_case_ins;
    bool is_only_lines;
} Flags;


char* strcasestr(const char* haystack, const char* needle) {
    if (haystack == NULL || needle == NULL) return NULL;

    size_t needle_len = strlen(needle);

    if (needle_len == 0) return (char*)haystack;

    while (*haystack) {
        if (strncasecmp(haystack, needle, needle_len) ==0) {
            return (char*)haystack;
        }
        haystack++;

    }

    return NULL;
}

void grep(FILE* src, Flags flags, const char* pattern) {
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, src) != -1) {
        bool match = false;

        if (flags.is_case_ins) {
            if (strcasestr(NULL, pattern)) match = true;
        } else {
            if (strstr(line, pattern)) match = true;
        }

        if (match) printf("%s", line);
    }

    free(line);
}


int main(int argc, char* argv[]) {
    int opt;
    Flags flags = { false, false };

    while ((opt = getopt(argc, argv, "in")) != -1) {
        switch (opt) {
            case 'i': flags.is_case_ins = true; break;
            case 'n': flags.is_only_lines = true; break;
            default:
                fprintf(stderr, "Usage ...\n");
                return 1;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [flags] pattern [file...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* pattern = argv[optind++];

    if (argc == optind || (argc > optind && strcmp("-", argv[optind]) == 0)) {
        grep(stdin, flags, pattern);
    } else if (argc > optind) {
        for (int i = optind; i < argc; i++) {
            FILE* src = fopen(argv[i], "r");

            if (!src) {
                fprintf(stderr, "Unable to open a file %s\n", argv[i]);
                continue;
            }

            grep(src, flags, pattern);
            fclose(src);
        }
    }

}
