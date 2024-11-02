#include "qtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

QTNode *create_quadtree(Image *image, double max_rmse) {
    
    QTNode *root = (QTNode *)malloc(sizeof(QTNode));
    if (root == NULL) {
        return NULL;
    }

    root->node_type = 'L';
    root->starting_row = 0;
    root->height = image->height;
    root->starting_col = 0;
    root->width = image->width;
    root->average_intensity = get_average_intensity(image, root);
    root->child1 = NULL;
    root->child2 = NULL;
    root->child3 = NULL;
    root->child4 = NULL;

    double RMSE = get_RMSE(image, root);

    if(RMSE > max_rmse){
        root->node_type = 'N';
        root->child1 = create_quadtree_helper(image, max_rmse, 0, root->height/2, 0, root->width/2);
        root->child2 = create_quadtree_helper(image, max_rmse, 0, root->height/2, root->starting_col + root->width/2, root->width/2);
        root->child3 = create_quadtree_helper(image, max_rmse, root->starting_row + root->height/2, root->height/2, 0, root->width/2);
        root->child4 = create_quadtree_helper(image, max_rmse, root->starting_row + root->height/2, root->height/2, root->starting_col + root->width/2, root->width/2);
    }
    else if(RMSE < max_rmse || RMSE == max_rmse){
        return root;
    }

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

    double RMSE = get_RMSE(image, node);

    if(RMSE > max_rmse){
        node->node_type = 'N';

        if(node->height == 1){
            node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, 1, node->starting_col, node->width/2);
            node->child2 = create_quadtree_helper(image, max_rmse, node->starting_row, 1, node->starting_col + node->width/2, node->width/2);
            node->child3 = NULL;
            node->child4 = NULL;

            return node;
        }
        else if(node->width == 1){
            node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col, 1);
            node->child2 = NULL;
            node->child3 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, node->height/2, node->starting_col, 1);
            node->child4 = NULL;

            return node;
        }

        node->child1 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col, node->width/2);
        node->child2 = create_quadtree_helper(image, max_rmse, node->starting_row, node->height/2, node->starting_col + node->width/2, node->width/2);
        node->child3 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, node->height/2, node->starting_col, node->width/2);
        node->child4 = create_quadtree_helper(image, max_rmse, node->starting_row + node->height/2, node->height/2, node->starting_col + node->width/2, node->width/2);
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

    int counter = 0;
    starting_row = node->starting_row;
    region_height = node->height;
    starting_col = node->starting_col;
    region_width = node->width;

    for(int i = starting_row; i < region_height; i++){
        for(int j = starting_col; j < region_width; j++){
            average_intensity += get_image_intensity(image, i, j);
            counter++;
        }
    }

    average_intensity = average_intensity / counter;

    return average_intensity;
}

double get_RMSE(Image *image, QTNode *node){
    double RMSE = 0;
    double average_intensity = node->average_intensity;
    unsigned int starting_row, region_height, starting_col, region_width;
    
    int counter = 0;
    starting_row = node->starting_row;
    region_height = node->height;
    starting_col = node->starting_col;
    region_width = node->width;

    for(int i = starting_row; i < region_height; i++){
        for(int j = starting_col; j < region_width; j++){
            RMSE += pow((get_image_intensity(image, i, j) - average_intensity), 2);
            counter++;
        }
    }

    RMSE = sqrt(RMSE / counter);
    
    return RMSE;
}

void save_qtree_as_ppm(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

QTNode *load_preorder_qt(char *filename) {
    (void)filename;
    return NULL;
}

void save_preorder_qt(QTNode *root, char *filename) {
    (void)root;
    (void)filename;
}

