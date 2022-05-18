#include "ExternalAccessor.h"

using namespace std;

ExternalAccessor::ExternalAccessor(string relativePath, Mount* mount) {
    this->mount = mount;
    this->path = joinPath(mount->mountPath, relativePath);
    this->_realPath = joinPath(mount->realFsPath, relativePath);
}

void ExternalAccessor::ensureExists() {
    ifstream f(this->_realPath);

    if (!f.good()) {
        auto parentDir = getParentPath(this->_realPath);
        char buff[250];

        parentDir.copy(buff, parentDir.size());

        if (!filesystem::exists(parentDir)) {
            filesystem::create_directories(parentDir);
        }
    }
}

void ExternalAccessor::writeData(char* data, long size) {
    this->ensureExists();

    ofstream f(this->_realPath, ios::binary);
    f.write(data, size);
    f.close();
}

char* ExternalAccessor::readData() {
    ifstream f(this->_realPath, ios::binary | ios::ate);

    this->size = f.tellg();
    f.seekg(0);

    auto buffer = new char[this->size];

    f.read(buffer, size);
    f.close();

    return buffer;
}

void ExternalAccessor::rename(string newName) {
    auto parentDir = getParentPath(this->_realPath);
    auto newPath = joinPath(parentDir, newName);

    filesystem::rename(this->_realPath, newPath);
}

void ExternalAccessor::copyTo(FileAccessor* target) {
    target->ensureExists();

    auto data = this->readData();

    if (data != nullptr) {
        target->writeData(data, this->size);
    }
}

string* ExternalAccessor::readDataAsString() {
    return new string(this->readData());
}

vector<File*>* ExternalAccessor::listFiles() {
    auto result = new vector<File*>();

    for (auto f : filesystem::directory_iterator(this->_realPath)) {
        auto name = f.path().filename().string();

        auto file = new File();

        name.copy(file->name, name.size());
        file->size = f.file_size();
        file->isDir = f.is_directory();

        result->push_back(file);
    }

    return result;
}

void ExternalAccessor::remove() {
    if (filesystem::exists(this->_realPath)) {
        filesystem::remove(this->_realPath);
    }
}
