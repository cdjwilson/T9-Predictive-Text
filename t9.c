// Cameron Wilson
// Fall Semester 2020 3rd Year
// t9 is a predictive text program for text on 9 keys, numbers 2-9 and # key

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 100

typedef struct Node {
  char *data;
  int number;
  struct Node *children[9];
  struct Node *parent;
} Node;

typedef struct Trie {

  struct Node *root;

} Trie;

int *findNumSequence(char *word);
void addData(struct Node *Node, char *word);
void addChild(struct Node *parent, int index, int num);
void add(struct Node *head, char *word);
Node *newNode(Node *parent, int num);
Trie *newTrie();
Node *searchTrie(Trie *trie, char *sequence);
void freeTrie(Trie *trie);
void freeNode(Node *node);
void freeSubNode(Node **node);
void freeRoot(Node *node);

// finds the number sequence needed to form the word provided
int *findNumSequence(char *word) {
  int size = strlen(word);
  static int numSequence[100];
  for (int i = 0; 100 > i; i++)
    numSequence[i] = 0; // set every number equal to 0
  for (int i = 0; i < size;
       i++) { // for every number in the word check what number would correspond
              // to it and add it to the sequence
    if (word[i] == ' ') {
      numSequence[i] = 0;
      break;
    } else if ('a' == word[i] || 'b' == word[i] || 'c' == word[i]) {
      numSequence[i] = 2;
    } else if ('d' == word[i] || 'e' == word[i] || 'f' == word[i]) {
      numSequence[i] = 3;
    } else if ('g' == word[i] || 'h' == word[i] || 'i' == word[i]) {
      numSequence[i] = 4;
    } else if ('j' == word[i] || 'k' == word[i] || 'l' == word[i]) {
      numSequence[i] = 5;
    } else if ('m' == word[i] || 'n' == word[i] || 'o' == word[i]) {
      numSequence[i] = 6;
    } else if ('p' == word[i] || 'q' == word[i] || 'r' == word[i] ||
               's' == word[i]) {
      numSequence[i] = 7;
    } else if ('t' == word[i] || 'u' == word[i] || 'v' == word[i]) {
      numSequence[i] = 8;
    } else if ('w' == word[i] || 'x' == word[i] || 'y' == word[i] ||
               'z' == word[i]) {
      numSequence[i] = 9;
    }
  }
  return numSequence;
}

// searches for the word created by the sequence of numbers provided
Node *searchTrie(Trie *trie, char *sequence) {
  Node *temp = trie->root;
  int num = 0;
  int i = 0;
  while (sequence[i + 1] != '\0') { // for each element in the sequence
    num = sequence[i] - 48;         // assign it a value
    if (num == -13) { // if it is "#" check if temp children is not NULL then
                      // set temp to that child
      if (temp->children[8] != NULL) {
        temp = temp->children[8];
      } else {
        return NULL;
      }
    } else {
      if (temp->children[num - 2] !=
          NULL) { // if its child at the index of the number is not NULL go on
        temp = temp->children[num - 2]; // set temp to its new child
      } else { // if it is NULL then return NULL because a word for that
               // sequence does not exist
        return NULL;
      }
    }
    i++;
  } // end of for loop should result in the temp node being equal to the a word
    // correlated to that number sequence
  return temp;
}

// adds the data to the node specified by allocating the space for that data and
// coping the word into that data spot
void addData(struct Node *Node, char *word) {
  Node->data = malloc(100 * sizeof(char));
  strcpy(Node->data, word);
}

// adds the child at the index of the parent node with the number specified
void addChild(struct Node *parent, int index, int num) {
  parent->children[index] = newNode(parent, num);
}

