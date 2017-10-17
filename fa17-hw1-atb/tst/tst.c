#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tst.h"

/* ############### TRIE FUNCTIONS #################
   These comprise the public interface for our Trie 
   ################################################ */

/* insert_tst is an idempotent function.
 * returns 1 if str was inserted into tree successfully,
   otherwise returns 0.
 * invokes _insert to insert word into internal tree. */

struct TSTnode* find_max(struct TSTnode* node);

int insert_tst(struct TSTnode** tree, const char* str) {
    return _insert(tree, str, 0);
}

/* search_tst returns 1 if word is in tree, otherwise 0. */
int search_tst(struct TSTnode* tree, const char* str) {
    if (!tree)
        return 0;
    if (*str < tree->self)
        return search_tst(tree->left, str);
    else if (*str > tree->self)
        return search_tst(tree->right, str);
    else if (*(str+1))
        return search_tst(tree->sub, str+1);
    else {
        return tree->word != NULL;
    }
}

/* delete_tst returns 1 if str was deleted from tree successfully,
   returns 0 if encounter any error, such as str does not exist. 
 * should invoke _delete to delete str from internal tree. */
int delete_tst(struct TSTnode** tree, const char* str) {
    if (search_tst(*tree, str)) {
        return _delete(tree, str);
    }
    return 0;
}

/* prefix_tst returns 1 if pre exists in tree, 0 otherwise */
int prefix_tst(struct TSTnode* tree, const char* pre) {
    if (!tree)
        return 0;
    if (*pre < tree->self)
        return prefix_tst(tree->left, pre);
    else if (*pre > tree->self)
        return prefix_tst(tree->right, pre);
    else if (*(pre+1))
        return prefix_tst(tree->sub, pre+1);
    else
        return 1;
}

/* clear_tst frees entire tree 
 * Remember to set old TSTnode pointer to NULL */
void clear_tst(struct TSTnode** tree) {
    // TODO: Complete for Part 2, Q4.
    // base case: if there aren't any children then delete yourself
    if (*tree == NULL) {
	return;
    }
    if ((*tree)-> word != NULL) {
	free((*tree)-> word);
	(*tree)->word = NULL;
    }
    if ((*tree) != NULL){
	clear_tst((*tree)->right);
	clear_tst((*tree)->left);
	clear_tst((*tree)->sub);
	free(*tree);
	(*tree) = NULL;
    }
}

/* ############### TRIE NODE FUNCTIONS #################
   These functions internally implement TST's public interface
   ##################################################### */

/* new_node returns a new node with provided character */
struct TSTnode* new_tst_node(char self) {
    // TODO: Complete for Part 2, Q1.
    struct TSTnode* temp = (struct TSTnode*) malloc(sizeof(struct TSTnode));
    if (temp == NULL) {
	return NULL;
    }
    temp->right = NULL;
    temp->left = NULL;
    temp->sub = NULL;
    temp->self= self;
    temp->word = NULL;
    return temp;
}

/* _insert inserts the string str (starting from character at $position into
      tst node @tree. A leaf TSTnode should have a copy of the inserted str
      * Returns 1 if the insertion was successful, otherwise returns 0. */
int _insert(struct TSTnode** tree, const char* str, int position) {
    // TODO: Complete for Part 2, Q2.
    // Hint: to copy a string, use strcpy

    // check if current node is Null, if so then loop through the string and create nodes
    if (*tree == NULL) {
	// check if the position is 1 shorter than length, if yes then set word to the start of the string and return 1
	char letter = *(str+position);
	*tree  = new_tst_node(letter); // create a new node
	//position += 1; // increment the position
	
	if (position == strlen(str)-1 ) {
	    // malloc space for the new string into (*tree)->word before copying in the string into the location
	    (*tree)->word = (char *)malloc(strlen(str) * sizeof(char)); // malloc enough space to hold the new string
	    if ((*tree)->word == NULL) {
		return 0; // not enough space
	    }
	    strcpy((*tree)->word, str);
	    return 1;
	}
        position += 1;
	//*tree = (*tree)->sub;
        _insert(&((*tree)->sub), str, position);
    }

    // check if you are inserting a prefix into the tst
    else if (position == strlen(str) - 1) {
	// malloc space for the new string into (*tree)->word before copying in the string into the location
	(*tree)->word = (char *)malloc(strlen(str) * sizeof(char)); // malloc enough space to hold the new string
	if ((*tree)->word == NULL) {
	    return 0; // not enough space
	}
	strcpy((*tree)->word, str);
	return 1;
    }
    
    // otherwise have the 3 cases, <, =, > and set the new node then recursive
   
    else if (*(str+position) > (*tree)->self) {
	// pass in the location where you want to create the rest of the tree
	_insert(&((*tree)->right), str, position);
    }
    else if (*(str+position) < (*tree)->self) {
	// character is greater than the character in the node
	_insert(&((*tree)->left), str, position);
    }
    else {
	// case where the letter equals the character in the node
	// increment position and don't create a new node
	_insert(&((*tree)->sub), str, position+1);
    }
    return 1;
}

/* _delete deletes the string str from tst tree 
 * Return 1 if the deletion was successful,
 * otherwise returns 0. */
