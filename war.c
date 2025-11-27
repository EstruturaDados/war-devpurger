#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para calloc, free, rand, srand
#include <time.h>   // Para time (usado no srand)

// --- Constantes Globais ---
#define MAX_TERRITORIOS 5
#define TAM_STRING 50
#define OPCAO_SAIR 0

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_STRING];
    char corDoExercito[TAM_STRING];
    int numeroDeTropas;
} Territorio;

// --- Protótipos das Funções ---
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int numTerritorios);
void liberarMemoria(Territorio* mapa);
void inicializarMapa(Territorio mapa[], int numTerritorios);

// Funções de interface com o usuário:
void exibirMenu();
void cadastrarTerritorio(Territorio* t, int indice);
void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio mapa[], int numTerritorios);
int simularDados();

// Função utilitária:
void limparBufferEntrada();


// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    // Inicializa a semente para a geração de números aleatórios.
    srand(time(NULL)); 
    
    // Aloca a memória dinamicamente.
    Territorio* mapa = alocarMapa(MAX_TERRITORIOS); 
    
    // Verifica se a alocação foi bem-sucedida.
    if (mapa == NULL) {
        printf("Erro ao alocar memória. Encerrando o programa.\n");
        return 1;
    }

    int escolha;
    
    printf("==========================================\n");
    printf("⚔️ PROJETO WAR ESTRUTURADO - Nível Aventureiro\n");
    printf("==========================================\n\n");

    // Preenche os territórios com dados iniciais (cadastro pelo usuário).
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
             limparBufferEntrada(); 
             continue; 
        }
        limparBufferEntrada(); 

        // Usa um 'switch' para chamar a função apropriada.
        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, MAX_TERRITORIOS); // NOVA FUNCIONALIDADE
                break;
            case 2:
                printf("\n✅ O estado atual do mapa foi atualizado acima.\n");
                break;
            case OPCAO_SAIR: 
                printf("\nEncerrando o Desafio WAR Estruturado. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção válida do menu.\n");
                break;
        }

        // Simula uma pausa para que o jogador possa ler (exceto na saída)
        if (escolha != OPCAO_SAIR) {
            printf("\nPressione ENTER para continuar...");
            getchar(); 
        }

    } while (escolha != OPCAO_SAIR);

    // 3. Limpeza:
    liberarMemoria(mapa); // Libera a memória alocada dinamicamente.
    
    return 0;
}

// --- Implementação das Funções ---

// Funções de setup e gerenciamento de memória:

/**
 * @brief Aloca dinamicamente a memória para o vetor de territórios usando calloc.
 * @param numTerritorios O número de elementos a serem alocados (5 neste nível).
 * @return Ponteiro para o bloco de memória alocada ou NULL em caso de falha.
 */
Territorio* alocarMapa(int numTerritorios) {
    // calloc(quantidade, tamanho_de_cada_elemento)
    // Além de alocar, calloc inicializa todos os bytes com zero, o que é útil.
    return (Territorio*)calloc(numTerritorios, sizeof(Territorio));
}

/**
 * @brief Libera a memória previamente alocada para o mapa usando free.
 * @param mapa O ponteiro para o bloco de memória a ser liberado.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

/**
 * @brief Itera sobre o mapa, solicitando os dados do usuário para cada território.
 * NOTA: Recebe 'mapa' como ponteiro (array), que é a mesma variável alocada dinamicamente.
 */
void inicializarMapa(Territorio mapa[], int numTerritorios) {
    for (int i = 0; i < numTerritorios; i++) {
        cadastrarTerritorio(&mapa[i], i + 1);
    }
}

// Funções de interface com o usuário:

/**
 * @brief Coleta todos os dados necessários (Nome, Cor, Tropas) para um único território.
 * @param t Ponteiro para a struct Territorio que será preenchida (passagem por referência).
 * @param indice O índice do território que está sendo cadastrado.
 */
void cadastrarTerritorio(Territorio* t, int indice) {
    printf("--- Cadastro do Território %d de %d ---\n", indice, MAX_TERRITORIOS);

    printf("Nome do Território: ");
    fgets(t->nome, TAM_STRING, stdin); 
    t->nome[strcspn(t->nome, "\n")] = 0; 

    printf("Cor do Exército Dominante: ");
    fgets(t->corDoExercito, TAM_STRING, stdin);
    t->corDoExercito[strcspn(t->corDoExercito, "\n")] = 0; 

    printf("Número de Tropas: ");
    if (scanf("%d", &t->numeroDeTropas) != 1 || t->numeroDeTropas <= 0) {
         printf("Erro: Entrada inválida. Atribuindo 1 tropa e continuando.\n");
         t->numeroDeTropas = 1; 
    }
    
    limparBufferEntrada();
    
    printf("\n");
}

/**
 * @brief Imprime todos os territórios cadastrados em um formato de tabela organizado.
 * @param mapa O vetor de estruturas Territorio a ser exibido (passagem por referência constante).
 * @param numTerritorios O tamanho do vetor.
 */
