/*
 * Big integer 
 * CSF Assignment 1
 * Bella Lu
 * xlu62@jhu.edu
 * Kyle Chiang Kim
 * kkim198@jhu.edu
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint32_t value.
// Only the least-significant 32 bits are initialized directly,
// all other bits are set to 0.
//
// Parameters:
//   val - A single uint32_t value
// 
// Return:
//  UInt256 value with all bits other than the least sig. is 0
UInt256 uint256_create_from_u32( uint32_t val ) {
  UInt256 result;

  // Sets it within the first bucket
  result.data[0] = val;

  // Sets the rest of the values to 0
  for (int i = 1; i < 8; i++) {
    result.data[i] = 0;
  }

  return result;
}

// Create a UInt256 value from an array of NWORDS uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
//
// Parameters:
//   data[8] - array of uint32_t values
// 
// Return:
//  UInt256 value which the array is stored
UInt256 uint256_create( const uint32_t data[8] ) {
  UInt256 result;

  // Copies over the data from the array
  for (int i = 0; i < 8; i++) {
    result.data[i] = data[i];
  }

  return result;
}

// Reverses the current string
//
// Parameters:
//   originalStr - the string that wants to be reversed
void reverseStr (char* originalStr){
  int length = strlen(originalStr);
  
  //Reverse the string 
  int j = length - 1;
  char temp;
  for (int i = 0; i < length/2; i++){
    temp = originalStr[j];
    originalStr[j] = originalStr[i];
    originalStr[i] = temp;
    j--;
  }
}

// Sets all the values equal to terminator character
//
// Parameters:
//   originalStr - the string that wants to have \0 
//               for all characters
//   length - The length of the original string
void resetStr (char* originalStr, int length){
  for (int i = 0; i < length; i++){
    originalStr[i] = '\0';
  }
}

// Gets the values from string and assign them to result
//
// Parameters:
//   result - The array of uint32_t where we are storing
//          the results
//   length - The length of the original string
//   revStr - (pointer of a string) The reverse of the  
//          original string in create from hex
void assignFromHex(uint32_t * result, int length, char * revStr){
  // Counting Measures
  int filledBuckets = 0;
  int tracker = 0;
  // Sets up arguements for strtoul
  char * endPtr; 
  char tempStr[9]; // Intialize the temp string
  resetStr(tempStr, 9);
  for (int i = 0; i < length; i++){ // Loops through in factors of 8 
    // Checks for each 32 bit part of the array and assign it correctly 
    if (tracker == 8){
      tracker = 0;
      reverseStr(tempStr);
      result[filledBuckets++] = strtoul(tempStr, &endPtr, 16);
      resetStr(tempStr, 8);
    }
    tempStr[tracker++] = revStr[i];
  }
  reverseStr(tempStr); // Upload the last bucket
  result[filledBuckets] = strtoul(tempStr, &endPtr, 16);
}

// Create a UInt256 value from a string of hexadecimal digits.
//
// Parameters:
//   hex - a pointer to a string that has hex
//
// Return:
//   UInt256 value which the string hex is stored
UInt256 uint256_create_from_hex( const char *hex ) {
  int length = strlen(hex);
  char revStr[length + 1];
  strcpy(revStr, hex);
  reverseStr(revStr);

  // Set up the storage of values
  uint32_t result[8];
  for (int i = 0; i < 8; i++){
    result[i] = 0x00000000U;
  }
  
  // Set the length to the max which is 64 if it is over
  length = (length > 64) ? 64 : length; 

  // Gets the values from hex
  assignFromHex (result, length, revStr);

  return uint256_create(result);
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
//
// Parameters:
//   val - a UINT256 value
//
// Return:
//   char pointer that is a string representation of the UInt256
char *uint256_format_as_hex( UInt256 val ) {
  char *hex = (char *)malloc(65);
  hex[0] = '\0';
  char tempStr[9];
  int shouldBreak = 0;
  for (int i = 7; i >= 0; i--){ // Goes through each 
    if (val.data[i] == 0) {
      continue;
    }
    if (shouldBreak == 0){ // Finds the greatest value then checks for overflow
      sprintf(tempStr, "%x", val.data[i]);
      shouldBreak = 1;
    } else {
      sprintf(tempStr, "%08x", val.data[i]);
    }
    strcat(hex, tempStr);
  }
  // 0 case i.e. only edge case
  if (hex[0] == '\0') hex[0] = '0', hex[1] = '\0';
  return hex;
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
//
// Parameters:
//   val - a Unsigned Int 256 value
//   index - the explicate 32 bit that we want
//
// Return:
//   Unsigned int 32 that is at the position of the UInt 256
uint32_t uint256_get_bits( UInt256 val, unsigned index ) {
  uint32_t bits;
  if (index >= 8) {
    return 0;
  }
  bits = val.data[index];
  return bits;
}

// Return 1 if bit at given index is set, 0 otherwise.
//
// Parameters:
//   val - a Unsigned Int 256 value
//   index - the explicate 32 bit that we want
//
// Return:
//   Int: 1 if bit at given index is set, 0 otherwise.
int uint256_is_bit_set( UInt256 val, unsigned index ) {
  if (index >= 265) {
    return 0;
  }
  unsigned arrIndex = index / 32;
  unsigned bitIndex = index % 32;
  return ((val.data[arrIndex]) & (1U << bitIndex)) != 0;
}

// Compute the sum of two UInt256 values.
//
// Parameters:
//   left - a Unsigned Int 256 value
//   right - a Unsigned Int 256 value
//
// Return:
//   Unsigned Int 256: The sum of left and right
UInt256 uint256_add( UInt256 left, UInt256 right ) {
  UInt256 sum;
  unsigned int tempSum = 0;
  int carryOver = 0;
  for (int i = 0; i < 8; i++){ // Goes over each bucket
    // Finds the sum of the two combined together
    tempSum = left.data[i] + right.data[i];
    if (carryOver == 1) { // Carries over one more
      tempSum = tempSum + 1;
    } 
    // Checks to see which one has a bigger value and then checks if it overloads
    carryOver = 0;
    if (left.data[i] > right.data[i]){
      if (tempSum < left.data[i]) {
        carryOver = 1;
      }
    } else if (tempSum < right.data[i]){
      carryOver = 1;
    }
    // Adds the overloaded or non-overloaded value to the corresponding bucket
    sum.data[i] = tempSum;
  }

  return sum;
}

// Compute the difference of two UInt256 values.
//
// Parameters:
//   left - a Unsigned Int 256 value
//   right - a Unsigned Int 256 value
//
// Return:
//   Unsigned Int 256: The difference of left and right
UInt256 uint256_sub( UInt256 left, UInt256 right ) {
  UInt256 result;

  // Negate the RHS
  UInt256 neg = uint256_negate(right);

  // Add LHS to the negated RHS
  result = uint256_add(left, neg);
  return result;
}

// Return the two's-complement negation of the given UInt256 value.
//
// Parameters:
//   val - a Unsigned Int 256 value
//
// Return:
//   Unsigned Int 256: The two's complement of val
UInt256 uint256_negate( UInt256 val ) {
  UInt256 result;

  // Traverse each "chunk" and negate 
  for (int i = 0; i < 8; ++i) {
    result.data[i] = ~val.data[i];
  }

  // Create one using previous func and add it
  UInt256 one = uint256_create_from_u32(1);
  result = uint256_add(result, one);

  // return uint256_add(result, one);
  return result;
}


// Compute the product of two UInt256 values.
//
// Parameters:
//   left - a Unsigned Int 256 value
//   right - a Unsigned Int 256 value
//
// Return:
//   Unsigned Int 256: The product of left and right
UInt256 uint256_mul( UInt256 left, UInt256 right ) {
  UInt256 product;

  // Initialize product
  for (int i = 0; i < 8; i ++){
    product.data[i] = 0;
  }

  for (int i = 0; i < 256; ++i) {
    // Check if bit is set to 1
    if (uint256_is_bit_set(left, i)) {
      // Left shift right by i bits
      UInt256 shifted = uint256_lshift(right, i);
      // Update product by adding the newly computed term
      product = uint256_add(product, shifted);
    }
  }
  return product;
}

// Shifts the bits to the left
//
// Parameters:
//   val - a Unsigned Int 256 value
//   shift - unsigned int and how much we can to shift by
//          Should be less than 256
//
// Return:
//   Unsigned Int 256: The shifted bits
UInt256 uint256_lshift( UInt256 val, unsigned shift ) {
  assert( shift < 256 );
  UInt256 result = {0};

  // Calculate number of "chunks" to shift and the residual shifts
  unsigned arrShift = shift / 32;
  unsigned bitShift = shift % 32;

  // Initialize result
  for (int i = 0; i < 8; i ++){
    result.data[i] = 0;
  }

  for (unsigned int i = 0; i < 8 - (arrShift); ++i) {
    // Move over the front portion of the "chunks"
    result.data[i + arrShift] |= val.data[i] << bitShift;
    if (bitShift != 0 && (i + arrShift) < 8) {
      // Add on the portion that should be carried over to the next "chunk"
      result.data[i + arrShift + 1] |= val.data[i] >> (32 - bitShift);
    }
  }

  return result;
}
