
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Stowrzyc circular buffer
// Czytac linijki od gory lecz w bufferze zostaje tyle ile jest n
void tail(FILE* fp, long n, const char* filename) {
    char** buffer = calloc(n, sizeof(char*));

    if (!buffer) {
        fprintf(stderr, "Calloc failed to allocate memory\n");
        exit(1);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t n_read;
    long count = 0;

    while ((n_read = getline(&line, &len, fp)) != -1 ) {
        free(buffer[count % n]);
        buffer[count % n] = strdup(line);
        count++;
    }

    long start = count > n ? count - n : 0;

    printf("=== %s ===\n", filename);

    for (long i = start; i < count; i++) {
        fputs(buffer[i % n], stdout);
        free(buffer[i % n]);
    }
    
    free(buffer);
    free(line); 
    
}

int main(int argc, char* argv[]) {
    int opt;
    long n = 1;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n': {
                long lines_to_read = strtol(optarg, NULL, 10);
                if (lines_to_read <= 0) {
                    fprintf(stderr, "-n value must be a non zero positive!\n");
                    return 1;
                }
                n = lines_to_read;
                break;
            }
        }
    }

    if (argc == optind || (argc > optind && strcmp(argv[optind], "-") == 0)) {
        tail(stdin, n, "standard input");
    } else if (argc > optind) {
        for (int i = optind; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");

            if (!fp) {
                fprintf(stderr, "Unable to open a file %s\n", argv[i]);
                continue;
            }

            tail(fp, n, argv[i]);
            fclose(fp);
        }
    }



    return 0;
}
