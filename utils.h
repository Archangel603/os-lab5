#ifndef LAB5_UTILS_H
#define LAB5_UTILS_H

#include <string>
#include <vector>
#include "File.h"

using namespace std;

template<typename T>
void removeFromVector(vector<T>* vec, T item) {
    vec->erase(
            std::remove(
                    vec->begin(),
                    vec->end(),
                    item
            ),
            vec->end()
    );
}

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

#endif //LAB5_UTILS_H
