//
// Created by mikha on 03.05.2022.
//

#ifndef LAB5_EXTERNALACCESSOR_H
#define LAB5_EXTERNALACCESSOR_H

#include "FileAccessor.h"
#include "Mount.h"
#include <vector>
#include <unordered_map>
#include "iostream"
#include "iomanip"
#include "fstream"
#include "sys/stat.h"
#include <sys/types.h>
#include <filesystem>
#include "utils.h"

class ExternalAccessor : public FileAccessor {
public:
    Mount* mount;

    ExternalAccessor(string relativePath, Mount* mount);

    void ensureExists() override;

    void writeData(char *data, long size) override;

    char* readData() override;

    void rename(string newName) override;

    void copyTo(FileAccessor *target) override;

    string* readDataAsString() override;

    vector<File*>* listFiles() override;

    void remove() override;

private:
    string _realPath;

};


#endif //LAB5_EXTERNALACCESSOR_H
