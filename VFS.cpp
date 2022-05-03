#include "VFS.h"

using namespace std;

void VFS::init(string *existing) {
    if (existing != nullptr) {
        return;
    }

    this->_files->push_back(new File {
        1, "", 0, 0, true
    });
    this->_files->push_back(new File {
        2, "file1.sh", 1,  5, false
    });
    this->_files->push_back(new File {
        3, "doc.txt", 1,  5, false
    });
    this->_files->push_back(new File {
        4, "dir1", 1,  5, true
    });
    this->_files->push_back(new File {
        5, "dz.docx", 4,  5, false
    });
    this->_files->push_back(new File {
        6, "another_dir", 4, 5, true
    });
    this->_idGen = 7;
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

    auto file = getFile(this->_files, mountPath);

    this->_files->erase(
        std::remove(
                this->_files->begin(),
                this->_files->end(),
                file
        ),
        this->_files->end()
    );

    this->_mounts->erase(
            std::remove(
                    this->_mounts->begin(),
                    this->_mounts->end(),
                    externalAccessor->mount
            ),
            this->_mounts->end()
    );

    delete externalAccessor;
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

    source->copyTo(target);
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

}

void VFS::load(string &realPath) {

}

FileAccessor* VFS::getAccessor(string& path, bool isDir) {
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

    return new DirectAccessor(path, file, this->_files, this->_data);
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

ExternalAccessor* VFS::tryGetExternalAccessor(string& path) {
    for (auto mount : *this->_mounts) {
        if (path.rfind(mount->mountPath, 0) == 0) {
            auto size = mount->mountPath.size();
            auto internalPath = path.substr(size, path.size() - size);

            return new ExternalAccessor(internalPath, mount);
        }
    }

    return nullptr;
}