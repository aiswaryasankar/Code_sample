#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ############### TRIE FUNCTIONS #################
   These comprise the public interface for our Trie 
   ################################################ */

/* Returns 1 if s was inserted into t successfully,
   otherwise returns 0. */
int insert_trie(struct Trie* t, char* s) {
	return insert_trie_node(t->root, s);
}

/* Returns 1 if s is in t, otherwise 0. */
int search_trie(struct Trie* t, char* s) {
	struct TrieNode* node = search_trie_node(t->root, s);
	if (node != NULL && node->wordEnds == 1) 
		return 1;
	return 0;
}

/* Returns 1 if s was deleted from t successfully,
   otherwise returns 0. */
int delete_trie(struct Trie* t, char* s) {
	return delete_trie_node(t->root, s);
}

/* Prints all words in t that start with prefix */
void prefixes_trie(struct Trie* t, char* prefix) {
	prefixes_trie_node(t->root, prefix);
}

/* Returns the number of bytes that
   t takes up in memory. */
unsigned int memory_trie(struct Trie* t) {
	return memory_trie_node(t->root);
}

/* Frees t */
void clear_trie(struct Trie* t) {
	clear_trie_node(t->root);
	free(t);
}

/* Initializes and returns a new Trie */
struct Trie* new_trie() {
	struct Trie* t = (struct Trie*) malloc(sizeof(struct Trie));
	t->root = new_trie_node();
	return t;
}

/* ############### TRIE NODE FUNCTIONS #################
   These functions implement the trie's public interface
   ##################################################### */

/* Inserts the string s into the trie rooted at node. 
   Returns 1 if the insertion was successful, 
   otherwise returns 0. */
int insert_trie_node(struct TrieNode* node, char* s) {

    int i = 0; // Start at the beginning of the string
    int letter = charToIndex(s[i]); // convert the character to an array index of children
    struct TrieNode* next = node->children[letter]; // Check if the node has this child

    // if the node doesn't have this child, iterate through word and add new nodes
    if (next == NULL) {
	// iterate until you hit the last null terminator for the string
	while (s[i] != '\0') {
	    // create a new node
	    struct TrieNode* newNode = new_trie_node();
	    
	    // check that the new node isn't NULL, if yes then return
	    if (newNode == NULL) {
		return 0; // insertion not successful because you can't create space for node
	    }
	    // append it to the node passed in
	    node -> children[letter] = newNode;
	    // set current node to the new Node
	    node = newNode;
	    // increment string index and get the next letter
	    i += 1;
	    letter = charToIndex(s[i]);
	}
	// after hitting the end of the word, update the last letter's wordEnds
	node -> wordEnds = 1; // wordEnds = 1 means word is over
	return 1; // successfully added to the word
    }
    else {
	// if the node has this child, recursively call with this node and the next char 
	return insert_trie_node(next, s+1);
    }
    
}

/* Searches for s starting from node, and returns TrieNode where s ends. 
   If s cannot be found, then NULL is returned. */
struct TrieNode* search_trie_node(struct TrieNode* node, char* s) {
    // Check if the current letter of the string is the null terminator
    // If it is then you've successfully found the entire string
    if (s[0] == '\0') {
	return node; // the current node is the last valid node
    }
    // Check if the current letter is a child of the current node
    // If it isn't then the string doesn't exist, return False
    int letter = charToIndex(s[0]);
    struct TrieNode* next = node->children[letter];
    if (next == NULL) {
	return NULL;
    }
    // If it isn't Null then recursively call with the next letter
    else {
	return search_trie_node(next, s+1);
    }	
}

/* Deletes s from the Trie rooted at node.
   Returns 1 if deletion was sucessful,
   otherwise returns 0. */
