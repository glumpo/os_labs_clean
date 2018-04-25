#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "help.h"
#include <errno.h>
#include <assert.h>

typedef struct {
    int fd;
    int min;
    int max;
} TFILE;

TFILE * OpenFile(char *path, int min, int max) {
    TFILE *file = (TFILE *)malloc(sizeof(TFILE));
    file->fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    file->min = min;
    file->max = max;

    if (file->fd < 0) {
        free(file);
        file = NULL;
    }

    return file;
}

int IsPrintable(char c) {
    return (c >= 33);
}
void PrintStatus(TFILE *file) {
    int symbolsCount = 0;
    int linesCount = 0;
    char c;

    while (read(file->fd, &c, 1) == 1) {
        if (IsPrintable(c)) {
            ++symbolsCount;
        } else if (c == '\n') {
            ++linesCount;
        }
    }

    if (c != '\n') {
        ++linesCount;
    }

    if (linesCount == 0) {
        ++linesCount;
    }

    printf("Number of lines: %d, number of symbols: %d\n", linesCount, symbolsCount);
    lseek(file->fd, 0, SEEK_SET);
}

void FindPattern(TFILE *file, char *mode, char *pattern) {
    char c;
    int i = 0;
    int len = strlen(pattern);

    struct stat stat;
    fstat(file->fd, &stat);
    lseek(file->fd, 0, SEEK_SET);
    int size = stat.st_size;

    char *tmp = (char *)mmap(NULL, size, PROT_READ, MAP_SHARED, file->fd, SEEK_SET);

    if (strcmp(mode, "/etc") == 0 || 1) {
        for (int i = 0; i < size - len + 1; ++i) {
            for (int j = 0; j < len; ++j) {
                if (pattern[j] != tmp[i + j]) {
                    if (strcmp(mode, "/begin") == 0) {
                        ++i;
                    }
                    break;
                }
                if (j == len - 1) {
                    if (strcmp(mode, "/end") == 0) {
                        if (i + j != size - 1 && IsPrintable(tmp[i + j + 1])) {
                            break;
                        }
                    }

                    printf("Yes\n");
                    lseek(file->fd, 0, SEEK_SET);
                    return;
                }
            }
        }
        printf("No\n");
    }

    lseek(file->fd, 0, SEEK_SET);
}

void PrintLine(TFILE *file, int number) {
    char c;
    int tmp = number;

    while (number != 1 && read(file->fd, &c, 1) == 1) {
        if (c == '\n') {
            --number;
        }
    }

    if (c == '\n') {
        ++number;
    }

    if (number != 1) {
        printf("Wrong line\n");
        return;
    }

    printf("Line %d: ", tmp);

    while (read(file->fd, &c, 1) == 1 && c != '\n') {
        printf("%c", c);
    }
    printf("\n");
    lseek(file->fd, 0, SEEK_SET);
}

void WriteWithin(TFILE *file, int position, char *text) {
    --position;
    lseek(file->fd, position, SEEK_SET);
    int fd = open("./", __O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    assert(fd >= 0);

    char c;
    while (read(file->fd, &c, 1) == 1) {
        assert(write(fd, &c, 1) == 1);
    }

    assert(ftruncate(file->fd, position) >= 0);

    lseek(file->fd, position, SEEK_SET);
    if (write(file->fd, text, strlen(text)) != strlen(text)) {
        printf("Can`t write in this file\n");
        lseek(file->fd, position, SEEK_SET);
    }

    lseek(fd, 0, SEEK_SET);
    while (read(fd, &c, 1) == 1) {
        if (write(file->fd, &c, 1) != 1) {
            printf("Can`t write in this file\n");
            return;
        }
    }
    lseek(file->fd, 0, SEEK_SET);
}

void EraseWithin(TFILE *file, int position, int count) {
    --position;
    lseek(file->fd, position + count, SEEK_SET);
    int fd = open("./", __O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("Can`t create temporary file\n");
        return;
    }

    char c;
    while (read(file->fd, &c, 1) == 1) {
        if (write(fd, &c, 1) != 1) {
            printf("Can`t write in temporary file\n");
            return;
        }
    }

    if (ftruncate(file->fd, position) < 0) {
        printf("Can`t erase from that position\n");
        return;
    }

    lseek(file->fd, position, SEEK_SET);
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &c, 1) == 1) {
        if (write(file->fd, &c, 1) != 1) {
            printf("Can`t write in this file\n");
            return;
        }
    }
    lseek(file->fd, 0, SEEK_SET);
}

