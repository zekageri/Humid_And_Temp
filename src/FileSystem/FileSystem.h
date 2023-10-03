#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class FileSystem{
    public:
        fs::LittleFSFS userFS;
        
        void begin();
        void setDebug(boolean state);

        int read(const char* path, JsonDocument& doc);
        int read(const char* path, char* buffer, size_t size);
        int read(const char* path, char* buffer, size_t size, fs::LittleFSFS &fs);

        boolean write(const char* path, uint8_t * buffer, size_t size);
        boolean write(const char* path, JsonDocument& doc);
        boolean write(const char* path, const char* message);
        boolean write(const char* path, const char* message, fs::LittleFSFS &fs);

        boolean append(const char* path, const char* message);
        boolean append(const char* path, const char* message, char terminator);
        boolean append(const char* path, JsonDocument& doc, char terminator = '\n');

        boolean createPath(const char* path, fs::LittleFSFS &fs);
        boolean createPath(const char* path);

        boolean rename(const char* oldPath, const char* newPath);
        boolean rename(const char* oldPath, const char* newPath, fs::LittleFSFS &fs);

        boolean remove(const char* path);
        boolean remove(const char* path, fs::LittleFSFS &fs);
        boolean removeDir(const char* path, fs::LittleFSFS &fs);

        boolean createDirectory(const char* path);
        boolean createDirectory(const char* path, fs::LittleFSFS &fs);

        boolean exists(const char* path);

        boolean getDirTree(const char* path, JsonDocument &tree);
        boolean getSystemDirTree(const char* path, JsonDocument &tree);
        
        int getFileSize(const char* path);
        File getFileObject(const char* path, const char* type);
        
    private:
        boolean debug = true;
};

inline FileSystem db;