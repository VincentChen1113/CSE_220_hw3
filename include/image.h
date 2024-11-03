#ifndef __IMAGE_H
#define __IMAGE_H

#include <stdio.h>
#include <stdlib.h>

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 

typedef struct Pixel
{
    unsigned char r, g, b;
} Pixel;


typedef struct Image {
    int height, width, max_Intensity;
    Pixel **image_data;
} Image;

Image *load_image(char *filename);
void delete_image(Image *image);
unsigned char get_image_intensity(Image *image, unsigned int row, unsigned int col);
unsigned short get_image_width(Image *image);
unsigned short get_image_height(Image *image);

unsigned int hide_message(char *message, char *input_filename, char *output_filename);
void write_hide_message(char character, Image *image, int row, int col);
char *reveal_message(char *input_filename);
char reveal_message_helper(Image *image, int row, int col);
unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename);
void reveal_image(char *input_filename, char *output_filename);
void fill_img_msg(Image *image, unsigned char msg);
void hide_img_in_Image( Image *sec_image, Image *image);
unsigned char resv_chr_from_img(Image *img);
void reserve_img_to_img(Image *img, Image *sec_image);
#endif // __IMAGE_H