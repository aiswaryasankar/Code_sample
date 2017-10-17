#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tst/tst.h"
#include "trie/trie.h"

#define TOKEN_SIZE 61
#define TOKEN_DELIM " \t\r\n\a"

char** split_line(char* line, size_t* argsize) {
	int tokensize = TOKEN_SIZE, position = 0;
	char** tokens = (char**) malloc(tokensize * sizeof(char*));
	if (!tokens) {
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	char* token = strtok(line, TOKEN_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;
		if (position >= tokensize) {
			tokensize += TOKEN_SIZE;
			char** new = realloc(tokens, tokensize*sizeof(char*));
			if (!new) {
				free(tokens);
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
			tokens = new;
		}
		token = strtok(NULL, TOKEN_DELIM);
	}
	tokens[position] = NULL;
	*argsize = position - 1;
	return tokens;
}

void insert(void* tree, char** args, int argsize, bool isTrie) {
	int i = 0;
	if (argsize == 0) {
		fprintf(stderr, "Error: insert requires at least one argument string.\n");
		return;
	}
	if (isTrie) {
		for (i = 0; i < argsize; i++) {
			if (insert_trie((struct Trie*) tree, args[1+i])) {
				fprintf(stderr, "Success: inserted %s.\n", args[1+i]);
			} else {
				fprintf(stderr, "insertion failed at %s.\n", args[1+i]);
				return;
			}
		}
		return;
	}
	for (i = 0; i < argsize; i++) {

		if (insert_tst((struct TSTnode**) tree, args[1+i])) {
			fprintf(stderr, "Success: inserted %s.\n", args[1+i]);
		} else {
			fprintf(stderr, "insertion failed at %s.\n", args[1+i]);
			return;
		}
	}
}

void delete(void* tree, char** args, int argsize, bool isTrie) {
	int i = 0;
	if (argsize == 0) {
		fprintf(stderr, "Error: delete requires at least one argument string.\n");
		return;
	}
	if (isTrie) {
		for (i = 0; i < argsize; i++) {
			if (delete_trie((struct Trie*) tree, args[1+i])) {
				fprintf(stderr, "Success: deleted %s\n", args[1+i]);
			} else {
				fprintf(stderr, "Error: dictionary does not contain %s\n", args[1+i]);
				return;
			}
		}
		return;
	}
	for (i = 0; i < argsize; i++) {
		if (delete_tst((struct TSTnode**) tree, args[1+i])) {
			fprintf(stderr, "Success: deleted %s\n", args[1+i]);
		} else {
			fprintf(stderr, "Error: dictionary does not contain %s\n", args[1+i]);
			return;
		}
	}
}

void search(void* tree, char** args, int argsize, bool isTrie) {
	if (argsize == 0) {
		fprintf(stderr, "Error: search requires an input prefix.\n");
		return;
	}
	if (argsize > 1) {
		fprintf(stderr, "Warning: search only takes in the first input prefix.\n");
	}
	if (isTrie)
		search_trie((struct Trie*) tree, args[1]);
	else {
		if (search_tst(*((struct TSTnode**)tree), args[1])) 
			fprintf(stderr, "word %s exists in dictionary.\n", args[1]);
		else
			fprintf(stderr, "Failure: word %s DOES NOT EXIST in dictionary.\n", args[1]);
	}
}

void prefix(void* tree, char** args, int argsize, bool isTrie) {
	if (argsize == 0) {
		fprintf(stderr, "Error: prefix requires an input prefix.\n");
		return;
	}
	if (argsize > 1) {
		fprintf(stderr, "Warning: prefix only takes in the first input prefix.\n");
	}
	if (isTrie)
		prefixes_trie((struct Trie*) tree, args[1]);
	else {
		if (prefix_tst(*((struct TSTnode**)tree), args[1])) 
			fprintf(stderr, "prefix %s exists in dictionary.\n", args[1]);
		else
			fprintf(stderr, "Failure: prefix %s DOES NOT EXIST in dictionary.\n", args[1]);
	}
}

void clear(void* tree, bool isTrie) {
	if (isTrie)
		clear_trie((struct Trie*) tree);
	else
		clear_tst((struct TSTnode**) tree);
}

void help(bool isTrie) {
	fprintf(stderr, "Available commands are: \n");
	fprintf(stderr, "\t insert [word] \t --insert given word to dictionary.\n");
	fprintf(stderr, "\t delete [word] \t -- delete given word from dictionary.\n");
	fprintf(stderr, "\t search [word] \t -- search given word from dictionary.\n");
	if (isTrie) {
		fprintf(stderr, "\t prefix [word] \t -- print words with matching prefix.\n");
		fprintf(stderr, "\t memory        \t -- return current memory size used by dictionary.\n");
	} else {
		fprintf(stderr, "\t prefix [word] \t -- search given prefix from dictionary.\n");
	}
	fprintf(stderr, "\t clear         \t -- clear entire dictionary.\n");
	fprintf(stderr, "\t quit          \t -- quit.\n");
}

void runDictionary(bool isTrie) {
	void* tree; // tree is either Trie* or TSTnode**
	char* treetype;
	struct TSTnode* root = NULL;

	if (isTrie) {
		tree = new_trie();
		treetype = "Trie# ";
	} else {
		tree = &root;
		treetype = "TST# ";
	}
	// start loop
	while (1) {
		char* line = NULL;
		size_t linesize, argsize;
		fprintf(stderr, "%s", treetype);
		getline(&line, &linesize, stdin);
		char** args = split_line(line, &argsize);
		const char* command = args[0];
		if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0)
			break;
		if (strcmp(command, "insert") == 0 || strcmp(command, "i") == 0) {
			insert(tree, args, argsize, isTrie);
		} else if (strcmp(command, "delete") == 0 || strcmp(command, "d") == 0) {
			delete(tree, args, argsize, isTrie);
		} else if (strcmp(command, "search") == 0 || strcmp(command, "s") == 0) {
			search(tree, args, argsize, isTrie);
		} else if (strcmp(command, "prefix") == 0 || strcmp(command, "p") == 0) {
			prefix(tree, args, argsize, isTrie);
		} else if (strcmp(command, "memory") == 0 || strcmp(command, "m") == 0) {
			if (isTrie)
				fprintf(stderr, "dictionary memory usage: %d.\n", memory_trie((struct Trie*) tree));
			else 
				fprintf(stderr, "memory is not implemented for TST.\n");
		} else if (strcmp(command, "clear") == 0 || strcmp(command, "c") == 0) {
			fprintf(stderr, "clearing dictionary...\n");
			clear(tree, isTrie);
			fprintf(stderr, "done.\n");
		} else if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) {
			help(isTrie);
		} else if (strcmp(command, "sl") == 0) {
			system("echo \"hmm you miss the hype train?\nLet me call the Train Station for ya...\"");
			system("sleep 5 && ~cs61c/sl");
		} else {
			fprintf(stderr, "Command Not Recognized.\n");
			help(isTrie);
		}
	}
	clear(tree, isTrie);
}
 
int main(int argc, char* argv[]) {
	if (argc == 1) {
		system("cowsay \"Hi, Your TSTree is on the way.\"");
		runDictionary(false);
	} else if (strcmp(argv[1], "-e") == 0) {
		system("cowsay \"Hi, Your Trie Tree is on the way.\"");
		runDictionary(true);
	} else if (strcmp(argv[1], "peijie") == 0 || strcmp(argv[1], "steven") == 0 || strcmp(argv[1], "61C_staff") == 0) {
		system("cowthink \"Hmm, what do you need from us?\"");
	} else {
		runDictionary(false);
	}
	return 0;
}










