#include <stdio.h>
#include <stdlib.h>


//Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray **readPGM(const char *filename, int *width, int *height)
{
    FILE *lenna = fopen(filename, "r");

    if (!lenna)
    {
        perror("Unable to read file provided");
        exit(-1);
    }

    char buffer[16];
    fgets(buffer, sizeof(buffer), lenna);


    if (buffer[0] != 'P' || buffer[1] != '5')
    {
        fprintf(stderr, "Invalid image format\n");
        fprintf(stderr, "Image must be a PGM and does not match expected value.\n");
        exit(-1);
    }

    fscanf(lenna, "%d %d", width, height);

    int maxPixelValue;

    fscanf(lenna, "%d", &maxPixelValue); // reads the pixel value
    int lineChar = fgetc(lenna);

    if(lineChar != '\n'){

        perror("Expected new line\n");
        fclose(lenna);
        exit(-1);
    }

    PixelGray **original_matrix = malloc(*height * sizeof(PixelGray *)); // allocates memory for height
    
    for (int i = 0; i < *height; ++i) // uses allocated memory for height it allocates memory for width
    {
        original_matrix[i] = malloc(*width * sizeof(PixelGray));
        fread(original_matrix[i], sizeof(PixelGray), *width, lenna);
    }

    fclose(lenna);
    return original_matrix;
}


// Function to write a 2D matrix as a PGM image
void writePGM(const char *filename, PixelGray **original_matrix, int *width, int *height)
{
    FILE *lenna = fopen(filename, "w");

    if (!lenna)
    {
        perror("Unable to open image for writing");
        exit(-1);
    }
    
    fprintf(lenna, "P5\n");
    fprintf(lenna, "%d %d\n", *width, *height);
    fprintf(lenna, "255\n");

    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            fputc(original_matrix[i][j].value, lenna);
        }
    }

    fclose(lenna);
}

// Function to threshold the image matrix
PixelGray **threshold(PixelGray **original_matrix, int *width, int *height)
{
    PixelGray **new_matrix = malloc(*height * sizeof(PixelGray *));
    for (int i = 0; i < *height; ++i)
    {
        new_matrix[i] = malloc(*width * sizeof(PixelGray));

        for (int j = 0; j < *width; ++j)
        {
            //Allows for knowing which values to change within the new matrix
            new_matrix[i][j].value = original_matrix[i][j].value > 80 ? 255 : 0;
        }
    }

    return new_matrix;
}


// Function to rotate the image matrix
PixelGray **rotate(PixelGray **original_matrix, int *width, int *height)
{
    static PixelGray **starting_matrix = NULL;
    int temp = 0;
    static int original_width = 0;
    static int original_height = 0;
    static int has_rotated = 0; //Flag is set up to allow for knowing if rotation was already called.
    PixelGray **new_matrix;

    //Based on how the code within the main function is setup the first time it is called it will rotate it
    if (has_rotated == 0)
    {
        // Store the original matrix along with height and width.
        // Needed so original values are stored and not forgotten when image gets rotated a second time. 
        starting_matrix = original_matrix;
        original_width = *width;
        original_height = *height;

        // 90 degree rotation
        new_matrix = malloc(*height * sizeof(PixelGray *));

        for (int i = 0; i < *height; ++i)
        {
            new_matrix[i] = malloc(*width * sizeof(PixelGray));

            for (int j = 0; j < *width; ++j)
            {
                new_matrix[i][j].value = original_matrix[j][i].value;
            }
        }

        temp = *width;
        *width = *height;
        *height = temp;
        has_rotated = 1; //Updates the flag so that when rotate is called it knows to use the original matrix and not the new matrix
    }
    else
    {
        // Flips the original matrix
        new_matrix = malloc(original_height * sizeof(PixelGray *));

        for (int i = 0; i < original_height; ++i)
        {
            new_matrix[i] = malloc(original_width * sizeof(PixelGray));

            for (int j = 0; j < original_width; ++j)
            {
                new_matrix[i][j].value = starting_matrix[i][j].value;
            }
        }
        
        *width = original_width;
        *height = original_height;
        has_rotated = 0;
    }

    return new_matrix;
}

//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}
