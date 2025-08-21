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

/**
 * Represents a single cache block.
 */
struct Block {
  uint32_t tag;
  uint32_t memory;
  bool valid, dirty;
  bool operator==(const Block& other) const {
    return (tag == other.tag && memory == other.memory);
  }
};

/**
 * Represents a cache set containing multiple blocks.
 */
struct Set {
  int currentNumBlocks = 0;
  int memoryIndex = 0;
  std::vector<Block*> blocks;
  std::queue<Block*> fifoQueue;
  std::list<Block*> lruLinkedList;
  std::unordered_map<uint32_t, int> blockIndexMap;
  std::unordered_map<Block*, std::list<Block*>::iterator> lruBlockMap;

  bool operator==(const Set& other) const {
    return (currentNumBlocks == other.currentNumBlocks && blocks == other.blocks);
  }

  
  ~Set() {
    // Deletes the blocks
    for (Block* blk : blocks) {
      delete blk;
    }
    blocks.clear();

    // Clears all the maps and lists
    lruLinkedList.clear();
    blockIndexMap.clear();
    lruBlockMap.clear();
    
    // Empties the queue
    while (!fifoQueue.empty()){
      fifoQueue.pop();
    }
    
  }
};

/**
 * Represents a cache structure with multiple sets.
 */
struct Cache {
  int MaxNumOfSets = 0;
  int MaxNumBlocks = 0;
  int MaxNumBytes = 0;
  int currentNumSets = 0;
  int output[7] = {0};
  bool writeA;
  bool writeT;
  bool lru;
  std::queue<Set*> fifoSetQueue;
  std::list<Set*> lruSetLinkedList;
  std::unordered_map<Set*, std::list<Set*>::iterator> lruSetMap;
  std::unordered_map<uint32_t, Set *> index_set;
  std::unordered_map<uint32_t, Block *> memory_block;

  // Destructor
  ~Cache(){
    //Empties the queue
    while (!fifoSetQueue.empty()){
      fifoSetQueue.pop();
    }

    // Clears the maps and lists
    lruSetLinkedList.clear();
    lruSetMap.clear();
    memory_block.clear();


    // Deletes all the sets
    std::unordered_map<uint32_t, Set*>::iterator it;
    for (it = index_set.begin(); it != index_set.end(); ++it) {
      delete(it->second);
    }

    index_set.clear();
  }
};


/**
 * Initializes a cache based on command-line arguments.
 * @param argv Command-line arguments.
 * @return Configured Cache object.
 */
Cache setUpCache(char **argv);

/**
 * Initializes a cache from string arguments.
 * @param argv Command-line arguments.
 * @param ourCache Existing Cache object.
 * @return Updated Cache object.
 */
Cache setUpStringCache(char **argv, Cache ourCache);

/**
 * Prints cache statistics.
 * @param cache Cache object to print statistics for.
 */
void printCacheStats(const Cache& cache);

/**
 * Validates command-line arguments.
 * @param argc Argument count.
 * @param argv Argument values.
 * @return True if arguments are valid, false otherwise.
 */
bool isValidArgs(int argc, char **argv);

/**
 * Checks if a string represents a positive power of two.
 * @param n Input string.
 * @return True if n is a positive power of two, false otherwise.
 */
bool postivePowerOfTwo(std::string n);

/**
 * Validates string-based cache configuration arguments.
 * @param argv Argument values.
 * @return True if arguments are valid, false otherwise.
 */
bool isValidStringArgs(char **argv);

/**
 * Prints cache upload details.
 * @param c Cache object.
 */
void printCacheUpload(const Cache c);

/**
 * Determines if eviction is required.
 * @param c Cache object.
 * @param s Set to check.
 * @return True if eviction is required, false otherwise.
 */
bool shouldEvict(const Cache &c, Set &s);

/**
 * Performs FIFO eviction policy.
 * @param c Cache object.
 * @param s Set to check.
 */
void fifoEvict(Cache &c, Set &s);

/**
 * Updates FIFO queue. 
 * @param c Cache object.
 * @param s Set to check.
 * @param b Block object.
 */
void updateFIFO(Cache &c, Set &s, Block *b);

/**
 * Performs LRU eviction policy.
 * @param c Cache object.
 * @param s Set to check.
 */
void lruEvict(Cache &c, Set &s);

/**
 * Handles eviction of a block.
 * @param c Cache object.
 * @param s Set to check.
 * @param removedBlock Block to remove
 */
void Evict(Cache &c, Set &s, Block *removedBlock);

/**
 * Updates LRU tracking on a cache hit.
 * @param c Cache object.
 * @param s Set to check.
 * @param b Block object.
 */
