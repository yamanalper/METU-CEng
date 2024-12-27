#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "File.h"
#include <vector>

class Directory{
public:

	File getFile(int index) const;
	size_t directorySize() const;
	bool isEmpty() const;
	size_t numFiles() const;

	void newFile(const File &file);
	void removeFile(int index);
	void removeFile(File &file);
	void removeBiggestFile();

private:
	std::vector<File> files;
};

#endif //DIRECTORY_H