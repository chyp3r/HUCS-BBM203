#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.
    GrayscaleImage image = secret_image.reconstruct();
    int width = image.get_width();
    int height = image.get_height();
    int total = message_length*7;
    if(width*height<total) {
        throw std::runtime_error("Error! Not enough pixel!");
    }
    int counter =0;
    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++) {
            if(counter>=width*height-total) {
                LSB_array.push_back(image.get_pixel(i,j)%2);
            }
            counter++;
        }
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.
    if(LSB_array.size()%7!=0) {
        throw std::runtime_error("Error! Not correct size for LSB Array!");
    }

    for(int i=0;i<LSB_array.size();i+=7) {
        int ascii=0;
        for (int j=0;j<7;j++) {
            ascii = LSB_array[i+j]|(ascii<<1);
        }
        message += static_cast<char>(ascii);

    }
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    for (char c : message) {
        std::bitset<7> bits(c);
        for (int i=6;i>=0;i--) {
            LSB_array.push_back(bits[i]);
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    int width = image.get_width();
    int height = image.get_height();
    int total = LSB_array.size();
    if(width*height<total) {
        throw std::runtime_error("Error! Not enough pixel!");
    }
    int start = width*height-total;
    int count =0;

    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++) {
            int pixel =i*width+j;
             if (pixel>=start && count<total) {
                image.get_data()[i][j] = LSB_array[count]|(image.get_pixel(i,j)&~1);
                 count++;
            }
        }
    }

    SecretImage secret_image(image);
    return secret_image;
}
