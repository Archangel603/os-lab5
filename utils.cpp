#include "utils.h"

void trimStartPathDelim(string& path) {
    if (path.empty())
        return;

    if (path[0] == '/') {
        if (path.size() == 1) {
            path = "";
            return;
        }

        path = path.substr(1, path.size() - 1);
    }
}

void trimEndPathDelim(string& path) {
    if (path.empty())
        return;

    if (path[path.size() - 1] == '/') {
        path = path.substr(0, path.size() - 1);
    }
}

string joinPath(string path1, string path2) {
    trimEndPathDelim(path1);
    trimStartPathDelim(path2);
    return path1 + "/" + path2;
}

vector<string>* split(string input, const string& delimiter) {
    auto res = new vector<string>();
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        res->push_back(token);

        input.erase(0, pos + delimiter.length());
    }

    res->push_back(input);

    return res;
}

string getParentPath(string path) {
    auto parts = split(path, "/");

    string result = (*parts)[0];

    for (int i = 1; i < parts->size() - 1; i++) {
        result = joinPath(result, (*parts)[i]);
    }

    return result;
}

string getFilename(string& path) {
    auto parts = split(path, "/");
    return (*parts)[parts->size() - 1];
}

vector<File*>* getFilesByParent(vector<File*>* files, File* parent) {
    auto v = new vector<File*>();

    long parentId = parent == nullptr ? 0 : parent->id;

    for (auto f : *files) {
        if (f->parentId == parentId) {
            v->push_back(f);
        }
    }

    return v;
}

File* getFile(vector<File*>* files, string name, File* parent) {
    auto children = getFilesByParent(files, parent);

    for (auto f : *children) {
        if (string(f->name) == name) {
            return f;
        }
    }

    return nullptr;
}

File* getFile(vector<File*>* files, long id) {
    for (auto f : *files) {
        if (f->id == id) {
            return f;
        }
    }

    return nullptr;
}

File* getFile(vector<File*>* files, string path) {
    trimEndPathDelim(path);
    auto parts = split(path, "/");
    File* result = nullptr;

    for (auto name : *parts) {
        result = getFile(files, name, result);

        if (result == nullptr)
            return nullptr;
    }

    return result;
}