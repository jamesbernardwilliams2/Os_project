#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class AllocationType {
    CONTIGUOUS,
    INDEXED
};

struct FileEntry {
    std::string name;
    int sizeBytes;
AllocationType type;
    std::vector<int> blocks;   // list of disk blocks allocated
};

class SimpleFileSystem {
public:
    SimpleFileSystem(int totalBlocks, int blockSize);

    // file operations
    bool createFile(const std::string& name, int sizeiBytes, AllocationType type);
    bool deleteFile(const std::string& name);
    bool writeFile(const std::string& name, const std::string& data);
    std::string readFile(const std::string& name);
    bool modifyFile(const std::string& name, const std::string& newData);

    // directory operations
    void listFiles() const;

private:
 int numBlocks;  
 int blockSize;
    std::vector<bool> freeMap;   // block bitmap
    std::unordered_map<std::string, FileEntry> directory;

    // allocation helpers
   int findContiguousRun(int blocksNeeded) const;
   bool allocateContiguous(FileEntry& entry, int blocksNeeded);
   bool allocateIndexed(FileEntry& entry, int blocksNeeded);
 

};
