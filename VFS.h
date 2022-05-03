#ifndef LAB5_VFS_H
#define LAB5_VFS_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include "File.h"
#include "FileAccessor.h"
#include "Mount.h"
#include "utils.h"
#include "DirectAccessor.h"
#include "ExternalAccessor.h"

using namespace std;

class VFS {

public:
    string getCwd();

    void init(string* existing);
    void pwd();
    void listDir();
    void changeDir(string path);

    void mount(string realPath, string mountPath);
    void umount(string mountPath);

    void writeToFile(string path, string content);

    // variant 1
    void renameFile(string path, string newName);
    void copyFile(string srcPath, string dstPath);
    void removeFile(string path);

    void save(string& realPath);
    void load(string& realPath);

private:
    long _idGen = 1;
    vector<File*>* _files = new vector<File*>();
    vector<Mount*>* _mounts = new vector<Mount*>();
    unordered_map<long, char*>* _data  = new unordered_map<long, char*>();
    string _currentDir = "/";

    FileAccessor* getAccessor(string& path, bool isDir);
    long getNextId();
    string toFullPath(string path);
    ExternalAccessor* tryGetExternalAccessor(string& path);
};


#endif //LAB5_VFS_H
