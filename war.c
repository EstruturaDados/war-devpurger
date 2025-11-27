#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>   

// --- Constantes Globais e Definições ---
#define MAX_TERRITORIOS 5
#define TAM_STRING 50
#define OPCAO_SAIR 0
#define OPCAO_ATACAR 1
#define OPCAO_VERIFICAR_MISSAO 2

// Definição das Cores e Alvos
#define COR_JOGADOR "Azul"
#define COR_DESTRUIR "Verde"

// Missão IDs (Nova Estrutura)
typedef enum {
    MISSAO_DESTRUIR_VERDE = 0,
    MISSAO_CONQUISTAR_TRES = 1,
    NUM_MISSOES = 2 
} MissaoID;

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_STRING];
    char corDoExercito[TAM_STRING];
    int numeroDeTropas;
} Territorio;

// Array de descrições das missões (para exibição)
const char* DESCRICOES_MISSOES[] = {
    "Destruir completamente o exército " COR_DESTRUIR ".",
    "Conquistar um total de 3 territórios."
};

// --- Protótipos das Funções ---
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int numTerritorios);
void liberarMemoria(Territorio* mapa);
void inicializarTerritoriosAutomatica(Territorio mapa[], int numTerritorios);

// Funções de interface com o usuário:
void exibirMenu();
void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios);
void exibirMissao(MissaoID idMissao);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio mapa[], int numTerritorios, const char* corJogador);
int simularDados();
MissaoID sortearMissao();
int verificarVitoria(const Territorio mapa[], int numTerritorios, MissaoID idMissao, const char* corJogador, const char* corAlvo);

// Função utilitária:
void limparBufferEntrada();


// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    srand(time(NULL)); 
    
    // Aloca a memória dinamicamente.
    Territorio* mapa = alocarMapa(MAX_TERRITORIOS); 
    
    if (mapa == NULL) {
        printf("Erro ao alocar memória. Encerrando o programa.\n");
        return 1;
    }

    // Inicialização automática dos territórios
    inicializarTerritoriosAutomatica(mapa, MAX_TERRITORIOS);

    // Sorteia a missão secreta
    MissaoID missaoDoJogador = sortearMissao();
    int escolha;
    int jogoVencido = 0; // Flag de vitória

    printf("==========================================\n");
    printf("👑 PROJETO WAR ESTRUTURADO - Nível Mestre\n");
    printf("==========================================\n");
    printf("Bem-vindo! Seu exército é a cor: %s.\n", COR_JOGADOR);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        exibirEstadoDoMapa(mapa, MAX_TERRITORIOS);
        exibirMissao(missaoDoJogador);
        exibirMenu();
        
        printf("Escolha sua ação: ");
        if (scanf("%d", &escolha) != 1) {
             printf("\nOpção inválida. Tente novamente.\n");
             limparBufferEntrada(); 
             continue; 
        }
        limparBufferEntrada(); 

        switch (escolha) {
            case OPCAO_ATACAR:
                faseDeAtaque(mapa, MAX_TERRITORIOS, COR_JOGADOR); 
                break;
            case OPCAO_VERIFICAR_MISSAO:
                if (verificarVitoria(mapa, MAX_TERRITORIOS, missaoDoJogador, COR_JOGADOR, COR_DESTRUIR)) {
                    printf("\n🎉🎉🎉 PARABÉNS! MISSÃO CUMPRIDA! 🎉🎉🎉\n");
                    jogoVencido = 1;
                } else {
                    printf("\n😕 Missão ainda NÃO foi cumprida. Continue atacando!\n");
                }
                break;
            case OPCAO_SAIR: 
                printf("\nEncerrando o Desafio WAR Estruturado. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida.\n");
                break;
        }

        if (jogoVencido) {
            escolha = OPCAO_SAIR; // Sai do loop após a vitória
        } else if (escolha != OPCAO_SAIR) {
            printf("\nPressione ENTER para continuar...");
            getchar(); 
        }

    } while (escolha != OPCAO_SAIR);

    // 3. Limpeza:
    liberarMemoria(mapa); 
    
    return 0;
}


