#include "headers.h"

int analyzesLine(char *line, int *id, long *capacity, long *consumption) {
  // Analyzes a CSV line separated by semicolons and extracts id, capacity, and
  // consumption

  char *tmp;

  // Extract id
  tmp = strtok(line, ";");
  if (tmp == NULL) {
    return ERROR_EXTRACT_ID; // error
  }
  *id = atoi(tmp);

  // Extract capacity
  tmp = strtok(NULL, ";");
  if (tmp == NULL) {
    *capacity = 0;
  } else {
    *capacity = atol(tmp);
  }

  // Extract consumption
  tmp = strtok(NULL, ";");
  if (tmp == NULL) {
    *consumption = 0;
  } else {
    *consumption = atol(tmp);
  }

  return 0; // success
}

int buildTreeFromData(Station **root, char *filename) {
  // Opens the file and constructs the AVL Tree line by line

  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening the file");
    return ERROR_FILE_OPEN;
  }

  char line[256];
  int id;
  long capacity, consumption;
  int eq; // Equilibrium of the tree

  // Skip the first two lines
  if (fgets(line, sizeof(line), file) == NULL) {
    fclose(file);
    return ERROR_FILE_READ;
  }
  if (fgets(line, sizeof(line), file) == NULL) {
    fclose(file);
    return ERROR_FILE_READ;
  }

  // Process subsequent lines
  while (fgets(line, sizeof(line), file)) {
    // Remove the newline character at the end of the line
    line[strcspn(line, "\n")] = '\0';

    // Analyze the line to extract id, capacity, and consumption
    int result = analyzesLine(line, &id, &capacity, &consumption);
    if (result != 0) {
      fprintf(stderr, "Error analyzing line: %s\n", line);
      fclose(file);
      freeTree(*root); // Free memory if there's an error
      *root = NULL;    // Reset the root pointer
      return ERROR_INVALID_FORMAT;
    }

    // Optionally, add checks here for valid id, capacity, and consumption
    // values
    if (id <= 0 || capacity < 0 || consumption < 0) {
      fprintf(stderr, "Invalid data in line: %s\n", line);
      fclose(file);
      freeTree(*root); // Free memory if invalid data is found
      *root = NULL;    // Reset the root pointer
      return ERROR_INVALID_FORMAT;
    }

    // Insert the station into the tree and check for errors
    *root = insertStation(*root, id, capacity, consumption, &eq);
    if (*root == NULL) {
      fprintf(stderr, "Error inserting node into the tree\n");
      fclose(file);
      freeTree(*root); // Free memory if insertion fails
      *root = NULL;    // Reset the root pointer
      return ERROR_TREE_INSERTION;
    }
  }

  // Close the file after processing all lines
  fclose(file);
  return 0; // Success
}

int collectResults(Station *root, FILE *outputFile) {
  if (root == NULL) {
    return 0; // Aucun nœud à traiter (fin de récursion)
  }

  // Parcourir le sous-arbre gauche
  int result = collectResults(root->left, outputFile);
  if (result != 0) {
    fprintf(stderr, "Error in left subtree with error code: %d\n", result);
    return result;
  }

  // Écrire les données du nœud actuel dans le fichier
  if (fprintf(outputFile, "%d;%ld;%ld\n", root->id, root->capacity,
              root->consumption) < 0) {
    perror("Error writing to output file");
    return ERROR_FILE_WRITE;
  }

  // Parcourir le sous-arbre droit
  result = collectResults(root->right, outputFile);
  if (result != 0) {
    fprintf(stderr, "Error in right subtree with error code: %d\n", result);
    return result;
  }

  return 0; // Succès
}

void freeTree(Station *root) {
  // Recursively frees the memory used by the AVL Tree
  if (root == NULL) {
    return;
  }

  freeTree(root->left);
  freeTree(root->right);

  free(root); // Free the current node
}
