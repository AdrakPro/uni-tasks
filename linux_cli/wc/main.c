#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    bool is_bytes;
    bool is_chars;
    bool is_lines;
    bool is_max_line_len;
    bool is_words;
} Flags;

void process_file(FILE* fp, const char* filename, Flags* flags) {
    size_t chars = 0;
    size_t bytes = 0;
    size_t lines = 0;
    size_t max_line_len = 0;
    size_t words = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, fp)) != -1) {
        lines++;

         // Inside while ((nread = getline(&line, &len, fp)) != -1) {
        size_t line_len = nread;
        // Exclude newline character from line length, if present
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line_len--;
         }

        if (line_len > max_line_len) {
            max_line_len = line_len;
        }
        chars += nread;
        bytes += nread; // For ASCII, chars == bytes

        // Word count
        bool in_word = false;
        for (ssize_t i = 0; i < nread; i++) {
            if (isspace((unsigned char)line[i])) {
                in_word = false;
            } else if (!in_word) {
                in_word = true;
                words++;
            }
        }
    }

    if (flags->is_lines) {
        printf("%zu ", lines);
    }
    if (flags->is_words) {
        printf("%zu ", words);
    }
    if (flags->is_chars) {
        printf("%zu ", chars);
    }
    if (flags->is_bytes) {
        printf("%zu ", bytes);
    }
    if (flags->is_max_line_len) {
        printf("%zu ", max_line_len);
    }
    if (filename) {
        printf("%s\n", filename);
    } else {
        printf("\n");
    }
    free(line);
}

int main(int argc, char* argv[]) {
    int opt;
    Flags flags = { false, false, false, false, false };

    while ((opt = getopt(argc, argv, "cmlLw")) != -1) {
        switch (opt) {
            case 'c': flags.is_chars = true; break;
            case 'm': flags.is_bytes = true; break;
            case 'l': flags.is_lines = true; break;
            case 'L': flags.is_max_line_len = true; break;
            case 'w': flags.is_words = true; break;
        }
    }
    // If no flags specified, default to lines, words, bytes
    if (!flags.is_lines && !flags.is_words && !flags.is_chars && !flags.is_bytes && !flags.is_max_line_len) {
        flags.is_lines = flags.is_words = flags.is_bytes = true;
    }

    if (argc == optind || (argv[optind] && strcmp(argv[optind], "-") == 0)) {
        process_file(stdin, NULL, &flags);
    } else if (argc > optind) {
        for (int i = optind; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");
            if (!fp) {
                fprintf(stderr, "Unable to open file %s.\n", argv[i]);
                continue;
            }
            process_file(fp, argv[i], &flags);
            fclose(fp);
        }
    }
    return 0;
}
