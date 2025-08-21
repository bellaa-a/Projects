/*
 * x86-64 assembly language implementations of functions
 */

	.section .text

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/*
 * TODO: define your helper functions here.
 * Don't forget to use the .globl directive to make
 * them visible to the test code.
 */

/*
 * Implementations of API functions
 */


/*
* Get the RBGA Value of a data point
*
* Parameters:
*   %edi - value of data
*   %rax - pointer/list of RGBA storage
*/
.globl getRGBA
getRGBA:
	/* Gets red value */
	movl %edi, %ebx /* Sets a temp value eax to the original value */
	shr $24, %ebx /* Rshifts this by 24 bits */
	movl %ebx, (%rax) /* Assign it into the array */

	/* Gets green value */
	movl %edi, %ebx /* Sets a temp value eax to the original value */
	shl $8, %ebx /* Lshifts by 8 */
	shr $24, %ebx /* Rshifts this by 24 bits */
	movl %ebx, 4(%rax) /* Assign it into the array */

	/* Gets blue value */
	movl %edi, %ebx /* Sets a temp value eax to the original value */
	shl $16, %ebx /* Lshifts by 8 */
	shr $24, %ebx /* Rshifts this by 24 bits */
	movl %ebx, 8(%rax) /* Assign it into the array */

	/* Gets alpha value */
	movl %edi, %ebx /* Sets a temp value eax to the original value */
	shl $24, %ebx /* Lshifts by 8 */
	shr $24, %ebx /* Rshifts this by 24 bits */
	movl %ebx, 12(%rax) /* Assign it into the array */

	ret

/*
* Get the RBGA Int Representation From the RGBA values
*
* Parameters:
*   (%rdi)    - value of red (0-255)
*   %4(%rdi)  - value of blue (0-255)
*   %8(%rdi)  - value of blue (0-255)
*   %12(%rdi) - value of alpha (0-255)
* Return:
*   The int representation (%eax)
*/
.globl getIntRepresentationOfRGB
getIntRepresentationOfRGB:
	movl (%rdi), %eax /* Add the value red into the parameter */
	shl $8, %eax /* Left shift 8 bits*/

	orl 4(%rdi), %eax /* Add the value blue into the parameter */
	shl $8, %eax /* Left shift 8 bits*/

	orl 8(%rdi), %eax /* Add the value green into the psarameter */
	shl $8, %eax /* Left shift 8 bits */

	orl 12(%rdi), %eax /* Add the value alpha into the parameter */
	ret 

.globl getKal2DRep
getKal2DRep:
	/* TODO: implement */
	ret
/*
 * Render an output image containing 4 copies of the original image,
 * refered to as A, B, C, and D in the following diagram:
 *
 *   +---+---+
 *   | A | B |
 *   +---+---+
 *   | C | D |
 *   +---+---+
 *
 * The width and height of the output image are (respectively) twice
 * the width and height of the input image.
 *
 * A is an exact copy of the original input image. B has only the
 * red color component values of the input image, C has only the
 * green color component values, and D has only the blue color component
 * values.
 *
 * Each of the copies (A-D) should use the same alpha values as the
 * original image.
 *
 * This transformation always succeeds.
 *
 * Parameters:
 *   %rdi - pointer to the input Image
 *   %rsi - pointer to the output Image (which will have
 *          width and height twice the width/height of the
 *          input image)
 */
	.globl imgproc_rgb
imgproc_rgb:
	subq $48, %rsp /* Make enough room in the stack for another temp array */
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r12d /* Get image height from struct */
	movq %r12, 16(%rsp) /* Move height into rsp */
    movl IMAGE_WIDTH_OFFSET(%rdi), %r12d /* Get image width from struct */
	movq %r12, 24(%rsp) /* Move width into later part of rsp */
	movq IMAGE_DATA_OFFSET(%rdi), %r14 /* Use %r14 as pointer to input data */
	movq IMAGE_DATA_OFFSET(%rsi), %r15 /* Use %r15 as pointer to output data */
    movq $0, %r10 /* Index for new image (count) */
    movq $0, %r8 /* Outter loop variable */
	mov %rdi, %r12 /* Retain the memory of %rdi */

	movq $0, %rdx /* Sets rdx to 0 to be the count for the loop */
    
