#ifndef LOCAL_STORAGE_HEADER
#define LOCAL_STORAGE_HEADER

#include <Arduino.h>
#include "LittleFS.h"

class LittleFSLocalStorage{
    private:
        bool _FSReady;
        static const int _writeDelay = 100;
    public:
        LittleFSLocalStorage();

        File open(const String&, const String&); // returns a File object
        String readFile(const String&);
        bool writeFile(const String&, const String&); // same as overwrite
        bool appendToFile(const String&, const String&);
        bool touch(const String&); //creates an empty file
        bool remove(const String&);
        bool rename(const String&, const String&); //same as moving a file - provid an absolute path
        bool copyFile(const String&, const String&);

        bool exists(const String&);

        Dir openDir(const String&);
        bool mkdir(const String&);
        bool rmdir(const String&);
        bool copyDir(const String&);
        bool moveDir(const String&);

        bool ls(const String&); //path to directory. Should directories end with a /??
};

static LittleFSLocalStorage localStorage{};

#endif