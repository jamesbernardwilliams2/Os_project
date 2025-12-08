#include "FileSystem.h" 

#include <algorithm>
#include <iostream>

SimpleFileSystem::SimpleFileSystem(int numBlocks, int blockSize) 
	:numBlocks(numBlocks),
	blockSize(blockSize),
	freeMap(numBlocks, true) {}

int SimpleFileSystem:: findContiguousRun(int blocksNeeded) const{
	int runStart = -1;
	int runLen = 0;

	for (int i = 0; i < numBlocks; ++i) {
	    if (freeMap[i]) {
		if (runLen ==0) runStart = i;
		runLen++;
                if (runLen >= blocksNeeded) {
			return runStart;
  		}
	} else {
	   runLen = 0;
           runStart = -1;
	}
  }
   return -1;
}

bool SimpleFileSystem::allocateContiguous(FileEntry& entry, int blocksNeeded) {
   int start = findContiguousRun(blocksNeeded);
   if (start < 0) return false;

   entry.blocks.clear();
   for (int i = 0; i < blocksNeeded; ++i) {
	int b = start + i;
	freeMap[b] = false;
	entry.blocks.push_back(b);
	}
	entry.type = AllocationType::CONTIGUOUS;
	return true;
}

bool SimpleFileSystem::allocateIndexed(FileEntry& entry, int blocksNeeded) {

	int totalNeeded = blocksNeeded + 1;

	entry.blocks.clear();
	for (int i = 0; i < numBlocks && (int)entry.blocks.size() < totalNeeded; ++i) {
	if (freeMap[i]) {
	freeMap[i] = false;
	entry.blocks.push_back(i);
	}
}
if ((int)entry.blocks.size() != totalNeeded) {
	for (int b : entry.blocks) {
	freeMap[b] = true;
}
entry.blocks.clear();
return false;
}
entry.type = AllocationType::INDEXED;
return true;
}

bool SimpleFileSystem::createFile(const std::string& name,
				  int sizeBytes, 
				  AllocationType type) {
	if (sizeBytes <= 0) return false;
	if (directory.find(name) != directory.end()) return false;

int blocksNeeded = (sizeBytes + blockSize - 1) / blockSize;

FileEntry entry;
entry.name = name;
entry.sizeBytes = sizeBytes;
entry.type = type;

bool ok = false;
if (type == AllocationType::CONTIGUOUS) {
	ok = allocateContiguous(entry, blocksNeeded);
	} else {
	    ok = allocateIndexed(entry, blocksNeeded);
}
if (!ok) return false;

directory[name] = entry;
return true;
}

bool SimpleFileSystem::deleteFile(const std::string& name) {
	auto it = directory.find(name);
	if (it ==  directory.end()){
 return false;
}

	for (int b : it->second.blocks) {
	    if (b >= 0 && b < numBlocks) {
              freeMap[b] = true;
	}
}

directory.erase(it);
return true;
}

std::string  SimpleFileSystem::readFile(const std::string& name) { 
	auto it = directory.find(name);
	if (it == directory.end()) { 
        return "";
}

int sizeBytes = it->second.sizeBytes;
//TODO: actually read
return std::string(sizeBytes, '?');

}

bool SimpleFileSystem::writeFile(const std::string& name,const std::string&data){ 
	auto it = directory.find(name);
	if (it == directory.end()) return false;
int sizeBytes = (int)data.size();
int currentBlocks = (it->second.sizeBytes + blockSize - 1) / blockSize;
int neededBlocks = (sizeBytes      + blockSize - 1) /blockSize;

if (neededBlocks <= currentBlocks) {
	it->second.sizeBytes = sizeBytes;
	return true;
}

for(int b : it->second.blocks) {
	if (b >= 0 && b < numBlocks) {
	freeMap[b] = true;
	}
}
it->second.blocks.clear();

FileEntry tmp = it->second;
tmp.sizeBytes = sizeBytes;

if(!allocateContiguous(tmp, neededBlocks)) {

	allocateContiguous(it->second, currentBlocks);
	return false;
}

	it->second = tmp;
	return true;
}
bool SimpleFileSystem::modifyFile(const std::string& name, const std::string& newData)
{

auto it = directory.find(name);
if (it == directory.end()) {
	return false;
}

int newSize = newData.size();
it->second.sizeBytes = newSize;

return writeFile(name, newData);
}
void SimpleFileSystem::listFiles() const {
	std::cout << "Files:\n";
	for (const auto& kv   : directory) {
	const std::string& name = kv.first;
	const FileEntry& entry = kv.second;
	std::cout << "  " << name
		  << " size=" << entry.sizeBytes
		  << " blocks="; 
	for (int b : entry.blocks) {
		std::cout << b << " ";
	}
	std::cout << "\n";
}
}
