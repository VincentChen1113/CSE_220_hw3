#include "image.h"
#include <stdlib.h>

Image *load_image(char *filename) {    
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        //have an error opening the file
        return NULL;
    }

    Image *image = malloc(sizeof(Image)); 
    if (image == NULL) {
        fclose(file);
        return NULL; 
    }

    char* header = malloc(10 * sizeof(char));
    fscanf(file, "%s", header);//read header and ignore it
    free(header);
    fscanf(file, "%d %d", &image->width, &image->height);
    fscanf(file, "%d", &image->max_Intensity);
    
    //allocate the space for pixels
    image->image_data = malloc(image->height * sizeof(Pixel *));
    if (image->image_data == NULL) {
        free(image);
        fclose(file);
        return NULL; 
    }

    for(int i = 0; i < image->height; i++){
        image->image_data[i] = malloc(image->width * sizeof(Pixel));

        if (image->image_data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(image->image_data[j]);
            }
            free(image->image_data);
            free(image);
            fclose(file);
            return NULL; 
        }
    }

     for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int red, green, blue;
            fscanf(file, "%d %d %d", &red, &green, &blue);
            image->image_data[i][j].r = red;
            image->image_data[i][j].g = green;
            image->image_data[i][j].b = blue;
        }
    }

    fclose(file);

    return image;
}

void delete_image(Image *image) {
    for(int i = 0; i < image->height; i++){
        free(image->image_data[i]);
    }
    free(image->image_data);
    free(image);
}

unsigned short get_image_width(Image *image) {
    return (short)image->width;
}

unsigned short get_image_height(Image *image) {
    return (short)image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    int pixel_intensity = image->image_data[row][col].r + image->image_data[row][col].b + image->image_data[row][col].g;
    return (char) pixel_intensity;
}

unsigned int hide_message(char *message, char *input_filename, char *output_filename) {
    (void)message;
    (void)input_filename;
    (void)output_filename;
    return 0;
}

char *reveal_message(char *input_filename) {
    (void)input_filename;
    return NULL;
}

unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
    (void)secret_image_filename;
    (void)input_filename;
    (void)output_filename;
    return 10;
}

void reveal_image(char *input_filename, char *output_filename) {
    (void)input_filename;
    (void)output_filename;
}
