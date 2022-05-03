#ifndef LAB5_UTILS_H
#define LAB5_UTILS_H

#include <string>
#include <vector>
#include "File.h"

using namespace std;

void trimStartPathDelim(string& path);

void trimEndPathDelim(string& path);

string joinPath(string path1, string path2);

vector<string>* split(string input, const string& delimiter);

string getParentPath(string path);

string getFilename(string& path);

vector<File*>* getFilesByParent(vector<File*>* files, File* parent);

File* getFile(vector<File*>* files, string name, File* parent);

File* getFile(vector<File*>* files, long id);

File* getFile(vector<File*>* files, string path);

string buildPath(vector<File*>* files, File* file);

#endif //LAB5_UTILS_H