int _delete(struct TSTnode** tree, const char* str) {
    // base case hit the length of the string
    if (strlen(str) == 1){
	// free the node, set it to Null
	free ((*tree)->word);
	free (*tree);
	(*tree) = NULL;
	return 1; // successfully deleted the node
    }
    // left
    else if (str[0] > (*tree)->self) {
	_delete(&((*tree)->right), str);
        //free(*tree);
	//	(*tree) = NULL;
    }
    // right
    else if (str[0] < (*tree)->self) {
	_delete(&((*tree)->left), str);
	//free(*tree);
	//(*tree) = NULL;
    }
    // center
    else {
	// case 1: has a sub or word ends at this node, don't free
	if ((*tree)->word != NULL | (*tree)->sub != NULL) {
	    _delete(&((*tree)->sub), str+1);
	    // don't free
	}
	// case 2: has a right node
	else if ((*tree)->left == NULL) {
	    _delete(&((*tree)->sub), str+1);
	    struct TSTnode* right = (*tree)->right; // save the right
	    free(*tree); // free the node 
	    (*tree) = right; // set the node equal to the right node
	}
	// case 3: has a left node
	else if ((*tree)->right == NULL) {
	    _delete(&((*tree)->sub), str+1);
	    struct TSTnode* left = (*tree)->left; // save the right
	    free(*tree); // free the node
	    (*tree) = left; // set the node equal to the right node
	}
	// case 4: has 2 nodes
	else {
	    _delete(&((*tree)->sub), str+1);
	    struct TSTnode* max = find_max((*tree)->left); // find the parent of node on the left
	    // two possible cases max->right is null, then make max the root
	    if ((max->right) == NULL) {
		(*tree) = max;
	    }
	    // max-> right is not null, then make max->right the root, max->right = max->right->left
	    else {
		struct TSTnode* oldLeft = max->right->left;
		(*tree) = max->right;
		max->right = oldLeft;
	    }
	    free(*tree);
	}
    }
}

int _delete1(struct TSTnode** tree, const char* str) {
    // TODO: Complete for Part 2, Q3.
    // Hint: remember to delete and free TSTnodes when necessary.
    // Hint: remember to re-structure your TSTree!
    // First find the node at this value
    // if the tree is NULL return 0
    if (*tree == NULL) {
	return 0;
    }
    else if (str[0] > (*tree)->self) {
	// need to find where the first letter occurs in the tree, not incrementing the pointer yet
	// the value is greater than current node, move to the right
	_delete(&((*tree)->right), str);
    }
    else if (str[0] < (*tree)->self) {
	_delete(&((*tree)->left), str);
    }
    else { // we have a valid match
	// case 1: node has no children
	if ((*tree)->right == NULL && (*tree)->left == NULL && (*tree)->sub == NULL) {
	    // free the node, set it to NULL and return 1 for valid deletion
	    free(*tree);
	    (*tree) = NULL;
	    return 1;
	}
	// case 2: node has a sub and you've hit the end of the string
	else if ((*tree)->sub != NULL && str == '/0') {
	    // free the word pointer for this node
	    free((*tree)->word);
	    return 1; // successfully removed the word from the tst
	}
	// case 3: node has one child
	else if ((*tree)->left == NULL) {
	    // set the other side to the following node
	    _delete(&((*tree)->right), str);
	    //struct TSTnode* temp = (struct TSTnode*)malloc(sizeof(struct TSTnode));
	    //temp = *tree; // save a pointer to the node being replaced
	    //(*tree) = (*tree)->right; // replace the node with its right child
	    //free(temp); // free the original node
	}
	else if ((*tree)->right == NULL) {
	    _delete(&((*tree)->left), str);
	    // set the other side to the following node
	    //struct TSTnode* temp = (struct TSTnode*)malloc(sizeof(struct TSTnode));
	    //temp = (*tree); // save a pointer to the node being replaced
	    //(*tree) = (*tree)->left; // replace the node with its right child
	    //free(temp); // free the original node 
	}
		 
	// case 4: node has 2 children
	else {
	    // find the max node on the left to replace the current node with
	    struct TSTnode* temp = (struct TSTnode*)malloc(sizeof(struct TSTnode));
	    temp = find_max((*tree)->left); // find the maximum element on the left side of the tree and replace

	    struct TSTnode* toReplace = (struct TSTnode*)malloc(sizeof(struct TSTnode));
	    toReplace = *tree; // save the current node before replacing
	    
	    // replace the value with the new value
	    (*tree) = temp;
	    free(toReplace); // free the original node
	    
	    // replace the left node
	    _delete(&((*tree)->left), str);
	    free(temp);
	}
    }
    return 0;
}

struct TSTnode* find_max(struct TSTnode* node) {
    // want to recursively call the right pointer of the given node until it is null
    if (node->right == NULL | node->right->right == NULL) {
	return node; // return this value because it is the rightmost element, greatest node on this side
    }
    return find_max(node->right); // continue the call on the right elements recursively
}


void _clear(struct TSTnode* node) {
    // TODO: Complete for Part2, Q4.
    // You may or may not need to use this function.
    
}

/* ############### HELPER FUNCTIONS ####################
   These functions you may find useful for helping to
   implement other functions.
   ##################################################### */

/* allocation_failed gets invoked when memroy allocation fails */
int allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    return 0;
}
