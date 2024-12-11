#include "../headers.h"

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
  int eq = 0;
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

Station *rotateLeft(Station *root) {
  // If the tree is not balanced, it will be balanced by rotating the root to
  // the right
  Station *pivot;
  int eq_root, eq_pivot;

  pivot = root->right;
  root->right = pivot->left;
  pivot->left = root;

  root->eq = eq_root - fmax(eq_pivot, 0) - 1;
  pivot->eq = fmin(fmin(eq_root - 2, eq_root + eq_pivot - 2), eq_pivot - 1);
  root = pivot;
  return root;
}

Station *rotateRight(Station *root) {
  // If the tree is not balanced, it will be balanced by rotating the root to
  // the left
  Station *pivot;
  int eq_root, eq_pivot;

  pivot = root->left;
  root->left = pivot->right;
  pivot->right = root;

  root->eq = eq_root - fmin(eq_pivot, 0) + 1;
  pivot->eq = fmax(fmax(eq_root + 2, eq_root + eq_pivot + 2), eq_pivot + 1);
  root = pivot;
  return root;
}

Station *doubleRotationLeft(Station *root) {
  // If the tree is not balanced, it will be balanced by rotating the root to
  // the right and then left
  root->right = rotateRight(root->right);
  return rotateLeft(root);
}

Station *doubleRotationRight(Station *root) {
  // If the tree is not balanced, it will be balanced by rotating the root to
  // the left and then right
  root->right = rotateLeft(root->right);
  return rotateRight(root);
}

Station *equiBalance(Station *root) {
  // To balance the tree
  if (root->eq >= 2) {
    if (root->right->eq >= 0) {
      return rotateLeft(root);
    } else {
      return doubleRotationLeft(root);
    }
  } else if (root->eq <= -2) {
    if (root->left->eq <= 0) {
      root = rotateRight(root);
    } else {
      return doubleRotationRight(root);
    }
  }
  return root;
}

Station *insertStation(Station *root, int id, long capacity, long consumption,
                       int *h) {
  // To insert a new station in the tree

  if (root == NULL) {
    *h = 1;
    return createStation(root, id, capacity, consumption);
  } else if (id < root->id) {
    root->left = insertStation(root, id, capacity, consumption, h);
    *h = -*h;
  } else if (id > root->id) {
    root->right = insertStation(root, id, capacity, consumption, h);
  } else {
    *h = 0;
    return root;
  }
  if (*h != 0) { // to update the height of the tree
    root->eq = root->eq + *h;
    root = equiBalance(root);
    if (root->eq == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return root;
}

Station *delMinStation(Station *root, int *id, int *h) {
  // To delete the minimum station in the tree from the root (needed for the
  // delete station function)
  Station *tmp;

  if (root->left == NULL) {
    *id = root->id;
    *h = -1;
    tmp = root;
    root = root->right;
    free(tmp);
    return root;
  } else {
    root->left = delMinStation(root->left, id, h);
    *h = -*h;
  }
  if (*h != 0) {
    root->eq = root->eq + *h;
    root = equiBalance(root);
    if (root->eq == 0) {
      *h = -1;
    } else {
      *h = 0;
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
    *h = -*h;
  } else if (root->right != NULL) {
    int minId;
    root->right = delMinStation(root->right, &minId, h);
    root->id = minId;
  } else {
    tmp = root;
    root = root->left;
    free(tmp);
    *h = -1;
  }
  if (root == NULL) {
    *h = 0;
  }
  if (*h != 0) {
    root->eq = root->eq + *h;
    root = equiBalance(root);
    if (root->eq == 0) {
      *h = 0;
    } else {
      *h = 1;
    }
  }
  return root;
}
