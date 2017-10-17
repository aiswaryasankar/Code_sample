#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int main(int argc, char const *argv[])
{
	fprintf(stderr, "--------BULK TRIE TESTS---------\n");

	/* code */
	struct Trie* t = new_trie();

	fprintf(stderr, "Testing Bulk Trie Insertion . . .\n");
	FILE* words_file = fopen("./words.txt", "r");
	char word[256];
	int numWords = 0;
	while (fscanf(words_file, "%s\n", word) != EOF) {
		if (insert_trie(t, word) == 0) {
			fprintf(stderr, "FAILED: Insertion failed for word %d: %s\n", numWords, word);
			return 0;
		}
		else {
			if (numWords % 1000 == 0)
				fprintf(stderr, "PASSED: Inserted word %d: %s\n", numWords, word);
		}
		numWords++;	
	}
	fprintf(stderr, "Test OK\n");

	fprintf(stderr, "Testing Bulk Trie Search . . .\n");
	words_file = fopen("./words.txt", "r");
	numWords = 0;
	while (fscanf(words_file, "%s\n", word) != EOF) {
		if (search_trie(t, word) == 0) {
			printf("FAILED: Could not find word %d: %s\n", numWords, word);
			return 0;
		}
		else {
			if (numWords % 1000 == 0)
				printf("PASSED: Found word %d: %s\n", numWords, word);
		}
		numWords++;	
	}
	fprintf(stderr, "Test OK\n");

	clear_trie(t);
	fprintf(stderr, "%s\n", "Cleared the trie. No memory leaks!");

	return 0;
}