#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>

typedef struct AVL{
  unsigned int id;             
  long capacity;           // Station capacity in kWh
  long consumption;        // Total consumption of connected clients
  struct AVL *left;        
  struct AVL *right;       
  int eq;                  // For verification if the tree respect AVL equilibrium
}Station; 


// Prototypes 

//AVL
Station *createStation(Station *root, int id, long capacity, long consumption);
Station *researchStation(Station *root, int id);
Station *rotateLeft(Station *root);
Station *rotateRight(Station *root);
Station *doubleRotationLeft(Station *root);
Station *doubleRotationRight(Station *root);
Station *equiBalance(Station *root);
Station *insertStation(Station *root, int id, long capacity, long consumption, int *h);
Station *delMinStation(Station *root, int *id, int *h);
Station *deleteStation(Station *root, int id, int *h);

// Read, Write, Calculate
int analyzesLine(char *line, int *id, long *capacity, long *consumption);
void buildTreeFromData(Station **root, char *filename);
long calculateTotalConsumption(Station *root);
void collectResults(Station *root, FILE *outputFile);
void freeTree(Station *root);


#endif