// --- Implementação das Funções ---

// Funções de setup e gerenciamento de memória:

Territorio* alocarMapa(int numTerritorios) {
    return (Territorio*)calloc(numTerritorios, sizeof(Territorio));
}

void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

/**
 * @brief Inicializa o mapa com valores predefinidos (automáticos).
 */
void inicializarTerritoriosAutomatica(Territorio mapa[], int numTerritorios) {
    printf("Inicializando mapa com 5 territórios predefinidos...\n");

    // Dados fixos (simulando um mapa inicial balanceado)
    // Índice 0: ID 1
    strcpy(mapa[0].nome, "Alaska");
    strcpy(mapa[0].corDoExercito, COR_JOGADOR);
    mapa[0].numeroDeTropas = 2;

    // Índice 1: ID 2
    strcpy(mapa[1].nome, "Mexico");
    strcpy(mapa[1].corDoExercito, "Vermelho");
    mapa[1].numeroDeTropas = 1;

    // Índice 2: ID 3 (Alvo de Destruição)
    strcpy(mapa[2].nome, "Brasil");
    strcpy(mapa[2].corDoExercito, COR_DESTRUIR);
    mapa[2].numeroDeTropas = 3;

    // Índice 3: ID 4
    strcpy(mapa[3].nome, "Japao");
    strcpy(mapa[3].corDoExercito, COR_JOGADOR);
    mapa[3].numeroDeTropas = 1;

    // Índice 4: ID 5
    strcpy(mapa[4].nome, "Italia");
    strcpy(mapa[4].corDoExercito, "Vermelho");
    mapa[4].numeroDeTropas = 2;
    
    printf("Inicialização concluída.\n");
}


// Funções de interface com o usuário:

void exibirMenu() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("%d. Iniciar Fase de Ataque\n", OPCAO_ATACAR);
    printf("%d. Verificar Missão\n", OPCAO_VERIFICAR_MISSAO);
    printf("%d. Sair do Jogo\n", OPCAO_SAIR);
}

void exibirEstadoDoMapa(const Territorio mapa[], int numTerritorios) {
    printf("\n--- ESTADO ATUAL DO MAPA (ID: 1 a %d) ---\n", numTerritorios);
    
    printf("| %-2s | %-20s | %-15s | %-10s |\n", "ID", "TERRITÓRIO", "EXÉRCITO", "TROPAS");
    printf("|----|----------------------|-----------------|------------|\n");

    for (int i = 0; i < numTerritorios; i++) {
        printf("| %-2d | %-20s | %-15s | %-10d |\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].corDoExercito, 
               mapa[i].numeroDeTropas);
    }

    printf("|----|----------------------|-----------------|------------|\n");
}

/**
 * @brief Exibe a descrição da missão atual do jogador.
 * @param idMissao O ID da missão sorteada.
 */
void exibirMissao(MissaoID idMissao) {
    if (idMissao >= 0 && idMissao < NUM_MISSOES) {
        printf("\n📜 Missão Secreta: %s\n", DESCRICOES_MISSOES[idMissao]);
    } else {
        printf("\n📜 Missão Secreta: Erro ao carregar a missão.\n");
    }
}


// Funções de lógica principal do jogo:

/**
 * @brief Sorteia e retorna um ID de missão aleatório para o jogador.
 * @return MissaoID A missão sorteada.
 */
MissaoID sortearMissao() {
    // rand() % NUM_MISSOES (rand() % 2) gera 0 ou 1.
    return (MissaoID)(rand() % NUM_MISSOES);
}

/**
 * @brief Verifica se o jogador cumpriu os requisitos de sua missão atual.
 * @param mapa O vetor de territórios.
 * @param idMissao O ID da missão a ser verificada.
 * @param corJogador A cor do exército do jogador.
 * @param corAlvo A cor do exército alvo (para a missão de destruição).
 * @return 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
 */
