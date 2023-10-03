#include "FileSystem/FileSystem.h"


/**
 * Reads data from a file in the file system.
 *
 * @param path The path of the file to be read.
 * @param buffer The buffer to store the read data.
 * @param size The size of the buffer.
 * @param fs The file system object.
 *
 * @return The number of bytes read from the file.
 *
 * @throws None.
 */
int FileSystem::read(const char* path, char* buffer, size_t size, fs::LittleFSFS &fs){
    if( !exists(path) ){
        if( debug ){
            Serial.printf("[FileSystem] - (read) File not found: %s\n",path);
        }
        return -1;
    }
    File file = fs.open(path, FILE_READ);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (read) File not found: %s\n",path);
        }
        return -1;
    }
    // Check if the file.size is greater than the buffer size
    if(file.size() > size){
        file.close();
        if(debug){
            Serial.printf("[FileSystem] - (read) File size is greater than buffer size\n");
        }
        return -1;
    }
    int readSize = 0;
    while (file.available()){
        buffer[readSize] = file.read();
        readSize++;
    }
    buffer[readSize] = '\0';
    file.close();
    if( debug && readSize > 0 ){
        Serial.printf("[FileSystem] - (read) Message read from file: %s\n",path);
    }
    return readSize;
}

/**
 * Reads data from a file given a path and stores it in a buffer.
 *
 * @param path The path of the file to read.
 * @param buffer The buffer to store the read data.
 * @param size The size of the buffer.
 *
 * @return The number of bytes read from the file.
 *
 * @throws None.
 */
int FileSystem::read(const char* path, char* buffer, size_t size){
    if( !exists(path) ){
        if( debug ){
            Serial.printf("[FileSystem] - (read) File not found: %s\n",path);
        }
        return -1;
    }
    File file = userFS.open(path, FILE_READ);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (read) File not found: %s\n",path);
        }
        return -1;
    }
    // Check if the file.size is greater than the buffer size
    if(file.size() > size){
        file.close();
        if(debug){
            Serial.printf("[FileSystem] - (read) File size is greater than buffer size\n");
        }
        return -1;
    }
    int readSize = 0;
    while (file.available()){
        buffer[readSize] = file.read();
        readSize++;
    }
    buffer[readSize] = '\0';
    file.close();
    if( debug && readSize > 0 ){
        Serial.printf("[FileSystem] - (read) Message read from file: %s\n",path);
    }
    return readSize;
}


/**
 * Reads the content of a file at the specified path and deserializes it into a JSON document.
 *
 * @param path The path of the file to be read.
 * @param doc The JSONDocument object to store the deserialized content.
 *
 * @return The number of bytes used by the JSON document if successful, otherwise -1.
 *
 * @throws None
 */
int FileSystem::read(const char* path, JsonDocument& doc){
    File file = userFS.open(path);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (getJson) File not found: %s\n",path);
        }
        return -1;
    }
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if( error == DeserializationError::Ok ){
        if( debug ){
            Serial.printf("[FileSystem] - (getJson) Message read from file: %s\n",path);
        }
        return doc.memoryUsage();
    }
    return -1;
}