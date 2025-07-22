// uniq
// Read lines from a file (or stdin if no file is specified).
// By default, print only unique consecutive lines (i.e., if two or more consecutive lines are identical, print only the first).
// Support a -c option: If given, prefix lines by the number of consecutive occurrences (like uniq -c).

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

typedef struct {
    char* line;
    size_t count;
} Map;

void uniq(FILE* fp, bool is_counting) {
    // Initial static initalization
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    size_t consecutive = 0;
    size_t lines_count = 0;
    size_t total_lines = 0;

    // find the number of lines to allocate
    while (getline(&line, &len, fp) != -1) {
        total_lines++;
    }

    rewind(fp);
    line = NULL;
    len = 0;

    // Read first line
    Map* line_map = calloc(total_lines, sizeof(Map));
    getline(&line, &len, fp);
    Map first_entry = { strdup(line), 1 };
    line_map[lines_count++] = first_entry; 
    
    while ((nread = getline(&line, &len, fp)) != -1) {
        if (strcmp(line_map[lines_count - 1].line, line) != 0) {
            consecutive = 0;
            Map entry = { strdup(line), ++consecutive };
            line_map[lines_count++] = entry;
        } else {
            line_map[lines_count - 1].count++; 
        }
    }


    for (size_t i = 0; i < lines_count; i++) {
        if (is_counting) {
            printf("%zu %s", line_map[i].count, line_map[i].line);
        } else {
            printf("%s", line_map[i].line);
        }
        free(line_map[i].line);
    }

    // cleanup
    free(line_map);
    free(line);
}

int main(int argc, char* argv[]) {
    int opt;
    bool is_counting = false;

    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt) {
            case 'c': is_counting = true; break;
        }
    }

    if (argc == optind || (argc > optind && strcmp(argv[optind], "-") == 0)) {
        uniq(stdin, is_counting);
    } else if (argc > optind) {
        for (int i = optind; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");

            if (!fp) {
                fprintf(stderr, "Unable to open a file %s\n", argv[i]);
                continue;
            }

            uniq(fp, is_counting);
            fclose(fp);
        }
    }

}
