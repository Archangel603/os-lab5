#ifndef LAB5_FILE_H
#define LAB5_FILE_H

struct File {
    long id;
    char name[250];
    long parentId;
    long size = 0;
    bool isDir = false;

    /*void writeTo(std::ostream& s) {
        s.write((char*)&this->id, sizeof(long));
        s.write((char*)&this->parentId, sizeof(long));
        s.write((char*)&this->startSegment, sizeof(long));
        s.write((char*)&this->endSegment, sizeof(long));
        s.write((char*)&this->size, sizeof(long));
    }

    static File* readFrom(std::istream& i) {
        auto file = new File();

        i.read((char*)&file->id, sizeof(long));
        i.read((char*)&file->parentId, sizeof(long));
        i.read((char*)&file->startSegment, sizeof(long));
        i.read((char*)&file->endSegment, sizeof(long));
        i.read((char*)&file->size, sizeof(long));

        return file;
    }*/
};

#endif //LAB5_FILE_H
