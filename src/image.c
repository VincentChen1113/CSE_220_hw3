#include "image.h"
#include <stdlib.h>
#include <string.h>

Image *load_image(char *filename) {    
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        //have an error opening the file
        return NULL;
    }

    Image *image = malloc(sizeof(Image)); 
    if (image == NULL) {
        fclose(file);
        free(image);
        return NULL; 
    }

    char* header = malloc(10 * sizeof(char));
    fscanf(file, "%s\n", header);//read header and ignore it
    if(strcmp(header, "P3") != 0){
        free(header);
        return NULL;
    }
    free(header);
        
    
    char* lines = malloc(256 * sizeof(char));
    if(lines == NULL){
        return NULL;
    }
    fgets(lines, 256, file);
    if(lines[0] == '#'){
        bzero(lines, 256);
        fgets(lines, 256, file);// skip comment
    }
    sscanf(lines, "%d %d\n", &image->width, &image->height);
    free(lines);
    fscanf(file, "%d\n", &image->max_Intensity);
    
    //allocate the space for pixels
    image->image_data = malloc(image->height * sizeof(Pixel *));
    if (image->image_data == NULL) {
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
            fscanf(file, "%d %d %d\n", &red, &green, &blue);
            image->image_data[i][j].r = (unsigned char) red;
            image->image_data[i][j].g = (unsigned char) green;
            image->image_data[i][j].b = (unsigned char) blue;
            /*if(i==0)
                printf("r: %d g: %d b: %d\n", image->image_data[i][j].r, image->image_data[i][j].g ,image->image_data[i][j].b);*/
        }
    }

    fclose(file);

    return image;
}

void delete_image(Image *image) {
    if (image == NULL) 
        return;
    for(int i = 0; i < image->height; i++){
        free(image->image_data[i]);
        image->image_data[i] = NULL;
    }
    free(image->image_data);
    free(image);
}

unsigned short get_image_width(Image *image) {
    return image->width;
}

unsigned short get_image_height(Image *image) {
    return image->height;
}

unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col) {
    int pixel_intensity = image->image_data[row][col].r;
    return pixel_intensity;
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