void SetLimit(TFILE *file, int min, int max) {
    int i = 0;
    int c;
    while (read(file->fd, &c, 1) == 1) {
        ++i;

        if (i + 1 > max) {
            break;
        }
    }

    if (i + 1 > max) {
        if (ftruncate(file->fd, max) < 0) {
            printf("Can`t trunc file\n");
            return;
        }
    } else if (i < min) {
        while (i < min) {
            if (write(file->fd, "0", 1) != 1) {
                return;
            }
            ++i;
        }
    }
    lseek(file->fd, 0, SEEK_SET);
}

void PrintFile(TFILE *file) {
    char c;
    while (read(file->fd, &c, 1) == 1) {
        printf("%c", c);
    }
    lseek(file->fd, 0, SEEK_SET);
}

int main(int argc, char *argv[]) {
    TFILE *cur = NULL;


    if (argc <= 2)
        PrintUsageAndExit();


    switch(CommandCode(argv[1])) {
    case STATUS:
        if (argc == 4 && CommandCode(argv[2]) == FILE) {

            cur = OpenFile(argv[3], DEFAULT_MIN, DEFAULT_MAX);
            assert(cur != NULL);

            PrintStatus(cur);
            return 0;
        } else {
            PrintWrongAndExit();
        }
        break;
    case GETLINE:
        if (argc == 5 &&
                IsDecimal(argv[2]) && CommandCode(argv[3]) == FILE) {

            cur = OpenFile(argv[4], DEFAULT_MIN, DEFAULT_MAX);
            if (cur == NULL) {
                printf("Can`t open file\n");
                return 2;
            }

            PrintLine(cur, atoi(argv[2]));
            return 0;
        } else {
            PrintWrongAndExit();
        }
        break;
    case FIND:
        if (argc == 6
                && CommandCode(argv[2]) == WHERETOSEARCH && CommandCode(argv[4]) == FILE) {

            cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
            if (cur == NULL) {
                printf("Can`t open file\n");
                return 2;
            }

            FindPattern(cur, argv[2], argv[3]);
            return 0;
        } else {
            PrintWrongAndExit();
        }
        break;
    case WRITE:
        if (argc == 6 && IsDecimal(argv[2]) && CommandCode(argv[4]) == FILE) {
            cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
            assert(cur != NULL);
            WriteWithin(cur, atoi(argv[2]), argv[3]);
            return 0;
        } else {
            PrintWrongAndExit();
        }
        break;
    case ERASE:
        if (argc == 6 && IsDecimal(argv[2]) && IsDecimal(argv[3]) &&
                CommandCode(argv[4]) == FILE) {

            cur = OpenFile(argv[5], DEFAULT_MIN, DEFAULT_MAX);
            if (cur == NULL) {
                printf("Can`t open file\n");
                return 2;
            }

            EraseWithin(cur, atoi(argv[2]), atoi(argv[3]));
            return 0;
        } else {
            PrintWrongAndExit();
        }
        break;
    case LIMIT:
        if (argc == 6 && IsDecimal(argv[2]) &&
                IsDecimal(argv[3]) && CommandCode(argv[4]) == FILE) {

            cur = OpenFile(argv[5], atoi(argv[2]), atoi(argv[3]));
            assert(cur != NULL);

            SetLimit(cur, atoi(argv[2]), atoi(argv[3]));
        } else {
            PrintWrongAndExit();
        }
        break;
    case PRINT:
        if (argc == 4
                && CommandCode(argv[2]) == FILE) {

            cur = OpenFile(argv[3], DEFAULT_MIN, DEFAULT_MAX);
            assert(cur != NULL);
            PrintFile(cur);
        } else {
            PrintWrongAndExit();
        }
        break;
    default:
        PrintWrongAndExit();
        break;
    }
    return 0;
}
