#include "File.h"

Block File::getBlock(int index) const
{
    if(index < 0 || index >= blocks.getSize()){
        return Block();
    }
    Node<Block>* item = blocks.getNodeAtIndex(index);
    return item->data;
}

size_t File::fileSize() const
{
    int list_size = blocks.getSize();
    Node<Block>* curr;
    size_t size = 0;
    for(int i = 0; i<list_size; i++){
        curr = blocks.getNodeAtIndex(i);
        size += curr->data.getSize();
    }
    return size;
    
}

int File::numBlocks() const
{
    int num = blocks.getSize();
    return num;
}

bool File::isEmpty() const
{
    if(blocks.isEmpty() == true){
        return true;
    }
    return false;
}

bool File::operator==(const File &rhs) const
{
    if(blocks.getSize() == rhs.blocks.getSize()){
        for(int i = 0; i<blocks.getSize(); i++){
            if(getBlock(i) == rhs.getBlock(i) && i == (blocks.getSize())-1){
                return true;
            }
        }
    }
    return false;
}

void File::newBlock(const Block &block)
{
    blocks.append(block);
}

void File::removeBlock(int index)
{
    blocks.removeNodeAtIndex(index);
}

void File::mergeBlocks(int sourceIndex, int destIndex)
{
    blocks.mergeNodes(sourceIndex,destIndex);
}

void File::printContents() const{
    if(isEmpty()){
        std::cout << "File is empty" << std::endl;
    }
    else{
        for(int i = 0; i < blocks.getSize(); i++){
            Node<Block> *block = blocks.getNodeAtIndex(i);
            std::cout << block->data.getContent();
        }
        std::cout << std::endl << fileSize() << std::endl;
    }
}
