/*
 * Tests for big integer
 * CSF Assignment 1
 * Bella Lu
 * xlu62@jhu.edu
 * Kyle Chiang Kim
 * kkim198@jhu.edu
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"

#include "uint256.h"

typedef struct {
  UInt256 zero; // the value equal to 0
  UInt256 one;  // the value equal to 1
  UInt256 max;  // the value equal to (2^256)-1
  UInt256 msb_set; // the value equal to 2^255
} TestObjs;

// Helper functions for implementing tests
void set_all( UInt256 *val, uint32_t wordval );

#define ASSERT_SAME( expected, actual ) \
do { \
  ASSERT( expected.data[0] == actual.data[0] ); \
  ASSERT( expected.data[1] == actual.data[1] ); \
  ASSERT( expected.data[2] == actual.data[2] ); \
  ASSERT( expected.data[3] == actual.data[3] ); \
  ASSERT( expected.data[4] == actual.data[4] ); \
  ASSERT( expected.data[5] == actual.data[5] ); \
  ASSERT( expected.data[6] == actual.data[6] ); \
  ASSERT( expected.data[7] == actual.data[7] ); \
} while ( 0 )

#define INIT_FROM_ARR( val, arr ) \
do { \
  for ( int i = 0; i < 8; ++i ) \
    val.data[i] = arr[i]; \
} while ( 0 )

// Functions to create and cleanup the test fixture object
TestObjs *setup( void );
void cleanup( TestObjs *objs );

// Declarations of test functions
void test_get_bits( TestObjs *objs );
void test_is_bit_set( TestObjs *objs );
void test_create_from_u32( TestObjs *objs );
void test_create( TestObjs *objs );
void test_create_from_hex( TestObjs *objs );
void test_format_as_hex( TestObjs *objs );
void test_add( TestObjs *objs );
void test_sub( TestObjs *objs );
void test_negate( TestObjs *objs );
void test_neg_overflow( TestObjs *objs );
void test_mul( TestObjs *objs );
void test_lshift( TestObjs *objs );

// Add Unit Tests
void test_add_complex_1();
void test_add_complex_2();
void test_add_complex_3();
void test_add_left_num_zero();
void test_add_right_num_zero();
void test_add_overflow();

// Sub Unit Tests
void test_sub_complex_1();
void test_sub_complex_2();
void test_sub_complex_3();
void test_sub_right_num_zero();
void test_sub_overflow();

// Mul Unit Tests
void test_mul_complex_1();
void test_mul_complex_2();
void test_mul_complex_3();

// Create From Hex Tests
void create_from_hex_complex_case_1();
void create_from_hex_complex_case_2();
void create_from_hex_complex_case_3();
void create_from_hex_complex_case_4();

// Format As Hex Tests
void format_as_hex_complex_case_1();
void format_as_hex_complex_case_2();
void format_as_hex_complex_case_3();
void format_as_hex_complex_case_4();

// Get Bits Tests
void get_bits_complex_case_1();
void get_bits_complex_case_2();
void get_bits_complex_case_3();
void get_bits_complex_case_4();
void get_bits_outside_index_range();

// Negate Tests
void test_negate_large_random();
void test_negate_boundary_value();
void test_negate_alternating_bits();
void test_negate_single_nonzero_chunk();

// Lshift Tests 
void test_lshift_max_shift();
void test_lshift_one_chunk();
void test_lshift_partial_chunk_shift();
void test_lshift_two_chunks_shift();
void test_lshift_large_random();
void test_lshift_max_value();


int main( int argc, char **argv ) {
  if ( argc > 1 )
    tctest_testname_to_execute = argv[1];

  TEST_INIT();

  TEST( test_get_bits );
  TEST( test_is_bit_set );
  TEST( test_create_from_u32 );
  TEST( test_create );
  TEST( test_create_from_hex );
  TEST( test_format_as_hex );
  TEST( test_add );
  TEST( test_sub );
  TEST( test_negate );
  TEST( test_neg_overflow );
  TEST( test_mul );
  TEST( test_lshift );

  // Unit Test For Add
  TEST( test_add_complex_1 );
  TEST( test_add_complex_2 );
  TEST( test_add_complex_3 );
  TEST( test_add_left_num_zero );
  TEST( test_add_right_num_zero );
  TEST( test_add_overflow );

  // Unit Test For Sub
  TEST( test_sub_complex_1 );
  TEST( test_sub_complex_2 );
  TEST( test_sub_complex_3 );
  TEST( test_sub_right_num_zero );
  TEST( test_sub_overflow );
  
  // Unit Test For Mul
  TEST( test_mul_complex_1 );
  TEST( test_mul_complex_2 );
  TEST( test_mul_complex_3 );

  // Unit Test For Create From Hex
  TEST( create_from_hex_complex_case_1 );
  TEST( create_from_hex_complex_case_2 );
  TEST( create_from_hex_complex_case_3 );
  TEST( create_from_hex_complex_case_4 );

  // Unit Test Format As Hex
  TEST( format_as_hex_complex_case_1 );
  TEST( format_as_hex_complex_case_2 );
  TEST( format_as_hex_complex_case_3 );
  TEST( format_as_hex_complex_case_4 );

  // Unit Test Get Bits
  TEST( get_bits_complex_case_1 );
  TEST( get_bits_complex_case_2 );
  TEST( get_bits_complex_case_3 );
  TEST( get_bits_complex_case_4 );
  TEST( get_bits_outside_index_range );

  // Unit Test Negate
  TEST( test_negate_large_random );
  TEST( test_negate_boundary_value );
  TEST( test_negate_alternating_bits );
  TEST( test_negate_single_nonzero_chunk );

  // Unit Test Lshift 
  TEST( test_lshift_max_shift );
  TEST( test_lshift_one_chunk );
  TEST( test_lshift_partial_chunk_shift );
  TEST( test_lshift_two_chunks_shift );
  TEST( test_lshift_large_random );
  TEST( test_lshift_max_value ); 

  TEST_FINI();
}

// Set all of the "words" of a UInt256 to a specific initial value
void set_all( UInt256 *val, uint32_t wordval ) {
  for ( unsigned i = 0; i < 8; ++i ) {
    val->data[i] = wordval;
  }
}

TestObjs *setup( void ) {
  TestObjs *objs = (TestObjs *) malloc( sizeof(TestObjs ) );

  // initialize several UInt256 values "manually"
  set_all( &objs->zero, 0 );
  set_all( &objs->one, 0 );
  objs->one.data[0] = 1U;
  set_all( &objs->max, 0xFFFFFFFFU );

  // create a value with only the most-significant bit set
  uint32_t msb_set_data[8] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0x80000000U };
  INIT_FROM_ARR( objs->msb_set, msb_set_data );

  return objs;
}

void cleanup( TestObjs *objs ) {
  free( objs );
}

void test_get_bits( TestObjs *objs ) {
  ASSERT( 0U == uint256_get_bits( objs->zero, 0 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 1 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 2 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 3 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 4 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 5 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 6 ) );
  ASSERT( 0U == uint256_get_bits( objs->zero, 7 ) );

  ASSERT( 1U == uint256_get_bits( objs->one, 0 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 1 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 2 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 3 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 4 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 5 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 6 ) );
  ASSERT( 0U == uint256_get_bits( objs->one, 7 ) );

  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 0 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 1 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 2 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 3 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 4 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 5 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 6 ) );
  ASSERT( 0xFFFFFFFFU == uint256_get_bits( objs->max, 7 ) );
}

void test_is_bit_set( TestObjs *objs ) {
  // no bits should be set in the zero value
  for ( int i = 0; i < 256; ++i )
    ASSERT( !uint256_is_bit_set( objs->zero, i ) );

  // only bit 0 should be set in the one value
  ASSERT( uint256_is_bit_set( objs->one, 0 ) );
  for ( int i = 1; i < 256; ++i )
    ASSERT( !uint256_is_bit_set( objs->one, i ) );

  // only bit 255 should be set in the msb_set value
  for ( int i = 0; i < 255; ++i )
    ASSERT( !uint256_is_bit_set( objs->msb_set, i ) );
  ASSERT( uint256_is_bit_set( objs->msb_set, 255 ) );

  // all bits should be set in the max value
  for ( int i = 0; i < 256; ++i )
    ASSERT( uint256_is_bit_set( objs->max, i ) );
}

void test_create_from_u32( TestObjs *objs ) {
  UInt256 zero = uint256_create_from_u32( 0U );
  UInt256 one = uint256_create_from_u32( 1U );

  ASSERT_SAME( objs->zero, zero );
  ASSERT_SAME( objs->one, one );
}

void test_create( TestObjs *objs ) {
  (void) objs;

  uint32_t data1[8] = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U };
  UInt256 val1 = uint256_create( data1 );
  ASSERT( 1U == val1.data[0] );
  ASSERT( 2U == val1.data[1] );
  ASSERT( 3U == val1.data[2] );
  ASSERT( 4U == val1.data[3] );
  ASSERT( 5U == val1.data[4] );
  ASSERT( 6U == val1.data[5] );
  ASSERT( 7U == val1.data[6] );
  ASSERT( 8U == val1.data[7] );
}

void test_create_from_hex( TestObjs *objs ) {
  
  UInt256 zero = uint256_create_from_hex( "0" );
  ASSERT_SAME( objs->zero, zero );

  UInt256 one = uint256_create_from_hex( "1" );
  ASSERT_SAME( objs->one, one );

  UInt256 max = uint256_create_from_hex( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
  ASSERT_SAME( objs->max, max );
  
}

void test_format_as_hex( TestObjs *objs ) {
  char *s;

  s = uint256_format_as_hex( objs->zero );
  ASSERT( 0 == strcmp( "0", s ) );
  free( s );

  s = uint256_format_as_hex( objs->one );
  ASSERT( 0 == strcmp( "1", s ) );
  free( s );

  s = uint256_format_as_hex( objs->max );
  ASSERT( 0 == strcmp( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", s ) );
  free( s );
}

void test_add( TestObjs *objs ) {
  UInt256 result;

  result = uint256_add( objs->zero, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_add( objs->zero, objs->one );
  ASSERT_SAME( objs->one, result );

  uint32_t two_data[8] = { 2U };
  UInt256 two;
  INIT_FROM_ARR( two, two_data );
  result = uint256_add( objs->one, objs->one );
  ASSERT_SAME( two, result );

  result = uint256_add( objs->max, objs->one );
  ASSERT_SAME( objs->zero, result );
}

void test_sub( TestObjs *objs ) {
  UInt256 result;

  result = uint256_sub( objs->zero, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_sub( objs->one, objs->one );
  ASSERT_SAME( objs->zero, result );

  result = uint256_sub( objs->zero, objs->one );
  ASSERT_SAME( objs->max, result );
}

void test_negate( TestObjs *objs ) {
  UInt256 result;

  result = uint256_negate( objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_negate( objs->one );
  ASSERT_SAME( objs->max, result );

  result = uint256_negate( objs->max );
  ASSERT_SAME( objs->one, result );
}

void test_neg_overflow( TestObjs *objs ) {
  UInt256 result;

  uint32_t two_data[8] = { 2U };
  UInt256 two;
  INIT_FROM_ARR( two, two_data );

  result = uint256_sub( objs->one, objs->max );
  ASSERT_SAME( two, result );
}

void test_mul( TestObjs *objs ) {
  UInt256 left, right, result;

  // some very basic tests

  result = uint256_mul( objs->one, objs->one );
  ASSERT_SAME( objs->one, result );

  result = uint256_mul( objs->one, objs->zero );
  ASSERT_SAME( objs->zero, result );

  result = uint256_mul( objs->zero, objs->one );
  ASSERT_SAME( objs->zero, result );

  UInt256 two = { { 2,0,0,0,0,0,0,0 } }, four = { { 4,0,0,0,0,0,0,0 } };
  result = uint256_mul( two, two );
  ASSERT_SAME( four, result );

  // a more complicated test

  // fc42c691d6284761fb49dd54f3a13eb * 4cfd2c7d8790c50c280ff0ff77617a8e = 4bdd4cc8b6067f7617c05917f828d17a26046ba5f436cb7df595f6c68c00a5a
  left.data[0] = 0x4f3a13ebU;
  left.data[1] = 0x1fb49dd5U;
  left.data[2] = 0x1d628476U;
  left.data[3] = 0xfc42c69U;
  left.data[4] = 0x0U;
  left.data[5] = 0x0U;
  left.data[6] = 0x0U;
  left.data[7] = 0x0U;
  right.data[0] = 0x77617a8eU;
  right.data[1] = 0x280ff0ffU;
  right.data[2] = 0x8790c50cU;
  right.data[3] = 0x4cfd2c7dU;
  right.data[4] = 0x0U;
  right.data[5] = 0x0U;
  right.data[6] = 0x0U;
  right.data[7] = 0x0U;
  result = uint256_mul(left, right);
  ASSERT(0x68c00a5aU == result.data[0]);
  ASSERT(0xdf595f6cU == result.data[1]);
  ASSERT(0x5f436cb7U == result.data[2]);
  ASSERT(0xa26046baU == result.data[3]);
  ASSERT(0x7f828d17U == result.data[4]);
  ASSERT(0x617c0591U == result.data[5]);
  ASSERT(0x8b6067f7U == result.data[6]);
  ASSERT(0x4bdd4ccU == result.data[7]);
}

void test_lshift( TestObjs *objs ) {
  UInt256 result;

  UInt256 two = { { 2,0,0,0,0,0,0,0 } }, four = { { 4,0,0,0,0,0,0,0 } };

  // some very basic tests

  result = uint256_lshift( objs->one, 0 );
  ASSERT_SAME( objs->one, result );

  result = uint256_lshift( objs->one, 1 );
  ASSERT_SAME( two, result );

  result = uint256_lshift( objs->one, 2 );
  ASSERT_SAME( four, result );

  // a more complicated test
  {
    // Test shifting 727767d07ccff5fe25cd125b4523e8c7db1b8d1a2c8a2830284d72bb872c33a5 left by 50 bit(s)
    uint32_t arr[8] = {0x872c33a5U, 0x284d72bbU, 0x2c8a2830U, 0xdb1b8d1aU, 0x4523e8c7U, 0x25cd125bU, 0x7ccff5feU, 0x727767d0U};
    UInt256 val;
    INIT_FROM_ARR( val, arr );
    uint32_t expected_arr[8] = {0x00000000U, 0xce940000U, 0xcaee1cb0U, 0xa0c0a135U, 0x3468b228U, 0xa31f6c6eU, 0x496d148fU, 0xd7f89734U};
    UInt256 expected;
    INIT_FROM_ARR( expected, expected_arr );
    result = uint256_lshift( val, 50U );
    ASSERT_SAME( expected, result );
  }
}

//A random addition provided by genfact.rb 
void test_add_complex_1() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xd5eb740cU;
  left.data[1] = 0xfe4b02d4U;
  left.data[2] = 0x9ad2396cU;
  left.data[3] = 0x40e38ee2U;
  left.data[4] = 0x1040b2e1U;
  left.data[5] = 0x540c5c5cU;
  left.data[6] = 0x6dc14ce5U;
  left.data[7] = 0x8c08cffU;
  right.data[0] = 0x2e725ed9U;
  right.data[1] = 0x177d7f1fU;
  right.data[2] = 0xd0c68a5dU;
  right.data[3] = 0x3d3d8a81U;
  right.data[4] = 0xe8271dd8U;
  right.data[5] = 0x8d7aa2d8U;
  right.data[6] = 0x28f44be7U;
  right.data[7] = 0xd94b5d7U;
  result = uint256_add(left, right);

  testResults.data[0] = 0x045dd2e5U;
  testResults.data[1] = 0x15c881f4U;
  testResults.data[2] = 0x6b98c3caU;
  testResults.data[3] = 0x7e211964U;
  testResults.data[4] = 0xf867d0b9U;
  testResults.data[5] = 0xe186ff34U;
  testResults.data[6] = 0x96b598ccU;
  testResults.data[7] = 0x165542d6U;

  ASSERT_SAME (result, testResults);
}

//A random addition provided by genfact.rb 
void test_add_complex_2() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xbb014251U;
  left.data[1] = 0x714d3bc2U;
  left.data[2] = 0x0d8613f8U;
  left.data[3] = 0xe86adf9fU;
  left.data[4] = 0x44263516U;
  left.data[5] = 0xb40277b9U;
  left.data[6] = 0x204e2445U;
  left.data[7] = 0x7488969U;
  right.data[0] = 0xc849f5f9U;
  right.data[1] = 0x6fae8739U;
  right.data[2] = 0x9400b660U;
  right.data[3] = 0x0f5e0e46U;
  right.data[4] = 0x76915641U;
  right.data[5] = 0xff41ea2eU;
  right.data[6] = 0x9fd1e136U;
  right.data[7] = 0x6de797U;
  result = uint256_add(left, right);

  testResults.data[0] = 0x834b384aU;
  testResults.data[1] = 0xe0fbc2fcU;
  testResults.data[2] = 0xa186ca58U;
  testResults.data[3] = 0xf7c8ede5U;
  testResults.data[4] = 0xbab78b57U;
  testResults.data[5] = 0xb34461e7U;
  testResults.data[6] = 0xc020057cU;
  testResults.data[7] = 0x7b67100U;

  ASSERT_SAME (result, testResults);
}

//A random addition provided by genfact.rb 
void test_add_complex_3() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xf375c6c4U;
  left.data[1] = 0x43fc553fU;
  left.data[2] = 0x949cad29U;
  left.data[3] = 0xcbe95d8dU;
  left.data[4] = 0xe00dc260U;
  left.data[5] = 0xcc028489U;
  left.data[6] = 0xc86f96c6U;
  left.data[7] = 0x58c7da6U;
  right.data[0] = 0x4eba22c4U;
  right.data[1] = 0xbcb1f405U;
  right.data[2] = 0xb0a0b36cU;
  right.data[3] = 0xbb5b3329U;
  right.data[4] = 0xfb476505U;
  right.data[5] = 0x78573504U;
  right.data[6] = 0xedebfdd2U;
  right.data[7] = 0x5c97c1cU;
  result = uint256_add(left, right);

  testResults.data[0] = 0x422fe988U;
  testResults.data[1] = 0x00ae4945U;
  testResults.data[2] = 0x453d6096U;
  testResults.data[3] = 0x874490b7U;
  testResults.data[4] = 0xdb552766U;
  testResults.data[5] = 0x4459b98eU;
  testResults.data[6] = 0xb65b9499U;
  testResults.data[7] = 0xb55f9c3U;

  ASSERT_SAME (result, testResults);
}

//Adding zero has any affect if zero is the left number
void test_add_left_num_zero() {
  UInt256 left, right, result, testResults;

  right.data[0] = 0xf375c6c4U;
  right.data[1] = 0x43fc553fU;
  right.data[2] = 0x949cad29U;
  right.data[3] = 0xcbe95d8dU;
  right.data[4] = 0xe00dc260U;
  right.data[5] = 0xcc028489U;
  right.data[6] = 0xc86f96c6U;
  right.data[7] = 0x58c7da6U;
  left.data[0] = 0x00000000U;
  left.data[1] = 0x00000000U;
  left.data[2] = 0x00000000U;
  left.data[3] = 0x00000000U;
  left.data[4] = 0x00000000U;
  left.data[5] = 0x00000000U;
  left.data[6] = 0x00000000U;
  left.data[7] = 0x00000000U;
  result = uint256_add(left, right);

  testResults.data[0] = 0xf375c6c4U;
  testResults.data[1] = 0x43fc553fU;
  testResults.data[2] = 0x949cad29U;
  testResults.data[3] = 0xcbe95d8dU;
  testResults.data[4] = 0xe00dc260U;
  testResults.data[5] = 0xcc028489U;
  testResults.data[6] = 0xc86f96c6U;
  testResults.data[7] = 0x58c7da6U;

  ASSERT_SAME (result, testResults);
}

//Adding zero has any affect if zero is the right number
void test_add_right_num_zero() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xf375c6c4U;
  left.data[1] = 0x43fc553fU;
  left.data[2] = 0x949cad29U;
  left.data[3] = 0xcbe95d8dU;
  left.data[4] = 0xe00dc260U;
  left.data[5] = 0xcc028489U;
  left.data[6] = 0xc86f96c6U;
  left.data[7] = 0x58c7da6U;
  right.data[0] = 0x00000000U;
  right.data[1] = 0x00000000U;
  right.data[2] = 0x00000000U;
  right.data[3] = 0x00000000U;
  right.data[4] = 0x00000000U;
  right.data[5] = 0x00000000U;
  right.data[6] = 0x00000000U;
  right.data[7] = 0x00000000U;
  result = uint256_add(left, right);

  testResults.data[0] = 0xf375c6c4U;
  testResults.data[1] = 0x43fc553fU;
  testResults.data[2] = 0x949cad29U;
  testResults.data[3] = 0xcbe95d8dU;
  testResults.data[4] = 0xe00dc260U;
  testResults.data[5] = 0xcc028489U;
  testResults.data[6] = 0xc86f96c6U;
  testResults.data[7] = 0x58c7da6U;

  ASSERT_SAME (result, testResults);
}

//Test Overflow With Addition
void test_add_overflow( TestObjs *objs ){
  UInt256 result;

  uint32_t two_data[8] = { 2U };
  UInt256 two;
  INIT_FROM_ARR( two, two_data );

  result = uint256_add( two, objs->max );
  ASSERT_SAME( objs -> one, result );
}

//A random subtraction test provided by genfact.rb 
void test_sub_complex_1() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xca6a21efU;
  left.data[1] = 0x27e67f54U;
  left.data[2] = 0x5a9f36c5U;
  left.data[3] = 0xa3b1813cU;
  left.data[4] = 0xf4e98e4aU;
  left.data[5] = 0x9029c4c6U;
  left.data[6] = 0xd9aab6f1U;
  left.data[7] = 0xfa3ec80U;
  right.data[0] = 0xbd77cd71U;
  right.data[1] = 0xb3a6c1b3U;
  right.data[2] = 0x1131b8abU;
  right.data[3] = 0xfe0d9846U;
  right.data[4] = 0xa613ed51U;
  right.data[5] = 0xdcc8b179U;
  right.data[6] = 0xe92bdc1eU;
  right.data[7] = 0x8490062U;
  result = uint256_sub(left, right);

  testResults.data[0] = 0x0cf2547eU;
  testResults.data[1] = 0x743fbda1U;
  testResults.data[2] = 0x496d7e19U;
  testResults.data[3] = 0xa5a3e8f6U;
  testResults.data[4] = 0x4ed5a0f8U;
  testResults.data[5] = 0xb361134dU;
  testResults.data[6] = 0xf07edad2U;
  testResults.data[7] = 0x75aec1dU;

  ASSERT_SAME (result, testResults);
}

//A random subtraction test provided by genfact.rb 
void test_sub_complex_2() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0x2d2b6703U;
  left.data[1] = 0x1c4f0403U;
  left.data[2] = 0x0dcf491aU;
  left.data[3] = 0x190b60f9U;
  left.data[4] = 0xf3e6068fU;
  left.data[5] = 0x3ec662e6U;
  left.data[6] = 0xfb4282dcU;
  left.data[7] = 0xea52b76U;

  right.data[0] = 0x3349e8d7U;
  right.data[1] = 0x15fa1247U;
  right.data[2] = 0xbaa75846U;
  right.data[3] = 0x48c2de2aU;
  right.data[4] = 0x14b80c47U;
  right.data[5] = 0x570394ffU;
  right.data[6] = 0xb9561629U;
  right.data[7] = 0x19174d9U;
  result = uint256_sub(left, right);

  testResults.data[0] = 0xf9e17e2cU;
  testResults.data[1] = 0x0654f1bbU;
  testResults.data[2] = 0x5327f0d4U;
  testResults.data[3] = 0xd04882ceU;
  testResults.data[4] = 0xdf2dfa47U;
  testResults.data[5] = 0xe7c2cde7U;
  testResults.data[6] = 0x41ec6cb2U;
  testResults.data[7] = 0xd13b69dU;

  ASSERT_SAME (result, testResults);
}

//A random subtraction test provided by genfact.rb 
void test_sub_complex_3() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xaec6b401U;
  left.data[1] = 0x0f634fb9U;
  left.data[2] = 0xcc9780e0U;
  left.data[3] = 0x906607aaU;
  left.data[4] = 0xf27e25f4U;
  left.data[5] = 0x5562f140U;
  left.data[6] = 0x0eca04dbU;
  left.data[7] = 0xd93705cU;
  right.data[0] = 0x81dda299U;
  right.data[1] = 0x1e94b376U;
  right.data[2] = 0x863d0694U;
  right.data[3] = 0x573337ddU;
  right.data[4] = 0x24137027U;
  right.data[5] = 0x47e43fcaU;
  right.data[6] = 0x6b7c29dfU;
  right.data[7] = 0xb940e5eU;
  result = uint256_sub(left, right);

  testResults.data[0] = 0x2ce91168U;
  testResults.data[1] = 0xf0ce9c43U;
  testResults.data[2] = 0x465a7a4bU;
  testResults.data[3] = 0x3932cfcdU;
  testResults.data[4] = 0xce6ab5cdU;
  testResults.data[5] = 0x0d7eb176U;
  testResults.data[6] = 0xa34ddafcU;
  testResults.data[7] = 0x1ff61fdU;

  ASSERT_SAME (result, testResults);
}

//Subtracting a zero from a number to make sure that it doesn't affect it
void test_sub_right_num_zero() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xf375c6c4U;
  left.data[1] = 0x43fc553fU;
  left.data[2] = 0x949cad29U;
  left.data[3] = 0xcbe95d8dU;
  left.data[4] = 0xe00dc260U;
  left.data[5] = 0xcc028489U;
  left.data[6] = 0xc86f96c6U;
  left.data[7] = 0x58c7da6U;

  right.data[0] = 0x00000000U;
  right.data[1] = 0x00000000U;
  right.data[2] = 0x00000000U;
  right.data[3] = 0x00000000U;
  right.data[4] = 0x00000000U;
  right.data[5] = 0x00000000U;
  right.data[6] = 0x00000000U;
  right.data[7] = 0x00000000U;

  result = uint256_sub(left, right);

  testResults.data[0] = 0xf375c6c4U;
  testResults.data[1] = 0x43fc553fU;
  testResults.data[2] = 0x949cad29U;
  testResults.data[3] = 0xcbe95d8dU;
  testResults.data[4] = 0xe00dc260U;
  testResults.data[5] = 0xcc028489U;
  testResults.data[6] = 0xc86f96c6U;
  testResults.data[7] = 0x58c7da6U;

  ASSERT_SAME (result, testResults);
}

//Subtracting a one from zero so that it overflows
void test_sub_overflow( TestObjs *objs ){
  UInt256 result;
  result = uint256_sub( objs->zero, objs->max );
  ASSERT_SAME( objs -> one, result );
}

//A random mul test provided by genfact.rb 
void test_mul_complex_1() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xab37b64fU;
  left.data[1] = 0xf24b5f61U;
  left.data[2] = 0x4bba9908U;
  left.data[3] = 0x4e41804U;
  left.data[4] = 0x00000000U;
  left.data[5] = 0x00000000U;
  left.data[6] = 0x00000000U;
  left.data[7] = 0x00000000U;
  right.data[0] = 0x958f651bU;
  right.data[1] = 0xdf86bddeU;
  right.data[2] = 0xc133321eU;
  right.data[3] = 0x6708b61U;
  right.data[4] = 0x00000000U;
  right.data[5] = 0x00000000U;
  right.data[6] = 0x00000000U;
  right.data[7] = 0x00000000U;
  result = uint256_mul(left, right);

  testResults.data[0] = 0xdaee6555U;
  testResults.data[1] = 0x450d1496U;
  testResults.data[2] = 0xe300a67cU;
  testResults.data[3] = 0x3d36452cU;
  testResults.data[4] = 0x43e6d6daU;
  testResults.data[5] = 0x56074bf1U;
  testResults.data[6] = 0x52cfe079U;
  testResults.data[7] = 0x1f7f04U;

  ASSERT_SAME (result, testResults);
}

//A random mul test provided by genfact.rb 
void test_mul_complex_2() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0xb47bbbe0U;
  left.data[1] = 0x4eae19e7U;
  left.data[2] = 0x6b8501edU;
  left.data[3] = 0xe4781cbU;
  left.data[4] = 0x00000000U;
  left.data[5] = 0x00000000U;
  left.data[6] = 0x00000000U;
  left.data[7] = 0x00000000U;
  right.data[0] = 0xe550ec5fU;
  right.data[1] = 0x7e38f750U;
  right.data[2] = 0x1158c952U;
  right.data[3] = 0x60310b5U;
  right.data[4] = 0x00000000U;
  right.data[5] = 0x00000000U;
  right.data[6] = 0x00000000U;
  right.data[7] = 0x00000000U;
  result = uint256_mul(left, right);

  testResults.data[0] = 0x211d3820U;
  testResults.data[1] = 0x31482318U;
  testResults.data[2] = 0x17bce602U;
  testResults.data[3] = 0xe1970e1dU;
  testResults.data[4] = 0x8e5ca74cU;
  testResults.data[5] = 0xc9b4a718U;
  testResults.data[6] = 0xdb8a8f93U;
  testResults.data[7] = 0x55d8cfU;

  ASSERT_SAME (result, testResults);
}

//A random mul test provided by genfact.rb 
void test_mul_complex_3() {
  UInt256 left, right, result, testResults;

  left.data[0] = 0x86f06a81U;
  left.data[1] = 0x337fe2dfU;
  left.data[2] = 0x6e2f7ccaU;
  left.data[3] = 0xde3a749U;
  left.data[4] = 0x00000000U;
  left.data[5] = 0x00000000U;
  left.data[6] = 0x00000000U;
  left.data[7] = 0x00000000U;
  right.data[0] = 0x218ec708U;
  right.data[1] = 0x0a4b617fU;
  right.data[2] = 0x88e54a83U;
  right.data[3] = 0x1b1c323U;
  right.data[4] = 0x00000000U;
  right.data[5] = 0x00000000U;
  right.data[6] = 0x00000000U;
  right.data[7] = 0x00000000U;
  result = uint256_mul(left, right);

  testResults.data[0] = 0xcedb9b08U;
  testResults.data[1] = 0xf52766f5U;
  testResults.data[2] = 0x31a81728U;
  testResults.data[3] = 0x81071d31U;
  testResults.data[4] = 0x61f0dfdfU;
  testResults.data[5] = 0xe02ecae6U;
  testResults.data[6] = 0x492d8dd8U;
  testResults.data[7] = 0x1788a4U;

  ASSERT_SAME (result, testResults);
}

//Testing a random generated hex actual maps correctly
void create_from_hex_complex_case_1() {
  UInt256 result;

  result.data[0] = 0x8f288110U;
  result.data[1] = 0x5cd9bbe1U;
  result.data[2] = 0xfeeb9e31U;
  result.data[3] = 0x74ca2b48U;
  result.data[4] = 0x649d11c4U;
  result.data[5] = 0x64f11534U;
  result.data[6] = 0x4123742cU;
  result.data[7] = 0x1035cdfcU;

  UInt256 max = uint256_create_from_hex( "1035cdfc4123742c64f11534649d11c474ca2b48feeb9e315cd9bbe18f288110" );
  ASSERT_SAME( result, max );
}

//Testing a random generated hex actual maps correctly
void create_from_hex_complex_case_2() {
  UInt256 result;

  result.data[7] = 0x11e4eb69U;
  result.data[6] = 0x3762fd3fU;
  result.data[5] = 0xa50bbadbU;
  result.data[4] = 0xc7ec76bcU;
  result.data[3] = 0x1f4dba8aU;
  result.data[2] = 0xc139f393U;
  result.data[1] = 0x343cbd95U;
  result.data[0] = 0xcb3f7471U;

  UInt256 max = uint256_create_from_hex( "11e4eb693762fd3fa50bbadbc7ec76bc1f4dba8ac139f393343cbd95cb3f7471" );
  ASSERT_SAME( result, max );
}

//Testing a random generated hex actual maps correctly
void create_from_hex_complex_case_3() {
  UInt256 result;

  result.data[7] = 0x84cb5f8U;
  result.data[6] = 0x629be339U;
  result.data[5] = 0x9567e7d8U;
  result.data[4] = 0xc54bc839U;
  result.data[3] = 0xd4f67e72U;
  result.data[2] = 0x733b182cU;
  result.data[1] = 0x37164d5dU;
  result.data[0] = 0x230abbb8U;

  UInt256 max = uint256_create_from_hex( "84cb5f8629be3399567e7d8c54bc839d4f67e72733b182c37164d5d230abbb8" );
  ASSERT_SAME( result, max );
}

//Testing a random generated hex actual maps correctly (with empty 32 u-int buckets)
void create_from_hex_complex_case_4() {
  UInt256 result;

  result.data[0] = 0x906cbfd3U;
  result.data[1] = 0x7e929ca9U;
  result.data[2] = 0x19c8efcaU;
  result.data[3] = 0xb43fd12U;
  result.data[4] = 0x00000000U;
  result.data[5] = 0x00000000U;
  result.data[6] = 0x00000000U;
  result.data[7] = 0x00000000U;

  UInt256 max = uint256_create_from_hex( "b43fd1219c8efca7e929ca9906cbfd3" );
  ASSERT_SAME( result, max );
}

//Testing a random generated hex actual formals correctly
void format_as_hex_complex_case_1() {
  UInt256 result;

  result.data[0] = 0x8f288110U;
  result.data[1] = 0x5cd9bbe1U;
  result.data[2] = 0xfeeb9e31U;
  result.data[3] = 0x74ca2b48U;
  result.data[4] = 0x649d11c4U;
  result.data[5] = 0x64f11534U;
  result.data[6] = 0x4123742cU;
  result.data[7] = 0x1035cdfcU;

  char * strResults = uint256_format_as_hex(result);
  ASSERT( 0 == strcmp( strResults , "1035cdfc4123742c64f11534649d11c474ca2b48feeb9e315cd9bbe18f288110" ));
  free(strResults);
}

//Testing a random generated hex actual formals correctly
void format_as_hex_complex_case_2() {
  UInt256 result;

  result.data[7] = 0x11e4eb69U;
  result.data[6] = 0x3762fd3fU;
  result.data[5] = 0xa50bbadbU;
  result.data[4] = 0xc7ec76bcU;
  result.data[3] = 0x1f4dba8aU;
  result.data[2] = 0xc139f393U;
  result.data[1] = 0x343cbd95U;
  result.data[0] = 0xcb3f7471U;

  char * strResults = uint256_format_as_hex(result);
  ASSERT( 0 == strcmp( strResults, "11e4eb693762fd3fa50bbadbc7ec76bc1f4dba8ac139f393343cbd95cb3f7471" ));
  free(strResults);
}

//Testing a random generated hex actual formals correctly
void format_as_hex_complex_case_3() {
  UInt256 result;

  result.data[7] = 0x84cb5f8U;
  result.data[6] = 0x629be339U;
  result.data[5] = 0x9567e7d8U;
  result.data[4] = 0xc54bc839U;
  result.data[3] = 0xd4f67e72U;
  result.data[2] = 0x733b182cU;
  result.data[1] = 0x37164d5dU;
  result.data[0] = 0x230abbb8U;

  char * strResults = uint256_format_as_hex(result);
  ASSERT( 0 == strcmp( strResults, "84cb5f8629be3399567e7d8c54bc839d4f67e72733b182c37164d5d230abbb8" ));
  free(strResults);
}

//Testing a random generated hex actual formals correctly (with empty 32 u-int buckets)
void format_as_hex_complex_case_4() {
  UInt256 result;

  result.data[0] = 0x906cbfd3U;
  result.data[1] = 0x7e929ca9U;
  result.data[2] = 0x19c8efcaU;
  result.data[3] = 0xb43fd12U;
  result.data[4] = 0x00000000U;
  result.data[5] = 0x00000000U;
  result.data[6] = 0x00000000U;
  result.data[7] = 0x00000000U;

  char * strResults = uint256_format_as_hex(result);
  ASSERT( 0 == strcmp(strResults, "b43fd1219c8efca7e929ca9906cbfd3" ));
  free(strResults);
}

// Gets bits for a random generated hex and checks if it is the same
void get_bits_complex_case_1() {
  UInt256 result;

  result.data[0] = 0x8f288110U;
  result.data[1] = 0x5cd9bbe1U;
  result.data[2] = 0xfeeb9e31U;
  result.data[3] = 0x74ca2b48U;
  result.data[4] = 0x649d11c4U;
  result.data[5] = 0x64f11534U;
  result.data[6] = 0x4123742cU;
  result.data[7] = 0x1035cdfcU;

  ASSERT( 0x8f288110U == uint256_get_bits( result, 0 ) );
  ASSERT( 0x5cd9bbe1U == uint256_get_bits( result, 1 ) );
  ASSERT( 0xfeeb9e31U == uint256_get_bits( result, 2 ) );
  ASSERT( 0x74ca2b48U == uint256_get_bits( result, 3 ) );
  ASSERT( 0x649d11c4U == uint256_get_bits( result, 4 ) );
  ASSERT( 0x64f11534U == uint256_get_bits( result, 5 ) );
  ASSERT( 0x4123742cU == uint256_get_bits( result, 6 ) );
  ASSERT( 0x1035cdfcU == uint256_get_bits( result, 7 ) );
}

// Gets bits for a random generated hex and checks if it is the same
void get_bits_complex_case_2() {
  UInt256 result;

  result.data[7] = 0x11e4eb69U;
  result.data[6] = 0x3762fd3fU;
  result.data[5] = 0xa50bbadbU;
  result.data[4] = 0xc7ec76bcU;
  result.data[3] = 0x1f4dba8aU;
  result.data[2] = 0xc139f393U;
  result.data[1] = 0x343cbd95U;
  result.data[0] = 0xcb3f7471U;

  ASSERT( 0x11e4eb69U == uint256_get_bits( result, 7 ) );
  ASSERT( 0x3762fd3fU == uint256_get_bits( result, 6 ) );
  ASSERT( 0xa50bbadbU == uint256_get_bits( result, 5 ) );
  ASSERT( 0xc7ec76bcU == uint256_get_bits( result, 4 ) );
  ASSERT( 0x1f4dba8aU == uint256_get_bits( result, 3 ) );
  ASSERT( 0xc139f393U == uint256_get_bits( result, 2 ) );
  ASSERT( 0x343cbd95U == uint256_get_bits( result, 1 ) );
  ASSERT( 0xcb3f7471U == uint256_get_bits( result, 0 ) );
}

// Gets bits for a random generated hex and checks if it is the same
void get_bits_complex_case_3() {
  UInt256 result;

  result.data[7] = 0x84cb5f8U;
  result.data[6] = 0x629be339U;
  result.data[5] = 0x9567e7d8U;
  result.data[4] = 0xc54bc839U;
  result.data[3] = 0xd4f67e72U;
  result.data[2] = 0x733b182cU;
  result.data[1] = 0x37164d5dU;
  result.data[0] = 0x230abbb8U;

  ASSERT( 0x230abbb8U == uint256_get_bits( result, 0 ) );
  ASSERT( 0x37164d5dU == uint256_get_bits( result, 1 ) );
  ASSERT( 0x733b182cU == uint256_get_bits( result, 2 ) );
  ASSERT( 0xd4f67e72U == uint256_get_bits( result, 3 ) );
  ASSERT( 0xc54bc839U == uint256_get_bits( result, 4 ) );
  ASSERT( 0x9567e7d8U == uint256_get_bits( result, 5 ) );
  ASSERT( 0x629be339U == uint256_get_bits( result, 6 ) );
  ASSERT( 0x84cb5f8U == uint256_get_bits( result, 7 ) );
}

// Gets bits for a random generated hex and checks if it is the same (with empty buckets)
void get_bits_complex_case_4() {
  UInt256 result;

  result.data[0] = 0x906cbfd3U;
  result.data[1] = 0x7e929ca9U;
  result.data[2] = 0x19c8efcaU;
  result.data[3] = 0xb43fd12U;
  result.data[4] = 0x00000000U;
  result.data[5] = 0x00000000U;
  result.data[6] = 0x00000000U;
  result.data[7] = 0x00000000U;

  ASSERT( 0x906cbfd3U == uint256_get_bits( result, 0 ) );
  ASSERT( 0x7e929ca9U == uint256_get_bits( result, 1 ) );
  ASSERT( 0x19c8efcaU == uint256_get_bits( result, 2 ) );
  ASSERT( 0xb43fd12U == uint256_get_bits( result, 3 ) );
  ASSERT( 0x00000000U == uint256_get_bits( result, 4 ) );
  ASSERT( 0x00000000U == uint256_get_bits( result, 5 ) );
  ASSERT( 0x00000000U == uint256_get_bits( result, 6 ) );
  ASSERT( 0x00000000U == uint256_get_bits( result, 7 ) );
}

// Gets bits for outside the range returns 0
void get_bits_outside_index_range() {
  UInt256 result;

  result.data[0] = 0x906cbfd3U;
  result.data[1] = 0x7e929ca9U;
  result.data[2] = 0x19c8efcaU;
  result.data[3] = 0xb43fd12U;
  result.data[4] = 0x00000000U;
  result.data[5] = 0x00000000U;
  result.data[6] = 0x00000000U;
  result.data[7] = 0x00000000U;

  ASSERT( 0 == uint256_get_bits( result, 8 ) );
}

void test_negate_large_random() {
  UInt256 large_value;
  // A large random value
  large_value.data[0] = 0xFEDCBA98;
  large_value.data[1] = 0x76543210;
  large_value.data[2] = 0xABCDEF12;
  large_value.data[3] = 0x34567890;
  large_value.data[4] = 0x13579BDF;
  large_value.data[5] = 0x2468ACE0;
  large_value.data[6] = 0xDEADBEAF;
  large_value.data[7] = 0xF00DCAFE;

  UInt256 result = uint256_negate(large_value);

  UInt256 expected_result;
  expected_result.data[0] = 0x01234568;
  expected_result.data[1] = 0x89ABCDEF;
  expected_result.data[2] = 0x543210ED;
  expected_result.data[3] = 0xCBA9876F;
  expected_result.data[4] = 0xECA86420;
  expected_result.data[5] = 0xDB97531F;
  expected_result.data[6] = 0x21524150;
  expected_result.data[7] = 0x0FF23501;

  ASSERT_SAME(expected_result, result);
}

void test_negate_boundary_value() {
  UInt256 boundary_value;
  boundary_value.data[0] = 0x80000000;  // Set the highest bit in the first chunk
  for (int i = 1; i < 8; ++i) {
    boundary_value.data[i] = 0x00000000;
  }

  UInt256 result = uint256_negate( boundary_value );

  UInt256 expected;
  expected.data[7] = 0xFFFFFFFF;
  expected.data[6] = 0xFFFFFFFF;
  expected.data[5] = 0xFFFFFFFF;
  expected.data[4] = 0xFFFFFFFF;
  expected.data[3] = 0xFFFFFFFF;
  expected.data[2] = 0xFFFFFFFF;
  expected.data[1] = 0xFFFFFFFF;
  expected.data[0] = 0x80000000;
  
  ASSERT_SAME( expected, result );
}

void test_negate_alternating_bits() {
  UInt256 alternating_value;
  for (int i = 0; i < 8; ++i) {
    alternating_value.data[i] = (i % 2 == 0) ? 0xAAAAAAAA : 0x55555555; // Alternating pattern
  }

  UInt256 result = uint256_negate( alternating_value );

  UInt256 expected;
  expected.data[7] = 0xAAAAAAAA;
  expected.data[6] = 0x55555555;
  expected.data[5] = 0xAAAAAAAA;
  expected.data[4] = 0x55555555;
  expected.data[3] = 0xAAAAAAAA;
  expected.data[2] = 0x55555555;
  expected.data[1] = 0xAAAAAAAA;
  expected.data[0] = 0x55555556;
  
  ASSERT_SAME( expected, result );
}

void test_negate_single_nonzero_chunk() {
  UInt256 single_nonzero_value;
  single_nonzero_value.data[0] = 0x00000000;
  single_nonzero_value.data[1] = 0x00000001;  // Only non-zero chunk
  for (int i = 2; i < 8; ++i) {
    single_nonzero_value.data[i] = 0x00000000;
  }

  UInt256 result = uint256_negate( single_nonzero_value );

  UInt256 expected;
  expected.data[7] = 0xFFFFFFFF;
  expected.data[6] = 0xFFFFFFFF;
  expected.data[5] = 0xFFFFFFFF;
  expected.data[4] = 0xFFFFFFFF;
  expected.data[3] = 0xFFFFFFFF;
  expected.data[2] = 0xFFFFFFFF;
  expected.data[1] = 0xFFFFFFFF;
  expected.data[0] = 0x00000000;
  
  ASSERT_SAME( expected, result );
}

void test_lshift_max_shift( TestObjs *objs ) {
  UInt256 result;

  // Shift by 255 bits, which is the maximum
  result = uint256_lshift( objs->one, 255 );

  UInt256 expected = { { 0, 0, 0, 0, 0, 0, 0, 0x80000000 } };
  ASSERT_SAME( expected, result );
}

void test_lshift_one_chunk( TestObjs *objs ) {
  UInt256 result;

  // Shift 1 left by 32 bits, which should move it to the second chunk
  result = uint256_lshift( objs->one, 32 );
  UInt256 expected = { { 0, 1, 0, 0, 0, 0, 0, 0 } };
  ASSERT_SAME( expected, result );
}

void test_lshift_partial_chunk_shift( TestObjs *objs ) {
  UInt256 result;

  // Shift 1 left by 33 bits
  result = uint256_lshift( objs->one, 33 );

  UInt256 expected = { { 0, 0x00000002, 0, 0, 0, 0, 0, 0 } };

  ASSERT_SAME( expected, result );
}

void test_lshift_two_chunks_shift( TestObjs *objs ) {
  UInt256 result;

  // Shift 1 left by 64 bits, moving into the second chunk
  result = uint256_lshift( objs->one, 64 );
  UInt256 expected = { { 0, 0, 1, 0, 0, 0, 0, 0 } };
  ASSERT_SAME( expected, result );
}

void test_lshift_large_random () {
  UInt256 large_value;
  large_value.data[7] = 0x727767d0U;
  large_value.data[6] = 0x7ccff5feU;
  large_value.data[5] = 0x25cd125bU;
  large_value.data[4] = 0x4523e8c7U;
  large_value.data[3] = 0xdb1b8d1aU;
  large_value.data[2] = 0x2c8a2830U;
  large_value.data[1] = 0x284d72bbU;
  large_value.data[0] = 0x872c33a5U;
  
  UInt256 result = uint256_lshift( large_value, 50U );
  
  UInt256 expected;
  expected.data[7] = 0xd7f89734U;
  expected.data[6] = 0x496d148fU;
  expected.data[5] = 0xa31f6c6eU;
  expected.data[4] = 0x3468b228U;
  expected.data[3] = 0xa0c0a135U;
  expected.data[2] = 0xcaee1cb0U;
  expected.data[1] = 0xce940000U;
  expected.data[0] = 0x00000000U;

  ASSERT_SAME( expected, result );
}

void test_lshift_max_value() {
  UInt256 max_value;
  max_value.data[7] = 0xFFFFFFFFU;
  max_value.data[6] = 0xFFFFFFFFU;
  max_value.data[5] = 0xFFFFFFFFU;
  max_value.data[4] = 0xFFFFFFFFU;
  max_value.data[3] = 0xFFFFFFFFU;
  max_value.data[2] = 0xFFFFFFFFU;
  max_value.data[1] = 0xFFFFFFFFU;
  max_value.data[0] = 0xFFFFFFFFU;
  
  UInt256 result = uint256_lshift( max_value, 1 );

  UInt256 expected;
  expected.data[7] = 0xFFFFFFFFU;
  expected.data[6] = 0xFFFFFFFFU;
  expected.data[5] = 0xFFFFFFFFU;
  expected.data[4] = 0xFFFFFFFFU;
  expected.data[3] = 0xFFFFFFFFU;
  expected.data[2] = 0xFFFFFFFFU;
  expected.data[1] = 0xFFFFFFFFU;
  expected.data[0] = 0xFFFFFFFEU;
  
  ASSERT_SAME( expected, result );
}