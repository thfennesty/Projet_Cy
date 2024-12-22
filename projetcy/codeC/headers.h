#ifndef HEADERS_H
#define HEADERS_H

#define ERROR_FILE_OPEN 1
#define ERROR_FILE_READ 2
#define ERROR_MEMORY_ALLOC 3
#define ERROR_TREE_INSERTION 4
#define ERROR_RESULT_COLLECT 5
#define ERROR_FILE_CLOSE 6
#define ERROR_INVALID_FORMAT 7
#define ERROR_NULL_POINTER 8
#define ERROR_FILE_WRITE 9
#define ERROR_TREE_BUILD 10
#define ERROR_EXTRACT_ID 11

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVL {
  unsigned int id;
  long capacity;    // Station capacity in kWh
  long consumption; // Total consumption of connected clients
  struct AVL *left;
  struct AVL *right;
  int eq; // For verification if the tree respect AVL equilibrium
} Station;

// Prototypes

// AVL
int height(Station *node);
Station *createStation(Station *root, int id, long capacity, long consumption);
Station *researchStation(Station *root, int id);
Station *rotateLeft(Station *root);
Station *rotateRight(Station *root);
Station *doubleRotationLeft(Station *root);
Station *doubleRotationRight(Station *root);
Station *equiBalance(Station *root);
Station *insertStation(Station *root, int id, long capacity, long consumption,
                       int *h);
Station *delMinStation(Station *root, int *id, int *h);
Station *deleteStation(Station *root, int id, int *h);

// Read, Write, Calculate
int analyzesLine(char *line, int *id, long *capacity, long *consumption);
int buildTreeFromData(Station **root, char *filename);
int collectResults(Station *root, FILE *outputFile);
void freeTree(Station *root);

#endif