#ifndef LAB5_DIRECTACCESSOR_H
#define LAB5_DIRECTACCESSOR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "FileAccessor.h"
#include "utils.h"
#include "File.h"

class DirectAccessor : public FileAccessor {

public:
    DirectAccessor(string path, File* file, vector<File*>* files, unordered_map<long, char*>* data);

    void ensureExists() override;

    void writeData(char *data, long size) override;

    char* readData() override;

    void rename(string newName) override;

    void copyTo(FileAccessor *target) override;

    string* readDataAsString() override;

    vector<File*>* listFiles() override;

    void remove() override;

private:
    File* _file;
    vector<File*>* _files;
    unordered_map<long, char*>* _data;
};


#endif //LAB5_DIRECTACCESSOR_H
