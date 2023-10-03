#include "FileSystem/FileSystem.h"

/**
 * Sets up the file system for the device.
 *
 * @return void
 *
 * @throws None
 */
void FileSystem::begin(){
    boolean userPartitionMounted = userFS.begin();
    if( debug ){
        Serial.printf("\n***** File System *****\n");

        Serial.printf("%s\n",userPartitionMounted?"User partition is mounted":"User partition is not mounted");
        Serial.printf("Size: %d byte\n",userPartitionMounted?userFS.totalBytes():0);
        Serial.printf("Unused: %d byte\n",userPartitionMounted?userFS.totalBytes() - userFS.usedBytes():0);
        Serial.printf("Used: %d byte\n",userPartitionMounted?userFS.usedBytes():0);

        Serial.printf("***** ********** *****\n\n");
    }
}

/**
 * Retrieves the directory tree starting from the specified path and stores it in the given JSON document.
 *
 * @param path The path of the directory to retrieve the tree from.
 * @param tree The JSON document to store the directory tree.
 *
 * @return Returns `true` if the directory tree was successfully retrieved and stored in the JSON document, `false` otherwise.
 *
 * @throws None
 */
boolean FileSystem::getDirTree(const char* path, JsonDocument &tree){
    File folder = userFS.open(path);
    if(!folder){
        if( debug ){
            Serial.printf("[FileSystem] - (getDirTree) Path not found: %s\n",path);
        }
        return false;
    }
    if( !folder.isDirectory() ){
        if( debug ){
            Serial.printf("[FileSystem] - (getDirTree) Path is not a directory: %s\n",path);
        }
        folder.close();
        return false;
    }
    File entry = folder.openNextFile();
    while(entry){
        JsonObject details = tree.createNestedObject();
        time_t epochSec = entry.getLastWrite();

        String name = entry.name();
        String path = entry.path();
        details["path"] = path;
        details["name"] = name;
        details["dir"]  = entry.isDirectory();
        details["size"] = entry.size();
        details["date"] = epochSec;
        entry.close();
        entry = folder.openNextFile();
    }
    folder.close();
    return true;
}


/**
 * Retrieves the size of a file given its path.
 *
 * @param path The path to the file.
 *
 * @return The size of the file in bytes.
 *
 * @throws None
 */
int FileSystem::getFileSize(const char* path){
    File file = userFS.open(path);
    if(!file){
        if( debug ){
            Serial.printf("[FileSystem] - (getFileSize) File not found: %s\n",path);
        }
        return -1;
    }
    int size = file.size();
    file.close();
    return size;
}

/**
 * Sets the debug mode of the FileSystem.
 *
 * @param state the boolean value representing the debug mode
 *
 * @throws ErrorType description of error
 */
void FileSystem::setDebug(boolean state){
    debug = state;
}

/**
 * Renames a file or directory from the old path to the new path.
 *
 * @param oldPath the path of the file or directory to be renamed
 * @param newPath the new path for the file or directory
 *
 * @return true if the renaming was successful, false otherwise
 *
 * @throws ErrorType if an error occurs during the renaming process
 */
boolean FileSystem::rename(const char* oldPath, const char* newPath){
    return userFS.rename(oldPath, newPath);
}

boolean FileSystem::rename(const char* oldPath, const char* newPath, fs::LittleFSFS &fs){
    return fs.rename(oldPath, newPath);
}

/**
 * Removes a file or directory specified by the given path.
 *
 * @param path the path to the file or directory to be removed
 *
 * @return true if the file or directory is successfully removed, false otherwise
 *
 * @throws ErrorType if an error occurs while removing the file or directory
 */
boolean FileSystem::remove(const char* path){
    if( !exists(path) ){
        if( debug ){
            Serial.printf("[FileSystem] - (remove) Path not found: %s\n",path);
        }
        return true;
    }
    boolean result = userFS.remove(path);
    if( debug ){
        Serial.printf("[FileSystem] - (remove) %s %s\n",path,result?"removed":"not removed");
    }
    return result;
}

