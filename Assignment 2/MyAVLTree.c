#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; // key of this item
	int  value;  // value (int) of this item 
	int height; // height of the subtree rooted at this node
	struct AVLTreeNode *parent; // pointer to parent
	struct AVLTreeNode *left; // pointer to left child
	struct AVLTreeNode *right; // pointer to right child
} AVLTreeNode;

// data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v) {
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree() {
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

// my helper function to get the height of the tree
int height(AVLTreeNode *node) {
	if (node == NULL) {
		return 0;
	}
	return node->height;
}

// my helper function
AVLTreeNode *BSTBasedOnSortedArray(int key_array[], int value_array[], int start, int end) {
	if (start > end) {	// base case
		return NULL;
	}
	int mid = (start + end) / 2;	// get the mid element and make it root
	AVLTreeNode *root = newAVLTreeNode(key_array[mid], value_array[mid]);
	root->left = BSTBasedOnSortedArray(key_array, value_array, start, mid - 1);
	root->right = BSTBasedOnSortedArray(key_array, value_array, mid + 1, end);
	return root;
}

// put your time complexity analysis of CreateAVLTree() here
//Time complexity: assume there are n pairs in the tree, as we need to go through all the nodes, hence, the time complexity is O(n).
AVLTree *CreateAVLTree(const char *filename) {
// put your code here
	// if (!strcmp(filename, "stdin")) {
	  	FILE *file = fopen(filename, "r");
		assert(file != NULL);
		int key, value, count = 0, temp = 0;	// count: used to find the number of key:value pairs
		char a, b, c;
		int temp_key[1000], temp_value[1000];	// used to store key:value pairs		
		while (fscanf(file, "%c%d%c%d%c", &a, &key, &b, &value, &c) != EOF) {
			temp++;
			if (temp % 2 == 1) {
				count++;
				temp_key[count - 1] = key;
				temp_value[count - 1] = value;
			}
		}
		int i;
		int key_array[count], value_array[count];	// used to store key:value pairs
		for (i = 0; i < count; i++) {
			key_array[i] = temp_key[i];
			value_array[i] = temp_value[i];
		}
		for (i = 1; i < count; i++) {	// use insertionSort algorithm to sort key_array[], and get the corresponding array for value
			int element = key_array[i], another_element = value_array[i];
			int j = i - 1;
			while (j >= 0 && key_array[j] > element) {
				key_array[j + 1] = key_array[j];
				value_array[j + 1] = value_array[j];
				j--;
			} 
			key_array[j + 1] = element;
			value_array[j + 1] = another_element;
		}
		AVLTree *avlTree = newAVLTree();
		avlTree->size = count;
		avlTree->root = BSTBasedOnSortedArray(key_array, value_array, 0, count - 1);
		fclose(file);
		return avlTree;
	// }
}

// my helper function for cloning
AVLTreeNode *clone(AVLTreeNode *node) {
	if (node == NULL) {
		return NULL;
	}
	AVLTreeNode *cloneNode = newAVLTreeNode(node->key, node->value);
	AVLTreeNode *leftClone = clone(node->left);
	AVLTreeNode *rightClone = clone(node->right);
	cloneNode->left = leftClone;
	cloneNode->right= rightClone;
	return cloneNode;
}

// put your time complexity analysis for CloneAVLTree() here
AVLTree *CloneAVLTree(AVLTree *T) {
// put your code here
	AVLTree *cloneTree = newAVLTree();
	cloneTree->size = T->size;
	cloneTree->root = clone(T->root);
	return cloneTree;
}

// my helper function for Search
// Just binary tree search
AVLTreeNode *searchNode(AVLTreeNode *node, int k) {
	if (node == NULL) {
		return NULL;
	}
	if (node->key > k) {
		return searchNode(node->left, k);
	} else if (node->key < k) {
		return searchNode(node->right, k);
	} else {
		return node;
	}
}

// put your time complexity analysis for Search() here
// Time complexity: pure BST, hence, the time complexity is O(log n). 
AVLTreeNode *Search(AVLTree *T, int k) {
// put your code here
	return searchNode(T->root, k);
}
 
// my helper function for leftRotation with the tree rooted by "node"
AVLTreeNode *leftRotation(AVLTreeNode *node) {
	AVLTreeNode *nodeRight = node->right;
	AVLTreeNode *leftOfNodeRight = nodeRight->left;

	nodeRight->left = node;	// rotation
	node->right = leftOfNodeRight;

	// update height
	node->height = height(node->left) > height(node->right) ? height(node->left) + 1 : height(node->right) + 1;
	nodeRight->height = height(nodeRight->left) > height(nodeRight->right) ? 
								height(nodeRight->left) + 1 : height(nodeRight->right) + 1;

	return nodeRight;	// this is the new root after rotation
}

// my helper function for rightRotation with the tree rooted by "node"
AVLTreeNode *rightRotation(AVLTreeNode *node) {
	AVLTreeNode *nodeLeft = node->left;
	AVLTreeNode *rightOfNodeLeft = nodeLeft->right;

	nodeLeft->right = node;	// rotation
	node->left = rightOfNodeLeft;

	// update height
	node->height = height(node->left) > height(node->right) ? height(node->left) + 1 : height(node->right) + 1;
	nodeLeft->height = height(nodeLeft->left) > height(nodeLeft->right) ? 
								height(nodeLeft->left) + 1 : height(nodeLeft->right) + 1;

	return nodeLeft;	// this is the new root after rotation
}


// my helper function for insertNode
// Firstly, insert the new node. Then rotate the tree in case of inbalance heights.
AVLTreeNode *insert(AVLTreeNode *node, int k, int v) {
	if (node == NULL) {	// create a new node
		return newAVLTreeNode(k, v);
	}
	if (k < node->key) {	// Firstly insert the new node
		node->left = insert(node->left, k, v);
	} else {	// After Search, there would not be 2 equal keys.
		node->right = insert(node->right, k, v);
	}

	// update the height of new node's ancestor
	node->height = height(node->left) > height(node->right) ? height(node->left) + 1: height(node->right) + 1;

	// process the inbalance situation.
	if (height(node->left) - height(node->right) > 1) {
		if (k < node->left->key) {
			return rightRotation(node);
		} else {
			node->left = leftRotation(node->left);
			return rightRotation(node);
		}
	} else if (height(node->left) - height(node->right) < -1) {
		if (k > node->right->key) {
			return leftRotation(node);
		} else {
			node->right = rightRotation(node->right);
			return leftRotation(node);
		}
	}

	return node;	// no inbalance situation, just return node
}

// put the time complexity analysis for InsertNode() here
// Time complexity: As the height of AVL tree is O(log n), then the insert node process is O(log n) as well.
int InsertNode(AVLTree *T, int k, int v) {
// put your code here
	if (Search(T, k) == NULL) {
		T->root = insert(T->root, k, v);
		T->size += 1;
		return 1;
	} else {
		return 0;
	}
}

// my helper function for DeleteNode
// Firstly, delete the target node. Then rotate the tree in case of inbalance heights.
AVLTreeNode *delete(AVLTreeNode *node, int k) {
	if (node == NULL) {
		return NULL;
	}

	// delete process
	if (k > node->key) {	// delete the k in the right part
		node->right = delete(node->right, k);
	} else if (k < node->key) {		// delete the k in the left part
		node->left = delete(node->left, k);
	} else {	// find the target node to be deleted, do deletion process.
		if (node->left == NULL && node->right == NULL) {	// no child, just delete this node.
			free(node);
		} else if (node->left == NULL) {  // left child is NULL, and right child is not NULL, then replace it with its right child.
			AVLTreeNode *temp = node;
			node = node->right;
			free(temp);
		} else {	// node has 2 children, we need to replace it with the inorder successor.
			AVLTreeNode *inorderSuccessor = node;
			while (inorderSuccessor->left != NULL) {
				inorderSuccessor = inorderSuccessor->left;
			}
			node->key = inorderSuccessor->key;	// copy the inorderSuccessor key
			node->right = delete(node->right, node->key);	// delete the inorderSuccessor node
		}
	}
	if (node == NULL) {
		return NULL;
	}

	// update current height.
	node->height = height(node->left) > height(node->right) ? height(node->left) + 1: height(node->right) + 1;

	// process the inbalance situation.
	// same process in the "insert" function.
	if (height(node->left) - height(node->right) > 1) {
		if (k < node->left->key) {
			return rightRotation(node);
		} else {
			node->left = leftRotation(node->left);
			return rightRotation(node);
		}
	} else if (height(node->left) - height(node->right) < -1) {
		if (k > node->right->key) {
			return leftRotation(node);
		} else {
			node->right = rightRotation(node->right);
			return leftRotation(node);
		}
	}

	return node;	// no inbalance situation, just return node

}

// put your time complexity for DeleteNode() here
// Time complexity: As the height of AVL tree is O(log n), then the delete node process is O(log n) as well.
int DeleteNode(AVLTree *T, int k) {
// put your code here
	if (Search(T, k) != NULL) {
		T->root = delete(T->root, k);
		T->size -= 1;
		return 1;
	} else {
		return 0;
	}
}

// my helper function for free AVL Tree node
void freeTree(AVLTreeNode *node) {
	if (node != NULL) {
		free(node->left);
		free(node->right);
		free(node);
	}
}

// put your time complexity analysis for freeAVLTree() here
// Time complexity: assume there are n pairs in the tree, as we need to free all the nodes, hence, the time complexity is O(n).
void FreeAVLTree(AVLTree *T) {
// put your code here	
	if (T != NULL) {
		freeTree(T->root);
	}
}

// my helper function for print AVL Tree
// According to the requirements, we need to print in increasing order of keys, then I choose inorder traversal.
void inorderPrint(AVLTreeNode *node) {
	if (node != NULL) {
		inorderPrint(node->left);
		printf("(%d, %d)\n", node->key, node->value);
		inorderPrint(node->right);
	}
}

// put your time complexity analysis for PrintAVLTree() here
//Time complexity: assume there are n pairs in the tree, as we need to go through all the nodes, hence, the time complexity is O(n).
void PrintAVLTree(AVLTree *T) {
// put your code here
	if (T->root != NULL) {
		inorderPrint(T->root);
	}
}


int main() { 
 int i,j;
 // AVLTree *tree1, *tree2, *tree3, *tree4;
 AVLTree *tree2, *tree3, *tree4;
 AVLTreeNode *node1;
 
 // tree1=CreateAVLTree("stdin");
 // PrintAVLTree(tree1);
 // FreeAVLTree(tree1);

 //you need to create the text file file1.txt
 // to store a set of items with distinct keys

 tree2 = CreateAVLTree("file1.txt");
 PrintAVLTree(tree2);
 tree3 = CloneAVLTree(tree2);
 printf("**********************\n");
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);

 tree4 = newAVLTree();
 j = InsertNode(tree4, 10, 10);
 for (i = 0; i < 15; i++) {
   j = InsertNode(tree4, i, i);
   if (j == 0) {
   	printf("(%d, %d) already exists\n", i, i);
   }
  }
  PrintAVLTree(tree4);
  node1 = Search(tree4,20);
  if (node1 != NULL){
  	printf("key= %d value= %d\n", node1->key, node1->value);
  } else {
  	printf("Key 20 does not exist\n");
  }

  for (i = 17; i > 0; i--)
  {
    j = DeleteNode(tree4, i);
	if (j == 0) 
	  printf("Key %d does not exist\n", i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 return 0; 
}
