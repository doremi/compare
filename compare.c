#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

size_t countlines(const char *file) {
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen(file, "r");
    if (!fp) {
        perror("fopen");
        return 0;
    }
    size_t lines = 0;
    while (getline(&line, &len, fp) != -1) {
        lines++;
    }
    if (line) {
        free(line);
    }
    fclose(fp);
    return lines;
}

bool exist(char ***buf, const char *str, size_t lines) {
    char **buffer = *buf;
    for (size_t i = 0; i < lines; ++i) {
        if (strcmp(buffer[i], str) == 0) {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s fileA fileB\n", argv[0]);
        return 1;
    }

    const char *filea = argv[1];
    const char *fileb = argv[2];
    FILE *fpa = fopen(filea, "r");
    if (!fpa) {
        perror(filea);
        return 2;
    }
    FILE *fpb = fopen(fileb, "r");
    if (!fpb) {
        fclose(fpa);
        perror(fileb);
        return 3;
    }

    // read all fileb into memory
    size_t lines = countlines(fileb);
    printf("lines: %lu\n", lines);

    int index = 0;
    size_t len = 0;
    ssize_t nread = 0;
    char **buffer = (char**)calloc(lines, sizeof(char**));
    printf("Reading %s...\n", fileb);
    while ((nread = getline(&buffer[index], &len, fpb)) != -1) {
        buffer[index][nread-1] = '\0'; // remove newline
        index++;
    }
    printf("Done.\n");

    const size_t filea_lines = countlines(filea);
    int i = 1;
    int exists = 0;
    char *line = NULL;
    len = 0;
    while ((nread = getline(&line, &len, fpa)) != -1) {
        line[nread-1] = '\0';
        if (exist(&buffer, line, index)) {
            ++exists;
        }
        printf("(%d/%ld)\r", i++, filea_lines);
    }
    printf("\nexists: %d\n", exists);
    free(line);
    for (int b = 0; b < index; ++b) {
        free(buffer[b]);
    }
    free(buffer);
    fclose(fpa);
    fclose(fpb);
    return 0;
}
