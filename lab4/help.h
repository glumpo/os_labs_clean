#ifndef HELP_H
#define HELP_H

#define HELP 1
#define EXIT 2
#define SWITCH 3
#define STATUS 4
#define GETLINE 5
#define FIND 6
#define WHERETOSEARCH 7
#define BEGIN 700
#define END 800
#define ETC 900
#define WRITE 10
#define ERASE 11
#define LIMIT 12
#define FILE 13
#define PRINT 14
#define NEWLINE 15
#define CLEAR 16
#define CLOSE 17
#define NOTCOMMAND -1

int IsDecimal(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
        ++i;
    }
    return 1;
}

int CommandCode(char *string) {
    if (strcmp(string, "--help") == 0) {
        return HELP;
    } else if (strcmp(string, "--exit") == 0) {
        return EXIT;
    } else if (strcmp(string, "--status") == 0) {
        return STATUS;
    } else if (strcmp(string, "--getline") == 0) {
        return GETLINE;
    } else if (strcmp(string, "--file") == 0) {
        return FILE;
    } else if (strcmp(string, "--find") == 0) {
        return FIND;
    } else if (strcmp(string, "-b") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "-e") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "-c") == 0) {
        return WHERETOSEARCH;
    } else if (strcmp(string, "--write") == 0) {
        return WRITE;
    } else if (strcmp(string, "--erase") == 0) {
        return ERASE;
    } else if (strcmp(string, "--limit") == 0) {
        return LIMIT;
    } else if (strcmp(string, "--print") == 0) {
        return PRINT;
    } else if (strcmp(string, "\n") == 0) {
        return NEWLINE;
    } else {
        return NOTCOMMAND;
    }
}

void PrintUsageAndExit() {
    printf("Usage:\n"
           "./a.out --command --file \"path\"\n"
           "commands:\n"
           "    --help - print usage\n"
           "    --status - print lines' and symbols' count\n"
           "    --getline \"number of line\" - print line\n"
           "    --find {-b/-e/-c} \"pattern\" - find word, which the pattern begins/ends/contains\n"
           "    --write \"position\" \"text\" - write text at the position\n"
           "    --erase \"position\" \"number of symbols\" - delete some symbols\n"
           "    --limit \"min number of symbols\" \"max number of symbols\" - set up min and max size of file\n");
    exit(0);
}

void PrintWrongAndExit() {
    printf("Wrong command\n"
           "Type --help for usage\n");
    exit(1);
}

const int DEFAULT_MIN = 0;
const int DEFAULT_MAX = __INT_MAX__;
#endif // HELP_H