.LoutterLoop:
    cmp 16(%rsp), %r8 /* Check if loop variable is less than height */
    jge .LendOutterLoop /* Jump to terminate both loops if greater than */
    movq $0, %r9 /* Inner loop variable */

.LinnerLoop:
    movq %r8, %rdx /* store i (outter loop variable) into temp variable */
    imulq 24(%rsp), %rdx /* Multiply i by width (%r13d) */
    addq %r9, %rdx /* i*width + j (inner loop variable) */

	leaq (%r14, %rdx, 4), %rbx /* Get the memory first parameter of 64 bits for get RGBA */

	movl (%rbx), %edi /* Set the value as the first parameter into 32 bits for get RGBA */
	movq %rsp, %rax /* Set the value of the second parameter for get RGBA */

	call getRGBA /* Get RGBA value of pixel */
	mov %r12, %rdi /* Restore the memory of %rdi */
	
	/* Original colors */
	movq (%rsp), %r11 /* Temp var */
	movl %r11d, 32(%rsp) /* Set the first parameter for getIntRepresentationOfRGB */
	movq 4(%rsp), %r11 /* Temp var */
	movl %r11d, 36(%rsp) /* Set the second parameter for getIntRepresentationOfRGB */
	movq 8(%rsp), %r11 /* Temp var */
	movl %r11d, 40(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */
	movq 12(%rsp), %r11 /* Temp var */
    movl %r11d, 44(%rsp) /* Set the fourth parameter for getIntRepresentationOfRGB */
	lea 32(%rsp), %rdi /* Moves the array into the parameter */
	call getIntRepresentationOfRGB
	movq %r10, %rdx /* set rdx to be count, rdx is a temp variable to calculate new indicies */
	movl %eax, (%r15, %rdx, 4) /* Set output of getIntRepresentationOfRGB to output image data */

	/* Red version */
	movq (%rsp), %r11 /* Temp var */
	movl %r11d, (%rdi) /* Set the first parameter for getIntRepresentationOfRGB */
	movl $0, 36(%rsp) /* Set the second parameter for getIntRepresentationOfRGB */
	movl $0, 40(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */
	movq 12(%rsp), %r11 /* Temp var */
    movl %r11d, 44(%rsp) /* Set the fourth parameter for getIntRepresentationOfRGB */
	lea 32(%rsp), %rdi /* Moves the array into the parameter */
	call getIntRepresentationOfRGB
	addq 24(%rsp), %rdx /* count + width */
	movl %eax, (%r15, %rdx, 4) /* Set output of getIntRepresentationOfRGB to output image data */

	/* Green version */
	movl $0, 32(%rsp) /* Set the first parameter for getIntRepresentationOfRGB */
	movq 4(%rsp), %r11 /* Temp var */
	movl %r11d, 36(%rsp) /* Set the second parameter for getIntRepresentationOfRGB */
	movl $0, 40(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */
	movq 12(%rsp), %r11 /* Temp var */
    movl %r11d, 44(%rsp) /* Set the fourth parameter for getIntRepresentationOfRGB */
	lea 32(%rsp), %rdi /*Moves the array into the parameter*/
	call getIntRepresentationOfRGB
	movq 16(%rsp), %rdx /* set rdx to be height */
	imulq 24(%rsp), %rdx /* height * width */
	shl $1, %rdx /* width * height * 2 */
	addq %r10, %rdx /* count + width * height * 2 */
	movl %eax, (%r15, %rdx, 4) /* Set output of getIntRepresentationOfRGB to output image data */

	/* Blue version */
	movl $0, 32(%rsp) /* Set the first parameter for getIntRepresentationOfRGB */
	movl $0, 36(%rsp) /* Set the second parameter for getIntRepresentationOfRGB */
	movq 8(%rsp), %r11 /* Temp var */
	movl %r11d, 40(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */
	movq 12(%rsp), %r11 /* Temp var */
    movl %r11d, 44(%rsp) /* Set the fourth parameter for getIntRepresentationOfRGB */
	lea 32(%rsp), %rdi /* Moves the array into the parameter */
	call getIntRepresentationOfRGB
	addq 24(%rsp), %rdx /* count + width * height * 2 + width */
	movl %eax, (%r15, %rdx, 4) /* Set output of getIntRepresentationOfRGB to output image data */

	incq %r9 /* Increment inner loop variable */ 
    incq %r10 /* Increment index for new image */
    cmp 24(%rsp), %r9 /* Check if loop variable is less than width */
    jge .LendInnerLoop /* End current inner loop iteration */
    
    jmp .LinnerLoop /* Jump to go to next iteration of inner loop */

.LendInnerLoop:
    
    incq %r8 /* Increment outter loop variable */ 
    addq 24(%rsp), %r10 /* Increase index for new image by width */
    jmp .LoutterLoop /* Jump to go to next iteration of outter loop */

.LendOutterLoop:
	addq $48, %rsp /* Restore rsp */
    ret

/*
 * void imgproc_grayscale( struct Image *input_img, struct Image *output_img );
 *
 * Transform image by converting each pixel to grayscale.
 * This transformation always succeeds.
 *
 * Parameters:
 *   %rdi - pointer to original struct Image
 *   %rsi - pointer to output struct Image
 */
	.globl imgproc_grayscale
imgproc_grayscale:
	movl IMAGE_WIDTH_OFFSET(%rdi), %r12d /* Sets r12 to be the width */
	imull IMAGE_HEIGHT_OFFSET(%rdi), %r12d /* Sets r12 to be the width * height to get the total length */
	movq IMAGE_DATA_OFFSET(%rdi), %r13 /* use %r13 as pointer to input data */
	movq IMAGE_DATA_OFFSET(%rsi), %r14 /* use %r14 as pointer to output data */

	subq $16, %rsp /* Make enough room in the stack for another temp array */
	movq $0, %rdx /* Set rdx as 0 */
	
.LinputLoop: /* Starts the for loop to iterate over each pixel */
	cmpq %r12, %rdx /* Checks if we have ran through all pixels */
	jge .LinputLoopDone /* Jumps to the end if we have traversed all pixels */
	
	leaq (%r13, %rdx, 4), %rbx /* Get the memory first parameter of 64 bits for get RGBA */
	movl (%rbx), %edi /* Set the value as the first parameter into 32 bits for get RGBA */
	movq %rsp, %rax /* Set the value of the second parameter for get RGBA */

	call getRGBA /* Get RGBA values of pixel*/
	movq %r8, %rdi /* Restore the memory of %rdi */

	movq $0, %r10 /* Grayscale value is 0/reset */

	movq (%rsp), %r15 /* Add red to the temp value */
	imulq $79, %r15 /* Multiply red by 79 */
	addq %r15, %r10 /* Add it to the grascale value */

	movq 4(%rsp), %r15 /* Add blue to the temp value */
	imulq $128, %r15 /* Multiply blue by 128 */
	addq %r15, %r10 /* Add it to the grascale value */

	movq 8(%rsp), %r15 /* Add green to the temp value */
	imulq $49, %r15 /* Multiply green by 49 */
	addq %r15, %r10 /* Add it to the grascale value */
	addq $4, %rbx /* Move pointer over by 1 index */

	movq %r10, %r15 /* Resuse r15 as a parameter for division */
	shr $8, %r15 /* Divide r15 by 256 */

	
	movl %r15d, (%rsp) /* Set the first parameter for getIntRepresentationOfRGB */
	movl %r15d, 4(%rsp) /* Set the second parameter for getIntRepresentationOfRGB */
	movl %r15d, 8(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */
	movq %rsp, %rdi /* Moves the array into the parameter */
	call getIntRepresentationOfRGB

	movl %eax, (%r14, %rdx, 4) /* Set output of getIntRepresentationOfRGB to output image data */
	
	incq %rdx /* Increments rdx for the for loop */

	jmp .LinputLoop /* Start loop again */

.LinputLoopDone:
	addq $16, %rsp /* Restore rsp */
	ret

/*
 * Render a "faded" version of the input image.
 *
 * See the assignment description for an explanation of how this transformation
 * should work.
 *
 * This transformation always succeeds.
 *
 * Parameters:
 *   %rdi - pointer to the input Image
 *   %rsi - pointer to the output Image
 *
 * Register use:
 *   %r8  - outer loop index (row index i)
 *   %r9  - inner loop index (column index j)
 *   %r10 - temporary pixel index storage
 *   %r11 - temporary register for calculations
 *   %r12 - backup of %rdi (input image struct pointer)
 *   %r13 - temporary storage for height/width calculations
 *   %r14 - pointer to input image data
 *   %r15 - pointer to output image data
 *   %rbx - pointer to current pixel data
 *
 * Stack memory:
 *   -(%rsp)   - original red component
 *   -4(%rsp)  - original green component
 *   -8(%rsp)  - original blue component
 *   -12(%rsp) - alpha component
 *   -16(%rsp) - image height
 *   -24(%rsp) - image width
 *   -32(%rsp) - faded red component
 *   -36(%rsp) - faded green component
 *   -40(%rsp) - faded blue component
 *   -44(%rsp) - alpha component
 */
	.globl imgproc_fade
imgproc_fade:
	subq $48, %rsp /* Make enough room in the stack for temp array */
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r12d /* Get image height from struct */
	movl %r12d, 16(%rsp) /* Move height into rsp */
    movl IMAGE_WIDTH_OFFSET(%rdi), %r12d /* Get image width from struct */
	movl %r12d, 24(%rsp) /* Move width into later part of rsp */
	movq IMAGE_DATA_OFFSET(%rdi), %r14 /* Use %r14 as pointer to input data */
	movq IMAGE_DATA_OFFSET(%rsi), %r15 /* Use %r15 as pointer to output data */
    movq $0, %r10 /* Index for new image (count) */
    movq $0, %r8 /* Outter loop variable */
	mov %rdi, %r12 /* Retain the memory of %rdi */
	movq $0, %rdx /* Sets rdx to 0 to be the count for the loop */

.LoutterForLoop:
    cmpl 16(%rsp), %r8d /* Check if loop variable is less than height */
    jge .LendOutterForLoop /* Jump to terminate both loops if greater than */
    movq $0, %r9 /* Inner loop variable */

.LinnerForLoop:
	movq %r8, %r10 /* store i (outter loop variable) into temp variable */
    imull 24(%rsp), %r10d /* Multiply i by width (%r13d) */
    addq %r9, %r10 /* i*width + j (inner loop variable) */
	leaq (%r14, %r10, 4), %rbx /* Get the memory first parameter of 64 bits for get RGBA */

	movl (%rbx), %edi /* Set the value as the first parameter to input_img -> data[count] */
	movq %rsp, %rax /* Set the value of the second parameter for get RGBA */

	call getRGBA /* Get RGBA value of pixel */
	mov %r12, %rdi /* Restore the memory of %rdi */

	/* Calculate row coords */
	movq $0, %rdx /* Cleanup for dividing */
	movq $0, %rax /* Cleanup for dividing */
	movq %r8, %r11 /* Index of pixel row (x) */
	imulq $2000, %r11 /* 2000 * x */
	movq %r11, %rax /* Move into rax, which is numerator */
	movq $0, %r13
	movl 16(%rsp), %r13d /* Set height to r13 */
	idiv %r13 /* rax / height, rax contains quotient */
	subq $1000, %rax /* floorRow */
	imulq %rax, %rax /* floorRow * floorRow */
	movq $1000000, %r13
	subq %rax, %r13 /* tr */

	/* Calculate col coords */
	movq $0, %rdx /* Cleanup for dividing */
	movq $0, %rax /* Cleanup for dividing */
	movq %r9, %r11 /* Index of pixel col (x) */
	imulq $2000, %r11 /* 20000 * x */
	movq %r11, %rax /* Move into rax, which is numerator */
	movq $0, %r10
	movl 24(%rsp), %r10d /* Set width to r10 */
	idiv %r10 /* rax / width, rax contains quotient */
	subq $1000, %rax /* floorCol */
	imulq %rax, %rax /* floorCol * floorCol */
	movq $1000000, %r10
	subq %rax, %r10 /* tc */

	imulq %r10, %r13 /* r13 = tr * tc */
	movq $0, %r10

	/* Faded red */
	movq $0, %rax /* Cleanup for dividing */
	movq %r13, %r11 /* Store tr * tc */
	movl (%rsp), %r10d /* Temp var */
	imulq %r10, %r11 /* tr * tc * RGBAValue[0] */
	movq %r11, %rax /* Move into rax, which is numerator */
	movq $0, %rdx /* Cleanup for dividing */
	movq $1000000000000, %r10
	idivq %r10 /* tr * tc * RGBAValue[0] / 1000000000000 */
	movl %eax, 32(%rsp) /* Set the first parameter for getIntRepresentationOfRGB */

	/* Faded green */
	movq $0, %rax /* Cleanup for dividing */
	movq %r13, %r11 /* Store tr * tc */
	movl 4(%rsp), %r10d /* Temp var */
	imulq %r10, %r11 /* tr * tc * RGBAValue[0] */
	movq %r11, %rax /* Move into rax, which is numerator */
	movq $0, %rdx /* Cleanup for dividing */
	movq $1000000000000, %r10
	idivq %r10 /* tr * tc * RGBAValue[1] / 1000000000000 */
	movl %eax, 36(%rsp) /* Set the seond parameter for getIntRepresentationOfRGB */

	/* Faded blue */
	movq $0, %rax /* Cleanup for dividing */
	movq %r13, %r11 /* r11 = r12 */
	movl 8(%rsp), %r10d /* Temp var */
	imulq %r10, %r11 /* tr * tc * RGBAValue[0] */
	movq %r11, %rax /* Move into rax, which is numerator */
	movq $0, %rdx /* Cleanup for dividing */
	movq $1000000000000, %r10
	idivq %r10 /* tr * tc * RGBAValue[2] / 1000000000000 */
	movl %eax, 40(%rsp) /* Set the third parameter for getIntRepresentationOfRGB */

	/* Alpha */
	movl 12(%rsp), %r11d /* Temp var */
    movl %r11d, 44(%rsp) /* Set the fourth parameter for getIntRepresentationOfRGB */

	lea 32(%rsp), %rdi /* Moves the array into the parameter */
	call getIntRepresentationOfRGB
	movq %r8, %r10 /* store i (outter loop variable) into temp variable */
    imull 24(%rsp), %r10d /* Multiply i by width (%r13d) */
    addq %r9, %r10 /* i*width + j (inner loop variable) */
	movl %eax, (%r15, %r10, 4) /* Set output of getIntRepresentationOfRGB to output image data */

	incq %r9 /* Increment inner loop variable */ 
    cmpl 24(%rsp), %r9d /* Check if loop variable is less than width */
    jge .LendInnerForLoop /* End current inner loop iteration */
    jmp .LinnerForLoop /* Jump to go to next iteration of inner loop */

.LendInnerForLoop:
    incq %r8 /* Increment outter loop variable */ 
    jmp .LoutterForLoop /* Jump to go to next iteration of outter loop */

.LendOutterForLoop:
	addq $48, %rsp /* Restore rsp */
    ret

/*
 * Render a "kaleidoscope" transformation of input_img in output_img.
 * The input_img must be square, i.e., the width and height must be
 * the same. Assume that the input image is divided into 8 "wedges"
 * like this:
 *
 *    +----+----+
 *    |\ A |   /|
 *    | \  |  / |
 *    |  \ | /  |
 *    | B \|/   |
 *    +----+----+
 *    |   /|\   |
 *    |  / | \  |
 *    | /  |  \ |
 *    |/   |   \|
 *    +----+----+
 *
 * The wedge labeled "A" is the one that will be replicated 8 times
 * in the output image. In the output image,
 *
 *    Wedge A will have exactly the same pixels as wedge A in
 *    the input image, in the same positions.
 *
 *    Wedge B will have exactly the same pixels as wedge A, but the
 *    column and row are transposed. (I.e., it is a reflection across
 *    the diagonal.)
 *
 *    All other wedges are mirrored versions of A such that the overall
 *    image is symmetrical across all four lines dividing the image
 *    (vertical split, horizontal split, and the two diagonal splits.)
 *
 * Parameters:
 *   input_img  - pointer to the input Image
 *   output_img - pointer to the output Image (in which the transformed
 *                pixels should be stored)
 *
 * Register use:
 *   %r8  - temporary storage for calculations
 *   %r9  - temporary storage for calculations
 *   %r10 - temporary pixel index storage
 *   %r11 - temporary register for calculations
 *   %r12 - pointer to input image data
 *   %r13 - pointer to output image data
 *   %r14 - temporary storage for height/width calculations
 *   %r15 - temporary register for indexing calculations
 *   %rbx - outer loop counter (row index i)
 *   %rbp - inner loop counter (column index j)
 *
 * Stack memory:
 *   -4(%rsp)   - image height
 *   -8(%rsp)   - image width
 *   -12(%rsp)  - computed midpoint (may be adjusted if odd)
 *   -16(%rsp)  - true midpoint (before adjustment)
 *   -20(%rsp)  - 2 * midpoint (used for mirroring calculations)
 *
 * Returns:
 *   1 if successful, 0 if the transformation fails because the
 *   width and height of input_img are not the same.
 */
	.globl imgproc_kaleidoscope
imgproc_kaleidoscope:
    push %rbp /* Retain rbp */
	subq $32, %rsp /* Make enough room in the stack for another temp array */
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r14d /* Get image height from struct */
	movl %r14d, (%rsp) /* Move height into rsp */
    movl IMAGE_WIDTH_OFFSET(%rdi), %r14d /* Get image width from struct */
	movl %r14d, 4(%rsp) /* Move width into later part of rsp */
	cmpl (%rsp), %r14d /* Makes sure that width and height are equal */
	je .EqualWidthAndHeight /* Makes sure that width and height are equal and jumps if not */

	movq $0, %rax /* Sets return value to 0 as it fails*/
	jmp .End /* Jumps to the end ignoring Equal */

.EqualWidthAndHeight:
	movq IMAGE_DATA_OFFSET(%rdi), %r12 /* Use %r12 as pointer to input data */
	movq IMAGE_DATA_OFFSET(%rsi), %r13 /* Use %r13 as pointer to output data */

	movl (%rsp), %eax /* Set rax to the height */
	movq $2, %r14 /* Move 2 to divide as $2 doesn't work with idiv*/
	movq $0, %rdx /* Intializes %rdx*/
	idiv %r14 /* Divide by 2 and set %rdx to the %rax mod 2 */

	movl %eax, 8(%rsp) /* Move the new midpoint to 8(%rsp) */
	movl %eax, 16(%rsp) /* Move the true midpoint to 16(%rsp) */

	movq $0, %r14 /* Move 0 to divide to compare two 64 registers*/
	cmp %rdx, %r14 /* Checks if it is even */
	je .WidthIsEven /* Jump if it is*/
	addl $1, 8(%rsp) /* Add one to midpoint if it is odd */
	
.WidthIsEven:
	movq 8(%rsp), %r14 /* Move midpoint into temp variable */
	shl $1, %r14 /* Multiple by 2 to get the new length */
	movl %r14d, 12(%rsp) /* Move into %r14 into 12(%rsp) */
	movq $0, %r14
	movq $0, %rbx /* This will be our counter as i */
	movq $0, %rbp /* This will be our counter as j */
	
.LoutterLoopKaleidoscope:
    cmpl 8(%rsp), %ebx /* Check if loop variable is less than midpoint */
    jge .LendOutterLoopKaleidoscope /* Jump to terminate both loops if i is greater than midpoint*/

    movq %rbx, %rbp /* Sets j to equal i*/

.LinnerLoopASlice:
	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d/* Set %r15 to i * length */
	addq %rbp, %r15 /* Set %r15 to i * length + j */
	movl (%r12, %r15, 4), %r14d /* Get the value from the input pixel */

	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d /* Set %r15 to i * (length) */
	addq %rbp, %r15 /* Set %r15 to i * (length) + j */
	movl %r14d, (%r13, %r15, 4) /* Set the output value */

	incq %rbp /* Increment inner loop variable (j) */ 
    cmpl 8(%rsp), %ebp /* Check if loop variable is less than midpoint */
    jge .LendInnerLoopASlice /* End current inner loop iteration */
    jmp .LinnerLoopASlice /* Jump to go to next iteration of inner loop */

.LendInnerLoopASlice:
	movq $0, %rbp /* Sets j to equal to 0*/

.LinnerLoopDiagonal:
	movq %rbp, %r15 /* Set %r15 to j */
	imull (%rsp), %r15d /* Set %r15 to j * (length) */
	addq %rbx, %r15 /* Set %r15 to j * (length) + i */
	movl (%r13, %r15, 4), %r14d /* Get the value from the output pixel */

	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d /* Set %r15 to i * (length) */
	addq %rbp, %r15 /* Set %r15 to i * (length) + j */
	movl %r14d, (%r13, %r15, 4) /* Set the output value */

	incq %rbp /* Increment inner loop variable (j) */ 
    cmpl %ebx, %ebp /* Check if j is less than i */
    jge .LendInnerLoopDiagonal /* End current inner loop iteration */
    jmp .LinnerLoopDiagonal /* Jump to go to next iteration of inner loop */

.LendInnerLoopDiagonal:
	movq $0, %rbp /* Sets j to equal to 0 */

.LinnerLoopAcrossY:
	movq $0, %r15
	movl 12(%rsp), %r15d /* Moves the (2 * midpoint) into %r15 */
	subq $1, %r15 /* Subtract it by 1 */
	subq %rbp, %r15 /* Subtract it by j */
	cmp (%rsp), %r15  /* Check to see if (2 * midpoint) - 1 - j == length */
	je .OutOfBoundsY /* Jumps if compare is true */

	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d /* Set %r15 to i * (length) */
	addq %rbp, %r15 /* Set %r15 to i * (length) + j */
	movl (%r13, %r15, 4), %r14d /* Get the value from the output pixel */

	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d /* Set %r15 to i * (length) */
	addl 12(%rsp), %r15d /* Set %r15 to i * (length) + (2 * midpoint) */
	subq %rbp, %r15 /* Set %r15 to i * (length) + (2 * midpoint) - j */
	subq $1, %r15 /* Set %r15 to i * (length) + (2 * midpoint) - j - 1 */
	movl %r14d, (%r13, %r15, 4) /* Set the output value */

.OutOfBoundsY:
	incq %rbp /* Increment inner loop variable (j) */ 
    cmpl 12(%rsp), %ebp /* Check if loop variable is less than fake midpoint */
    jge .LendInnerLoopAcrossY /* End current inner loop iteration */
    jmp .LinnerLoopAcrossY /* Jump to go to next iteration of inner loop */

.LendInnerLoopAcrossY:
	movq $0, %rbp /* Sets j to equal to 0 */

.LinnerLoopAcrossX:
	movq $0, %r15
	movl 12(%rsp), %r15d /* Moves the (2 * midpoint) into %r15 */
	subq $1, %r15 /* Subtract it by 1 */
	subq %rbx, %r15 /* Subtract it by i */
	cmpl (%rsp), %r15d  /* Check to see if (2 * midpoint) - 1 - i == length */
	je .LendInnerLoopAcrossX /* Jumps if compare is true */

	movq %rbx, %r15 /* Set %r15 to i */
	imull (%rsp), %r15d /* Set %r15 to i * (length) */
	addq %rbp, %r15 /* Set %r15 to i * (length) + j */
	movl (%r13, %r15, 4), %r14d /* Get the value from the output pixel */

	movl 12(%rsp), %r15d /* Set %r15 to (2 * midpoint) */
	subq $1, %r15 /* Set %r15 to (2 * midpoint) - 1 */
	subq %rbx, %r15 /* Set %r15 to (2 * midpoint) - i - 1 */
	imull (%rsp), %r15d /* Set %r15 to ((2 * midpoint) - i - 1) * (length) */
	addq %rbp, %r15 /* Set %r15 to ((2 * midpoint) - i - 1) * (length) + j */
	movl %r14d, (%r13, %r15, 4) /* Set the output value */

	incq %rbp /* Increment inner loop variable (j) */ 
    cmpl 12(%rsp), %ebp /* Check if loop variable is less than the (2 * midpoint) */
    jge .LendInnerLoopAcrossX /* End current inner loop iteration */
    jmp .LinnerLoopAcrossX /* Jump to go to next iteration of inner loop */

.LendInnerLoopAcrossX:
    incq %rbx /* Increment outter loop variable */ 
    jmp .LoutterLoopKaleidoscope /* Jump to go to next iteration of outter loop */

.LendOutterLoopKaleidoscope:
	movq $1, %rax /* Sets return value to 1 as it succeed*/
	
.End:
	addq $32, %rsp /* Restore rsp */
    pop %rbp /* Restore rbp */
	ret

	/* This avoids linker warning about executable stack */
.section .note.GNU-stack,"",@progbits

/*
vim:ft=gas:
*/
