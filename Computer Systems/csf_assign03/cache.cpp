#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <queue>
#include <list>
#include <algorithm>
#include <unordered_map>
#include "cache.h"

// For debugging
void printCacheUpload (const Cache c) {
  std::cout << "Max Num Of Sets: " << c.MaxNumOfSets << "\n";
  std::cout << "Max Num Blocks: " << c.MaxNumBlocks << "\n";
  std::cout << "Max Num Bytes: " << c.MaxNumBytes << "\n";
  std::cout << "current Num Sets: " << c.currentNumSets << "\n";
  std::cout << "writeA: " << c.writeA << "\n";
  std::cout << "writeT: " << c.writeT << "\n";
  std::cout << "lru: " << c.lru << "\n";
}

// Final results
void printCacheStats(const Cache& cache) {
  std::cout << "Total loads: " << cache.output[0] << std::endl;
  std::cout << "Total stores: " << cache.output[1] << std::endl;
  std::cout << "Load hits: " << cache.output[2] << std::endl;
  std::cout << "Load misses: " << cache.output[3] << std::endl;
  std::cout << "Store hits: " << cache.output[4] << std::endl;
  std::cout << "Store misses: " << cache.output[5] << std::endl;
  std::cout << "Total cycles: " << cache.output[6] << std::endl;
}

bool isValidArgs(int argc, char **argv) {
  //Checks that it is 7 arguements (including ./csim)
  if (argc != 7){
    std::cerr << "Not correct number of arguments";
    return false;
  }

  //Checks that they are positive values of two
  if (!(postivePowerOfTwo(argv[1]) && postivePowerOfTwo(argv[2]) && postivePowerOfTwo(argv[3]))) {
    return false;
  }

  //Checks that it is it at least 4 bytes
  int numberOfBytes = std::atoi(argv[3]);
  if (numberOfBytes < 4){
    std::cerr << "Num of bytes are too low";
    return false;
  }

  return isValidStringArgs(argv);
}

bool isValidStringArgs(char **argv) {
  //Checks that the argument are valid in constant time
  if (strcmp(argv[4], "write-allocate") != 0 && strcmp(argv[4], "no-write-allocate") != 0) {
    std::cerr << "Not a valid parameter";
    return false;
  }

  if (strcmp(argv[5], "write-through") != 0 && strcmp(argv[5], "write-back") != 0) {
    std::cerr << "Not a valid parameter";
    return false;
  }

  if (strcmp(argv[6], "lru") != 0 && strcmp(argv[6], "fifo") != 0) {
    std::cerr << "Not a valid parameter";
    return false;
  }

  //Checks the special conditions
  if (strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0) {
    std::cerr << "Not a valid parameter";
    return false;
  }

  return true;
}

bool postivePowerOfTwo(std::string n) {
  //Checks that it is all numbers
  for (int i = 0; i < int(n.length()); i++){
    if (!isdigit(n[i])){
      std::cerr << "Not a number";
      return false;
    }
  }

  //Checks that it is positive
  int trueN = std::stoi(n);
  if (trueN <= 0){
    std::cerr << "Not positive";
    return false;
  }

  //Checks that it is a power of 2
  if (!(trueN & (trueN - 1)) == 0) {
    std::cerr << "Not a power of 2";
    return false;
  }
  
  return true;
}

Cache setUpCache(char **argv){
  int trueNumberOfSets = std::atoi(argv[1]);
  int trueMaxNumBlocks = std::atoi(argv[2]);
  int trueMaxNumBytes = std::atoi(argv[3]);

  //Intializes the caches
  Cache ourCache = {trueNumberOfSets, trueMaxNumBlocks, trueMaxNumBytes, 0, {}, false, false, false, {}, {}, {}, {}, {}};

  return setUpStringCache(argv, ourCache);
} 

