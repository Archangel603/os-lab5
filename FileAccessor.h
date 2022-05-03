#ifndef LAB5_FILEACCESSOR_H
#define LAB5_FILEACCESSOR_H

#include <string>
#include "File.h"
#include <vector>

using namespace std;

class FileAccessor {
public:
    string name;
    string path;
    long size;
    bool isDir;

    virtual void ensureExists() = 0;
    virtual void writeData(char* data, long size) = 0;
    virtual char* readData() = 0;
    virtual void rename(string newName) = 0;
    virtual void copyTo(FileAccessor* target) = 0;
    virtual string* readDataAsString() = 0;
    virtual vector<File*>* listFiles() = 0;
    virtual void remove() = 0;
};

#endif //LAB5_FILEACCESSOR_H