int verificarVitoria(const Territorio mapa[], int numTerritorios, MissaoID idMissao, const char* corJogador, const char* corAlvo) {
    int countTerritoriosJogador = 0;
    int alvoDestruido = 1; // Assume que o alvo foi destruído até que se prove o contrário

    for (int i = 0; i < numTerritorios; i++) {
        // Conta os territórios do jogador
        if (strcmp(mapa[i].corDoExercito, corJogador) == 0) {
            countTerritoriosJogador++;
        }
        
        // Verifica se o exército alvo ainda existe
        if (strcmp(mapa[i].corDoExercito, corAlvo) == 0) {
            alvoDestruido = 0;
        }
    }

    // Lógica para cada tipo de missão
    switch (idMissao) {
        case MISSAO_DESTRUIR_VERDE:
            // A missão é cumprida se o exército alvo (Verde) não tiver mais nenhum território.
            return alvoDestruido; 
        
        case MISSAO_CONQUISTAR_TRES:
            // A missão é cumprida se o jogador tiver 3 ou mais territórios.
            return (countTerritoriosJogador >= 3);
            
        default:
            return 0;
    }
}

/**
 * @brief Simula o lançamento de um único dado (valor de 1 a 6).
 */
int simularDados() {
    return (rand() % 6) + 1;
}

/**
 * @brief Gerencia a interface e a lógica de ataque entre dois territórios.
 */
void faseDeAtaque(Territorio mapa[], int numTerritorios, const char* corJogador) {
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

    int idxAtacante = idAtacante - 1;
    int idxDefensor = idDefensor - 1;

    Territorio* atacante = &mapa[idxAtacante];
    Territorio* defensor = &mapa[idxDefensor];

    // Validações Essenciais
    if (idxAtacante == idxDefensor) {
        printf("🚫 Você não pode atacar a si mesmo!\n");
        return;
    }
    // Nível Mestre: O atacante DEVE ser o jogador
    if (strcmp(atacante->corDoExercito, corJogador) != 0) {
        printf("🚫 O território %s não pertence ao seu exército (%s) e não pode atacar.\n", atacante->nome, corJogador);
        return;
    }
    if (atacante->numeroDeTropas <= 1) {
        printf("🚫 O território %s precisa de pelo menos 2 tropas para atacar (1 fica de guarnição).\n", atacante->nome);
        return;
    }
    // Ataque entre o mesmo exército
    if (strcmp(atacante->corDoExercito, defensor->corDoExercito) == 0) {
         printf("🚫 Ataque amigável cancelado! Ambos pertencem ao exército %s.\n", atacante->corDoExercito);
         return;
    }

    // --- Simulação dos Dados (Simplificada: 1 dado vs 1 dado) ---
    int dadoAtacante = simularDados();
    int dadoDefensor = simularDados();

    printf("\n⚔️ BATALHA INICIADA: %s (%s) vs %s (%s)\n", 
           atacante->nome, atacante->corDoExercito, 
           defensor->nome, defensor->corDoExercito);
    printf("   Dados: Atacante (%d) vs Defensor (%d)\n", dadoAtacante, dadoDefensor);

    // Lógica da Batalha: Empate favorece o atacante (conforme seu requisito)
    if (dadoAtacante >= dadoDefensor) { 
        printf("   Vitória do ATACANTE! %s perde 1 tropa.\n", defensor->nome);
        defensor->numeroDeTropas--;
        
        // Verifica Conquista
        if (defensor->numeroDeTropas == 0) {
            printf("\n🎉 CONQUISTA! O território %s foi conquistado por %s!\n", defensor->nome, atacante->corDoExercito);
            // Troca de dono
            strcpy(defensor->corDoExercito, atacante->corDoExercito);
            // Move uma tropa
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
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}