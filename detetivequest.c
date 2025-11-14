#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"

// ------------------------------------
// IMPLEMENTAÇÕES DA ÁRVORE DE CÔMODOS
// ------------------------------------

// Cria um novo nó de cômodo
RoomNode* create_room_node(const char* name, const char* clue) {
    RoomNode* newNode = (RoomNode*)malloc(sizeof(RoomNode));
    if (newNode == NULL) {
        perror("Erro ao alocar RoomNode");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newNode->static_clue, clue, MAX_NAME_LEN - 1);
    newNode->static_clue[MAX_NAME_LEN - 1] = '\0';
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Constrói a Árvore Binária de Cômodos (Exemplo Básico)
RoomNode* build_room_tree() {
    // Você deve criar uma estrutura significativa de cômodos
    // (Exemplo: Biblioteca, Cozinha, Sala de Estar, etc.)
    RoomNode* root = create_room_node("Hall", "Pista do Hall");
    root->left = create_room_node("Cozinha", "Faca faltando");
    root->right = create_room_node("Biblioteca", "Livro rasgado");
    root->left->left = create_room_node("Quarto", "Relogio quebrado");
    // Adicione mais cômodos e conexões...
    return root;
}

// Visita o cômodo, exibe a pista e a coleta (se a pista não for vazia)
void visit_room(RoomNode* room, GameState* state) {
    printf("\nVocê está no cômodo: *%s*\n", room->name);
    
    if (strlen(room->static_clue) > 0) {
        printf("Pista encontrada: *%s*\n", room->static_clue);
        // Insere a pista na BST de pistas coletadas
        state->collected_clues_bst = insert_clue_bst(
            state->collected_clues_bst, 
            room->static_clue
        );
        state->clue_count++;
        // Uma vez que a pista é coletada, podemos 'zerar' ela no cômodo
        // para que não seja coletada novamente, dependendo da regra do jogo.
        // strncpy(room->static_clue, "", MAX_NAME_LEN - 1); 
    } else {
        printf("Nenhuma pista nova encontrada neste cômodo.\n");
    }
}

// ------------------------------------
// IMPLEMENTAÇÕES DA BST DE PISTAS
// ------------------------------------

// Insere uma nova pista na BST de forma ordenada
ClueNode* insert_clue_bst(ClueNode* root, const char* clue) {
    if (root == NULL) {
        ClueNode* newNode = (ClueNode*)malloc(sizeof(ClueNode));
        if (newNode == NULL) {
            perror("Erro ao alocar ClueNode");
            exit(EXIT_FAILURE);
        }
        strncpy(newNode->clue, clue, MAX_NAME_LEN - 1);
        newNode->clue[MAX_NAME_LEN - 1] = '\0';
        newNode->left = NULL;
        newNode->right = NULL;
        printf("Pista adicionada à sua coleção.\n");
        return newNode;
    }
    
    int cmp = strcmp(clue, root->clue);
    if (cmp < 0) {
        root->left = insert_clue_bst(root->left, clue);
    } else if (cmp > 0) {
        root->right = insert_clue_bst(root->right, clue);
    } else {
        // Pista já existe, não insere novamente.
        printf("Pista já coletada: %s\n", clue);
    }
    return root;
}

// Percorre a BST (Em Ordem) para listar as pistas coletadas
void list_collected_clues(ClueNode* root) {
    if (root != NULL) {
        list_collected_clues(root->left);
        printf("* %s\n", root->clue);
        list_collected_clues(root->right);
    }
}

// ------------------------------------
// IMPLEMENTAÇÕES DA TABELA HASH
// ------------------------------------

// Função Hash Simples: soma dos valores ASCII dos caracteres (Módulo HASH_SIZE)
unsigned int hash_function(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % HASH_SIZE;
}

// Cria e inicializa a Tabela Hash
HashTable* create_hash_table() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) {
        perror("Erro ao alocar HashTable");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

// Insere um par (Pista, Suspeito) na Tabela Hash
void insert_hash_table(HashTable* ht, const char* clue, const char* suspect) {
    unsigned int index = hash_function(clue);
    
    HashEntry* newEntry = (HashEntry*)malloc(sizeof(HashEntry));
    if (newEntry == NULL) {
        perror("Erro ao alocar HashEntry");
        exit(EXIT_FAILURE);
    }
    
    strncpy(newEntry->clue_key, clue, MAX_NAME_LEN - 1);
    newEntry->clue_key[MAX_NAME_LEN - 1] = '\0';
    strncpy(newEntry->suspect_value, suspect, MAX_NAME_LEN - 1);
    newEntry->suspect_value[MAX_NAME_LEN - 1] = '\0';
    
    // Adiciona no início da lista encadeada (para colisões)
    newEntry->next = ht->table[index];
    ht->table[index] = newEntry;
}

// Inicializa a tabela hash com os pares Pista/Suspeito
void initialize_suspect_hash(HashTable* ht) {
    // Estas são as "chaves" (pistas) que levam aos "valores" (suspeitos)
    insert_hash_table(ht, "Faca faltando", "Suspeito A");
    insert_hash_table(ht, "Livro rasgado", "Suspeito B");
    insert_hash_table(ht, "Relogio quebrado", "Suspeito A");
    insert_hash_table(ht, "Pista do Hall", "Suspeito C");
    insert_hash_table(ht, "Copos sujos", "Suspeito B");
    // Você deve adicionar mais pares que façam sentido para o seu jogo.
}

// Verifica se Pelo menos DUAS pistas coletadas apontam para o suspeito acusado
int check_accusation(HashTable* ht, ClueNode* collected_clues_root, const char* accused_suspect) {
    int match_count = 0;
    
    // Função auxiliar (recursiva) para percorrer a BST e verificar
    void check_clues_against_hash(ClueNode* root) {
        if (root == NULL || match_count >= 2) return;
        
        check_clues_against_hash(root->left);
        
        unsigned int index = hash_function(root->clue);
        HashEntry* current = ht->table[index];
        
        // Percorre a lista encadeada no índice hash
        while (current != NULL) {
            // Verifica se a chave (pista) na hash corresponde à pista coletada
            if (strcmp(current->clue_key, root->clue) == 0) {
                // Se o suspeito da pista (valor da hash) for igual ao suspeito acusado
                if (strcmp(current->suspect_value, accused_suspect) == 0) {
                    printf("> Pista coletada '%s' aponta para %s.\n", root->clue, accused_suspect);
                    match_count++;
                }
                break; // Assumindo que cada pista única só mapeia para um suspeito
            }
            current = current->next;
        }

        if (match_count < 2) {
            check_clues_against_hash(root->right);
        }
    }
    
    check_clues_against_hash(collected_clues_root);
    
    return match_count >= 2;
}


// ------------------------------------
// LÓGICA E INTERAÇÃO DO JOGO
// ------------------------------------

// Lógica de exploração interativa (simplificada)
void interactive_exploration(GameState* state) {
    char choice;
    
    // O loop continua até o jogador decidir acusar (sair)
    while (1) {
        visit_room(state->current_room, state);
        
        printf("\nO que deseja fazer? (e)squerda, (d)ireita, ou (s)air para acusar: ");
        scanf(" %c", &choice);
        
        if (choice == 's' || choice == 'S') {
            break; // Sai do loop para a acusação
        } else if (choice == 'e' || choice == 'E') {
            if (state->current_room->left != NULL) {
                state->current_room = state->current_room->left;
            } else {
                printf("Não há cômodo à esquerda. Fique onde está.\n");
            }
        } else if (choice == 'd' || choice == 'D') {
            if (state->current_room->right != NULL) {
                state->current_room = state->current_room->right;
            } else {
                printf("Não há cômodo à direita. Fique onde está.\n");
            }
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// Inicia o programa e a lógica principal
void start_game() {
    printf("--- Jogo de Mistério: Início ---\n");
    
    // 1. Inicializa o estado do jogo e as estruturas
    GameState state;
    state.collected_clues_bst = NULL;
    state.clue_count = 0;
    
    // Constrói a árvore de cômodos e define o cômodo inicial
    RoomNode* room_tree_root = build_room_tree();
    state.current_room = room_tree_root;
    
    // Cria e popula a tabela hash de suspeitos
    state.suspect_hash_table = create_hash_table();
    initialize_suspect_hash(state.suspect_hash_table);
    
    // 2. Permite ao jogador explorar (coletar pistas)
    interactive_exploration(&state);
    
    // 3. Ao final, lista as pistas e solicita a acusação
    printf("\n--- FASE DE ACUSAÇÃO ---\n");
    printf("Pistas Coletadas (%d):\n", state.clue_count);
    list_collected_clues(state.collected_clues_bst);
    
    char accused[MAX_NAME_LEN];
    printf("Acuse um suspeito (Ex: Suspeito A, Suspeito B, Suspeito C): ");
    // Limpa o buffer antes de ler a string
    int c; while ((c = getchar()) != '\n' && c != EOF) {} 
    if (fgets(accused, MAX_NAME_LEN, stdin) != NULL) {
        // Remove o '\n' lido pelo fgets
        accused[strcspn(accused, "\n")] = 0; 
    }
    
    // 4. Verifica o desfecho
    printf("\nVerificando a acusação contra *%s*...\n", accused);
    
    if (check_accusation(state.suspect_hash_table, state.collected_clues_bst, accused)) {
        printf("\n✅ *SUCESSO!* Pelo menos duas pistas confirmam sua acusação. O mistério foi resolvido!\n");
    } else {
        printf("\n❌ *FALHA!* Menos de duas pistas suportam sua acusação. O suspeito escapou!\n");
    }

    // 5. Liberação de memória (Obrigatório em C)
    // As funções para liberar a memória das estruturas (árvores e hash) devem ser implementadas.
    printf("\nFim do Jogo. Memória deve ser liberada.\n");
}

// ------------------------------------
// FUNÇÃO MAIN
// ------------------------------------

int main() {
    start_game();
    return 0;
}