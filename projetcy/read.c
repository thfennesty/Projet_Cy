#include "headers.h"

int read_element (const char* filepath){
  FILE* file = fopen(filepath, "r");
  if (!file) {
      perror("Erreur lors de l'ouverture du fichier");
      exit(1);
   }
  int num;
  if (file){
    fscanf(file, "%d", &num);
  }
  return num;
}

