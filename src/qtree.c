#include "qtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

QTNode *create_quadtree(Image *image, double max_rmse) {
    
    QTNode *root;


    root = create_quadtree_helper(image, max_rmse, 0, image->height, 0, image->width); 

    return root;
}

QTNode *create_quadtree_helper(Image *image, double max_rmse, int starting_row, int height, int starting_col, int width){
    QTNode *node = (QTNode *)malloc(sizeof(QTNode));
    if (node == NULL) {
        return NULL;
    }

    node->node_type = 'L';
    node->starting_row = starting_row;
    node->height = height;
    node->starting_col = starting_col;
    node->width = width;
    node->average_intensity = get_average_intensity(image, node);
    node->child1 = NULL;
    node->child2 = NULL;
    node->child3 = NULL;
    node->child4 = NULL;

    if(node->height == 1 && node->width == 1){
        return node;
    }

    double RMSE = get_RMSE(image, node);

    if(RMSE > max_rmse){
        node->node_type = 'N';

        if(node->height == 1){
            node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, 1, node->starting_col, node->width/2);
            node->child2 = create_quadtree_helper(image, max_rmse, node->starting_row, 1, node->starting_col + node->width/2, (node->width - node->width/2));
            node->child3 = NULL;
            node->child4 = NULL;

            return node;
        }
        else if(node->width == 1){
            node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col, 1);
            node->child2 = NULL;
            node->child3 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, (node->height - node->height/2), node->starting_col, 1);
            node->child4 = NULL;

            return node;
        }

        node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col, node->width/2);
        node->child2 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col + node->width/2, (node->width - node->width/2));
        node->child3 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, (node->height - node->height/2), node->starting_col, node->width/2);
        node->child4 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, (node->height - node->height/2), node->starting_col + node->width/2, (node->width - node->width/2));
    }
    else if(RMSE < max_rmse || RMSE == max_rmse){
        return node;
    }

    return node;
} 


QTNode *get_child1(QTNode *node) {
    return node->child1;
}

QTNode *get_child2(QTNode *node) {
    return node->child2;
}

QTNode *get_child3(QTNode *node) {
    return node->child3;
}

QTNode *get_child4(QTNode *node) {
    return node->child4;
}

unsigned char get_node_intensity(QTNode *node) {
    return node->average_intensity;
}

void delete_quadtree(QTNode *root) {
    if(root == NULL){ 
        return;  
    }
    
    delete_quadtree(root->child1);
    delete_quadtree(root->child2);
    delete_quadtree(root->child3);
    delete_quadtree(root->child4);

    free(root);
}

double get_average_intensity(Image *image, QTNode *node){
    double average_intensity = 0;
    unsigned int starting_row, region_height, starting_col, region_width;

    //int counter = 0;
    starting_row = node->starting_row;
    region_height = node->height;
    starting_col = node->starting_col;
    region_width = node->width;
    double n = region_height * region_width;

    for(unsigned int i = starting_row; i < (starting_row + region_height); i++){
        for(unsigned int j = starting_col; j < (starting_col + region_width); j++){
            average_intensity += get_image_intensity(image, i, j);
            //counter++;
        }
    }

    average_intensity = average_intensity / n;

    return average_intensity;
}

double get_RMSE(Image *image, QTNode *node){
    double RMSE = 0;
    double average_intensity = node->average_intensity;
    unsigned int starting_row, region_height, starting_col, region_width;
    
    double counter = 0;
    starting_row = node->starting_row;
    region_height = node->height;
    starting_col = node->starting_col;
    region_width = node->width;

    for(unsigned int i = starting_row; i < (starting_row + region_height); i++){
        for(unsigned int j = starting_col; j < (starting_col + region_width); j++){
            RMSE += pow((get_image_intensity(image, i, j) - average_intensity), 2);
            counter++;
        }
    }

    RMSE = sqrt(RMSE / counter);
    
    return RMSE;
}

int line = 0;

void print_QTree(QTNode *root){
    if(root == NULL){
        return;
    }
    printf("%d, %c %d %d %d %d %d\n", line, root->node_type, root->average_intensity, root->starting_row, root->height, root->starting_col, root->width);
    line++;
    /*if(line % 256 == 255){
        printf("\n");
    }*/
    print_QTree(root->child1);
    print_QTree(root->child2);
    print_QTree(root->child3);
    print_QTree(root->child4);

}

