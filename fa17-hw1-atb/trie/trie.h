#define TRIE_NODE_SIZE 108
#define TRIE_SIZE 4
#define NUM_LETTERS 26

struct TrieNode {
	struct TrieNode* children[NUM_LETTERS];
	int wordEnds; // 1 if word ends at this node, 0 otherwise
};

struct Trie {
	struct TrieNode* root;
};

/* Public Trie Interface */

int insert_trie(struct Trie* t, char* s);

int search_trie(struct Trie* t, char* s);

int delete_trie(struct Trie* t, char* s);

void prefixes_trie(struct Trie* t, char* prefix);

unsigned int memory_trie(struct Trie* t);

void clear_trie(struct Trie* t);

struct Trie* new_trie();

/* Helper Functions */
void clear_trie_node(struct TrieNode* node);
unsigned int memory_trie_node(struct TrieNode* node);
int delete_trie_node(struct TrieNode* node, char* s);
int insert_trie_node(struct TrieNode* node, char* s);
void all_words_trie_node(struct TrieNode* node, char* wordSoFar);
void prefixes_trie_node(struct TrieNode* node, char* prefix);
struct TrieNode* search_trie_node(struct TrieNode* node, char* s);
struct TrieNode* new_trie_node();
int charToIndex(char c);
int indexToChar(int i);
void strcharcpy(char* dst, char* src, char c);
