#ifndef LAB5_FILE_H
#define LAB5_FILE_H

struct File {
    long id;
    char name[250];
    long parentId;
    long size = 0;
    bool isDir = false;

    void clearName() {
        for (int i = 0; i < 250; ++i) {
            this->name[i] = 0;
        }
    }
};

#endif //LAB5_FILE_H
