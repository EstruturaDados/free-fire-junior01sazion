#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para a função clock()

// ----------------------------------------------------
// 1. Definição da Estrutura e Constantes
// ----------------------------------------------------

#define MAX_COMPONENTES 20
#define NOME_CHAVE "Chip Central" // Componente chave para a busca binária

/**
 * Componente: Define o molde para cada parte da torre de fuga.
 */
typedef struct {
    char nome[30];      // Ex: "Chip Central"
    char tipo[20];      // Ex: "Controle", "Suporte"
    int prioridade;     // Ex: 1 a 10 (onde 10 é a mais alta)
} Componente;

// ----------------------------------------------------
// 2. Variáveis Globais
// ----------------------------------------------------

Componente torreComponentes[MAX_COMPONENTES];
int totalComponentes = 0;
// 0: Não Ordenado, 1: Ordenado por Nome (Bubble), 2: Ordenado por Tipo (Insertion), 3: Ordenado por Prioridade (Selection)
int statusOrdenacao = 0; 

// ----------------------------------------------------
// 3. Protótipos das Funções
// ----------------------------------------------------

void menuPrincipal();
void limparBuffer();
void cadastrarComponentes();
void mostrarComponentes();

// Algoritmos de Ordenação
long long bubbleSortNome();
long long insertionSortTipo();
long long selectionSortPrioridade();

// Algoritmos de Busca
void buscaBinariaPorNome();

// Utilitários de Medição
void medirDesempenho(long long (*algoritmoFuncao)());
char* getStatusNome();

