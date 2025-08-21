// C implementations of image processing functions

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "imgproc.h"


// Get the RBGA Value of a data point
//
// Parameters:
//   val  - value of data
//   result - pointer/list of RGBA storage
//        red -> index 0
//        green -> index 1
//        blue -> index 2
//        alpha -> index 3
void getRGBA (uint32_t val, uint32_t * result){
  uint32_t original = val;

  // Gets the alpha value
  result[3] = (val << 24) >> 24;

  // Gets the blue value 
  val = original;
  result[2] = (val << 16) >> 24;

  // Gets the green value 
  val = original;
  result[1] = (val << 8) >> 24;

  // Gets the red value 
  val = original;
  result[0] = (val >> 24);
}

// Get the RBGA Int Representation From the RGBA values
//
// Parameters:
//   red  - value of red (0-255)
//   green  - value of blue (0-255)
//   blue  - value of blue (0-255)
//   alpha  - value of alpha (0-255)
// Return:
//   The int representation
uint32_t getIntRepresentationOfRGB (uint32_t results[4]){
  uint32_t result = 0;

  // Sets the red value
  result |= results[0];

  // Sets the green value
  result = result << 8;
  result |= results[1];

  // Sets the blue value
  result = result << 8;
  result |= results[2];

  // Sets the alpha value
  result = result << 8;
  result |= results[3];

  return result;
}



// Gets the kaledioscope as a 2D array
//
// Parameters:
//   totalLength - the correct length to account for odd
//   length - just midpoints * 2 as it is the "fake" length
//   midpoint  - half the length
//   data  -  1D array of the input image
//   result  - 2D array to store the result
void getKal2DRep(int totalLength, int length, int midpoint, uint32_t * data, uint32_t result[length][length]){
  //Gets the slice A
  for (int i = 0; i < midpoint; i ++){
    for (int j = i; j < midpoint; j++){
      result[i][j] = data[i * totalLength + j];
    }

    //Copies it over the diagonal
    for (int j = 0; j < i; j ++){
      result[i][j] = result[j][i];
    }

    //Then copies it over the x-axis
    for (int j = 0; j < midpoint; j++){
      result[i][length - j - 1] = result[i][j];
    }

    //Then reflects it across
    for (int j = 0; j < length; j++){
      result[length - i - 1][j] = result[i][j];
    }
  }
}

// Convert input pixels to grayscale.
// This transformation always succeeds.
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
void imgproc_grayscale( struct Image *input_img, struct Image *output_img ) {
  uint32_t RGBAValue[4];
  uint32_t tempVar[4];
  uint32_t grayValue;
  int totalLength = input_img -> width * input_img -> height;

  // Gets and assign grayscale for all pixels
  for (int i = 0; i < totalLength; i++) {
    // Gets RGBA values and find grayscale of values
    getRGBA(input_img -> data[i], RGBAValue);
    grayValue = (79 * RGBAValue[0] + 128 * RGBAValue[1] + 49 * RGBAValue[2])/256;

    // Assigned the grayscale version of pixels
    tempVar[0] = grayValue;
    tempVar[1] = grayValue;
    tempVar[2] = grayValue;
    tempVar[3] = RGBAValue[3];

    output_img -> data[i] = getIntRepresentationOfRGB (tempVar);
  }
}

// Render an output image containing 4 replicas of the original image,
// refered to as A, B, C, and D in the following diagram:
//
//   +---+---+
//   | A | B |
//   +---+---+
//   | C | D |
//   +---+---+
//
// The width and height of the output image are (respectively) twice
// the width and height of the input image.
//
// A is an exact copy of the original input image. B has only the
// red color component values of the input image, C has only the
// green color component values, and D has only the blue color component
// values.
//
// Each of the copies (A-D) should use the same alpha values as the
// original image.
//
// This transformation always succeeds.
//
// Parameters:
//   input_img - pointer to the input Image
//   output_img - pointer to the output Image (which will have
//                width and height twice the width/height of the
//                input image)
void imgproc_rgb( struct Image *input_img, struct Image *output_img ) {
  uint32_t RGBAValue[4];
  uint32_t tempVar[4];
  int height = input_img -> height;
  int width = input_img -> width;
  int count = 0;
  

  // Goes through each pixel in input
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      getRGBA(input_img -> data[i*width+j], RGBAValue);
      tempVar[3] =  RGBAValue[3];
      // Set each quardrant in output, calculates new index
      output_img -> data[count] = getIntRepresentationOfRGB(RGBAValue);

      tempVar[0] = RGBAValue[0];
      tempVar[1] = 0;
      tempVar[2] = 0;
      output_img -> data[count+width] = getIntRepresentationOfRGB(tempVar);

      tempVar[0] = 0;
      tempVar[1] = RGBAValue[1];
      output_img -> data[count+width*height*2] = getIntRepresentationOfRGB(tempVar);

      tempVar[1] = 0;
      tempVar[2] = RGBAValue[2];
      output_img -> data[count+width*height*2+width] = getIntRepresentationOfRGB(tempVar);
      count++;
    }
    count+=width;
  }
}