void hitLRU(Cache &c, Set &s, Block &b);

/**
 * Updates LRU tracking on a cache miss.
 * @param c Cache object.
 * @param s Set to check.
 * @param b Block object.
 */
void missLRU(Cache &c, Set &s, Block *b);

/**
 * Adds a new block to a set.
 * @param c Cache object.
 * @param s Set to check.
 */
void addToSet(Cache &c, Set &s);

/**
 * Handles set eviction.
 * @param c Cache object.
 */
void setEvict(Cache &c);

/**
 * Loads data into the cache in a direct-mapped configuration.
 * @param cache Reference to the cache object.
 * @param iss Input string stream containing memory instructions.
 * @param s Memory address string.
 * @param num_index Index of the cache set.
 */
void directLoad(Cache& cache, std::istringstream& iss, std::string s, int num_index);

/**
 * Stores data into the cache in a direct-mapped configuration.
 * @param cache Reference to the cache object.
 * @param iss Input string stream containing memory instructions.
 * @param s Memory address string.
 * @param num_index Index of the cache set.
 */
void directStore(Cache& cache, std::istringstream& iss, std::string s, int num_index);

/**
 * Adds a new block to a newly created set in the cache.
 * If a set does not exist for the given memory index, this function creates a new set
 * and inserts a new block into it.
 *
 * @param cache Reference to the cache object.
 * @param s Pointer to the new set where the block will be added.
 * @param memory The memory address associated with the new block.
 * @param tag The tag associated with the block.
 * @param load Indicates whether the operation is a load (true) or a store (false).
 */
void addNewBlockToNewSet(Cache &cache, Set *s, uint32_t memory, uint32_t tag, bool load);

/**
 * Adds a new block to an existing set when handling a load operation.
 * This function places a new block into a cache set that already exists, 
 * ensuring proper handling of cache policies.
 *
 * @param cache Reference to the cache object.
 * @param s Pointer to the existing set where the block will be added.
 * @param memory The memory address associated with the new block.
 * @param tag The tag associated with the block.
 */
void addNewBlockToOldSetForLoad(Cache &cache, Set *s, uint32_t memory, uint32_t tag);

/**
 * Adds a new block to an existing set when handling a store operation.
 * This function places a new block into a cache set that already exists,
 * ensuring that store policies (write-allocate, write-through, etc.) are followed.
 *
 * @param cache Reference to the cache object.
 * @param s Pointer to the existing set where the block will be added.
 * @param memory The memory address associated with the new block.
 * @param tag The tag associated with the block.
 */
void addNewBlockToOldSetForStore(Cache &cache, Set *s, uint32_t memory, uint32_t tag);

/**
 * Reads instructions from a file and processes them in the cache.
 * @param cache Reference to the cache object.
 */
void read_file(Cache& cache);

/**
 * Loads a memory block into the cache.
 * @param cache Reference to the cache object.
 * @param iss Input string stream containing memory instructions.
 * @param s Memory address string.
 * @param num_index Index of the cache set.
 */
void load(Cache& cache, std::istringstream& iss, std::string s, int num_index);

/**
 * Stores a memory block into the cache.
 * @param cache Reference to the cache object.
 * @param iss Input string stream containing memory instructions.
 * @param s Memory address string.
 * @param num_index Index of the cache set.
 */
void store(Cache& cache, std::istringstream& iss, std::string s, int num_index);

/**
 * Handles a cache hit by updating relevant cache structures.
 * @param cache Reference to the cache object.
 * @param memory Memory address that caused the hit.
 * @param num_index Index of the cache set.
 * @param num_offset Offset within the cache block.
 * @param load Indicates if the operation is a load (true) or store (false).
 */
void hit(Cache& cache, uint32_t memory, int num_index, int num_offset, bool load);

/**
 * Handles a cache miss by fetching data from memory and updating the cache.
 * @param cache Reference to the cache object.
 * @param memory Memory address that caused the miss.
 * @param num_index Index of the cache set.
 * @param num_offset Offset within the cache block.
 * @param load Indicates if the operation is a load (true) or store (false).
 */
void miss(Cache& cache, uint32_t memory, int num_index, int num_offset, bool load);

/**
 * Handles cache miss eviction based on the replacement policy.
 * @param lru Indicates if LRU replacement policy is used (true) or FIFO (false).
 * @param existed_set Pointer to the cache set where eviction occurs.
 * @param block_ptr Pointer to the block to be evicted.
 * @param cache Reference to the cache object.
 */
void miss_evict(bool lru, Set* existed_set, Block* block_ptr, Cache& cache);