void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios) {
    printf("\n--- ESTADO ATUAL DO MAPA (ID: 1 a %d) ---\n", numTerritorios);
    
    // Cabeçalho
    printf("| %-2s | %-20s | %-15s | %-10s |\n", "ID", "TERRITÓRIO", "EXÉRCITO", "TROPAS");
    printf("|----|----------------------|-----------------|------------|\n");


    for (int i = 0; i < numTerritorios; i++) {
        // Exibe o ID (i + 1) para facilitar a escolha do jogador.
        printf("| %-2d | %-20s | %-15s | %-10d |\n", 
               i + 1, // ID
               mapa[i].nome, 
               mapa[i].corDoExercito, 
               mapa[i].numeroDeTropas);
    }

    printf("|----|----------------------|-----------------|------------|\n");
}

/**
 * @brief Imprime na tela o menu de ações disponíveis para o jogador.
 */
void exibirMenu() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Iniciar Fase de Ataque\n");
    printf("2. Verificar estado do Mapa\n");
    printf("0. Sair do Jogo\n");
}

// Funções de lógica principal do jogo:

/**
 * @brief Simula o lançamento de um único dado (valor de 1 a 6).
 * @return O valor aleatório do dado.
 */
int simularDados() {
    // rand() % 6 gera um número de 0 a 5. Somamos 1 para obter 1 a 6.
    return (rand() % 6) + 1;
}

/**
 * @brief Gerencia a interface e a lógica de ataque entre dois territórios.
 * @param mapa O vetor de territórios.
 * @param numTerritorios O tamanho do mapa.
 */
void faseDeAtaque(Territorio mapa[], int numTerritorios) {
    int idAtacante, idDefensor;
    
    printf("\n====== FASE DE ATAQUE ======\n");
    printf("Escolha o Território ATACANTE (ID 1 a %d): ", numTerritorios);
    if (scanf("%d", &idAtacante) != 1 || idAtacante < 1 || idAtacante > numTerritorios) {
        printf("ID de atacante inválido.\n");
        limparBufferEntrada();
        return;
    }

    printf("Escolha o Território DEFENSOR (ID 1 a %d): ", numTerritorios);
    if (scanf("%d", &idDefensor) != 1 || idDefensor < 1 || idDefensor > numTerritorios) {
        printf("ID de defensor inválido.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    // Ajusta os IDs para os índices do vetor (0 a 4).
    int idxAtacante = idAtacante - 1;
    int idxDefensor = idDefensor - 1;

    // Obtenção dos ponteiros para os territórios
    Territorio* atacante = &mapa[idxAtacante];
    Territorio* defensor = &mapa[idxDefensor];

    // Validações básicas
    if (idxAtacante == idxDefensor) {
        printf("🚫 Você não pode atacar a si mesmo!\n");
        return;
    }
    if (atacante->numeroDeTropas <= 1) {
        printf("🚫 O território %s precisa de pelo menos 2 tropas para atacar (1 fica de guarnição).\n", atacante->nome);
        return;
    }
    if (strcmp(atacante->corDoExercito, defensor->corDoExercito) == 0) {
         printf("🚫 O território %s pertence à mesma cor do exército que %s. Ataque amigável cancelado!\n", atacante->nome, defensor->nome);
         return;
    }

    // --- Simulação dos Dados (Simplificada: 1 dado vs 1 dado) ---
    int dadoAtacante = simularDados();
    int dadoDefensor = simularDados();

    printf("\n⚔️ BATALHA INICIADA: %s vs %s\n", atacante->nome, defensor->nome);
    printf("   Dados: Atacante (%d) vs Defensor (%d)\n", dadoAtacante, dadoDefensor);

    // Lógica da Batalha: Maior dado vence. Empate favorece o Defensor no WAR original,
    // mas o seu enunciado pede: "Empates favorecem o atacante". Vamos seguir seu enunciado.
    if (dadoAtacante >= dadoDefensor) { // Atacante vence ou empata (seu requisito)
        printf("   Vitória do ATACANTE! %s perde 1 tropa.\n", defensor->nome);
        defensor->numeroDeTropas--;
        
        // Verifica Conquista
        if (defensor->numeroDeTropas == 0) {
            printf("\n🎉 CONQUISTA! O território %s foi conquistado por %s!\n", defensor->nome, atacante->corDoExercito);
            // Troca de dono
            strcpy(defensor->corDoExercito, atacante->corDoExercito);
            // Move uma tropa (O WAR exige mover no mínimo 1)
            atacante->numeroDeTropas--;
            defensor->numeroDeTropas++;
            printf("   Uma tropa foi movida de %s para %s.\n", atacante->nome, defensor->nome);
        }
    } else { // Defensor vence (DadoDefensor > DadoAtacante)
        printf("   Vitória do DEFENSOR! %s perde 1 tropa.\n", atacante->nome);
        atacante->numeroDeTropas--;
    }

    printf("-------------------------------------------\n");
}

// Função utilitária:
/**
 * @brief Limpa o buffer de entrada do teclado.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}