// Render a "faded" version of the input image.
//
// See the assignment description for an explanation of how this transformation
// should work.
//
// This transformation always succeeds.
//
// Parameters:
//   input_img - pointer to the input Image
//   output_img - pointer to the output Image
void imgproc_fade( struct Image *input_img, struct Image *output_img ) {
  int64_t floorRow, floorCol;
  uint64_t tr, tc;
  uint32_t RGBAValue[4];
  uint32_t faded[4];
  int height = input_img -> height;
  int width = input_img -> width;
  int count = 0;
 
  // Goes through each pixel in input
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      getRGBA(input_img -> data[count], RGBAValue);
      // Calculate gradient for both coordinate
        floorRow = ((int)(2000 * ((float)i/height))) - 1000;
        floorCol = ((int)(2000 * ((float)j/width))) - 1000;
        tr = 1000000 - (floorRow * floorRow);
        tc = 1000000 - (floorCol * floorCol);
        
        // Calculate new rgb values
        faded[0] = (tr * tc * RGBAValue[0]) / 1000000000000L;
        faded[1] = (tr * tc * RGBAValue[1]) / 1000000000000L;
        faded[2] = (tr * tc * RGBAValue[2]) / 1000000000000L;
        faded[3] = RGBAValue[3];
      output_img -> data[count++] = getIntRepresentationOfRGB(faded);
    }
  }
}

// Render a "kaleidoscope" transformation of input_img in output_img.
// The input_img must be square, i.e., the width and height must be
// the same. Assume that the input image is divided into 8 "wedges"
// like this:
//
//    +----+----+
//    |\ A |   /|
//    | \  |  / |
//    |  \ | /  |
//    | B \|/   |
//    +----+----+
//    |   /|\   |
//    |  / | \  |
//    | /  |  \ |
//    |/   |   \|
//    +----+----+
//
// The wedge labeled "A" is the one that will be replicated 8 times
// in the output image. In the output image,
//
//    Wedge A will have exactly the same pixels as wedge A in
//    the input image, in the same positions.
//
//    Wedge B will have exactly the same pixels as wedge A, but the
//    column and row are transposed. (I.e., it is a reflection across
//    the diagonal.)
//
//    All other wedges are mirrored versions of A such that the overall
//    image is symmetrical across all four lines dividing the image
//    (vertical split, horizontal split, and the two diagonal splits.)
//
// Parameters:
//   input_img  - pointer to the input Image
//   output_img - pointer to the output Image (in which the transformed
//                pixels should be stored)
//
// Returns:
//   1 if successful, 0 if the transformation fails because the
//   width and height of input_img are not the same.
int imgproc_kaleidoscope( struct Image *input_img, struct Image *output_img ) {
  //Error Handling
  if (input_img -> width != input_img -> height) {
    return 0;
  }

  //Changes depending if it is odd or even
  int midpoint = input_img -> width/2;
  if (input_img -> width % 2 == 1){
    midpoint =  (input_img -> width + 1)/2;
  }
  
  //Get it in terms of a two-d array
  uint32_t tempResult[midpoint * 2][midpoint * 2];
  getKal2DRep(input_img -> width, midpoint * 2, midpoint, input_img -> data, tempResult);

  //Transcibes it over to the normal data
  int count = 0;
  for (int i = 0; i < output_img -> height; i++){
    for (int j = 0; j < output_img -> width; j++){
      output_img -> data[count++] = tempResult[i][j];
    }
  }

  return 1;
}