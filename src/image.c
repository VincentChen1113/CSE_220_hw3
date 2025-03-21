#include "image.h"
#include "qtree.h"
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
    while(lines[0] == '#'){
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
    
    FILE *output = fopen(output_filename, "w");

    Image *image = load_image(input_filename);


    int length = strlen(message);
    unsigned int write_counter = 0;
    int end_length = (image->height * image->width) / 8;

    int row = 0, col = 0;

    for(int i = 0; i < length + 1; i++){
        if(i == end_length - 1){
            write_hide_message(0, image, row, col);
            break;
        }else{
            write_hide_message(message[i], image, row, col);
        }
        if(i != length){
            write_counter++;
        }
        col += 8;
        if(col >= image->width){
            row++; 
            col -= image->width;
            if(row >= image->height){
               break;
            }
        }
    }
    
    
    ppm_write(image, output);

    delete_image(image);
    fclose(output);

    return write_counter;
}

void write_hide_message(char character, Image *image, int row, int col){
    
    unsigned char intensity = 0;
    int x = row, y = col;
    unsigned char mask = 0x80;
    
    for(int i = 0; i < 8; i++){
        intensity = image->image_data[x][y].r;
        intensity &= 0xFE;
        intensity |= (character & (mask >> i)) >> (7 - i);

        image->image_data[x][y].r = intensity;
        image->image_data[x][y].g =intensity;
        image->image_data[x][y].b = intensity;



        y++;
        if(y >= image->width){
            x++;
            y = 0;
            if(x >= image->height){
                return;
            }
        }
    }

}

char *reveal_message(char *input_filename) {

    Image *image = load_image(input_filename);
    
    char *message = malloc(1024 * 256 * sizeof(char));
    if( message == NULL){
        return NULL;
    }

    int length = (image->height * image->width);

    int row = 0, col = 0;

    for(int i = 0; i < length; i++){
        char charater = reveal_message_helper(image, row, col);

        message[i] = charater;
        col += 8;

        if(charater == 0){
            break;
        }

        if(col >= image->width){
            col -= image->width;
            row++;
            if(row >= image->height){
                break;
            }
        }
    }

    delete_image(image);

    return message;
}

char reveal_message_helper(Image *image, int row, int col){
    
    int mask = 0x01;
    char message = 0;
    unsigned char intensity;
    int x = row, y = col;
    for(int i = 0; i < 8; i++){
        intensity = image->image_data[x][y].r;
        message |= (intensity & mask) << (7-i);
        y++;
        if(y >= image->width){
            y = 0;
            x++;
            if(x >= image->height){
                break;
            }
        }
    }

    return message;
}

int x=0,y=0;

int fill_img_msg(Image *image, unsigned char msg) {
	unsigned char enc = msg;
	int j = 0;
    for (j = 0; j < 8; j++) {

        image->image_data[x][y].r = (image->image_data[x][y].r & 0xfe) | ((enc&0x80)>>7);
        image->image_data[x][y].g = (image->image_data[x][y].g & 0xfe) | ((enc&0x80)>>7);
        image->image_data[x][y].b = (image->image_data[x][y].b & 0xfe)  | ((enc&0x80)>>7);

        enc = enc << 1;
        y++;
        if (y >= image->width) {
            y = 0;
            x++;
            if (x >= image->height)
                return 0;
        }
    }
  return 1;

}

int hide_img_in_Image(Image *sec_image, Image *image) {

    int i = 0 ,j = 0, ret = 0;
    unsigned char width = 0, height = 0;   

    x = 0, y = 0;
    width = sec_image->width;
    height = sec_image->height;
   
    ret = fill_img_msg(image, width);
    if(ret == 0) {
        return ret;
    }
    ret = fill_img_msg(image, height);
    if(ret == 0) {
        return ret;
    }
    for (i = 0;i <sec_image->height;i++) {
	   for (j = 0;j<sec_image->width; j++){
            unsigned char enc = sec_image->image_data[i][j].r;
	   	    ret = fill_img_msg(image,enc);
            if(ret == 0){
                return ret;
            }
	   }
   }
    return ret;
}


unsigned int hide_image(char *secret_image_filename, char *input_filename, char *output_filename) {
   FILE *fp = fopen(output_filename, "w");
   Image *image = load_image(input_filename);
   Image *sec_img = load_image(secret_image_filename);

   int ret = hide_img_in_Image(sec_img,image);

   ppm_write(image,fp);

   delete_image(image);
   delete_image(sec_img);

   fclose(fp);
   
   return ret;
}

unsigned char resv_chr_from_img(Image *img) {
    unsigned char msg = 0;
    int i = 0;
    msg = 0;

    for (i = 0;i < 8;i++) {
        msg = msg | (img->image_data[x][y].r & 0x1);
        if (i!=7)
            msg = msg <<1;
        y++;
        if (y >= img->width) {
            x++;
            y = 0;
            if (x >= img->height)
                break;
        }
    }

	return msg;
}



void reserve_img_to_img(Image *img, Image *sec_image) {
	unsigned char width,height;

	width = resv_chr_from_img(img);
	height = resv_chr_from_img(img);

	sec_image->image_data = malloc(sizeof(Pixel *) * height);
	sec_image->max_Intensity = 255;
	sec_image->width = width;
	sec_image->height = height;

	for (int i = 0;i < height;i++)
		sec_image->image_data[i] = malloc(sizeof(Pixel) * width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			unsigned char img_pix=0;
			img_pix= resv_chr_from_img(img);
			sec_image->image_data[i][j].r = img_pix;
			sec_image->image_data[i][j].g = img_pix;
			sec_image->image_data[i][j].b = img_pix;
		}
	}

}

void reveal_image(char *input_filename, char *output_filename) {
   Image *image = load_image(input_filename);
   FILE *fp = fopen(output_filename, "w");

   x=0;
   y=0;

   Image *sec_image = malloc(sizeof(Image));
   if (image==NULL)
	   printf("file open fail!\n");

   reserve_img_to_img(image, sec_image);
   ppm_write(sec_image, fp);
   fclose(fp);

   delete_image(image);
   delete_image(sec_image);

}




