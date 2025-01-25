#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100
double A = 0.618033;
int collisions = 0;

struct hash_table {
	int value;
	int key;
};
typedef struct hash_table HashTable;

int division(int key) {
	return key % MAX;
}

int broj_znamenki(int num) {
	int count = 0;
	while (num != 0) {
		num /= 10;
		count++;
	}
	return count;
}

int midsquare(int key) {
	int n = broj_znamenki(key);
	int max = broj_znamenki(MAX - 1);
	int start_index = (n / 2) - (max / 2);
	if (start_index < 0)
		start_index = 0;
	int result = 0;
	for (int i = 0; i < max;i++) {
		int digit = (key / (int)pow(10, n - start_index - 1)) % 10;
		result = result * 10 + digit;
		start_index++;
	}
	return result;
}

int fold_shift(int key) {
	if (key / 10 == 0) return key;
	if (key == 0) return 0;
	int bz = broj_znamenki(key);
	int tmp = key / (int)pow(10, bz - broj_znamenki(MAX - 1));
	return tmp + fold_shift(key % (int)pow(10, bz - broj_znamenki(MAX - 1)));
}

int fold_boundry(char* key) {
	int bz = strlen(key);
	if (bz < 3) return -1;
	int dio = bz / 3;
	int ostatak = bz % 3;
	int s1 = 0;
	int e1 = dio + (ostatak > 0 ? 1 : 0);
	int s2 = e1;
	int e2 = s2 + dio + (ostatak > 1 ? 1 : 0);
	int s3 = e2;
	int e3 = bz;
	int lijevo = 0, sredina = 0, desno = 0;
	for (int i = s1; i < e1; i++) {
		if (lijevo == 0) {
			lijevo = key[i] - '0';
		}
		else {
			lijevo *= 10;
			lijevo += key[i] - '0';
		}
	}
	for (int i = s2; i < e2; i++) {
		if (sredina == 0) {
			sredina = key[i] - '0';
		}
		else {
			sredina *= 10;
			sredina += key[i] - '0';
		}
	}
	for (int i = s3; i < e3; i++) {
		if (desno == 0) {
			desno = key[i] - '0';
		}
		else {
			desno *= 10;
			desno += key[i] - '0';
		}
	}
	return lijevo + flip(sredina) + desno;
}

int flip(int key) {
	int result = 0;
	while (key != 0) {
		int digit = key % 10;
		result = result * 10 + digit;
		key /= 10;
	}
	return result;
}

int multiplication(int key) {
	double kA = key * A;
	double kA_ostatak = kA - (int)kA;
	return (int)(MAX * kA_ostatak);
}

void linear_probing(HashTable* table, int value) {
	int key = division(value);
	int tmp = key;
	if (table[key].value == 0) {
		table[key].value = value;
		table[key].key = key;
	}
	else {
		while (table[tmp].value != 0) {
			collisions++;
			if (table[tmp].value == value) {
				return;
			}
			tmp = (tmp + 1) % MAX; 
			if (tmp == key) { 
				return;
			}
		}
		table[tmp].value = value;
		table[tmp].key = tmp;
	}
}

void quadratic_probing(HashTable* table, int value) {
	int key = division(value);
	int tmp = key;
	int i = 1;
	if (table[key].value == 0) {
		table[key].value = value;
		table[key].key = key;
	}
	else {
		while (table[tmp].value != 0) {
			collisions++;
			if (table[tmp].value == value) {
				return;
			}
			tmp = (tmp + i * i) % MAX;
			i++;
			if (tmp == key) {
				return;
			}
		}
		table[tmp].value = value;
		table[tmp].key = tmp;
	}
}

int main() {
	HashTable table[MAX];
	for (int i = 0; i < MAX; i++) {
		table[i].value = 0;
		table[i].key = 0;
	}
	quadratic_probing(table, 123);
	quadratic_probing(table, 223);
	quadratic_probing(table, 323);
	quadratic_probing(table, 423);
	for (int i = 0; i < MAX; i++) {
		printf("[%d] %d\n", table[i].key, table[i].value);
	}
	printf("Broj kolizija: %d\n", collisions);
	return 0;
}	