void fill_image(Image *image, QTNode *node){
    unsigned char intensity = node->average_intensity;
    for(int i = node->starting_row; i < (node->starting_row + node->height); i++){
        for(int j = node->starting_col; j < (node->starting_col + node->width); j++){
            image->image_data[i][j].r = intensity;
            image->image_data[i][j].g = intensity;
            image->image_data[i][j].b = intensity;
        }
    }

}

void tree_traversal(QTNode *root, Image *image){
    if(root == NULL){
        return;
    }
    if(root->node_type == 'L'){
        fill_image(image, root);
    }
    else{
        tree_traversal(root->child1, image);
        tree_traversal(root->child2, image);
        tree_traversal(root->child3, image);
        tree_traversal(root->child4, image);
    }

}

void ppm_write(Image *image, FILE *file){
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", image->height, image->width);
    fprintf(file, "255\n");
    for(int i = 0; i < image->height; i++){
        for(int j = 0; j < image->width; j++){
            fprintf(file, "%d %d %d ", image->image_data[i][j].r, image->image_data[i][j].g, image->image_data[i][j].b);
        }
    }

}

void save_qtree_as_ppm(QTNode *root, char *filename) {

    Image *image = malloc(sizeof(Image));
    if(image == NULL){
        return;
    }
    image->height = root->height;
    image->width = root->width;

    image->image_data = malloc(image->height * sizeof(Pixel *));
    if (image->image_data == NULL) {
        return; 
    }

    for(int i = 0; i < image->height; i++){
        image->image_data[i] = malloc(image->width * sizeof(Pixel));
        bzero(image->image_data[i], image->width * sizeof(Pixel));
        if (image->image_data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(image->image_data[j]);
            }
            free(image->image_data);
            free(image);
            return; 
        }
    }

    tree_traversal(root, image);
    
    FILE *file = fopen(filename, "w");

    ppm_write(image, file);
    fclose(file);
    
    delete_image(image);

}



QTNode *load_preorder_qt(char *filename) {
    FILE *file = fopen(filename, "r");
    
    if(file == NULL){
        return NULL;
    }

    QTNode *root = NULL;
    root = load_preorder_qt_helper(file);

    fclose(file);
    return root;
}

QTNode *load_preorder_qt_helper(FILE *file){
    char node_type;
    int average_intensity;
    int starting_row, height, starting_col, width;

    fscanf(file, "%c %d %d %d %d %d\n",&node_type, &average_intensity, &starting_row, &height, &starting_col, &width);

    QTNode *node = malloc(sizeof(QTNode));
    if(node == NULL){
        return NULL;
    }

    node->node_type = node_type;
    node->average_intensity = average_intensity;
    node->starting_row = starting_row;
    node->height = height;
    node->starting_col = starting_col; 
    node->width = width;

    if(node->node_type == 'L'){
        node->child1 = NULL;
        node->child2 = NULL;
        node->child3 = NULL;
        node->child4 = NULL;
        return node;
    }
    else{
        if(node->height == 1){
            node->child1 = load_preorder_qt_helper(file);
            node->child2 = load_preorder_qt_helper(file);
            node->child3 = NULL;
            node->child4 = NULL;

            return node;
        }
        else if(node->width == 1){
            node->child1 = load_preorder_qt_helper(file);
            node->child2 = NULL;
            node->child3 = load_preorder_qt_helper(file);
            node->child4 = NULL;

            return node;
        }
        node->child1 = load_preorder_qt_helper(file);
        node->child2 = load_preorder_qt_helper(file);
        node->child3 = load_preorder_qt_helper(file);
        node->child4 = load_preorder_qt_helper(file);
    }
    
    return node;
}

void save_preorder_qt(QTNode *root, char *filename) {
    FILE *file = fopen(filename, "w");

    write_tree(root, file);

    fclose(file);
}

void write_tree(QTNode *node, FILE *file){
    if(node == NULL){
        return;
    }
    fprintf(file, "%c %d %d %d %d %d\n", node->node_type, node->average_intensity, node->starting_row, node->height, node->starting_col, node->width);
    
    write_tree(node->child1, file);
    write_tree(node->child2, file);
    write_tree(node->child3, file);
    write_tree(node->child4, file);

}

