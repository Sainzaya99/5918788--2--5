#include <stdio.h>
#include <stdlib.h>

#define DATA_COUNT   100   
#define SEARCH_COUNT 50    
#define MAX_VALUE    1000   

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(int value) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node* insert_bst(Node* root, int value, int* cmp) {
    if (root == NULL) {              
        return create_node(value);
    }
    Node* cur = root;
    while (1) {
        (*cmp)++;                   
        if (value < cur->data) {
            if (cur->left == NULL) {
                cur->left = create_node(value);
                break;
            }
            cur = cur->left;
        }
        else {
            if (cur->right == NULL) {
                cur->right = create_node(value);
                break;
            }
            cur = cur->right;
        }
    }
    return root;
}

int search_bst(Node* root, int key, int* cmp) {
    Node* cur = root;
    while (cur != NULL) {
        (*cmp)++;                  
        if (key == cur->data) return 1;
        else if (key < cur->data) cur = cur->left;
        else cur = cur->right;
    }
    return 0;
}

int search_sequential(int arr[], int n, int key, int* cmp) {
    for (int i = 0; i < n; i++) {
        (*cmp)++;                   
        if (arr[i] == key) return 1;
    }
    return 0;
}

int tree_height(Node* root) {
    if (root == NULL) return 0;
    int l = tree_height(root->left);
    int r = tree_height(root->right);
    return (l > r ? l : r) + 1;
}

void free_tree(Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int already_used(int arr[], int count, int value) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == value) return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    unsigned int seed = 2026;
    if (argc >= 2) seed = (unsigned int)atoi(argv[1]);
    srand(seed);

    int arr[DATA_COUNT];
    int keys[SEARCH_COUNT];
    Node* root = NULL;
    int build_cmp = 0;

    printf("=== Assignment 05: Sequential Search vs BST Search (seed = %u) ===\n\n", seed);

    int count = 0;
    while (count < DATA_COUNT) {
        int v = rand() % (MAX_VALUE + 1);          
        if (already_used(arr, count, v)) continue; 
        arr[count] = v;
        root = insert_bst(root, v, &build_cmp);    
        count++;
    }

    printf("[1] The 100 generated integers (in generated order)\n");
    for (int i = 0; i < DATA_COUNT; i++) {
        printf("%4d", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n[2] Total comparisons while building the BST: %d\n\n", build_cmp);

    for (int i = 0; i < SEARCH_COUNT; i++) {
        keys[i] = rand() % (MAX_VALUE + 1);
    }

    printf("[3] The 50 generated search keys\n");
    for (int i = 0; i < SEARCH_COUNT; i++) {
        printf("%4d", keys[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    int seq_total = 0, bst_total = 0;
    int found_count = 0;
    int seq_found_total = 0, bst_found_total = 0; 
    int seq_fail_total = 0, bst_fail_total = 0;    
    int seq_max = 0, bst_max = 0;

    printf("[4] Search results\n");
    printf("%-4s %-8s %-10s %-12s %-12s\n", "No.", "Key", "Result", "Seq(cmp)", "BST(cmp)");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < SEARCH_COUNT; i++) {
        int seq_cmp = 0, bst_cmp = 0;
        int seq_res = search_sequential(arr, DATA_COUNT, keys[i], &seq_cmp);
        int bst_res = search_bst(root, keys[i], &bst_cmp);

        if (seq_res != bst_res) {
            printf("ERROR: the two searches disagree! key = %d\n", keys[i]);
            return 1;
        }

        printf("%-4d %-8d %-10s %-12d %-12d\n", i + 1, keys[i],
            seq_res ? "Found" : "Not found", seq_cmp, bst_cmp);

        seq_total += seq_cmp;
        bst_total += bst_cmp;
        if (seq_cmp > seq_max) seq_max = seq_cmp;
        if (bst_cmp > bst_max) bst_max = bst_cmp;
        if (seq_res) {
            found_count++;
            seq_found_total += seq_cmp;
            bst_found_total += bst_cmp;
        }
        else {
            seq_fail_total += seq_cmp;
            bst_fail_total += bst_cmp;
        }
    }

    printf("\n[5] Summary\n");
    printf("Number of searches : %d (found %d, not found %d)\n\n",
        SEARCH_COUNT, found_count, SEARCH_COUNT - found_count);

    printf("Sequential Search\n");
    printf("  Total comparisons   : %d\n", seq_total);
    printf("  Average comparisons : %.2f\n", (double)seq_total / SEARCH_COUNT);
    printf("  Max comparisons     : %d\n\n", seq_max);

    printf("BST Search\n");
    printf("  Total comparisons   : %d\n", bst_total);
    printf("  Average comparisons : %.2f\n", (double)bst_total / SEARCH_COUNT);
    printf("  Max comparisons     : %d\n\n", bst_max);

    printf("[6] Extra statistics\n");
    if (found_count > 0)
        printf("Average comparisons (successful searches) : Seq %.2f / BST %.2f\n",
            (double)seq_found_total / found_count,
            (double)bst_found_total / found_count);
    if (found_count < SEARCH_COUNT)
        printf("Average comparisons (failed searches)     : Seq %.2f / BST %.2f\n",
            (double)seq_fail_total / (SEARCH_COUNT - found_count),
            (double)bst_fail_total / (SEARCH_COUNT - found_count));
    printf("BST height (counted in nodes)             : %d\n\n", tree_height(root));

    free_tree(root);
    return 0;
}