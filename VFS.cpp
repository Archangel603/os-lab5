#include "VFS.h"

using namespace std;

string VFS::getCwd() {
    return this->_currentDir;
}

void VFS::init(string *existing) {
    if (existing != nullptr) {
        this->load(*existing);
        return;
    }

    this->_files->push_back(new File {
        1, "", 0, 0, true
    });
    this->_files->push_back(new File {
        2, "file1.sh", 1,  0, false
    });
    this->_files->push_back(new File {
        3, "doc.txt", 1,  0, false
    });
    this->_files->push_back(new File {
        4, "dir1", 1,  0, true
    });
    this->_files->push_back(new File {
        5, "dz.docx", 4,  0, false
    });
    this->_files->push_back(new File {
        6, "another_dir", 4, 0, true
    });
    this->_idGen = 7;

    this->writeToFile("file1.sh", "echo \"Hello, world!\"");
    this->writeToFile("doc.txt", "<You can see here a recipe of very tasty waffles>");
    this->writeToFile("/dir1/dz.docx", "Nasty document");
}

void VFS::pwd() {
    cout << this->_currentDir << endl;
}

void VFS::listDir() {
    auto accessor = this->getAccessor(this->_currentDir, true);
    auto children = accessor->listFiles();

    if (children->empty()) {
        cout << "Directory is empty" << endl;
        return;
    }

    cout << left
         << setw(4) << "TYPE" << "\t"
         << setw(30) << "NAME" << "\t"
         << "SIZE" << endl;

    for (auto c : *children) {
        const char* type = c->isDir ? "DIR" : "FILE";
        cout << left
            << setw(4) << type << "\t"
            << setw(30) << c->name << "\t"
            << c->size << endl;
    }
}

void VFS::changeDir(string path) {

    if (path.empty()) {
        cout << "Unable to change dir" << endl;
        return;
    }

    if (path == "..") {
        if (this->_currentDir == "/") {
            cout << "Unable to change dir" << endl;
            return;
        }

        this->_currentDir = getParentPath(this->_currentDir);

        return;
    }

    path = this->toFullPath(path);
    auto testAccessor = this->getAccessor(path, false);

    if (testAccessor == nullptr) {
        cout << "Target does not exist" << endl;
        return;
    }

    if (!testAccessor->isDir) {
        cout << "Target is not a directory" << endl;
        return;
    }

    this->_currentDir = path;
}

void VFS::mount(string realPath, string mountPath) {
    auto fullPath = this->toFullPath(mountPath);
    auto name = getFilename(fullPath);
    auto parentPath = getParentPath(fullPath);
    auto parentDir = getFile(this->_files, parentPath);

    auto f = new File();
    f->id = this->getNextId();
    f->parentId = parentDir->id;
    f->isDir = true;
    name.copy(f->name, name.size());

    auto mount = new Mount();
    mount->mountPath = fullPath;
    mount->realFsPath = realPath;

    this->_files->push_back(f);
    this->_mounts->push_back(mount);
}

void VFS::umount(string mountPath) {
    auto fullPath = this->toFullPath(mountPath);

    auto externalAccessor = tryGetExternalAccessor(fullPath);

    if (!externalAccessor) {
        cout << "No mount at this point" << endl;
        return;
    }

    removeFromVector(this->_files, getFile(this->_files, fullPath));
    removeFromVector(this->_mounts, externalAccessor->mount);
}

void VFS::writeToFile(string path, string content) {
    auto fullPath = this->toFullPath(path);
    auto accessor = this->getAccessor(fullPath, false);

    if (!accessor) {
        cout << "Unable to write to file" << endl;
        return;
    }

    char* buffer = new char[content.size()];
    content.copy(buffer, content.size());

    accessor->writeData(buffer, content.size());
}

void VFS::renameFile(string path, string newName) {
    auto fullPath = this->toFullPath(path);
    auto accessor = this->getAccessor(fullPath, false);

    if (!accessor) {
        cout << "Unable to rename file" << endl;
        return;
    }

    accessor->rename(newName);
}

void VFS::copyFile(string srcPath, string dstPath) {
    srcPath = this->toFullPath(srcPath);
    dstPath = this->toFullPath(dstPath);

    auto source = this->getAccessor(srcPath, false);
    auto target = this->getAccessor(dstPath, false);

    source->copyTo(target.get());
}

void VFS::removeFile(string path) {
    path = this->toFullPath(path);

    auto accessor = this->getAccessor(path, false);

    if (!accessor) {
        cout << "Unable to delete file" << endl;
        return;
    }

    accessor->remove();
}

void VFS::save(string &realPath) {
    if (this->_mounts->size() > 0) {
        cout << "Please unmount all dirs before save" << endl;
        return;
    }

    ofstream f(realPath, ios::binary);

    for (auto file : *this->_files) {
        f.write((char*)file, sizeof(File));
    }

    f.write("\0", 1);

    for (auto el : *this->_data) {
        auto file = getFile(this->_files, el.first);
        f.write((char*)&el.first, sizeof(long));
        f.write(el.second, file->size);
    }

    f.close();
    cout << "Saved vfs" << endl;
}

void VFS::load(string &realPath) {
    this->_files->clear();
    this->_data->clear();

    ifstream inputFs(realPath, ios::binary | ios::ate);
    long long fSize = inputFs.tellg();
    inputFs.seekg(0);

    char* buffer = new char[fSize];
    char* end = buffer + fSize;
    inputFs.read(buffer, fSize);
    inputFs.close();

    while (buffer[0] != '\0') {
        auto file = (File*)buffer;
        this->_files->push_back(file);
        buffer = buffer + sizeof(File);
        this->_idGen = file->id + 1;
    }

    buffer++;

    while (buffer != end) {
        auto fileId = *(long*)buffer;
        buffer += sizeof(long);

        auto file = getFile(this->_files, fileId);

        (*this->_data)[fileId] = buffer;
        buffer += file->size;
    }

    cout << "Loaded vfs" << endl;
}

unique_ptr<FileAccessor> VFS::getAccessor(string& path, bool isDir) {
    auto externalAccessor = tryGetExternalAccessor(path);

    if (externalAccessor)
        return externalAccessor;

    auto file = getFile(this->_files, path);

    if (!file) {
        auto name = getFilename(path);
        file = new File();

        file->id = this->getNextId();
        name.copy(file->name, name.size());
        file->isDir = isDir;
    }

    return std::make_unique<DirectAccessor>(path, file, this->_files, this->_data);
}

long VFS::getNextId() {
    return this->_idGen++;
}

string VFS::toFullPath(string path) {
    if (path[0] != '/') {
        path = joinPath(this->_currentDir, path);
    }

    return path;
}

unique_ptr<ExternalAccessor> VFS::tryGetExternalAccessor(string& path) {
    for (auto mount : *this->_mounts) {
        if (path.rfind(mount->mountPath, 0) == 0) {
            auto size = mount->mountPath.size();
            auto internalPath = path.substr(size, path.size() - size);

            return std::make_unique<ExternalAccessor>(internalPath, mount, true);
        }
    }

    return nullptr;
}
