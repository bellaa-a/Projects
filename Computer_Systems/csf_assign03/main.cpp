#include "cache.h"

int main( int argc, char **argv ) {
  // Return 1 if the command line args aren't correct
  if (!isValidArgs(argc, argv)){
    return 1;
  }
  
  Cache c = setUpCache(argv); // Sets Up the Cache
  read_file(c); // Decipher file
  printCacheStats(c); // Pring final results
  
  return 0;
}