Cache setUpStringCache(char **argv, Cache ourCache){
  //Checks if it is write-allocate else it must be the other
  
  if (strcmp (argv[4], "write-allocate") == 0) {
    ourCache.writeA = true;
  } else {
    ourCache.writeA = false;
  }

  //Checks if it is write-through else it must be the other
  if (strcmp (argv[5], "write-through") == 0) {
    ourCache.writeT = true;
  } else {
    ourCache.writeT = false;
  }

  //Checks if it is lru else it must be the other
  if (strcmp (argv[6], "lru") == 0) {
    ourCache.lru = true;
  } else {
    ourCache.lru = false;
  }

  return ourCache;
}

void load(Cache& cache, std::istringstream& iss, std::string s, int num_index, int num_offset) {
  cache.output[0]++; // Total loads

  if (!(std::getline(iss, s, ' '))) { // Set second element
      std::cerr << "Error: Unexpected format in trace file (load)" << std::endl;
      return;
  }
  s = s.substr(2);

  try {
    uint32_t memory = (std::stoul(s, nullptr, 16) >> num_offset) << num_offset;
    if (cache.memory_block.find(memory) != cache.memory_block.end()) { // Load hit
      hit(cache, memory, num_index, num_offset, true);
    } else {
      miss(cache, memory, num_index, num_offset, true);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: Invalid memory address in trace file: " << s << std::endl;
  }
}

void directLoad(Cache& cache, std::istringstream& iss, std::string s, int num_index, int num_offset) {
  cache.output[0]++;

  //Gets memory
  if (!(std::getline(iss, s, ' '))) { // Set second element
    std::cerr << "Error: Unexpected format in trace file (store)" << std::endl;
    return;
  }
  s = s.substr(2);

  uint32_t memory = (std::stoul(s, nullptr, 16) >> num_offset) << num_offset;

  // Compute set index and tag.
  uint32_t setIndex = (memory >> num_offset) & ((1 << num_index) - 1);
  uint32_t tag = memory >> (num_offset + num_index);

  // Create a set if it isn't in it
  if (cache.index_set.find(setIndex) == cache.index_set.end()) {
    Set * newSet = new Set();
    cache.index_set[setIndex] = newSet;
    cache.currentNumSets++;
  }
  
  Set * ourSet = cache.index_set[setIndex];

  // Direct mapping: maximum one block per set.
  if (ourSet->currentNumBlocks == 0) {
    addNewBlockToNewSet(cache, ourSet, memory, tag, true);
  } else { 
    addNewBlockToOldSetForLoad(cache, ourSet, memory, tag);
  }
}

void addNewBlockToNewSet(Cache &cache, Set * s, uint32_t memory, uint32_t tag, bool load) {
  // Load misses
  if (load){
    cache.output[3]++;
  } else {
    cache.output[5]++;
  }

  cache.output[6] += (1 + 100 * (cache.MaxNumBytes / 4)); 

  // Creates a new block and adds it to the list
  Block * newBlock = new Block(); 
  newBlock -> tag = tag;
  newBlock -> memory = memory;

  // For a store miss:
  if (!load) {
    if (cache.writeT) { // write-back
      cache.output[6] += (1 + 100);
    } else {            // write-through
      newBlock -> dirty = true;
      cache.output[6] += 1;
    }
  }

  s->blocks.push_back(newBlock);
  s->currentNumBlocks = 1; 
  cache.memory_block[memory] = newBlock;
}

void addNewBlockToOldSetForLoad(Cache &cache, Set * s, uint32_t memory, uint32_t tag) {
  // Creates a new block and replace it in the list
  Block * block = s -> blocks[0];
  if (block -> tag == tag) {
    // This will increase the hit
    cache.output[2]++; 
    cache.output[6]++; 
  } else {
    // This will increase the miss
    cache.output[3]++; 
    cache.output[6] += (1 + 100 * (cache.MaxNumBytes / 4));
    block -> tag = tag;
    block -> memory = memory;
    cache.memory_block[memory] = block;
  }
}

void addNewBlockToOldSetForStore(Cache &cache, Set * s, uint32_t memory, uint32_t tag) {
  // A block is present.
  Block * block = s->blocks[0];
  if (block -> tag == tag) { //Hits
    cache.output[4]++; 
    if (!cache.writeT) {
      cache.output[6]++;
    } else {
      cache.output[6] += (1 + 100);
    }
  } else {
    // Misses
    if (!cache.writeT && block -> dirty) {
      cache.output[6] += 100 * (cache.MaxNumBytes / 4);
    }

    if (cache.writeA) {
      cache.output[6] += (1 + 120 * (cache.MaxNumBytes / 4));

      //Adds it to the set
      block -> tag = tag;
      block -> memory = memory;
      cache.memory_block[memory] = block;

      if (cache.writeT) {
        cache.output[6] += (1 + 100);
      } else {
        cache.output[6] += 1;
        block -> dirty = true;
      }
    } else {
      // No-write-allocate: just update counters.
      cache.output[6] += 100;
    }
    cache.output[5]++;
  }
}

void store(Cache& cache, std::istringstream& iss, std::string s, int num_index, int num_offset) {
  cache.output[1]++; // Total stores

  if (!(std::getline(iss, s, ' '))) { // Set second element
    std::cerr << "Error: Unexpected format in trace file (store)" << std::endl;
    return;
  }
  s = s.substr(2);

  try {
    uint32_t memory = (std::stoul(s, nullptr, 16) >> num_offset) << num_offset;
    if (cache.memory_block.find(memory) != cache.memory_block.end()) { // Store hit
      hit(cache, memory, num_index, num_offset, false);
    } else {
      
      if (cache.writeA) { // Write allocate
        miss(cache, memory, num_index, num_offset, false); // Modifies cache
      } else { // No write allocate
        cache.output[5]++; // Store misses, doesnt modify cache
        cache.output[6] += 100; // Total cycles
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: Invalid memory address in trace file: " << s << std::endl;
  }
}

void directStore(Cache& cache, std::istringstream& iss, std::string s, int num_index, int num_offset) {
  cache.output[1]++;
  if (!(std::getline(iss, s, ' '))) { // Set second element
    std::cerr << "Error: Unexpected format in trace file (store)" << std::endl;
    return;
  }
  s = s.substr(2);

  // Gets memory
  uint32_t memory = (std::stoul(s, nullptr, 16) >> num_offset) << num_offset;

  uint32_t setIndex = (memory >> num_offset) & ((1 << num_index) - 1);
  uint32_t tag = memory >> (num_offset + num_index);

  // Create a set if it isn't in it
  if (cache.index_set.find(setIndex) == cache.index_set.end()) {
    Set * newSet = new Set();
    cache.index_set[setIndex] = newSet;
    cache.currentNumSets++;
  }
  
  Set * ourSet = cache.index_set[setIndex];

  if (ourSet->currentNumBlocks == 0) {
    if (cache.writeA) { // Write-allocate
      addNewBlockToNewSet(cache, ourSet, memory, tag, false); 
    } else { // No write-allocate
      cache.output[5]++;
      cache.output[6] += 100;
    }
  } else {
    addNewBlockToOldSetForStore(cache, ourSet, memory, tag);
  }
}

void hit(Cache& cache, uint32_t memory, int num_index, int num_offset, bool load) {
  if (load) {
    cache.output[2]++; // Load hits
  } else {
    cache.output[4]++; // Store hits

    if (!cache.writeT) {
      // Marks block as dirty
      Block* block_ptr = cache.memory_block[memory];
      block_ptr->dirty = true;
    } else {
      cache.output[6] += (1 + 100);
    }
  }
  cache.output[6]++; // Total cycles

  uint32_t memory_index = (memory >> num_offset) & ((1 << num_index) - 1);

  Set* existed_set = cache.index_set[memory_index];
  Block* block_ptr = cache.memory_block[memory];
  existed_set -> memoryIndex = memory_index;
  if (cache.lru){
    hitLRU(cache, *existed_set, *block_ptr);
  }
}

void miss(Cache& cache, uint32_t memory, int num_index, int num_offset, bool load) {
  if (load) {
    cache.output[3]++; // Load misses
  } else {
    cache.output[5]++; // Store misses
  }
  cache.output[6] += 1 + 100 * (cache.MaxNumBytes / 4);

  if (!load && cache.writeT) { //Penalty for write though
    cache.output[6] += (1 + 100); 
  }

  uint32_t memory_tag = memory >> (num_index + num_offset);
  uint32_t memory_index = (memory >> num_offset) & ((1 << num_index) - 1);

  if (cache.currentNumSets > cache.MaxNumOfSets){
    setEvict(cache);
    cache.currentNumSets--;
  }

  if (cache.index_set.find(memory_index) == cache.index_set.end()) { // New index
    Set *s = new Set();
    s->memoryIndex = memory_index;
    cache.index_set[memory_index] = s;
    cache.currentNumSets++;
    addToSet(cache, *s);
  }

  Set* existed_set = cache.index_set[memory_index];
  Block* block_ptr = new Block{memory_tag, memory, true, false};

  if (!load && !cache.writeT) { // Set it to dirty
    block_ptr -> dirty = true;
  }

  miss_evict(cache.lru, existed_set, block_ptr, cache);
  cache.memory_block[memory] = block_ptr; // Add new block to map
}

void setEvict(Cache &c) {
  // Finds the removing set
  Set * removeS;
  if (c.lru) {
    removeS = c.lruSetLinkedList.back();
    c.lruSetLinkedList.pop_back();
  } else {
    removeS = c.fifoSetQueue.front();
    c.fifoSetQueue.pop();
  }

  // Removes the blocks and set from the map
  for (Block * curBlock : removeS -> blocks) {
    c.memory_block.erase(curBlock -> memory);
  }

  c.index_set.erase(removeS -> memoryIndex);

  // Free the set to prevent a memory leak
  delete removeS;
}

void addToSet(Cache &c, Set &s){
  // Finds the correct set
  if (c.lru) {
    c.lruSetLinkedList.push_front(&s);
    std::list<Set*>::iterator it = c.lruSetLinkedList.begin();
    c.lruSetMap[&s] = it;
  } else {
    c.fifoSetQueue.push(&s);
  }
}

void miss_evict(bool lru, Set* existed_set, Block* block_ptr, Cache& cache) {
  if (lru) {
    missLRU(cache, *existed_set, block_ptr);
  } else {
    updateFIFO(cache, *existed_set, block_ptr);
  }
}

void read_file(Cache& cache) {
  std::string line;

  int num_index = std::log2(cache.MaxNumOfSets); // Number of bits for index
  int num_offset = std::log2(cache.MaxNumBytes); // Number of bits for offset, times 3 for changing from bytes to bits
  while (std::getline(std::cin, line)) { // Go through each line of the file
    std::istringstream iss(line); 
    std::string s;

    std::getline(iss, s, ' '); // Get first element of line (l or s), split by a space 
    if (s.c_str()[0] == 'l') { // Load
      if (cache.MaxNumBlocks == 1) { //Direct Cache (faster)
        directLoad(cache, iss, s, num_index, num_offset);
      } else { 
        load(cache, iss, s, num_index, num_offset);
      }
    } else if (s.c_str()[0] == 's') { // Store
      if (cache.MaxNumBlocks == 1) { //Direct Cache (faster)
        directStore(cache, iss, s, num_index, num_offset);
      } else { 
        store(cache, iss, s, num_index, num_offset);
      }
    } else {
      std::cerr << "First element of file is not l or s" << std::endl;
    }
  }
}

void updateFIFO(Cache &c, Set &s, Block * b) {
  if (shouldEvict(c, s)) {
    fifoEvict(c, s);
  }
  s.currentNumBlocks++;
  
  // Now perform insertion
  s.fifoQueue.push(b);
  s.blocks.push_back(b);
  s.blockIndexMap[b -> memory] = int(s.blocks.size()) - 1;
}

void fifoEvict (Cache &c, Set &s) {
  // Finds the block to remove
  Block * removedBlock = s.fifoQueue.front();
  s.fifoQueue.pop();
  s.lruBlockMap.erase(removedBlock);

  Evict(c, s, removedBlock);
}

void missLRU(Cache &c, Set &s, Block * b) {
  if (shouldEvict(c, s)) {
    lruEvict(c, s);
  }

  s.currentNumBlocks++;
  
  // Add to front of list
  s.lruLinkedList.push_front(b);
  s.lruBlockMap[b] = s.lruLinkedList.begin();

  // Move set to the front in cache LRU list in O(1) time
  std::list<Set*>::iterator setIt = c.lruSetMap[&s];   
  c.lruSetLinkedList.erase(setIt);
  c.lruSetLinkedList.push_front(&s);
  c.lruSetMap[&s] = c.lruSetLinkedList.begin();

  s.blocks.push_back(b);
  s.blockIndexMap[b -> memory] = int(s.blocks.size()) - 1;
}

void hitLRU(Cache &c, Set &s, Block &b) {
  // Add to front of list
  std::list<Block*>::iterator blockIt = s.lruBlockMap[&b];   
  s.lruLinkedList.erase(blockIt);
  s.lruLinkedList.push_front(&b);
  s.lruBlockMap[&b] = s.lruLinkedList.begin();

  // Move set to the front in cache LRU list in O(1) time
  std::list<Set*>::iterator setIt = c.lruSetMap[&s];   
  c.lruSetLinkedList.erase(setIt);
  c.lruSetLinkedList.push_front(&s);
  c.lruSetMap[&s] = c.lruSetLinkedList.begin();
}

void lruEvict (Cache &c, Set &s) {
  // Finds the block to remove
  Block * removedBlock = s.lruLinkedList.back();
  s.lruLinkedList.pop_back();
  s.lruBlockMap.erase(removedBlock);

  Evict(c, s, removedBlock);
}

void Evict (Cache &c, Set &s, Block * removedBlock){
  if (!c.writeT) {
    c.output[6] += 30;
  }

  // Finds the position
  std::unordered_map<uint32_t, int>::iterator it = s.blockIndexMap.find(removedBlock -> memory);
  int index = it->second;

  //Checks that it is valid
  if (it != s.blockIndexMap.end()) {
    // Swap the back index and the current and remove
    if (index != int(s.blocks.size()) - 1) {
      s.blocks[index] = s.blocks.back();
      s.blockIndexMap[s.blocks[index]->memory] = index;
    }
    s.blocks.pop_back();
    // Erase the removed block's entry from the map
    s.blockIndexMap.erase(it);
  }
   
  // If we swapped, update the index of the block that was moved.
  if (index < int(s.blocks.size())) {
    std::unordered_map<uint32_t, int>::iterator it2 = s.blockIndexMap.find(s.blocks[index] -> memory);
    it2->second = index;
  }
 
  // Removes it from the hitting map
  std::unordered_map<uint32_t, Block *> ::iterator itBlock = c.memory_block.find(removedBlock -> memory);
  if (itBlock != c.memory_block.end()) {
    c.memory_block.erase(itBlock);
  }

  if (!c.writeT && removedBlock -> dirty) {
    c.output[6] += 100 * (c.MaxNumBytes / 4); 
  }

  s.currentNumBlocks--;
  delete removedBlock;
  removedBlock = nullptr;
}

bool shouldEvict(const Cache &c, Set &s){
  // Checks if adding one will cause eviction
  return (c.MaxNumBlocks <= s.currentNumBlocks);
}
