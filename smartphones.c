#include <stdio.h>
#include <string.h>

#define maximo 100
#define maximo_marca 30
#define maximo_modelo 40

typedef struct {
    char marca[maximo_marca];
    char modelo[maximo_modelo];
    int ano;
    int memoria;
    float preco;
} Smartphone;

int encontrar_posicao(Smartphone lista[], int n, float preco) {
    int i = 0;
    while (i < n && lista[i].preco < preco) {
        i++;
    }
    return i;
} 

void inserir_ordenado(Smartphone lista[], int *n, Smartphone novo) {
    int pos = encontrar_posicao(lista, *n, novo.preco);
    for (int i = *n; i > pos; i--) {
        lista[i] = lista[i - 1];
    }
    lista[pos] = novo;
    (*n)++;
}

int ler_arquivo(Smartphone lista[], int *n) {
    FILE *arquivo = fopen("smartphones.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Smartphone temp;
    while (fgets(temp.marca, maximo_marca, arquivo)) {
        fgets(temp.modelo, maximo_modelo, arquivo);
        fscanf(arquivo, "%d", &temp.ano);
        fscanf(arquivo, "%d", &temp.memoria);
        fscanf(arquivo, "%f", &temp.preco);
        fgetc(arquivo);

        temp.marca[strcspn(temp.marca, "\n")] = '\0';
        temp.modelo[strcspn(temp.modelo, "\n")] = '\0';

        inserir_ordenado(lista, n, temp);
    }

    fclose(arquivo);
    return 1;
}

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

int main() {
    Smartphone lista[maximo];
    int quantidade = 0;

    if (ler_arquivo(lista, &quantidade)) {
        printf("\nIniciando Catalogador de Smartphones!\n");
        printf("Leitura concluida. Total de smartphones: %d\n", quantidade);
        exibir_todos(lista, quantidade);
    }

    return 0;
}

/*
No main eu ja coloquei pra chamar a função que exibe todos os celulares pra mostrar q ta
funcionando, mas no programa final, o usuario deve escolher entre as opções, ent nao precisa
mostrar assim q rodar o codigo.
*/