// ----------------------------------------------------
// 4. Função Principal (main)
// ----------------------------------------------------
int main() {
    int opcao;

    printf("=========================================================\n");
    printf(" || Sistema de Priorização da Torre de Fuga - DESAFIO FINAL ||\n");
    printf("=========================================================\n");

    // Cadastro inicial
    cadastrarComponentes();

    while (1) {
        printf("\n--- ESTRATÉGIA DE MONTAGEM ---\n");
        printf("Status Atual: %s\n", getStatusNome());
        printf("1. Bubble Sort (Ordenar por Nome)\n");
        printf("2. Insertion Sort (Ordenar por Tipo)\n");
        printf("3. Selection Sort (Ordenar por Prioridade)\n");
        printf("4. Busca Binária (Componente Chave)\n");
        printf("5. Visualizar Componentes\n");
        printf("6. Sair do Sistema\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = 0;
        }
        limparBuffer();
        
        switch (opcao) {
            case 1:
                medirDesempenho(bubbleSortNome);
                statusOrdenacao = 1;
                break;
            case 2:
                medirDesempenho(insertionSortTipo);
                statusOrdenacao = 2;
                break;
            case 3:
                medirDesempenho(selectionSortPrioridade);
                statusOrdenacao = 3;
                break;
            case 4:
                buscaBinariaPorNome();
                break;
            case 5:
                mostrarComponentes();
                break;
            case 6:
                printf("\n--- Fuga Cancelada. Sistema Finalizado. ---\n");
                return EXIT_SUCCESS;
            default:
                printf("\n[ERRO] Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// ----------------------------------------------------
// 5. Funções de Apoio e Entrada de Dados
// ----------------------------------------------------

/**
 * @brief Limpa o buffer de entrada.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Obtém o nome do status de ordenação atual.
 * @return String com o status.
 */
char* getStatusNome() {
    switch (statusOrdenacao) {
        case 1: return "Ordenado por NOME (Bubble Sort)";
        case 2: return "Ordenado por TIPO (Insertion Sort)";
        case 3: return "Ordenado por PRIORIDADE (Selection Sort)";
        default: return "NÃO ORDENADO";
    }
}

/**
 * @brief Permite ao usuário cadastrar os componentes da torre.
 */
void cadastrarComponentes() {
    int i;
    char temp[30];

    printf("\n--- CADASTRO DE COMPONENTES (%d Máx) ---\n", MAX_COMPONENTES);
    printf("Quantos componentes deseja cadastrar (1-%d)? ", MAX_COMPONENTES);
    
    if (scanf("%d", &totalComponentes) != 1 || totalComponentes <= 0 || totalComponentes > MAX_COMPONENTES) {
        printf("[ERRO] Quantidade inválida. Definindo 5 componentes padrão.\n");
        totalComponentes = 5;
    }
    limparBuffer();

    for (i = 0; i < totalComponentes; i++) {
        printf("\nComponente #%d:\n", i + 1);

        // Entrada do Nome
        printf("Nome (Ex: Chip Central): ");
        if (fgets(torreComponentes[i].nome, 30, stdin) == NULL) continue;
        torreComponentes[i].nome[strcspn(torreComponentes[i].nome, "\n")] = 0; // Remove newline

        // Entrada do Tipo
        printf("Tipo (Ex: Controle, Suporte): ");
        if (fgets(torreComponentes[i].tipo, 20, stdin) == NULL) continue;
        torreComponentes[i].tipo[strcspn(torreComponentes[i].tipo, "\n")] = 0; // Remove newline

        // Entrada da Prioridade
        printf("Prioridade (1=Baixa, 10=Alta): ");
        if (scanf("%d", &torreComponentes[i].prioridade) != 1) torreComponentes[i].prioridade = 5;
        limparBuffer();
    }
    printf("\nCadastro de %d componentes finalizado.\n", totalComponentes);
    mostrarComponentes();
}

/**
 * @brief Exibe o vetor de componentes de forma formatada.
 */
void mostrarComponentes() {
    if (totalComponentes == 0) {
        printf("\n[INFO] Nenhum componente cadastrado.\n");
        return;
    }

    printf("\n--- LISTA ATUAL DE COMPONENTES (%s) ---\n", getStatusNome());
    printf("| %-4s | %-25s | %-15s | %-10s |\n", "ID", "Nome", "Tipo", "Prioridade");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < totalComponentes; i++) {
        printf("| %-4d | %-25s | %-15s | %-10d |\n", 
               i + 1, 
               torreComponentes[i].nome, 
               torreComponentes[i].tipo, 
               torreComponentes[i].prioridade);
    }
    printf("----------------------------------------------------------------\n");
}

// ----------------------------------------------------
// 6. Funções de Medição de Desempenho
// ----------------------------------------------------

/**
 * @brief Mede o tempo de execução e exibe o resultado da ordenação.
 * @param algoritmoFuncao Ponteiro para a função de ordenação.
 */
void medirDesempenho(long long (*algoritmoFuncao)()) {
    clock_t inicio, fim;
    double tempo_execucao;
    long long comparacoes;

    // 1. Medir Tempo
    inicio = clock();
    comparacoes = algoritmoFuncao();
    fim = clock();

    // Calcula o tempo em segundos
    tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    // 2. Exibir Resultados
    mostrarComponentes();
    printf("\n--- ANÁLISE DE DESEMPENHO ---\n");
    printf("Algoritmo: %s\n", getStatusNome());
    printf("Comparações Realizadas: %lld\n", comparacoes);
    printf("Tempo de Execução (s): %f\n", tempo_execucao);
    printf("-----------------------------\n");
}


// ----------------------------------------------------
// 7. Algoritmos de Ordenação
// ----------------------------------------------------

/**
 * @brief Ordena os componentes por Nome (string) usando Bubble Sort.
 * @return Número de comparações realizadas.
 */
long long bubbleSortNome() {
    long long comparacoes = 0;
    int i, j;
    Componente temp;
    
    // Bubble Sort
    for (i = 0; i < totalComponentes - 1; i++) {
        for (j = 0; j < totalComponentes - 1 - i; j++) {
            comparacoes++; // Conta a comparação de nome
            
            // Compara os Nomes: se o item atual (j) for 'maior' que o próximo (j+1)
            if (strcmp(torreComponentes[j].nome, torreComponentes[j+1].nome) > 0) {
                // Troca (Swap)
                temp = torreComponentes[j];
                torreComponentes[j] = torreComponentes[j+1];
                torreComponentes[j+1] = temp;
            }
        }
    }
    return comparacoes;
}

/**
 * @brief Ordena os componentes por Tipo (string) usando Insertion Sort.
 * @return Número de comparações realizadas.
 */
long long insertionSortTipo() {
    long long comparacoes = 0;
    int i, j;
    Componente chave;

    // Insertion Sort
    for (i = 1; i < totalComponentes; i++) {
        chave = torreComponentes[i]; // Elemento a ser inserido
        j = i - 1;

        // Desloca os elementos maiores que a chave para a direita
        while (j >= 0) {
            comparacoes++; // Conta a comparação de tipo
            
            // Compara o Tipo: se o item anterior (j) for 'maior' que a chave
            if (strcmp(torreComponentes[j].tipo, chave.tipo) > 0) {
                torreComponentes[j + 1] = torreComponentes[j];
                j = j - 1;
            } else {
                break;
            }
        }
        torreComponentes[j + 1] = chave; // Insere a chave na posição correta
    }
    return comparacoes;
}

/**
 * @brief Ordena os componentes por Prioridade (int) usando Selection Sort.
 * @return Número de comparações realizadas.
 */
long long selectionSortPrioridade() {
    long long comparacoes = 0;
    int i, j, indiceMinimo;
    Componente temp;

    // Selection Sort
    for (i = 0; i < totalComponentes - 1; i++) {
        indiceMinimo = i;
        for (j = i + 1; j < totalComponentes; j++) {
            comparacoes++; // Conta a comparação de prioridade
            
            // Compara a Prioridade: procura o elemento com a menor prioridade
            if (torreComponentes[j].prioridade < torreComponentes[indiceMinimo].prioridade) {
                indiceMinimo = j;
            }
        }
        // Troca o elemento mínimo encontrado com o elemento da posição atual (i)
        if (indiceMinimo != i) {
            temp = torreComponentes[i];
            torreComponentes[i] = torreComponentes[indiceMinimo];
            torreComponentes[indiceMinimo] = temp;
        }
    }
    return comparacoes;
}

// ----------------------------------------------------
// 8. Algoritmo de Busca
// ----------------------------------------------------

/**
 * @brief Busca binária pelo componente-chave (Chip Central).
 * Requer ordenação prévia por NOME (Bubble Sort).
 */
void buscaBinariaPorNome() {
    if (statusOrdenacao != 1) {
        printf("\n[ERRO] A Busca Binária por Nome requer ordenação por NOME (Opção 1)!\n");
        return;
    }

    long long comparacoes = 0;
    int inicio = 0;
    int fim = totalComponentes - 1;
    int meio, resultadoComparacao;
    int encontrado = 0;

    // Implementação da Busca Binária
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        
        // Compara o nome no meio com o nome chave
        resultadoComparacao = strcmp(torreComponentes[meio].nome, NOME_CHAVE);
        comparacoes++; 
        
        if (resultadoComparacao == 0) {
            encontrado = 1;
            break; // Componente-chave encontrado
        } else if (resultadoComparacao < 0) {
            // Nome chave é 'maior', busca na metade direita
            inicio = meio + 1;
        } else {
            // Nome chave é 'menor', busca na metade esquerda
            fim = meio - 1;
        }
    }

    printf("\n--- RESULTADO DA BUSCA BINÁRIA ---\n");
    printf("Componente Buscado: %s\n", NOME_CHAVE);

    if (encontrado) {
        printf("SUCESSO! Componente-Chave Encontrado na posição %d.\n", meio + 1);
        printf(" -> Prioridade de Montagem: %d\n", torreComponentes[meio].prioridade);
    } else {
        printf(" FALHA! Componente-Chave '%s' não foi encontrado. A torre não pode ser ativada.\n", NOME_CHAVE);
    }
    printf("Comparações Realizadas: %lld\n", comparacoes);
    printf("----------------------------------\n");
}