// adds the word provided to the trie by getting the number sequence of the word
// and adding at as children of its numbers
void add(struct Node *head, char *word) {
  int *numS;
  // numS = (int*)malloc(100*sizeof(int)); // sequence of numbers needed to get
  // the word
  numS = findNumSequence(word);
  Node *temp = head;
  int i = 0;
  // while the num in s is not 0
  while (numS[i] != 0) {
    // check the child of temp to see if it has the number for that letter
    // already added
    if (NULL != temp->children[numS[i] - 2]) { // check if temps child at
                                               // (numS[i]-2) is equal to NULL
      temp = temp->children[numS[i] - 2]; // not equal to NULL set temp equal to that child
    } else { // if the child at (numS[i]-2) is NULL add the new node with the
             // number it belongs to and set temp to that new child
      addChild(temp, numS[i] - 2, numS[i]);
      temp = temp->children[numS[i] - 2];
    }
    i++;
  } // end of while loop, found or created the path for the word and temp is
    // equal to the child node equal to the final number in numS if temps data
    // is NULL just add the data
  if (temp->data == NULL) {
    addData(temp, word);
  } else { // otherwise while temps child at index 8 is not null
    while (temp->children[8] != NULL) {
      temp = temp->children[8]; // set temp equal to its pound child
    } // end of while loop temp will be equal to a node with the pound child
      // equal to null; check if the the node we are at has data if it does go
      // to the # node of next one
    addChild(temp, 8, 10);
    temp = temp->children[8];
    addData(temp, word);
  }
}

// creates a new node by allocating space for the node and setting all its
// children to null
Node *newNode(Node *parent, int num) {
  Node *n = malloc(1 * sizeof(Node));
  n->data = NULL;
  n->number = num;
  for (int i = 0; i < 9; i++) {
    n->children[i] = NULL;
  }
  n->parent = parent;
  return n;
}

// creates a new trie by allocated the memory for it and setting the root equal
// to a new node
Trie *newTrie() {
  Trie *b = malloc(1 * sizeof(Trie));
  b->root = newNode(NULL, 0);
  return b;
}

// frees all of the children
void freeSubNode(Node **node) {
  for (int i = 0; 9 > i; i++) {
    if (node[i] != NULL) {
      freeSubNode(node[i]->children);
      freeNode(node[i]);
    }
  }
}

// frees the tries root and passes the double pointer of its children into
// freeSubNode
void freeRoot(Node *node) {
  freeSubNode(node->children);
  freeNode(node);
}
// frees the nodes data then the node
void freeNode(Node *node) {
  free(node->data);
  free(node);
}

// frees the trie and everything it has in it
void freeTrie(Trie *trie) {
  freeRoot(trie->root);
  free(trie);
}

int main(int argc, char **argv) {
  // if File is not provided in the commnand line
  if (argc < 2) {
    printf("missing FILE argument\n");
    exit(1);
  }
  char *filename = argv[1];

  FILE *file;
  char buffer[BUFFER_LEN];
  file = fopen(filename, "r");
  // declare Trie
  Trie *trie = newTrie();
  // read all lines
  while (fgets(buffer, BUFFER_LEN, file)) {
    // add each word to trie
    add(trie->root, buffer);
  }
  fclose(file);
  Node *word = NULL;
  printf("Enter \"exit\" to quit.\n");
  printf("Enter Key Sequence (or \"#\" for next word):\n");
  while (fgets(buffer, 100, stdin)) {
    if (strncmp(buffer, "exit", 4) == 0) {
      break;
    }
    if (strncmp(buffer, "#", 1) ==
        0) { // if requesting next word with # check if the current Node is NULL
             // and check if it has a next child
      if (word != NULL) {
        if (word->children[8] != NULL) { // if it has a child print it
          word = word->children[8];
          printf("%s", word->data);
        } else { // otherwise print one doesnt exist in dictionary
          printf("There are no more T9onyms.\n");
        }
      } else {
        printf("Please enter a valid request.\n");
      }
    } else {
      word = searchTrie(trie, buffer);
      if (word == NULL) {
        printf("Not found in current dictionary.\n");
      } else {
        printf("%s", word->data);
      }
    }
    printf("Enter Key Sequence (or \"#\" for next word):\n");
  } // end of while loop when the program has been exited by the user
  freeTrie(trie);
}
