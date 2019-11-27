#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it) {
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList() {
	struct DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// Assume there are n integers in filename.txt, as I make a single while-loop, then the time complexity is O(n).
DLList *CreateDLListFromFileDlist(const char *filename) {	
 // put your code here
	if (!strcmp(filename, "stdin")) {
		int count = 0;		// count: used to find the first node
		int data;
		DLList *dllist = newDLList();
		while (scanf("%d", &data) == 1) {
			count++;
			DLListNode *node = newDLListNode(data);
			if (count == 1) {
				dllist->first = node;
				dllist->last = node;
				dllist->size++;
			} else {
				dllist->last->next = node;
				node->prev = dllist->last;
				dllist->last = node;
				dllist->size++;
			}
		}
		return dllist;
	} else {
		FILE *file = fopen(filename, "r");
		assert(file != NULL);
		int count = 0;		// count: used to find the first node
		int data;
		DLList *dllist = newDLList();
		while (fscanf(file, "%d", &data) != EOF) {
			count++;
			DLListNode *node = newDLListNode(data);
			if (count == 1) {
				dllist->first = node;
				dllist->last = node;
				dllist->size++;
			} else {
				dllist->last->next = node;
				node->prev = dllist->last;
				dllist->last = node;
				dllist->size++;
			}
		}
		fclose(file);
		return dllist;
	}
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// Assume u->size = n, as I make a single for-loop, then the time complexity is O(n).
DLList *cloneList(struct DLList *u) {
 // put your code here
	if (u == NULL) {
		return NULL;
	}
	DLList *dllist = newDLList();
	int i, count = 0, len = u->size;	// count: used to find the first node
	DLListNode *node = u->first;
	for (i = 0; i < len; i++) {
		count++;
		if (count == 1) {
			DLListNode *temp = newDLListNode(node->value);	// create a clone node named as temp
			dllist->first = temp;
			dllist->last = temp;
			// dllist->size++;
		} else {
			DLListNode *temp = newDLListNode(node->value);
			dllist->last->next = temp;
			temp->prev = dllist->last;
			dllist->last = temp;
			// dllist->size++;
		}
		node = node->next;
	}
	dllist->size = u->size;
	return dllist;
}

// compute the union of two DLLists u and v
// put your time complexity analysis for intersection() here
// Assume u->size = m and v->size = n, as I made a nested for-for loop, then the time complexity is O(m*n).
DLList *setUnion(struct DLList *u, struct DLList *v) {
 // put your code here
	DLList *U = cloneList(u);	// need to make a clone, otherwise u and v would change 
	DLList *V = cloneList(v);
	if (U == NULL) {	// anyone of u and v is NULL, then return the another one
		return V;
	}
	if (V == NULL) {
		return U;
	}
	DLListNode *UNode, *VNode;
	for (VNode = V->first; VNode != NULL; VNode = VNode->next) {
		int num = VNode->value;
		int flag = 0;  // flag = 0 indicates there is no common value, flag = 1 indicates there is a common value 
		for (UNode = U->first; UNode != NULL; UNode = UNode->next) {
			if (num == UNode->value) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			DLListNode *temp = newDLListNode(VNode->value);
			U->last->next = temp;
			temp->prev = U->last;
			U->last = temp;
			U->size++;
		}
	}
	return U;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// Assume u->size = m and v->size = n, as I made a nested for-for loop, then the time complexity is O(m*n).
DLList *setIntersection(struct DLList *u, struct DLList *v) {
 // put your code here
	DLList *U = cloneList(u);	// need to make a clone, otherwise u and v would change 
	DLList *V = cloneList(v);
	if (U == NULL || V == NULL) {	// anyone of u and v is NULL, then return NULL
		return NULL;
	}
	DLList *dllist = newDLList();	// make a new double linked list to store common values
	DLListNode *UNode, *VNode;
	int count = 0;	// count: used to find the first common node
	for (VNode = V->first; VNode != NULL; VNode = VNode->next) {
		int num = VNode->value;
		int flag = 0;	// flag = 0 indicates there is no common value, flag = 1 inficates there is a common value 
		for (UNode = U->first; UNode != NULL; UNode = UNode->next) {
			if (num == UNode->value) {
				flag = 1;
				count++;
				break;
			}
		}
		if (flag == 1) {
			if (count == 1) {
				DLListNode *temp = newDLListNode(num);	
				dllist->first = temp;
				dllist->last = temp;
				dllist->size++;
			} else {
				DLListNode *temp = newDLListNode(num);
				dllist->last->next = temp;
				temp->prev = dllist->last;
				dllist->last = temp;
				dllist->size++;
			}
		}
	}
	return dllist;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// Assume L->size = n, as we need to free all the nodes, then the time complexity is O(n).
void freeDLList(struct DLList *L) {
 // put your code here
	if (L != NULL) {
		DLListNode *p = L->first;
		while (p != NULL) {
			DLListNode *temp = p->next;
			free(p);
			p = temp;
			L->first = temp;
			// L->size--;
		}
		L->size = 0;
	}
}

// display items of a DLList
// put your time complexity analysis for printDDList() here
// Assume u->size = n, as we need to print all the nodes, then the time complexity is O(n).
void printDLList(struct DLList *u) {
 // put your code here
	if (u != NULL) {
		DLListNode *n = u->first;
		int i = 0, len = u->size;
		for (i = 0; i < len; i++) {
			printf("%d\n", n->value);
			n = n->next;
		}
	}
}


int main(void) {
 DLList *list1, *list2, *list3, *list4;

 list1 = CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2 = CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);

 list3 = setUnion(list1, list2);
 printDLList(list3);

 list4 = setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1 = CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2 = CreateDLListFromFileDlist("stdin");

 list3 = cloneList(list1);
 printDLList(list3);
 list4 = cloneList(list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}
