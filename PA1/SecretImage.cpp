#include "SecretImage.h"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    width = image.get_width();
    height = image.get_height();

    upper_triangular = new int[width*(width+1)/2];

    int x=0,counter=0;
    while(x<width) {
        for(int i = x;i<width;i++) {
            upper_triangular[counter] = image.get_pixel(x,i);
            counter++;
        }
        x++;
    }

    lower_triangular = new int[width*(width-1)/2];

    x=1,counter = 0;
    while(x<width) {
        for(int i = 0;i<x;i++) {
            lower_triangular[counter] = image.get_pixel(x,i);
            counter++;
        }
        x++;
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    upper_triangular = upper;
    lower_triangular = lower;
    width = w;
    height = h;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;

}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int x=0,counter=0;
    while(x<width) {
        for(int i = x; i<width;i++) {
            image.get_data()[x][i] = upper_triangular[counter];
            counter++;
        }
        x++;
    }

    x=1,counter =0;
    while(x<width) {
        for(int i = 0;i<x;i++) {
            image.get_data()[x][i] = lower_triangular[counter];
            counter++;
        }
        x++;
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.

    int x=0,counter=0;
    while(x<width) {
        for(int i = x; i<width;i++) {
            upper_triangular[counter] = image.get_pixel(x,i);
            counter++;
        }
        x++;
    }

    x=1,counter =0;
    while(x<width) {
        for(int i = 0;i<x;i++) {
            lower_triangular[counter] = image.get_pixel(x,i);
            counter++;
        }
        x++;
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // 1. Write width and height on the first line, separated by a single space.
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    std::ofstream file(filename);

    if(file.is_open()) {
        file<<width<<" "<<height<<std::endl;

        int upper = width*(width+1)/2;
        for (int i = 0; i<upper;++i) {
            file << upper_triangular[i];
            if (i != upper-1) {
                file<<" ";
            }
        }
        file<<std::endl;

        int lower =  width*(width-1)/2;
        for (int i = 0; i<lower;i++) {
            file << lower_triangular[i];
            if (i != lower-1) {
                file<<" ";
            }
        }
        file<<std::endl;

        file.close();
    }
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    std::ifstream file(filename);
    int width, height;

    file>>width>>height;
    int upper = width*(width+1)/2;
    int lower =  width*(width-1)/2;

    int* upper_triangular = new int[upper];
    int* lower_triangular = new int[lower];

    for (int i = 0; i<upper;i++) {
        file>>upper_triangular[i];
    }
    for (int i = 0; i<lower;i++) {
        file>>lower_triangular[i];
    }
    file.close();

    SecretImage secret_image(width,height,upper_triangular,lower_triangular);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
