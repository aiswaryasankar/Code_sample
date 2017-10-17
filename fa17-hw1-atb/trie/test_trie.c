#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int main(int argc, char const *argv[])
{
    /*    struct Trie* t = new_trie();

    char* words[1] = {"Hi"};
    for (int i = 0; i < 1; i++){
	insert_trie(t, words[i]);
    }
    printf("Total memory of trie: %d\n", memory_trie(t));

    for (int i = 0; i < 1; i++)
	delete_trie(t, words[i]);

    printf("Total memory of trie: %d\n", memory_trie(t));
    
    */
	struct Trie* t = new_trie();

	char* words[10] = {"at", "ate", "ask", "asking", "panda", "pants",
						"pant", "defend", "defund", "debrief"};

	int i;
	for (i = 0; i < 10; i++) {
		if (insert_trie(t, words[i]) == 0) 
			printf("Insertion failed!\n");
		else
			printf("Inserted %s\n", words[i]);
	}

	for (i = 0; i < 10; i++)
		printf("%s exists? %d\n", words[i], search_trie(t, words[i]));

	printf("Total nodes in trie: %d\n", memory_trie(t));

	//	char * dst = "Hello";
	//char * src = "";
	//strcpy(dst, src);
	
	prefixes_trie(t, "de");
	prefixes_trie(t, "at");
	prefixes_trie(t, "pan");
	prefixes_trie(t, "z");
	prefixes_trie(t, "panther");

	for (i = 0; i < 5; i++)
		delete_trie(t, words[i]);

	for (i = 0; i < 10; i++)
		printf("%s exists? %d\n", words[i], search_trie(t, words[i]));

	printf("Total nodes in trie: %d\n", memory_trie(t));

	clear_trie(t);
	printf("%s\n", "Cleared the trie.");
	  
	return 0;
}
