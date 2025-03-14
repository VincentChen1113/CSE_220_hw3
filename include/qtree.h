#ifndef QTREE_H
#define QTREE_H

#include <stdio.h>
#include <stdlib.h>

#include "image.h"

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 

typedef struct QTNode {
    char node_type;
    unsigned char average_intensity;
    int starting_row, height, starting_col, width;
    struct QTNode *child1, *child2, *child3, *child4;   
} QTNode;

void print_QTree(QTNode *root);
void fill_image(Image *image, QTNode *node);
void tree_traversal(QTNode *root, Image *image);
void ppm_write(Image *image, FILE *file);
void save_qtree_as_ppm(QTNode *root, char *filename);

QTNode *create_quadtree(Image *image, double max_rmse);
QTNode *create_quadtree_helper(Image *image, double max_rmse, int starting_row, int height, int starting_col, int width);  
QTNode *get_child1(QTNode *node);
QTNode *get_child2(QTNode *node);
QTNode *get_child3(QTNode *node);
QTNode *get_child4(QTNode *node);
unsigned char get_node_intensity(QTNode *node);
double get_average_intensity(Image *image, QTNode *node);
double get_RMSE(Image *image, QTNode *node);
void delete_quadtree(QTNode *root);
void save_qtree_as_ppm(QTNode *root, char *filename); 

QTNode *load_preorder_qt(char *filename);
QTNode *load_preorder_qt_helper(FILE *file);

void save_preorder_qt(QTNode *root, char *filename);  
void write_tree(QTNode *node, FILE *file);

#endif // QTREE_H