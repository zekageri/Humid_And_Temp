#include "FileSystem/FileSystem.h"

/**
 * Writes a JSON document to a file in the file system.
 *
 * @param path The path of the file to write the JSON document to.
 * @param doc The JSON document to write to the file.
 *
 * @return True if the JSON document was successfully written to the file, false otherwise.
 *
 * @throws FileNotFound If the file specified by the path does not exist.
 */
boolean FileSystem::write(const char* path, JsonDocument& doc){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_WRITE);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (writeJson) File not found: %s\n",path);
        }
        return false;
    }
    size_t result = serializeJson(doc, file);
    file.close();

    return result != 0;
}

/**
 * Writes the given message to a file at the specified path.
 *
 * @param path the path to the file
 * @param message the message to write
 *
 * @return true if the message was successfully written to the file, false otherwise
 *
 * @throws ErrorType if the file is not found
 */
boolean FileSystem::write(const char* path, const char* message, fs::LittleFSFS &fs){
    if( !createPath(path) ){ return false; }
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (write) File not found: %s\n",path);
        }
        return false;
    }
    size_t result = file.print(message);
    file.close();
    if( debug && result > 0 ){
        Serial.printf("[FileSystem] - (write) Message written to file: %s\n",path);
    }
    return result > 0;
}

boolean FileSystem::write(const char* path, uint8_t * buffer, size_t size){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_WRITE);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (write) File not found: %s\n",path);
        }
        return false;
    }
    size_t result = file.write(buffer, size);
    file.close();
    if( debug && result > 0 ){
        Serial.printf("[FileSystem] - (write) Message written to file: %s\n",path);
    }
    return result > 0;
}

/**
 * Writes the given message to a file at the specified path.
 *
 * @param path the path to the file
 * @param message the message to write
 *
 * @return true if the message was successfully written to the file, false otherwise
 *
 * @throws ErrorType if the file is not found
 */
boolean FileSystem::write(const char* path, const char* message){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_WRITE);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (write) File not found: %s\n",path);
        }
        return false;
    }
    size_t result = file.print(message);
    file.close();
    if( debug && result > 0 ){
        Serial.printf("[FileSystem] - (write) Message written to file: %s\n",path);
    }
    return result > 0;
}

/**
 * Appends a message to the specified file path.
 *
 * @param path the path of the file to append the message to
 * @param message the message to be appended to the file
 *
 * @return true if the message was successfully appended, false otherwise
 *
 * @throws None
 */
boolean FileSystem::append(const char* path, const char* message){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_APPEND);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (append) File not found: %s\n",path);
        }
        return false;
    }
    file.print(message);
    file.close();
    return true;
}

/**
 * Appends a message to a file in the file system.
 *
 * @param path The path to the file.
 * @param message The message to append.
 * @param terminator The character to append after the message.
 *
 * @return true if the message was successfully appended, false otherwise.
 *
 * @throws ErrorType if the file was not found.
 */
boolean FileSystem::append(const char* path, const char* message, char terminator){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_APPEND);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (append) File not found: %s\n",path);
        }
        return false;
    }
    file.print(message);
    file.print(terminator);
    file.close();
    return true;
}

boolean FileSystem::append(const char* path, JsonDocument& doc, char terminator){
    if( !createPath(path) ){ return false; }
    File file = userFS.open(path, FILE_APPEND);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (append) File not found: %s\n",path);
        }
        return false;
    }
    boolean result = serializeJson(doc,file);
    if( result ){
        file.print(terminator);
        if( debug ){
            Serial.printf("[FileSystem] - (append) Json Message written to file: %s\n",path);
        }
    }else if( debug ){
        Serial.printf("[FileSystem] - (append) Failed to append json to file: %s\n",path);
    }
    
    file.close();
    return result;
}