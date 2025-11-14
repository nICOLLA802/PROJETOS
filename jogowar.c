#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------------------
// Estruturas
// -------------------------------
typedef struct {
    char nome[30];
    int dono;        // ID do jogador
    int tropas;
} Territorio;

// -------------------------------
// Protótipos
// -------------------------------
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanhoMapa);
void exibirMissao(char* missao);
void liberarMemoria(char* missao);
void exibirMapa(Territorio* mapa, int tamanho);

// -------------------------------
// Função principal
// -------------------------------
int main() {
    srand(time(NULL));

    // Vetor de missões
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas inimigas",
        "Controlar todos os territorios azuis",
        "Manter 5 territorios por 3 turnos",
        "Conquistar 2 territorios da cor vermelha"
    };
    int totalMissoes = 5;

    // Criar missão do jogador via malloc
    char* missaoJogador = (char*) malloc(200 * sizeof(char));

    atribuirMissao(missaoJogador, missoes, totalMissoes);

    printf("\nMissao atribuida ao jogador:\n");
    exibirMissao(missaoJogador);

    // Criar mapa simples:
    Territorio mapa[5] = {
        {"A", 1, 3},
        {"B", 2, 4},
        {"C", 1, 6},
        {"D", 1, 2},
        {"E", 2, 5}
    };

    printf("\n--- MAPA INICIAL ---\n");
    exibirMapa(mapa, 5);

    // Verificar missão
    int resultado = verificarMissao(missaoJogador, mapa, 5);

    if (resultado)
        printf("\n*** Jogador cumpriu sua missao! ***\n");
    else
        printf("\nMissao ainda nao cumprida.\n");

    liberarMemoria(missaoJogador);

    return 0;
}

// -------------------------------
// Implementações
// -------------------------------

// Sorteia missão e copia para o destino
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibir missão
void exibirMissao(char* missao) {
    printf("%s\n", missao);
}

// Lógica simples de verificação da missão
int verificarMissao(char* missao, Territorio* mapa, int tamanhoMapa) {

    // Exemplo SIMPLES de verificação:
    // “Conquistar 3 territorios seguidos” = jogador ter 3 territórios seguidos
    if (strstr(missao, "Conquistar 3 territorios seguidos") != NULL) {
        int contador = 0;

        for (int i = 0; i < tamanhoMapa; i++) {
            if (mapa[i].dono == 1)
                contador++;
            else
                contador = 0;

            if (contador >= 3)
                return 1;
        }
        return 0;
    }

    // Outras missões podem ser acrescentadas
    if (strstr(missao, "Eliminar todas as tropas inimigas") != NULL) {
        for (int i = 0; i < tamanhoMapa; i++)
            if (mapa[i].dono == 2) return 0;
        return 1;
    }

    return 0; // Caso missão ainda não implementada
}

// Exibir mapa
void exibirMapa(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("Territorio %s | Dono: %d | Tropas: %d\n",
               mapa[i].nome, mapa[i].dono, mapa[i].tropas);
    }
}

// Liberar memória da missão
void liberarMemoria(char* missao) {
    free(missao);
}