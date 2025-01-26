#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define d 26
#define ALPHABET_SIZE 26

void naivni_alg(char* uzorak, char* tekst, int m, int n) {
	for (int i = 0; i <= n - m; i++) {
		int j;
		for (j = 0;j < m;j++) {
			if (tekst[i + j] != uzorak[j])
				break;
		}
		if (j == m) {
			printf("Uzorak pronaden na poziciji %d\n", i);
			return;
		}
	}
	printf("Uzorak nije pronaden\n");
}

int rabin_karp(char* uzorak, char* tekst) {
	int m = strlen(uzorak);
	int n = strlen(tekst);
	int i, j;
	int p = 0, t = 0;
	for (i = 0; i < m; i++) {
		p = p + (uzorak[i] * pow(d, m - (i + 1)));
		t = t + (tekst[i] * pow(d, m - (i + 1)));
	}
	for (i = 0; i <= n - m; i++) {
		if (p == t) {
			for (j = 0; j < m; j++) {
				if (tekst[i + j] != uzorak[j])
					break;
			}
			if (j == m)
				printf("Uzorak pronadjen: %d \n", i + 1);
		}
		if (i < n - m) {
			t = (t - tekst[i] * pow(d, m - 1)) * d + (tekst[i + m] * pow(d, 0));
		}
	}
	return -1;
}

int* kreiranje_lps(char* uzorak) {
	int* lps = (int*)malloc(50 * sizeof(int));
	lps[0] = 0;
	int len = 0;
	int i = 1;
	while (i < strlen(uzorak)) {
		if (*(uzorak + i) == *(uzorak + len)) {
			len++;
			lps[i] = len;
			i++;
		}
		else {
			if (len == 0) {
				lps[i] = 0;
				i++;
			}
			else {
				len = lps[len - 1];
			}
		}
	}
	return lps;
}

int knuth_morris_pratt(char* uzorak, char* tekst) {
	int* lps = kreiranje_lps(uzorak);
	int m = strlen(uzorak);
	int n = strlen(tekst);
	int i = 0, j = 0;
	int usporedbe = 0;
	while (i < n) {
		usporedbe++;
		if (uzorak[j] == tekst[i]) {
			if (j == m - 1) {
				printf("Number of comparisons: %d\n", usporedbe);
				return i - (m - 1);
			}
			i++; j++;
		}
		else if (j > 0) {
			j = lps[j - 1];
		}
		else
			i++;
	}
	printf("Number of comparisons: %d\n", usporedbe);
	return -1;
}

struct TrieNode {
	char value;
	struct TrieNode* children[ALPHABET_SIZE];
	int end;
};
typedef struct TrieNode TrieNode;

TrieNode* insert(TrieNode* root, char* word) {
	TrieNode* temp = root;
	int i = 0;
	for (i = 0;word[i] != '\0';i++) {
		int index = word[i] - 'A';
		if (temp->children[index] == NULL) {
			temp->children[index] = (TrieNode*)malloc(sizeof(TrieNode));
			temp->children[index]->value = word[i];
			for (int j = 0;j < ALPHABET_SIZE;j++) {
				temp->children[index]->children[j] = NULL;
			}
			temp->children[index]->end = 0;
		}
		temp = temp->children[index];
	}
	temp->end = 1;
	return root;
}

void print_trie(TrieNode* root) {
	if (root == NULL)
		return;

	TrieNode* queue[1000];
	int front = 0, rear = 0;

	queue[rear++] = root;

	while (front < rear) {
		int levelSize = rear - front;
		while (levelSize > 0) {
			TrieNode* node = queue[front++];
			if (node->value != ' ')
				if (node->end == 1)
					printf("%c ", node->value);
				else
					printf("%c ", node->value + 32);

			for (int i = 0; i < ALPHABET_SIZE; i++) {
				if (node->children[i] != NULL) {
					queue[rear++] = node->children[i];
				}
			}
			levelSize--;
		}
		printf("\n");
	}
}

int	broji_pocetak_prefiksom(TrieNode* root, char* prefiks) {
	TrieNode* temp = root;
	int i = 0;
	for (i = 0; prefiks[i] != '\0';i++) {
		if (temp->children[prefiks[i] - 'A'] != NULL)
			temp = temp->children[prefiks[i] - 'A'];
		else
			return 0;
	}
	return count_words(temp);
}

int count_words(TrieNode* root) {
	if (root == NULL)
		return 0;
	int count = 0;
	if (root->end)
		count = 1;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (root->children[i] != NULL) {
			count += count_words(root->children[i]);
		}
	}
	return count;
}

int main() {
	char* uzorak = "abcabcabcabc";
	char* tekst = "abcdabceabcdabce";
	int* lps = kreiranje_lps(uzorak);
	printf("%d", knuth_morris_pratt(uzorak, tekst));
	/*TrieNode* root = (TrieNode*)malloc(sizeof(TrieNode));
	root->value = ' ';
	for (int i = 0;i < ALPHABET_SIZE;i++) {
		root->children[i] = NULL;
	}
	root->end = 0;
	root = insert(root, "ARGON");
	root = insert(root, "ARGETA");
	root = insert(root, "ARO");
	root = insert(root, "AVION");
	root = insert(root, "LUK");
	root = insert(root, "LUKOVI");
	root = insert(root, "LUKOBRAN");
	printf("%d\n", broji_pocetak_prefiksom(root, "AR"));*/
	return 0;
}