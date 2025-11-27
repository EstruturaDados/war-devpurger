#include <stdio.h>
#include <string.h>
// Incluindo stdlib.h para a função exit(), caso necessário.
#include <stdlib.h> 

// --- Constantes Globais ---
#define MAX_TERRITORIOS 5
#define TAM_STRING 50
#define OPCAO_SAIR 0

// --- Estrutura de Dados ---
// Define a estrutura para um território.
typedef struct {
    char nome[TAM_STRING];
    char corDoExercito[TAM_STRING];
    int numeroDeTropas;
} Territorio;

// --- Protótipos das Funções (Interface do Nível Novato) ---
// Funções de setup:
void inicializarMapa(Territorio mapa[], int numTerritorios);

// Funções de interface com o usuário:
void exibirMenu();
void cadastrarTerritorio(Territorio* t, int indice);
void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios);

// Função utilitária:
void limparBufferEntrada();


// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    Territorio mapa[MAX_TERRITORIOS]; // Vetor estático conforme Nível Novato
    int escolha;
    
    printf("==========================================\n");
    printf(" PROJETO WAR ESTRUTURADO - Nível Novato\n");
    printf("==========================================\n\n");

    // Preenche os territórios com seus dados iniciais (cadastro pelo usuário).
    inicializarMapa(mapa, MAX_TERRITORIOS);
    
    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Exibe o estado atual do mapa.
        exibirEstadoDoMapa(mapa, MAX_TERRITORIOS);
        
        // Exibe o menu de ações e lê a escolha.
        exibirMenu();
        
        printf("Escolha sua ação: ");
        if (scanf("%d", &escolha) != 1) {
             printf("\nOpção inválida. Tente novamente.\n");
             limparBufferEntrada(); // Limpa para evitar loops infinitos no scanf
             continue; // Pula para a próxima iteração
        }
        limparBufferEntrada(); // Limpa o buffer após scanf

        // Usa um 'switch' para chamar a função apropriada.
        switch (escolha) {
            case 1:
                // No Nível Novato, a Opção 1 apenas informa que a funcionalidade virá depois.
                printf("\n Funcionalidade de ATAQUE (Nível Aventureiro) indisponível no momento.\n");
                break;
            case 2:
                // No Nível Novato, a Opção 2 apenas verifica o estado (que já foi exibido).
                printf("\n✅ O estado atual do mapa foi atualizado acima.\n");
                break;
            case OPCAO_SAIR: // Opção 0
                printf("\nEncerrando o Desafio WAR Estruturado. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção válida do menu.\n");
                break;
        }

        // Simula uma pausa para que o jogador possa ler (exceto na saída)
        if (escolha != OPCAO_SAIR) {
            printf("\nPressione ENTER para continuar...");
            getchar(); // Espera o Enter
        }

    } while (escolha != OPCAO_SAIR);

    // 3. Limpeza:
    // No Nível Novato com vetor estático, não há memória dinâmica para liberar.
    
    return 0;
}


// --- Implementação das Funções ---

// Função utilitária:
/**
 * @brief Limpa o buffer de entrada do teclado após o uso de scanf ou para leituras consecutivas.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funções de interface com o usuário:
/**
 * @brief Coleta todos os dados necessários (Nome, Cor, Tropas) para um único território.
 * @param t Ponteiro para a struct Territorio que será preenchida (passagem por referência).
 * @param indice O índice do território que está sendo cadastrado.
 */
void cadastrarTerritorio(Territorio* t, int indice) {
    printf("--- Cadastro do Território %d de %d ---\n", indice, MAX_TERRITORIOS);

    // Nome (Usando fgets para strings com espaço)
    printf("Nome do Território: ");
    fgets(t->nome, TAM_STRING, stdin); 
    // Remove o '\n' lido pelo fgets.
    t->nome[strcspn(t->nome, "\n")] = 0; 

    // Cor do Exército
    printf("Cor do Exército Dominante: ");
    fgets(t->corDoExercito, TAM_STRING, stdin);
    // Remove o '\n' lido pelo fgets.
    t->corDoExercito[strcspn(t->corDoExercito, "\n")] = 0; 

    // Número de Tropas
    printf("Número de Tropas: ");
    if (scanf("%d", &t->numeroDeTropas) != 1) {
         printf("Erro: Entrada inválida. Atribuindo 1 tropa e continuando.\n");
         t->numeroDeTropas = 1; 
    }
    
    // Limpa o buffer após o scanf para o próximo loop
    limparBufferEntrada();
    
    printf("\n");
}

/**
 * @brief Itera sobre o vetor de territórios, solicitando os dados do usuário para cada um.
 * @param mapa O vetor de estruturas Territorio a ser preenchido.
 * @param numTerritorios O tamanho do vetor.
 */
void inicializarMapa(Territorio mapa[], int numTerritorios) {
    for (int i = 0; i < numTerritorios; i++) {
        // Passamos o endereço do elemento (&mapa[i]) para a função cadastrarTerritorio.
        cadastrarTerritorio(&mapa[i], i + 1);
    }
}

/**
 * @brief Imprime todos os territórios cadastrados em um formato de tabela organizado.
 * @param mapa O vetor de estruturas Territorio a ser exibido (passagem por referência constante).
 * @param numTerritorios O tamanho do vetor.
 */
void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    
    // Cabeçalho
    printf("| %-20s | %-15s | %-10s |\n", "TERRITÓRIO", "EXÉRCITO", "TROPAS");
    printf("|----------------------|-----------------|------------|\n");


    for (int i = 0; i < numTerritorios; i++) {
        // O uso do 'const' garante que, mesmo recebendo uma referência, a função não altere os dados.
        printf("| %-20s | %-15s | %-10d |\n", 
               mapa[i].nome, 
               mapa[i].corDoExercito, 
               mapa[i].numeroDeTropas);
    }

    printf("|----------------------|-----------------|------------|\n");
}

/**
 * @brief Imprime na tela o menu de ações disponíveis para o jogador.
 */
void exibirMenu() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Atacar (Em desenvolvimento)\n");
    printf("2. Verificar estado do Mapa\n");
    printf("0. Sair do Jogo\n");
}