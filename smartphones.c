#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAX_SMARTPHONES 1000
#define MAX_MARCA 21
#define MAX_MODELO 31
// Define o tipo de smartphone que guarda informações de um celular
typedef struct { //Agrupa diferentes tipos de dados em uma estrutura
    char marca[MAX_MARCA];
    char modelo[MAX_MODELO];
    int ano;
    int memoria;
    float preco;
} Smartphone;

// Insere ordenado por preço
void inserirOrdenado(Smartphone smartphones[], int *n, Smartphone novo) {
    if (*n >= MAX_SMARTPHONES) {
        printf("Capacidade máxima atingida.\n");
        return;
    }
    int i = *n - 1;
    while (i >= 0 && smartphones[i].preco > novo.preco) {
        smartphones[i + 1] = smartphones[i];
        i--;
    }
    smartphones[i + 1] = novo;
    (*n)++;
}

// Lê dados do usuário e insere no vetor
void inserirSmartphone(Smartphone smartphones[], int *n) {
    Smartphone novo;
    printf("Marca: ");
    scanf(" %[^\n]", novo.marca);
    printf("Modelo: ");
    scanf(" %[^\n]", novo.modelo);
    printf("Ano: ");
    scanf("%d", &novo.ano);
    printf("Memoria (GB): ");
    scanf("%d", &novo.memoria);
    printf("Preco (R$): ");
    scanf("%f", &novo.preco);

    inserirOrdenado(smartphones, n, novo);
    printf("Smartphone inserido com sucesso!\n");
}

// Remove smartphones com memória inferior à informada
void removerPorMemoria(Smartphone smartphones[], int *n, int memoriaMin) {
    int i, j = 0;
    for (i = 0; i < *n; i++) {
        if (smartphones[i].memoria >= memoriaMin) {
            smartphones[j++] = smartphones[i];
        }
    }
    int removidos = *n - j;
    *n = j;

    if (removidos > 0)
        printf("%d smartphone(s) removido(s).\n", removidos);
    else
        printf("Nenhum smartphone removido.\n");
}

// Lê smartphones do arquivo
int lerArquivo(Smartphone lista[], int *n) {
    FILE *arquivo = fopen("smartphones.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Smartphone temp;
    while (fgets(temp.marca, MAX_MARCA, arquivo)) {
        fgets(temp.modelo, MAX_MODELO, arquivo);
        fscanf(arquivo, "%d", &temp.ano);
        fscanf(arquivo, "%d", &temp.memoria);
        fscanf(arquivo, "%f", &temp.preco);
        fgetc(arquivo); // consome '\n'

        temp.marca[strcspn(temp.marca, "\n")] = '\0';
        temp.modelo[strcspn(temp.modelo, "\n")] = '\0';

        inserirOrdenado(lista, n, temp);
    }

    fclose(arquivo);
    return 1;
}

// Exibe todos os smartphones
void exibirTodos(Smartphone lista[], int n) {
    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    for (int i = 0; i < n; i++) {
        printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
               lista[i].marca,
               lista[i].modelo,
               lista[i].ano,
               lista[i].memoria,
               lista[i].preco);
    }

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("Total de smartphones: %d\n\n", n);
}

// Buscar por marca
void buscarPorMarca(Smartphone lista[], int n) {
    char marcaBusca[MAX_MARCA];
    printf("Digite a marca que deseja buscar: ");
    scanf(" %[^\n]", marcaBusca);

    int encontrados = 0;
    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    for (int i = 0; i < n; i++) {
        if (strcasecmp(lista[i].marca, marcaBusca) == 0) {
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("| %-76s |\n", "Nenhum smartphone encontrado com essa marca.");

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("Foram encontrados: %d dispositivo(s).\n", encontrados);
}

// Exibe smartphones em uma faixa de preço
void exibirPorFaixaPreco(Smartphone lista[], int n) {
    float min, max;
    printf("Digite o preco minimo: ");
    scanf("%f", &min);
    printf("Digite o preco maximo: ");
    scanf("%f", &max);

    int encontrados = 0;
    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    for (int i = 0; i < n; i++) {
        if (lista[i].preco >= min && lista[i].preco <= max) {
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("Nenhum smartphone encontrado na faixa R$ %.2f - R$ %.2f.\n", min, max);
    else
        printf("Foram encontrados: %d smartphone(s).\n", encontrados);
}

/*
No main eu ja coloquei pra chamar a função que exibe todos os celulares pra mostrar q ta
funcionando, mas no programa final, o usuario deve escolher entre as opções, ent nao precisa
mostrar assim q rodar o codigo.
*/