boolean FileSystem::remove(const char* path, fs::LittleFSFS &fs){
    if( !fs.exists(path) ){
        if( debug ){
            Serial.printf("[FileSystem] - (remove) Path not found: %s\n",path);
        }
        return true;
    }
    boolean result = fs.remove(path);
    if( debug ){
        Serial.printf("[FileSystem] - (remove) %s %s\n",path,result?"removed":"not removed");
    }
    return result;
}

boolean FileSystem::removeDir(const char* path, fs::LittleFSFS &fs){
    if( !fs.exists(path) ){
        if( debug ){
            Serial.printf("[FileSystem] - (removeDir) Path not found: %s\n",path);
        }
        return true;
    }
    boolean result = fs.rmdir(path);
    if( debug ){
        Serial.printf("[FileSystem] - (removeDir) %s %s\n",path,result?"removed":"not removed");
    }
    return result;
}

/**
 * Creates a directory in the file system.
 *
 * @param path the path of the directory to be created
 *
 * @return true if the directory was created successfully, false otherwise
 *
 * @throws ErrorType if an error occurs during the directory creation process
 */
boolean FileSystem::createDirectory(const char* path){
    boolean result = userFS.mkdir(path);
    if( debug ){
        Serial.printf("[FileSystem] - %s %s\n",path,result?"created":"not created");
    }
    return result;
}

boolean FileSystem::createDirectory(const char* path, fs::LittleFSFS &fs){
    boolean result = fs.mkdir(path);
    if( debug ){
        Serial.printf("[FileSystem] - %s %s\n",path,result?"created":"not created");
    }
    return result;
}

/**
 * Creates a path in the file system.
 *
 * @param path the path to create
 *
 * @return true if the path is created successfully, false otherwise
 *
 * @throws None
 */
boolean FileSystem::createPath(const char* path, fs::LittleFSFS &fs){
    if(fs.exists(path)){
        if( debug ){
            Serial.printf("[FileSystem] - (createPath) Path already exists: %s\n",path);
        }
        return true;
    }

    if (strchr(path, '/')) {
        if( debug ){
            Serial.printf("[FileSystem] - Creating path: %s\n",path);
        }
        char *pathStr = strdup(path);
        if (pathStr) {
            char *ptr = strchr(pathStr, '/');
            while (ptr) {
                *ptr = 0;
                fs.mkdir(pathStr);
                *ptr = '/';
                ptr = strchr(ptr+1, '/');
            }
        }
        free(pathStr);
        return true;
    }
    return false;
}

/**
 * Creates a path in the file system.
 *
 * @param path the path to create
 *
 * @return true if the path is created successfully, false otherwise
 *
 * @throws None
 */
boolean FileSystem::createPath(const char* path){
    if(userFS.exists(path)){
        if( debug ){
            Serial.printf("[FileSystem] - (createPath) Path already exists: %s\n",path);
        }
        return true;
    }

    if (strchr(path, '/')) {
        if( debug ){
            Serial.printf("[FileSystem] - Creating path: %s\n",path);
        }
        char *pathStr = strdup(path);
        if (pathStr) {
            char *ptr = strchr(pathStr, '/');
            while (ptr) {
                *ptr = 0;
                userFS.mkdir(pathStr);
                *ptr = '/';
                ptr = strchr(ptr+1, '/');
            }
        }
        free(pathStr);
        return true;
    }
    return false;
}

/**
 * Check if a file or directory exists.
 *
 * @param path The path of the file or directory to check.
 *
 * @return True if the file or directory exists, false otherwise.
 *
 * @throws None.
 */
boolean FileSystem::exists(const char* path){
    return userFS.exists(path);
}

/**
 * Retrieves a file object from the file system.
 *
 * @param path the path of the file to retrieve
 * @param type the type of the file to retrieve
 *
 * @return a file object representing the file at the given path
 *
 * @throws ErrorType if an error occurs during the retrieval process
 */
File FileSystem::getFileObject(const char* path, const char* type){
    return userFS.open(path, type);
}