int delete_trie_node(struct TrieNode* node, char* s) {
	if (s[0] == '\0') {
		node->wordEnds = 0;
		return 1;
	}

	int letter = charToIndex(s[0]);
	struct TrieNode* next = node->children[letter];
	int retval = 1;
	int i;

	if (next != NULL) {
		retval = delete_trie_node(next, s + 1);
		if (retval == 1) {
			if (node->wordEnds != 0)
				return retval;
			// If the node has no other children and no word
			// ends at that node, we should remove this node
			for (i = 0; i < NUM_LETTERS; i++)
				if (next->children[i] != NULL)
					return retval;
			free(next);
			node->children[letter] = NULL;
		}
	} else {
		retval = 0;
	}
	return retval;
}

/* Prints all words present in Trie rooted at node, that start
   with prefix. */
void prefixes_trie_node(struct TrieNode* node, char* prefix) {
	struct TrieNode* startNode = search_trie_node(node, prefix);
	if (startNode != NULL)
		all_words_trie_node(startNode, prefix);
}

/* Prints all words present in Trie rooted at node, with wordSoFar prepended. */
void all_words_trie_node(struct TrieNode* node, char* wordSoFar) {
	if (node->wordEnds == 1)
		printf("%s\n", wordSoFar);

	size_t len = strlen(wordSoFar);

	int i;
	for (i = 0; i < NUM_LETTERS; i++) {
		struct TrieNode* next = node->children[i];
		if (next != NULL) {
			char word[len + 2];
			char currLetter = indexToChar(i);
			strcharcpy(word, wordSoFar, currLetter);
			all_words_trie_node(next, word);
		}
	}
}

/* Frees Trie rooted at node */
void clear_trie_node(struct TrieNode* node) {
	int letter;
	for (letter = 0; letter < NUM_LETTERS; letter++) {
		if (node->children[letter] != NULL)
			clear_trie_node(node->children[letter]);
	}
	free(node);
}

/* Returns the number of bytes that the Trie rooted at node
   takes up in memory. */
unsigned int memory_trie_node(struct TrieNode* node) {
	// Starting from node, traverse down every node of the Trie that holds a letter and return the number of bytes in the tree
    // Same as just counting all the nodes in the tree

    // Loop through all the nodes in the children array
    // If not null then call the method with the node and add its byte count
    // No need for a base case because it will just not have any children
    int size = sizeof(*node);
    for (int i = 0; i < NUM_LETTERS; i++){
	if (node->children[i] != NULL) {
	    printf("Char is %c\n", indexToChar(i));
	    size += memory_trie_node(node->children[i]);
	}
    }
    return size;
}

/* Initializes and returns a new TrieNode using dynamic memory allocation. */
struct TrieNode* new_trie_node() {
	struct TrieNode* newNode = (struct TrieNode*) malloc(sizeof(struct TrieNode));
	if (newNode == NULL)
		return NULL;
	int i;
	for (i = 0; i < NUM_LETTERS; i++)
		newNode->children[i] = NULL;

	newNode->wordEnds = 0;

	return newNode;
}

/* ############### HELPER FUNCTIONS ####################
   These functions you may find useful for helping to
   implement other functions.
   ##################################################### */

int charToIndex(char c) {
	return (int) (c - 97);
}

int indexToChar(int i) {
	return (char) (i + 97);
}

/* Copies string src into dst, then copies c into dst,
	and null terminates dst. Assumes sizeof(dst) >= strlen(src) + 2 
	e.g. strcharcpy(a, "61C", '!') would result in a = "61C!" */
void strcharcpy(char* dst, char* src, char c) {
    // Part 1
	// Need to iterate through the array by moving the pointers forward
    int length = strlen(src);
    // since we know the length of src, we can iterate through that array
    // and increment the pointer of dst each time
    dst = (char *) malloc((strlen(src)+2) * sizeof(char));

    for (int i = 0; i < length; i++) {
	*dst = *src; // get the value at this position of src and copy into dst
	// increment both of the pointers
	dst++; // don't want to dereference them because that will inc value
	src++;
    }
    // add the char c to the end of dst
    *dst = c;
    *(dst + 1) = '\0'; // add the null terminator to the end of dst
    // now if you print dst it should have the longer string
    
}

