#pragma once


#include <stdbool.h>



typedef struct TreeNode {
	void *value;
	struct TreeNode **children;
} TreeNode;

typedef struct {
	TreeNode root;
} Tree;


Tree* createTree();

bool treeInsert(Tree* tree, char *key, void *value);
void treeRemove(Tree* tree, char *key);
void treeDetach(Tree* tree, char *key);

void freeNodes(TreeNode *node, bool free_value);
void clearTree(Tree *tree, bool free_value);
void removeTree(Tree *tree, bool free_values);

void* treeGet(TreeNode *node, char *key);
void* treeGetUnterminated(TreeNode *node, char *key, size_t length);

void* dictionaryLookup(Tree *tree, char *key);
void* dictionaryLookupUnterminated(Tree *tree, char *key, size_t length);