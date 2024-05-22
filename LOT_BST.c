#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct treeNode {
    struct treeNode *leftPtr;
    int data;
    struct treeNode *rightPtr;
};

typedef struct treeNode TreeNode;
typedef TreeNode *TreeNodePtr;

void insertNode(TreeNodePtr *treePtr, int value);
void inOrder(TreeNodePtr treePtr);
void preOrder(TreeNodePtr treePtr);
void postOrder(TreeNodePtr treePtr);
void levelOrder(TreeNodePtr treePtr);

int main(void) {
    TreeNodePtr rootPtr = NULL;
    int choice, item;
    srand(time(NULL));
    printf("Program Pohon Sederhana\n");

    do {
        printf("\nMenu:\n");
        printf("1. Tambahkan Node\n");
        printf("2. Urutan InOrder\n");
        printf("3. Urutan PreOrder\n");
        printf("4. Urutan PostOrder\n");
        printf("5. Urutan LevelOrder\n");
        printf("6. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Masukkan nilai: ");
                scanf("%d", &item);
                insertNode(&rootPtr, item);
                break;
            case 2:
                printf("\nUrutan InOrder: ");
                inOrder(rootPtr);
                printf("\n");
                break;
            case 3:
                printf("\nUrutan PreOrder: ");
                preOrder(rootPtr);
                printf("\n");
                break;
            case 4:
                printf("\nUrutan PostOrder: ");
                postOrder(rootPtr);
                printf("\n");
                break;
            case 5:
                printf("\nUrutan LevelOrder: ");
                levelOrder(rootPtr);
                printf("\n");
                break;
            case 6:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
                break;
        }
    } while (choice != 6);
    return 0;
}

void insertNode(TreeNodePtr *treePtr, int value) {
    if (*treePtr == NULL) {
        *treePtr = (TreeNodePtr)malloc(sizeof(TreeNode));
        if (*treePtr != NULL) {
            (*treePtr)->data = value;
            (*treePtr)->leftPtr = NULL;
            (*treePtr)->rightPtr = NULL;
        } else {
            printf("Memori tidak cukup.\n");
        }
    } else {
        if (value < (*treePtr)->data) {
            insertNode(&((*treePtr)->leftPtr), value);
        } else if (value > (*treePtr)->data) {
            insertNode(&((*treePtr)->rightPtr), value);
        } else {
            printf("Data sudah ada di dalam pohon.\n");
        }
    }
}

void inOrder(TreeNodePtr treePtr) {
    if (treePtr != NULL) {
        inOrder(treePtr->leftPtr);
        printf("%d ", treePtr->data);
        inOrder(treePtr->rightPtr);
    }
}

void preOrder(TreeNodePtr treePtr) {
    if (treePtr != NULL) {
        printf("%d ", treePtr->data);
        preOrder(treePtr->leftPtr);
        preOrder(treePtr->rightPtr);
    }
}

void postOrder(TreeNodePtr treePtr) {
    if (treePtr != NULL) {
        postOrder(treePtr->leftPtr);
        postOrder(treePtr->rightPtr);
        printf("%d ", treePtr->data);
    }
}

void levelOrder(TreeNodePtr treePtr) {
    if (treePtr == NULL) {
        return;
    }
    TreeNodePtr queue[100];
    int front = 0, rear = 0;
    queue[rear++] = treePtr;

    while (front < rear) {
        TreeNodePtr current = queue[front++];
        printf("%d ", current->data);
        if (current->leftPtr != NULL) {
            queue[rear++] = current->leftPtr;
        }
        if (current->rightPtr != NULL) {
            queue[rear++] = current->rightPtr;
        }
    }
}
