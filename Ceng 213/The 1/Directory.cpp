#include "Directory.h"

File Directory::getFile(int index) const
{
    if(index < 0 || index >= files.size()){
        return File();
    }
    return files[index];
}

size_t Directory::directorySize() const
{
    size_t size = 0;
    if(files.empty()){
        return size;
    }
    for( int i = 0; i < (files.size()) ; i++){
        size = size + files[i].fileSize();
    }
    return size;
}

bool Directory::isEmpty() const
{
    if(files.empty()){
        return true;
    }
    return false;
}

size_t Directory::numFiles() const
{
    return files.size();
}

void Directory::newFile(const File &file)
{
    files.push_back(file);
}

void Directory::removeFile(int index)
{
    if(index < 0 || index >= files.size()){
        return;
    }
    files.erase(files.begin()+ index);
}

void Directory::removeFile(File &file)
{
    int num = files.size();
    int i;
    for( i = 0; i<num ; i++){
        if(files[i] == file){
            removeFile(i);
        }
    }
}

void Directory::removeBiggestFile()
{
    if(isEmpty()){
        return;
    }
    File biggest = files[0];
    int num = files.size();
    int i;
    for( i = 0; i<num ; i++){
        if(files[i].fileSize() > biggest.fileSize()){
            biggest = files[i];
        }
    }
    removeFile(biggest);
}
