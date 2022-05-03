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

    string command = "";

    while (true) {
        try {
            cout << "|vfs| " << vfs->getCwd() << ":";

            getline(cin, command);

            if (command == "quit")
                break;

            auto args = *split(command, " ");

            if (args[0] == "init") {
                if (args.size() > 1) {
                    vfs->init(&args[1]);
                }
                else {
                    vfs->init(nullptr);
                }

                continue;
            }

            if (args[0] == "save") {
                vfs->save(args[1]);
                continue;
            }

            if (args[0] == "load") {
                vfs->load(args[1]);
                continue;
            }

            if (args[0] == "pwd") {
                vfs->pwd();
                continue;
            }

            if (args[0] == "ls") {
                vfs->listDir();
                continue;
            }

            if (args[0] == "cd") {
                vfs->changeDir(args[1]);
                continue;
            }

            if (args[0] == "mount") {
                vfs->mount(args[1], args[2]);
                continue;
            }

            if (args[0] == "umount") {
                vfs->umount(args[1]);
                continue;
            }

            if (args[0] == "cp") {
                vfs->copyFile(args[1], args[2]);
                continue;
            }

            if (args[0] == "rename") {
                vfs->renameFile(args[1], args[2]);
                continue;
            }

            if (args[0] == "rm") {
                vfs->removeFile(args[1]);
                continue;
            }
        }
        catch(...) {
            cout << "Error" << endl;
        }
    }
}
