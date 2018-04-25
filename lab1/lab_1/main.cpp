#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILE_OLD	"input.txt"
#define FILE		"in.txt"
#define FILE_RES	"out.txt"
#define FILE_RES_2	"out2.txt"

using namespace std;

int getFileSize(const char * fileName)
{
    struct stat file_stat;
    stat(fileName, &file_stat);
    return file_stat.st_size;
}

int main(void)
{
    clock_t begin, end;
    begin = clock();
    ifstream in_first(FILE_OLD);

    if (!in_first.is_open()) {
        cout << "Can not open first file" << endl;
        return 0;
    }
    in_first.close();

    if (rename(FILE_OLD, FILE) != 0) {
        cout << "Rename error" << endl;
        return 0;
    }

    if (mkdir("tmp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        cout << "Folder has been created" << endl;
    } else {
        cout << "Folder creating error" << endl;
    }

    ifstream in(FILE);
    ofstream out(FILE_RES);
    ofstream out2(FILE_RES_2);
    string tmp;
    cout << "File size: " << getFileSize(FILE) << endl;

    cout << "Copying file... ";

    while (!in.eof()) {
        in >> tmp;
        out << tmp;
        out2 << tmp;
    }
    in.close();
    out.close();
    out2.close();
    cout << "OK" << endl;

    cout << "Removing file `out2.txt`... ";
    remove(FILE_RES_2);
    cout << "OK" << endl;

    end = clock();
    long double ttl_time = (long double) (end - begin) / (long double) CLOCKS_PER_SEC;
    cout << "Working time: " << ttl_time << endl;

    return 0;
}
