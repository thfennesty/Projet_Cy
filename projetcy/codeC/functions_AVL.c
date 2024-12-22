#include "headers.h"

Station *createStation(Station *root, int id, long capacity, long consumption) {
  // To create a new station
  Station *new = malloc(sizeof(Station));
  if (new == NULL) {
    exit(1);
  }

  new->id = id;
  new->capacity = capacity;
  new->consumption = consumption;
  new->left = NULL;
  new->right = NULL;
  new->eq = 0;
  return new;
}

Station *researchStation(Station *root, int id) {
  // To find a station in the tree
  if (root == NULL) {
    return NULL;
  } else if (id == root->id) {
    return root;
  } else if (id < root->id) {
    return researchStation(root->left, id);
  } else {
    return researchStation(root->right, id);
  }
}

int height(Station *node) {
    if (node == NULL) {
        return -1;  // La hauteur d'un nœud NULL est -1
    }
    return fmax(height(node->left), height(node->right)) + 1;  // Hauteur maximale entre les sous-arbres gauche et droit
}

Station *rotateLeft(Station *root) {
  Station *pivot = root->right;
  root->right = pivot->left;
  pivot->left = root;

  root->eq = fmax(height(root->left), height(root->right)) + 1;
  pivot->eq = fmax(height(pivot->left), height(pivot->right)) + 1;

  return pivot;
}

Station *rotateRight(Station *root) {
  Station *pivot = root->left;
  root->left = pivot->right;
  pivot->right = root;

  root->eq = fmax(height(root->left), height(root->right)) + 1;
  pivot->eq = fmax(height(pivot->left), height(pivot->right)) + 1;

  return pivot;
}



Station *doubleRotationLeft(Station *root) {
  root->right = rotateRight(root->right);
  return rotateLeft(root);
}

Station *doubleRotationRight(Station *root) {
  root->left = rotateLeft(root->left);
  return rotateRight(root);
}


Station *equiBalance(Station *root) {
    if (root->eq >= 2) {
        // Rotation à gauche
        if (root->right->eq >= 0) {
            return rotateLeft(root);
        } else {
            return doubleRotationLeft(root);
        }
    } else if (root->eq <= -2) {
        // Rotation à droite
        if (root->left->eq <= 0) {
            return rotateRight(root);
        } else {
            return doubleRotationRight(root);
        }
    }
    return root;  // Si aucun déséquilibre n'est détecté
}


Station *insertStation(Station *root, int id, long capacity, long consumption, int *h) {
    if (root == NULL) {
        *h = 1;  // Le cas de base : l'arbre est vide, on insère une station
        return createStation(root, id, capacity, consumption);
    }

    // Si la station existe déjà, on met à jour sa consommation
    if (id == root->id) {
        root->consumption += consumption;
        *h = 0;  // Pas de modification de la hauteur de l'arbre
        return root;
    }

    // Insérer récursivement à gauche ou à droite selon la valeur de l'ID
    if (id < root->id) {
        root->left = insertStation(root->left, id, capacity, consumption, h);
    } else {
        root->right = insertStation(root->right, id, capacity, consumption, h);
    }

    // Si la hauteur de l'arbre change, mettre à jour le facteur d'équilibre
    if (*h != 0) {
        root->eq += *h;
        root = equiBalance(root);

        if (root->eq == 0) {
            *h = 0;  // L'arbre est maintenant équilibré
        } else {
            *h = 1;  // La hauteur de l'arbre a augmenté
        }
    }

    return root;
}




Station *delMinStation(Station *root, int *id, int *h) {
  // To delete the minimum station in the tree from the root
  Station *tmp;

  if (root->left == NULL) {
    *id = root->id;
    *h = -1;
    tmp = root;
    root = root->right;
    free(tmp); // Free memory of the removed node
    return root;
  } else {
    root->left = delMinStation(root->left, id, h);
    if (*h != 0) { // Update balance after deletion
      root->eq = root->eq + *h;
      root = equiBalance(root);
      if (root->eq == 0) *h = -1;
      else *h = 0;
    }
  }
  return root;
}

Station *deleteStation(Station *root, int id, int *h) {
  // To delete a station in the tree
  Station *tmp;

  if (root == NULL) {
    *h = 0;
    return root;
  } else if (id > root->id) {
    root->right = deleteStation(root->right, id, h);
  } else if (id < root->id) {
    root->left = deleteStation(root->left, id, h);
    if (*h != 0) *h = -*h;
  } else { // id == root->id
    if (root->right != NULL) {
      int minId;
      root->right = delMinStation(root->right, &minId, h);
      root->id = minId;
    } else {
      tmp = root;
      root = root->left;
      free(tmp); // Free memory of the deleted node
      *h = -1;
    }
  }

  if (root == NULL) *h = 0;
  if (*h != 0) { // Update balance after deletion
    root->eq = root->eq + *h;
    root = equiBalance(root);
    if (root->eq == 0) *h = 0;
    else *h = 1;
  }
  return root;
}
