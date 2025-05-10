#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Constantes para limites máximos
#define maximo 100         // Capacidade máxima da lista
#define maximo_marca 21    // Tamanho máximo para nomes de marcas
#define maximo_modelo 31   // Tamanho máximo para modelos

// Estrutura para armazenar dados de smartphones
typedef struct {
    char marca[maximo_marca];   // Nome da marca
    char modelo[maximo_modelo]; // Modelo do aparelho
    int ano;                    // Ano de lançamento
    int memoria;                // Capacidade de armazenamento em GB
    float preco;                // Preço em reais
} Smartphone;

// -----------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------
int encontrar_posicao(Smartphone lista[], int n, float preco);  // Encontra posição para inserção ordenada
void inserir_ordenado(Smartphone lista[], int *n, Smartphone novo);  // Insere smartphone mantendo ordem
int ler_arquivo(Smartphone lista[], int *n);  // Carrega dados do arquivo
void exibir_todos(Smartphone lista[], int n);  // Mostra todos os registros
void buscar_por_marca(Smartphone lista[], int n, const char *marca_busca);  // Filtra por marca
void exibir_por_faixa_preco(Smartphone lista[], int n, float preco_min, float preco_max);  // Filtra por preço
void filtrar_combinado(Smartphone lista[], int n);  // Filtro com múltiplos critérios
void inserir_smartphone(Smartphone smartphones[], int *n);  // Adiciona novo smartphone
void remover_por_memoria(Smartphone smartphones[], int *n, int memoria_min);  // Remove por memória mínima
void limpar_buffer();  // Limpa buffer de entrada

