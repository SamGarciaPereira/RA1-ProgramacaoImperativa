#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maximo 100
#define maximo_marca 21
#define maximo_modelo 31

// Define o tipo de smartphone que guarda informações de um celular
typedef struct { //Agrupa diferentes tipos de dados em uma estrutura
    char marca[maximo_marca];
    char modelo[maximo_modelo];
    int ano;
    int memoria;
    float preco;
} Smartphone;

//Encontra a posição correta na lista para inserir um novo smartphone, de forma que a lista continue ordenada por preço (crescente).
int encontrar_posicao(Smartphone lista[], int n, float preco) {
    int i = 0;
    while (i < n && lista[i].preco < preco) {
        i++;
    }
    return i;
} 

//Insere um novo smartphone na lista já ordenada, usando a posição encontrada pela função anterior, e desloca os elementos para não sobrescrever.
void inserir_ordenado(Smartphone lista[], int *n, Smartphone novo) {
    int pos = encontrar_posicao(lista, *n, novo.preco);
    for (int i = *n; i > pos; i--) {
        lista[i] = lista[i - 1];
    }
    lista[pos] = novo;
    (*n)++;
}

// Lê os smartphones de um arquivo chamado "smartphones.txt" e os insere ordenadamente na lista.
int ler_arquivo(Smartphone lista[], int *n) {
    FILE *arquivo = fopen("smartphones.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Smartphone temp; // Variável temporária para armazenar dados de cada smartphone

    while (fgets(temp.marca, maximo_marca, arquivo)) { //le o texto armazena ele em temp.marca, numero maximo e a entrada é o arquivo
        fgets(temp.modelo, maximo_modelo, arquivo); //Usa fgets para ler as strings (marca e modelo).
        fscanf(arquivo, "%d", &temp.ano); //le os dados do arquivo, %d representa inteiro e vai armazenar esse numero na variavel ano dentro de temp
        fscanf(arquivo, "%d", &temp.memoria);
        fscanf(arquivo, "%f", &temp.preco);
        fgetc(arquivo);

        temp.marca[strcspn(temp.marca, "\n")] = '\0'; //Remove o \n do final das strings.
        temp.modelo[strcspn(temp.modelo, "\n")] = '\0';

        inserir_ordenado(lista, n, temp);
    }

    fclose(arquivo);
    return 1; //Retorna 1 se conseguiu ler com sucesso, ou 0 se deu erro.
}

//Exibe todos os smartphones na tela em formato de tabela organizada, mostrando: marca, modelo, ano, memória e preço.
void exibir_todos(Smartphone lista[], int n) {
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

// Função para buscar smartphones por marca
void buscar_por_marca(Smartphone lista[], int n, const char *marca_busca) {
    int encontrados = 0;

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    for (int i = 0; i < n; i++) {
        if (strcasecmp(lista[i].marca, marca_busca) == 0) { //strcasecmp é pra ignorar diferença de minuscula ou maiuscula
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("| %-76s |\n", "Nenhum smartphone encontrado com essa marca.");
    }

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("Foram encontrados: %d dispositivos.", encontrados);
}

// Função para exibir smartphones por faixa de preço
void exibir_por_faixa_preco(Smartphone lista[], int n, float preco_min, float preco_max) {
    int encontrados = 0;

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    for (int i = 0; i < n; i++) {
        if (lista[i].preco >= preco_min && lista[i].preco <= preco_max) {
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++;
        }
    }

    printf("+----------------------+----------------------+------+----------+------------+\n");

    if (encontrados == 0) {
        printf("Nenhum smartphone encontrado na faixa de preço R$ %.2f a R$ %.2f.\n", preco_min, preco_max);
    } else {
        printf("Foram encontrados: %d", encontrados);
    }
}

// Lê dados do usuário e insere no vetor
void inserir_smartphone(Smartphone smartphones[], int *n) {
    Smartphone novo;
    printf("Marca: ");
    fgets(novo.marca, maximo_marca, stdin);
    novo.marca[strcspn(novo.marca, "\n")] = '\0';

    printf("Modelo: ");
    fgets(novo.modelo, maximo_modelo, stdin);
    novo.modelo[strcspn(novo.modelo, "\n")] = '\0';

    printf("Ano: ");
    scanf("%d", &novo.ano);

    printf("Memoria (GB): ");
    scanf("%d", &novo.memoria);

    printf("Preco (R$): ");
    scanf("%f", &novo.preco);

    inserir_ordenado(smartphones, n, novo);
    printf("Smartphone inserido com sucesso!\n");
}

// Remove smartphones com memória inferior à informada
void remover_por_memoria(Smartphone smartphones[], int *n, int memoria_min) {
    int i, j = 0;
    for (i = 0; i < *n; i++) {
        if (smartphones[i].memoria >= memoria_min) {
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

int main() {
    system("chcp 65001"); // Esse comando faz os acentos funcionarem, tem que deixar no topo.

}

/*
Opa! Mudei o nome das funções para o padrão que estávamos usando:
minúsculo e separado com underline.

---> O código não vai rodar pq não tem nada na função int main().

Falta fazer ela seguindo o que o professor pediu:

Operações permitidas no programa:
1. Exibir a relação completa de registros de smartphones.
2. Exibir todos os registros de smartphones de uma marca específica,
   fornecida pelo usuário via teclado.
3. Exibir todos os registros de smartphones cujo preço esteja entre
   um valor mínimo e um valor máximo, fornecidos pelo usuário via teclado.
4. Inserir um novo registro de smartphone no vetor. Todos os dados
   do smartphone devem ser fornecidos pelo usuário via teclado.
   A inserção deve manter a ordem não-decrescente do vetor com respeito aos preços.
5. Remover todos os registros de smartphones do vetor cuja memória
   seja inferior a um valor fornecido pelo usuário via teclado.
6. Sair do programa. A interação com o usuário deve ficar em loop,
   permitindo que as operações acima sejam realizadas até que o usuário opte por sair.
*/