#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#define MAX 4

struct element {
	int x;
	int height;
	struct element* next[MAX];
};

struct list {
	int height;
	struct element* header;
};

struct list* inicijalizacija() {
	struct list* head = (struct list*)malloc(sizeof(struct list));
	if (head == NULL)
		return NULL;
	head->height = MAX;
	head->header = (struct element*)malloc(sizeof(struct element));
	if (head->header == NULL)
		return NULL;
	for (int i = 0; i < MAX; i++) {
		head->header->next[i] = NULL;
	}
	head->header->x = -1;
	return head;
}

int random() {
	int result = 1;
	while (result < MAX && rand()>(RAND_MAX / 2))
		++result;
	return result;
}

void ubaci_novi(struct list* head, int x) {
	struct element* prev[MAX];
	struct element* curr = head->header;
	int level = head->height - 1;
	while (curr != NULL && level >= 0) {
		prev[level] = curr;
		if (curr->next[level] == NULL) {
			level--;
		}
		else {
			if (curr->next[level]->x > x)
				level--;
			else if (curr->next[level]->x == x)
				return 0; 
			else
				curr = curr->next[level];
		}
	}
	struct element* novi_element = (struct element*)malloc(sizeof(struct element));
	if (novi_element == NULL)
		return NULL;
	int h = random();
	novi_element->height = h;
	novi_element->x = x;
	for (int i = MAX - 1; i > novi_element->height; i--)
		novi_element->next[i] == NULL;
	for (int i = novi_element->height - 1; i >= 0; i--) {
		novi_element->next[i] = prev[i]->next[i];
		prev[i]->next[i] = novi_element;
	}
}

void trazi(struct list* head, int x) {
	struct element* current = head->header;
	for (int i = head->height - 1; i >= 0;i--) {
		while (current->next[i] != NULL && current->next[i]->x < x)
			current = current->next[i];
		if (current->next[i] != NULL && current->next[i]->x == x) {
			printf("Element %d je pronadjen na levelu %d\n", x, i + 1);
			break;
		}
	}
}

void obrisi(struct list* head, int x) {
	struct element* prev[MAX];
	struct element* current = head->header;
	int flag = 0;
	for (int i = 0; i < MAX;i++)
		prev[i] = NULL;
	for (int i = head->height - 1; i >= 0;i--) {
		while (current->next[i] != NULL && current->next[i]->x < x)
			current = current->next[i];
		if (current->next[i] != NULL) {
			if (current->next[i]->x == x) {
				flag = 1;
				prev[i] = current;
			}
			else
				prev[i] = NULL;
		}
	}
	if (flag == 1) {
		struct element* tmp = NULL;
		for (int i = 0; i < MAX;i++) {
			if (prev[i] != NULL) {
				prev[i]->next[i] = prev[i]->next[i]->next[i];
			}
		}
		printf("Broj je obrisan\n");
	}
}

void print_skip_list(struct list* head) {
	for (int i = MAX - 1; i >= 0; i--) {
		struct element* curr = head->header->next[i];
		printf("Level %d \t ", i + 1);
		while (curr != NULL) {
			printf("%d ", curr->x);
			curr = curr->next[i];
		}
		printf("\n");
	}
	printf("\n");
}

struct list* spoji_liste(struct list* lista1, struct list* lista2) {
	for (int i = 0; i < MAX; i++) {
		struct element* second_first = lista2->header->next[i];
		if (second_first == NULL)
			continue;
		struct element* first_last = lista1->header->next[i];
		if (first_last == NULL) {
			lista1->header->next[i] = second_first;
			continue;
		}
		while (first_last->next[i] != NULL)
			first_last = first_last->next[i];
		first_last->next[i] = second_first;
	}
	return lista1;
}

struct list* spoji_novo(struct list* lista1, struct list* lista2) {
	for (int i = 0; i < MAX; i++) {
		struct element* curr_1 = lista1->header;
		struct element* curr_2 = lista2->header->next[i];
		while (curr_2 != NULL) {
			while (curr_1->next[i] != NULL && curr_1->next[i]->x < curr_2->x)
				curr_1 = curr_1->next[i];
			struct element* tmp = curr_2->next[i];
			curr_2->next[i] = curr_1->next[i];
			curr_1->next[i] = curr_2;
			curr_2 = tmp;
		}
	}
	return lista1;
}

int main() {
	srand(time(NULL));
	struct list* lista1 = inicijalizacija();
	ubaci_novi(lista1, 5);
	ubaci_novi(lista1, 6);
	ubaci_novi(lista1, 1);
	ubaci_novi(lista1, 8);
	ubaci_novi(lista1, 10);
	ubaci_novi(lista1, 11);
	ubaci_novi(lista1, 2);
	ubaci_novi(lista1, 3);

	struct list* lista2 = inicijalizacija();
	ubaci_novi(lista2, 50);
	ubaci_novi(lista2, -60);
	ubaci_novi(lista2, -12);
	ubaci_novi(lista2, 80);
	ubaci_novi(lista2, -100);
	ubaci_novi(lista2, 110);
	ubaci_novi(lista2, -20);
	ubaci_novi(lista2, 30);
	
	print_skip_list(lista1);
	print_skip_list(lista2);

	lista1 = spoji_novo(lista1, lista2);
	print_skip_list(lista1);
	return 0;
}