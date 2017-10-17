#ifndef _TST_H
#define _TST_H

struct TSTnode {
	char* word; // NULL if no word ends at this node
	char self;
	struct TSTnode *left, *sub, *right;
};

/* Public Trie Interface */
int insert_tst(struct TSTnode** tree, const char* word);

int search_tst(struct TSTnode* tree, const char* word);

int delete_tst(struct TSTnode** tree, const char* word);

int prefix_tst(struct TSTnode* tree, const char* pre);

void clear_tst(struct TSTnode** tree);

/* Helper Functions */
struct TSTnode* new_tst_node(char self);
int _insert(struct TSTnode** root, const char* word, int position);
int _delete(struct TSTnode** root, const char* word);
void _clear(struct TSTnode* node);
int allocation_failed();

#endif
