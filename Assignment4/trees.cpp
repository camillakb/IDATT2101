#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue>

// Structure for Node.
typedef struct NodeStruct {
  const char *word;
  struct NodeStruct *left;
  struct NodeStruct *right;
} Node;

// Method to create a new node.
Node *newNode(const char *word) {
  Node *res = (Node *)(malloc(sizeof(Node)));

  res->word = word;
  res->left = NULL;
  res->right = NULL;

  return res;
}

// Insert a node into a binary tree.
void insert(Node *root, const char *word) {
  // If the current node in the tree is empty, put the word there.
  if (root == NULL) {
    root = newNode(word);

    // If the word is equal to the word in the current node in the tree,
    // do nothing (this avoids duplicates).
  } else if (strcmp(word, root->word) == 0) {
    return;

    // If the new word is before the word in the current node alphabetically,
    // place the word as the left child to the current node.
  } else if (strcmp(word, root->word) < 0) {
    if (root->left == NULL) {
      root->left = newNode(word);
    } else {
      insert(root->left, word);
    }

    // If the new word is after the word in the current node alphabetically,
    // place the word as the right child to the current node.
  } else if (strcmp(word, root->word) > 0) {
    if (root->right == NULL) {
      root->right = newNode(word);
    } else {
      insert(root->right, word);
    }
  }
}

// Method to print the resulting data.
void printResult(Node *root) {
  if (root) {
    printResult(root->left);
    printf("%s ", root->word);
    printResult(root->right);
  }
}

// Method to print the resulting tree.
void printTree(Node *root, int width) {
  // Using two queues, one for the all the nodes, and one temporary queue to
  // hold nodes for each level in the tree.
  Node *current;
  std::queue<Node *> queue;
  std::queue<Node *> temp;
  temp.push(root);
  int padlen;
  const char *toPrint;

  while (!temp.empty()) {
    queue.swap(temp);

    while (!queue.empty()) {
      current = queue.front();
      queue.pop();

      if (current) {
        //If the current node exists, add it to the tree and create two children (left and right).
        toPrint = current->word;
        temp.push(current->left);
        temp.push(current->right);
        //If the current node does not exist, enter a blank string to keep the tree structure.
      } else {
        toPrint = "";
      }
      
      //Formatting to keep the nodes centered.
      padlen = (width - strlen(toPrint)) / 2;
      printf("%*s%s%*s", padlen, "", toPrint, padlen, "");
    }

    printf("\n");
    width = width / 2;
  }
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Please enter data to insert into the tree.\n");
    exit(1);
  }

  Node *rootNode = newNode(argv[1]);

  for (int i = 2; i < argc; i++) {
    insert(rootNode, argv[i]);
  }

  printTree(rootNode, 64);
  printf("\n");
  printResult(rootNode);
  printf("\n");

  return 0;
}