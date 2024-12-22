#include "codeC/headers.h"

int main(int argc, char *argv[]) {
    // Vérification de la présence d'un fichier d'entrée passé en argument

    if (argc < 2) {  // Il faut au moins deux arguments : le nom du programme et le fichier
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return ERROR_FILE_READ;
    }

    // Utilisation du fichier passé en argument
    char *file_to_use = argv[1];
    printf("Using file: %s\n", file_to_use);  // Affiche le chemin du fichier passé

    // Vérification si le fichier peut être ouvert
    FILE *file = fopen(file_to_use, "r");
    if (!file) {
        perror("Error opening the input file");
        return ERROR_FILE_READ;
    }

    // Construction de l'arbre à partir des données du fichier
    Station *root = NULL;
    int error_code = buildTreeFromData(&root, file_to_use);
    if (error_code != 0) {
        fprintf(stderr, "Failed to build tree with error code: %d\n", error_code);
        fclose(file);
        return error_code;
    }

    // Ouverture du fichier de sortie
    FILE *outputFile = fopen("output.csv", "w");
    if (!outputFile) {
        perror("Error opening the output file");
        fclose(file);  // Libère le fichier d'entrée avant de quitter
        freeTree(root);  // Libère l'arbre
        return ERROR_FILE_OPEN;
    }
    printf("Output file opened successfully.\n");

    // Collecte des résultats à partir de l'arbre et écriture dans le fichier
    error_code = collectResults(root, outputFile);
    if (error_code != 0) {
        fprintf(stderr, "Failed to collect results with error code: %d\n", error_code);
        fclose(outputFile);
        fclose(file);  // Libère le fichier d'entrée avant de quitter
        freeTree(root);  // Libère l'arbre
        return error_code;
    }

    printf("Results collected and written to the output file successfully.\n");

    // Fermeture des fichiers
    fclose(outputFile);
    fclose(file);

    // Libération de l'arbre
    freeTree(root);

    return 0;
}
