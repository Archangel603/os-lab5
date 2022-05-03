#include "DirectAccessor.h"

DirectAccessor::DirectAccessor(string path, File* file, vector<File*>* files, unordered_map<long, char*>* data) {
    this->_file = file;
    this->_files = files;
    this->_data = data;

    this->name = file->name;
    this->isDir = this->_file->isDir;
    this->path = std::move(path);
    this->size = file->size;
}

void DirectAccessor::ensureExists() {
    auto existing = getFile(this->_files, this->_file->id);

    if (!existing) {
        auto parent = getFile(this->_files, getParentPath(this->path));

        if (!parent) {
            cout << "Unable to create file" << endl;
            return;
        }

        this->_file->parentId = parent->id;
        this->_files->push_back(this->_file);
    }
}

void DirectAccessor::writeData(char* data, long size) {
    this->ensureExists();

    this->_file->size = size;
    this->size = size;

    (*this->_data)[this->_file->id] = data;
}

char* DirectAccessor::readData() {
    if (this->_data->count(this->_file->id) == 0) {
        return nullptr;
    }

    return this->_data->at(this->_file->id);
}

void DirectAccessor::rename(string newName) {
    this->_file->clearName();
    newName.copy(this->_file->name, newName.size());
    this->name = this->_file->name;
}

void DirectAccessor::copyTo(FileAccessor* target) {
    target->ensureExists();

    char* data = this->readData();

    if (data != nullptr) {
        target->writeData(data, this->size);
    }
}

string* DirectAccessor::readDataAsString() {
    return new string(this->readData(), this->size);
}

vector<File*>* DirectAccessor::listFiles() {
    return getFilesByParent(this->_files, this->_file);
}

void DirectAccessor::remove() {
    if (this->isDir) {
        auto children = getFilesByParent(this->_files, this->_file);

        if (!children->empty()) {
            cout << "Directory is not empty" << endl;
            return;
        }
    }

    this->_data->erase(this->_file->id);
    removeFromVector(this->_files, this->_file);
}