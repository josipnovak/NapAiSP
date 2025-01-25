#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define d 26

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

int rabin_karp(char *uzorak, char *tekst) {
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
	while (i < n) {
		if (uzorak[j] == tekst[i]) {
			if (j == m - 1) {
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
	return -1;
}



int main() {
	char* uzorak = "abc";
	char* tekst = "aaabc";
	int* lps = kreiranje_lps(uzorak);
	printf("%d", knuth_morris_pratt(uzorak, tekst));
	return 0;
}