// -----------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------
int main() {
    system("chcp 65001");  // Configura codificação para UTF-8 (Windows)

    Smartphone lista[maximo];  // Lista de smartphones
    int quantidade = 0;        // Quantidade atual de itens
    int opcao;                 // Opção do menu

    // Carrega dados do arquivo
    if (!ler_arquivo(lista, &quantidade)) {
        printf("Erro ao carregar dados. Verifique o arquivo smartphones.txt\n");
        return 1;
    }

    // Loop do menu principal
    do {
        printf("\n=== MENU ===\n");
        printf("1. Exibir todos\n");
        printf("2. Buscar por marca\n");
        printf("3. Filtrar por preco\n");
        printf("4. Filtro combinado\n");
        printf("5. Inserir novo\n");
        printf("6. Remover por memoria\n");
        printf("7. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();  // Limpa buffer após scanf

        // Executa operação escolhida
        switch(opcao) {
            case 1: 
                exibir_todos(lista, quantidade);  // Lista completa
                break;
            case 2: {  // Busca por marca
                char marca[21];
                printf("Marca: ");
                fgets(marca, 21, stdin);
                marca[strcspn(marca, "\n")] = '\0';
                buscar_por_marca(lista, quantidade, marca);
                break;
            }
            case 3: {  // Filtro de preço
                float min, max;
                printf("Preco minimo: ");
                scanf("%f", &min);
                printf("Preco maximo: ");
                scanf("%f", &max);
                limpar_buffer();
                exibir_por_faixa_preco(lista, quantidade, min, max);
                break;
            }
            case 4: 
                filtrar_combinado(lista, quantidade);  // Filtro avançado
                break;
            case 5:  // Inserção de novo
                if(quantidade < maximo) 
                    inserir_smartphone(lista, &quantidade);
                else 
                    printf("Limite maximo atingido!\n");
                break;
            case 6: {  // Remoção por memória
                int mem;
                printf("Memoria minima: ");
                scanf("%d", &mem);
                limpar_buffer();
                remover_por_memoria(lista, &quantidade, mem);
                break;
            }
            case 7: 
                printf("Saindo...\n"); 
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while(opcao != 7);

    return 0;
}

// -----------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// -----------------------------------------------

// Limpa o buffer de entrada para evitar problemas com entradas residuais
void limpar_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

// Encontra a posição correta para inserir mantendo ordenação por preço
int encontrar_posicao(Smartphone lista[], int n, float preco) {
    int i = 0;
    while (i < n && lista[i].preco < preco) i++;
    return i;
}

// Insere novo smartphone na posição correta (ordenação por preço)
void inserir_ordenado(Smartphone lista[], int *n, Smartphone novo) {
    if(*n >= maximo) {
        printf("Lista cheia!\n");
        return;
    }
    int pos = encontrar_posicao(lista, *n, novo.preco);
    for(int i = *n; i > pos; i--) 
        lista[i] = lista[i-1];
    lista[pos] = novo;
    (*n)++;
}

// Carrega dados do arquivo para a lista
int ler_arquivo(Smartphone lista[], int *n) {
    FILE *arq = fopen("smartphones.txt", "r");
    if(arq == NULL) return 0;

    Smartphone temp;
    while(fgets(temp.marca, maximo_marca, arq)) {
        // Processa dados do arquivo
        temp.marca[strcspn(temp.marca, "\n")] = '\0';
        fgets(temp.modelo, maximo_modelo, arq);
        temp.modelo[strcspn(temp.modelo, "\n")] = '\0';

        fscanf(arq, "%d %d %f", &temp.ano, &temp.memoria, &temp.preco);
        fgetc(arq);  // Remove \n residual

        inserir_ordenado(lista, n, temp);  // Insere ordenadamente
    }
    fclose(arq);
    return 1;
}

// Exibe todos os smartphones em formato de tabela
void exibir_todos(Smartphone lista[], int n) {
    printf("\n%-20s %-20s %4s %7s %10s\n", "Marca", "Modelo", "Ano", "Memoria", "Preco");
    for(int i = 0; i < n; i++) {
        printf("%-20s %-20s %4d %5d GB %10.2f\n",
            lista[i].marca,
            lista[i].modelo,
            lista[i].ano,
            lista[i].memoria,
            lista[i].preco
        );
    }
    printf("\nTotal: %d\n", n);
}

// Busca smartphones por marca (case insensitive)
void buscar_por_marca(Smartphone lista[], int n, const char *marca_busca) {
    int encontrados = 0;
    printf("\nResultados para '%s':\n", marca_busca);
    for(int i = 0; i < n; i++) {
        if(strcasecmp(lista[i].marca, marca_busca) == 0) {
            printf("%-20s %-20s %4d %5d GB %10.2f\n",
                lista[i].marca,
                lista[i].modelo,
                lista[i].ano,
                lista[i].memoria,
                lista[i].preco
            );
            encontrados++;
        }
    }
    printf(encontrados == 0 ? "Nenhum encontrado!\n" : "\nTotal: %d\n", encontrados);
}

// Filtra smartphones por faixa de preço
void exibir_por_faixa_preco(Smartphone lista[], int n, float min, float max) {
    int encontrados = 0;
    printf("\nSmartphones entre R$%.2f e R$%.2f:\n", min, max);
    for(int i = 0; i < n; i++) {
        if(lista[i].preco >= min && lista[i].preco <= max) {
            printf("%-20s %-20s %4d %5d GB %10.2f\n",
                lista[i].marca,
                lista[i].modelo,
                lista[i].ano,
                lista[i].memoria,
                lista[i].preco
            );
            encontrados++;
        }
    }
    printf(encontrados == 0 ? "Nenhum encontrado!\n" : "\nTotal: %d\n", encontrados);
}

// Filtro avançado com múltiplos critérios combinados
void filtrar_combinado(Smartphone lista[], int n) {
    char marca[21];
    int mem_min = -1, mem_max = -1, ano_min = -1, ano_max = -1;
    float preco_min = -1, preco_max = -1;

    // Coleta critérios de filtragem
    printf("\n=== FILTRO COMBINADO ===\n");
    printf("Marca (deixe em branco para qualquer): ");
    fgets(marca, 21, stdin);
    marca[strcspn(marca, "\n")] = '\0';

    printf("Memoria minima (GB): ");
    scanf("%d", &mem_min);
    printf("Memoria maxima (GB): ");
    scanf("%d", &mem_max);

    printf("Ano minimo: ");
    scanf("%d", &ano_min);
    printf("Ano maximo: ");
    scanf("%d", &ano_max);

    printf("Preco minimo (R$): ");
    scanf("%f", &preco_min);
    printf("Preco maximo (R$): ");
    scanf("%f", &preco_max);
    limpar_buffer();

    // Aplica filtros e exibe resultados
    printf("\nResultados:\n");
    int encontrados = 0;
    for(int i = 0; i < n; i++) {
        if(
            (strlen(marca) == 0 || strcasecmp(lista[i].marca, marca) == 0) &&
            (mem_min == -1 || lista[i].memoria >= mem_min) &&
            (mem_max == -1 || lista[i].memoria <= mem_max) &&
            (ano_min == -1 || lista[i].ano >= ano_min) &&
            (ano_max == -1 || lista[i].ano <= ano_max) &&
            (preco_min == -1 || lista[i].preco >= preco_min) &&
            (preco_max == -1 || lista[i].preco <= preco_max)
        ) {
            printf("%-20s %-20s %4d %5d GB %10.2f\n",
                lista[i].marca,
                lista[i].modelo,
                lista[i].ano,
                lista[i].memoria,
                lista[i].preco
            );
            encontrados++;
        }
    }
    printf(encontrados == 0 ? "Nenhum resultado!\n" : "\nTotal: %d\n", encontrados);
}

// Adiciona novo smartphone à lista (com ordenação)
void inserir_smartphone(Smartphone smartphones[], int *n) {
    Smartphone novo;
    printf("\nNovo smartphone:\n");
    
    // Coleta dados do novo smartphone
    printf("Marca: ");
    fgets(novo.marca, 21, stdin);
    novo.marca[strcspn(novo.marca, "\n")] = '\0';
    
    printf("Modelo: ");
    fgets(novo.modelo, 31, stdin);
    novo.modelo[strcspn(novo.modelo, "\n")] = '\0';
    
    printf("Ano: ");
    scanf("%d", &novo.ano);
    
    printf("Memoria (GB): ");
    scanf("%d", &novo.memoria);
    
    printf("Preco (R$): ");
    scanf("%f", &novo.preco);
    limpar_buffer();
    
    inserir_ordenado(smartphones, n, novo);  // Insere ordenadamente
    printf("Adicionado com sucesso!\n");
}

// Remove smartphones com memória abaixo do valor especificado
void remover_por_memoria(Smartphone smartphones[], int *n, int memoria_min) {
    int novo_indice = 0;
    for(int i = 0; i < *n; i++) {
        if(smartphones[i].memoria >= memoria_min) {
            smartphones[novo_indice++] = smartphones[i];  // Mantém os que atendem
        }
    }
    printf("%d removido(s)\n", *n - novo_indice);
    *n = novo_indice;  // Atualiza quantidade
}