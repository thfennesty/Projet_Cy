#include "../headers.h"


int analyzesLine(char *line, int *id, long *capacity, long *consumption) {
    // Analyzes a CSV line separated by semicolons and extracts id, capacity, and consumption
    
    char *tmp;

    // Extract id 
    tmp = strtok(line, ";");
    if (tmp == NULL) {
        return 1;} //error
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

    return 0;  //success
}


void buildTreeFromData(Station **root, char *filename) {
    // Opens the file and constructs the AVL Tree line by line
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening the file");
        return;  
    }

    char line[256];
    int id;
    long capacity, consumption;
    int eq; //equilibrum of the tree

    // Read the first line to get the number of lines (this is not necessary for functionality)
    if (fgets(line, sizeof(line), file)) {
    }

    // Read the remaining lines and insert the data into the tree
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Removes \0

        int result = analyzesLine(line, &id, &capacity, &consumption);
        if (result == 0) {
            *root = insertStation(*root, id, capacity, consumption, &eq);  // Insert into AVL tree
        } else {
            return;
        }
    }

    fclose(file); 
}


long calculateTotalConsumption(Station *root) {
    // Recursively calculates the total consumption of the nodes in the AVL Tree
    if (root == NULL) {
        return 0;  
    }
    long leftConsumption = calculateTotalConsumption(root->left);
    long rightConsumption = calculateTotalConsumption(root->right);

    return root->consumption + leftConsumption + rightConsumption;
}


void collectResults(Station *root, FILE *outputFile) {
    // Traverses the tree in ascending order and writes the results to a CSV file
    if (root == NULL) {
        return; 
    }

    collectResults(root->left, outputFile);  
    
    // Write the current data to the output file
    fprintf(outputFile, "%d;%ld;%ld\n", root->id, root->capacity, root->consumption);

    collectResults(root->right, outputFile);  // Recur on the right subtree
}


void freeTree(Station *root) {
    // Recursively frees the memory used by the AVL Tree
    if (root == NULL) {
        return;  
    }

    freeTree(root->left);  
    freeTree(root->right); 

    free(root);  // Free the current node
}
