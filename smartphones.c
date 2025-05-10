#include <stdio.h>
#include <string.h>

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

//Lê os smartphones de um arquivo chamado "smartphones.txt" e os insere ordenadamente na lista.
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


int main() {
    Smartphone lista[maximo]; //Cria a lista de smartphones.
    int quantidade = 0;

    if (ler_arquivo(lista, &quantidade)) { //Chama a função que le o arquivo e preenche o vetor lista
        printf("\nIniciando Catalogador de Smartphones!\n");
        printf("Leitura concluida.\n");
        exibir_todos(lista, quantidade);

        char marca_procurada[maximo_marca];

        printf("Digite a marca que deseja buscar: ");
        fgets(marca_procurada, maximo_marca, stdin); //le o que o usuário digitou e armazena no vetor marca procurada
        marca_procurada[strcspn(marca_procurada, "\n")] = '\0'; // retorna o índice do primeiro \n encontrado.

        buscar_por_marca(lista, quantidade, marca_procurada);

        float min, max;
        printf("\n\nDigite o preço mínimo: ");
        scanf("%f", &min);
        printf("Digite o preço máximo: ");
        scanf("%f", &max);

        exibir_por_faixa_preco(lista, quantidade, min, max);

    }

    return 0;
}

/*
No main eu ja coloquei pra chamar a função que exibe todos os celulares pra mostrar q ta
funcionando, mas no programa final, o usuario deve escolher entre as opções, ent nao precisa
mostrar assim q rodar o codigo.
*/