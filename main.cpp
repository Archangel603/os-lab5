#include <iostream>
#include "File.h"
#include "VFS.h"

using namespace std;

void runTests(VFS* vfs) {
    vfs->init(nullptr);

    vfs->pwd();
    vfs->listDir();
    vfs->changeDir("dir1");
    vfs->pwd();
    vfs->listDir();
    vfs->changeDir("..");
    vfs->pwd();
    vfs->listDir();
    vfs->changeDir("/dir1/another_dir/");
    vfs->pwd();
    vfs->listDir();

    vfs->changeDir("/");
    vfs->pwd();
    vfs->copyFile("file1.sh", "/dir1/another_dir/script.sh");
    vfs->listDir();
    vfs->changeDir("/dir1/another_dir");
    vfs->pwd();
    vfs->listDir();

    vfs->changeDir("/");

    vfs->mount("C:\\Dev", "dev");
    vfs->pwd();
    vfs->listDir();

    vfs->changeDir("dev/temp");
    vfs->pwd();
    vfs->listDir();

    vfs->copyFile("testFile.txt", "/myFile.txt");
    vfs->changeDir("/");
    vfs->pwd();
    vfs->listDir();

    vfs->writeToFile("myFile.txt", "Listen, my boy");
    vfs->copyFile("myFile.txt", "/dev/temp/virtualFile.txt");

    vfs->pwd();
    vfs->umount("/dev");
    vfs->listDir();
}

int main() {
    auto vfs = new VFS();
    //runTests(vfs);



    /*File files[] = {
        { 1, "file1", 1, 0, 0, 80 },
        { 2, "file2", 1, 1, 5, 4096 },
    };

    ofstream testFs("./VFS.dat", ios::binary);

    testFs.write((char*)files, size(files) * sizeof(File));
    testFs << '\0';
    testFs.close();

    ifstream inputFs("./VFS.dat", ios::binary | ios::ate);
    long long fSize = inputFs.tellg();
    inputFs.seekg(0);

    char* buffer = new char[fSize];
    inputFs.read(buffer, fSize);
    inputFs.close();

    File** loadedFiles = new File*[fSize / sizeof(File)];
    int totalFiles = 0;

    while (buffer[0] != '\0') {
        loadedFiles[totalFiles++] = (File*)buffer;
        buffer = buffer + sizeof(File);
    }

    *//*while (inputFs.peek() != '\0') {
        loadedFiles[totalFiles++] = File::readFrom(inputFs);
    }*//*

    for (int i = 0; i < totalFiles; i++) {
        printf("%s\n", loadedFiles[i]->name);
    }

    return 0;*/
}
