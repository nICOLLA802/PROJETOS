#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

typedef struct {
    Peca elementos[TAM_FILA];
    int inicio, fim, tamanho;
} Fila;

typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

int contador_id = 0;
char tipos[] = {'I', 'O', 'T', 'L'};

Peca gerarPeca() {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contador_id++;
    return p;
}

void inicializaFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    for(int i=0; i<TAM_FILA; i++)
        f->elementos[i] = gerarPeca();
    f->fim = TAM_FILA;
    f->tamanho = TAM_FILA;
}

void inicializaPilha(Pilha *p) {
    p->topo = -1;
}

void exibeEstado(Fila *f, Pilha *p) {
    printf("Fila de peças: ");
    for(int i=0; i<f->tamanho; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->elementos[idx].nome, f->elementos[idx].id);
    }
    printf("Pilha de reserva: (Topo -> base): ");
    for(int i=p->topo; i>=0; i--) {
        printf("[%c %d] ", p->elementos[i].nome, p->elementos[i].id);
    }
    printf("\n");
}

void jogarPeca(Fila *f) {
    if(f->tamanho > 0) {
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->tamanho--;
        // Gera nova peça, se possível
        if(f->tamanho < TAM_FILA) {
            int novoidx = (f->inicio + f->tamanho) % TAM_FILA;
            f->elementos[novoidx] = gerarPeca();
            f->fim = (novoidx + 1) % TAM_FILA;
            f->tamanho++;
        }
    } else {
        printf("Fila vazia!");
    }
}

void reservarPeca(Fila *f, Pilha *p) {
    if(f->tamanho > 0 && p->topo < TAM_PILHA - 1) {
        p->elementos[++p->topo] = f->elementos[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->tamanho--;
        // Gera nova peça, se possível
        if(f->tamanho < TAM_FILA) {
            int novoidx = (f->inicio + f->tamanho) % TAM_FILA;
            f->elementos[novoidx] = gerarPeca();
            f->fim = (novoidx + 1) % TAM_FILA;
            f->tamanho++;
        }
    } else {
        printf("Não é possível reservar.");
    }
}

void usarPecaReserva(Pilha *p) {
    if(p->topo >= 0) {
        p->topo--;
    } else {
        printf("Pilha vazia!");
    }
}

void trocarPeca(Fila *f, Pilha *p) {
    if(f->tamanho > 0 && p->topo >= 0) {
        Peca temp = f->elementos[f->inicio];
        f->elementos[f->inicio] = p->elementos[p->topo];
        p->elementos[p->topo] = temp;
    } else {
        printf("Não há peça para trocar.");
    }
}

void trocaMultipla(Fila *f, Pilha *p) {
    if(f->tamanho >= 3 && p->topo >= 2) {
        for(int i=0; i<3; i++) {
            int idx = (f->inicio + i) % TAM_FILA;
            Peca temp = f->elementos[idx];
            f->elementos[idx] = p->elementos[p->topo - i];
            p->elementos[p->topo - i] = temp;
        }
    } else {
        printf("Não é possível realizar troca múltipla.");
    }
}

int main() {
    Fila fila;
    Pilha pilha;
    inicializaFila(&fila);
    inicializaPilha(&pilha);

    int opcao;
    do {
        exibeEstado(&fila, &pilha);
        printf("Opções:
    1 - Jogar peça
    2 - Reservar peça
    3 - Usar peça reserva
    4 - Trocar peça atual
    5 - Troca múltipla
    0 - Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarPecaReserva(&pilha); break;
            case 4: trocarPeca(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
            case 0: printf("Saindo..."); break;
            default: printf("Opção inválida.");
        }
    } while(opcao != 0);

    return 0;
}