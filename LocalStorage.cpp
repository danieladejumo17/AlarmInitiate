#include "LocalStorage.h"

// TODO: IGNORE ALL OTHER FILE OPERATIONS IF !_FSReady
LittleFSLocalStorage::LittleFSLocalStorage() : _FSReady{false}
{
    _FSReady = LittleFS.begin();
    if (!_FSReady)
    {
        //raise Exception
        Serial.println("Little FS could not be mounted");
    }
}

File LittleFSLocalStorage::open(const String &path, const String &mode)
{
    return LittleFS.open(path.c_str(), mode.c_str());
}

String LittleFSLocalStorage::readFile(const String &path)
// call exists before calling this function to verify a file can be read
{
    File file = open(path, "r");
    if (file)
    {
        String content;
        while (file.available())
        {
            content += static_cast<char>(file.read());
        }
        file.close();
        return content;
    }
    
    file.close();
    return "";
}

bool LittleFSLocalStorage::writeFile(const String &path, const String &content)
{
    File file = open(path, "w");
    if (file)
    {
        file.print(content);
        delay(_writeDelay);
        file.close();
        return true;
    }

    return false;
}

bool LittleFSLocalStorage::appendToFile(const String &path, const String &content)
{
    File file = open(path, "a");
    if (file)
    {
        file.print(content);
        delay(_writeDelay);
        file.close();
        return true;
    }

    return false;
}

bool LittleFSLocalStorage::touch(const String &path)
{
    return writeFile(path, "");
}

bool LittleFSLocalStorage::remove(const String &path)
{
    return LittleFS.remove(path);
}

bool LittleFSLocalStorage::rename(const String &pathFrom, const String &pathTo)
{
    return LittleFS.rename(pathFrom, pathTo);
}

bool LittleFSLocalStorage::copyFile(const String &source, const String &dest)
{
    return writeFile(dest, readFile(source));
}

bool LittleFSLocalStorage::exists(const String &path)
{
    return LittleFS.exists(path);
}

Dir LittleFSLocalStorage::openDir(const String &path)
{
    return LittleFS.openDir(path);
}

bool LittleFSLocalStorage::mkdir(const String &path)
{
    return LittleFS.mkdir(path);
}

bool LittleFSLocalStorage::rmdir(const String &path)
{
    return LittleFS.rmdir(path);
}
