#define _CRT_SECURE_NO_WARNINGS
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

struct hash_table_string {
	char* value;
	int key;
};
typedef struct hash_table_string HashTableString;

int string_to_int(char* key) {
	int result = 0;
	for (int i = 0; i < strlen(key);i++) {
		result += key[i];
	}
	return result;
}

char* int_to_string(int key) {
	static char str[20];
	sprintf(str, "%d", key);
	return str;
}

int division(int key) {
	return key % MAX;
}

int division_string(char* key) {
	int key_int = string_to_int(key);
	return key_int % MAX;
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

int midsquare_string(char* key_string) {
	int key = string_to_int(key_string);
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

int fold_shift_string(char* key) {
	int key_int = string_to_int(key);
	printf("%d\n", key_int);
	if (key_int / 10 == 0) return key_int;
	if (key_int == 0) return 0;
	int bz = broj_znamenki(key_int);
	int tmp = key_int / (int)pow(10, bz - broj_znamenki(MAX - 1));
	return tmp + fold_shift_string(int_to_string(key_int % (int)pow(10, bz - broj_znamenki(MAX - 1))));
}

int fold_boundry(int key) {
	int bz = broj_znamenki(key);
	int dio = bz / 3;
	int ostatak = bz % 3;
	int prvi_dio = dio + (ostatak > 0 ? 1 : 0);
	int drugi_dio = dio + (ostatak > 1 ? 1 : 0);
	int treci_dio = dio;
	int lijevo = 0, sredina = 0, desno = 0;
	lijevo = key / (int)pow(10, bz - prvi_dio);
	desno = key % (int)pow(10, treci_dio);
	sredina = (key / (int)pow(10, treci_dio)) % (int)pow(10, drugi_dio);
	printf("%d %d %d\n", lijevo, sredina, desno);
	return (lijevo + flip(sredina) + desno)%MAX;
}

int fold_boundry_string(char* key) {
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
	return (lijevo + flip(sredina) + desno) % MAX;
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

int multiplication_string(char* key) {
	int result = 0;
	for (int i = 0; i < strlen(key); i++) {
		result += key[i];
	}
	double kA = result * A;
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

void linear_probing_string(HashTableString* table, char* value) {
	int key = division_string(value);
	printf("%s: %d\n", value, key);
	int tmp = key;
	if (table[key].value == NULL) {
		table[key].value = value;
		table[key].key = key;
	}
	else {
		while (table[tmp].value != NULL) {
			collisions++;
			if (strcmp(table[tmp].value, value) == 0) {
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

void quadratic_probing_string(HashTableString* table, char* value) {
	int key = division_string(value);
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
	HashTableString table[MAX];
	for (int i = 0; i < MAX; i++) {
		table[i].value = 0;
		table[i].key = 0;
	}
	quadratic_probing_string(table, "Pero");
	quadratic_probing_string(table, "Marko");
	quadratic_probing_string(table, "Josip");
	quadratic_probing_string(table, "Macak");
	quadratic_probing_string(table, "Pas");
	quadratic_probing_string(table, "Macka");
	quadratic_probing_string(table, "Magistrala");
	quadratic_probing_string(table, "Karlobag");
	quadratic_probing_string(table, "Auto");
	quadratic_probing_string(table, "Pernica");
	
	for (int i = 0; i < MAX; i++) {
		printf("[%d] %s\n", table[i].key, table[i].value);
	}
	printf("Broj kolizija: %d\n", collisions);
	return 0;
}	