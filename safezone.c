#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// ------------------- PROTÓTIPOS -------------------
void cadastrarComponentes(Componente v[], int *n);
void exibirComponentes(Componente v[], int n);

void bubbleSortNome(Componente v[], int n, long *comparacoes);
void insertionSortTipo(Componente v[], int n, long *comparacoes);
void selectionSortPrioridade(Componente v[], int n, long *comparacoes);

int buscaBinaria(Componente v[], int n, char chave[]);

void trocar(Componente *a, Componente *b);

// --------------------------------------------------
int main() {
    Componente componentes[MAX];
    int n = 0;
    int opcao;
    long comparacoes;
    clock_t inicio, fim;
    double tempo;

    cadastrarComponentes(componentes, &n);

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Exibir componentes\n");
        printf("2 - Ordenar por nome (Bubble Sort)\n");
        printf("3 - Ordenar por tipo (Insertion Sort)\n");
        printf("4 - Ordenar por prioridade (Selection Sort)\n");
        printf("5 - Buscar componente-chave (por nome)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                exibirComponentes(componentes, n);
                break;

            case 2:
                comparacoes = 0;
                inicio = clock();
                bubbleSortNome(componentes, n, &comparacoes);
                fim = clock();
                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

                printf("\nOrdenado por NOME (Bubble Sort).\n");
                printf("Comparacoes: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                break;

            case 3:
                comparacoes = 0;
                inicio = clock();
                insertionSortTipo(componentes, n, &comparacoes);
                fim = clock();
                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

                printf("\nOrdenado por TIPO (Insertion Sort).\n");
                printf("Comparacoes: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                break;

            case 4:
                comparacoes = 0;
                inicio = clock();
                selectionSortPrioridade(componentes, n, &comparacoes);
                fim = clock();
                tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

                printf("\nOrdenado por PRIORIDADE (Selection Sort).\n");
                printf("Comparacoes: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                break;

            case 5: {
                char chave[30];
                printf("Digite o nome do componente a buscar: ");
                getchar();
                fgets(chave, 30, stdin);
                chave[strcspn(chave, "\n")] = 0;

                int pos = buscaBinaria(componentes, n, chave);

                if (pos >= 0) {
                    printf("Componente encontrado:\n");
                    printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                        componentes[pos].nome,
                        componentes[pos].tipo,
                        componentes[pos].prioridade
                    );
                } else {
                    printf("Componente NAO encontrado.\n");
                }
                break;
            }
        }

    } while (opcao != 0);

    return 0;
}

// --------------------------------------------------
// CADASTRO
// --------------------------------------------------
void cadastrarComponentes(Componente v[], int *n) {
    printf("Quantos componentes deseja cadastrar (max 20)? ");
    scanf("%d", n);

    if (*n > MAX) *n = MAX;

    for (int i = 0; i < *n; i++) {
        printf("\nComponente %d:\n", i + 1);
        printf("Nome: ");
        getchar();
        fgets(v[i].nome, 30, stdin);
        v[i].nome[strcspn(v[i].nome, "\n")] = 0;

        printf("Tipo: ");
        fgets(v[i].tipo, 20, stdin);
        v[i].tipo[strcspn(v[i].tipo, "\n")] = 0;

        printf("Prioridade (1 a 10): ");
        scanf("%d", &v[i].prioridade);
    }
}

// --------------------------------------------------
// EXIBIR
// --------------------------------------------------
void exibirComponentes(Componente v[], int n) {
    printf("\n===== LISTA DE COMPONENTES =====\n");
    for (int i = 0; i < n; i++) {
        printf("%d) Nome: %s | Tipo: %s | Prioridade: %d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

// --------------------------------------------------
// ORDENAÇÕES
// --------------------------------------------------
void trocar(Componente *a, Componente *b) {
    Componente aux = *a;
    *a = *b;
    *b = aux;
}

void bubbleSortNome(Componente v[], int n, long *comparacoes) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                trocar(&v[j], &v[j + 1]);
            }
        }
    }
}

void insertionSortTipo(Componente v[], int n, long *comparacoes) {
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0 && strcmp(v[j].tipo, chave.tipo) > 0) {
            (*comparacoes)++;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

void selectionSortPrioridade(Componente v[], int n, long *comparacoes) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (v[j].prioridade < v[menor].prioridade) {
                menor = j;
            }
        }
        trocar(&v[i], &v[menor]);
    }
}

// --------------------------------------------------
// BUSCA BINÁRIA (após bubble sort por nome)
// --------------------------------------------------
int buscaBinaria(Componente v[], int n, char chave[]) {
    int ini = 0, fim = n - 1;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;

        int cmp = strcmp(chave, v[meio].nome);

        if (cmp == 0)
            return meio;
        else if (cmp > 0)
            ini = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}