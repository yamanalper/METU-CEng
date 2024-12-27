#ifndef FILE_H
#define FILE_H

#include "LinkedList.h"
#include "Block.h"

class File{
public:

	Block getBlock(int index) const;
	size_t fileSize() const;
	int numBlocks() const;
	bool isEmpty() const;
	bool operator==(const File &rhs) const;

	void newBlock(const Block &block);
	void removeBlock(int index);
	void mergeBlocks(int sourceIndex, int destIndex);
	void printContents() const;

private:
	LinkedList<Block> blocks;
};
#endif //FILE_H