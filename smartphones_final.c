#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Biblioteca para funções gerais, como system() que usamos no main()

#define maximo 100             // Número máximo de smartphones na lista
#define maximo_marca 21        // Tamanho máximo da string da marca (20 + '\0')
#define maximo_modelo 31       // Tamanho máximo da string do modelo (30 + '\0')

// Estrutura que representa um smartphone com marca, modelo, ano, memória e preço
typedef struct {
    char marca[maximo_marca];
    char modelo[maximo_modelo];
    int ano;
    int memoria;
    float preco;
} Smartphone;

// Encontra a posição correta na lista para inserir um novo smartphone, de forma que a lista continue ordenada por preço (crescente).
int encontrar_posicao(Smartphone lista[], int n, float preco) {
    int i = 0;
    // Avança enquanto o preço do item atual for menor que o do novo smartphone
    while (i < n && lista[i].preco < preco) {
        i++;
    }
    // Retorna a posição onde o novo item deve ser inserido
    return i;
} 

//Insere um novo smartphone na lista já ordenada, usando a posição encontrada pela função anterior, e desloca os elementos para não sobrescrever.
void inserir_ordenado(Smartphone lista[], int *n, Smartphone novo) {
    // Descobre onde o novo smartphone deve entrar
    int pos = encontrar_posicao(lista, *n, novo.preco);
    // Desloca os elementos para a direita, abrindo espaço para o novo item.
    for (int i = *n; i > pos; i--) {
        lista[i] = lista[i - 1];
    }
    // Insere o novo smartphone na posição certa
    lista[pos] = novo;
    (*n)++; // Atualiza a quantidade total de itens na lista
}

// Lê os smartphones de um arquivo chamado "smartphones.txt" e os insere ordenadamente na lista.
int ler_arquivo(Smartphone lista[], int *n) {
    FILE *arquivo = fopen("smartphones.txt", "r"); // Tenta abrir o arquivo no modo leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0; // Retorna 0 se não conseguiu abrir
    }

    Smartphone temp; // Variável temporária para armazenar dados de cada smartphone

    // Lê os dados do arquivo enquanto ainda houver linhas
    while (fgets(temp.marca, maximo_marca, arquivo)) {
        fgets(temp.modelo, maximo_modelo, arquivo); // Usa fgets para ler as strings (marca e modelo)
        fscanf(arquivo, "%d", &temp.ano);           // Lê o ano (inteiro)
        fscanf(arquivo, "%d", &temp.memoria);       // Lê a memória (inteiro)
        fscanf(arquivo, "%f", &temp.preco);         // Lê o preço (float)
        fgetc(arquivo); // Lê o \n depois do preço, só pra limpar o buffer

        // Remove o \n do final das strings (evita problemas na hora de comparar ou exibir)
        temp.marca[strcspn(temp.marca, "\n")] = '\0';
        temp.modelo[strcspn(temp.modelo, "\n")] = '\0';

        // Insere o smartphone lido na posição correta para manter a lista ordenada por preço
        inserir_ordenado(lista, n, temp);
    }

    fclose(arquivo); // Fecha o arquivo após a leitura
    return 1; // Retorna 1 se conseguiu ler com sucesso
}

// Exibe todos os smartphones na tela em formato de tabela organizada, mostrando: marca, modelo, ano, memória e preço.
void exibir_todos(Smartphone lista[], int n) {
    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");
    
    // Exibe os dados de cada smartphone da lista
    for (int i = 0; i < n; i++) {
        printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n", 
               lista[i].marca,
               lista[i].modelo,
               lista[i].ano,
               lista[i].memoria,
               lista[i].preco);
    }
    
    printf("+----------------------+----------------------+------+----------+------------+\n");
    
    printf("Total de smartphones: %d\n", n);
}


// Função para buscar smartphones por marca
// A busca ignora maiúsculas e minúsculas para tornar a pesquisa mais flexível.
void buscar_por_marca(Smartphone lista[], int n, const char *marca_busca) {
    int encontrados = 0; // Variável para contar quantos smartphones da marca foram encontrados.

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    // Percorre a lista de smartphones para encontrar os que correspondem à marca fornecida.
    for (int i = 0; i < n; i++) {
        // Compara a marca do smartphone com a marca buscada, aqui ignora maiúsculas e minúsculas.
        if (strcasecmp(lista[i].marca, marca_busca) == 0) { 
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++; // Incrementa o contador de smartphones encontrados.
        }
    }

    if (encontrados == 0) {
        printf("| %-74s |\n", "Nenhum smartphone encontrado com essa marca.");
    }

    printf("+----------------------+----------------------+------+----------+------------+\n");

    printf("Foram encontrados: %d dispositivos.\n", encontrados);
}

// Função para exibir smartphones cuja faixa de preço esteja dentro do intervalo especificado pelo usuário
void exibir_por_faixa_preco(Smartphone lista[], int n, float preco_min, float preco_max) {
    int encontrados = 0; // Variável para contar os smartphones encontrados na faixa de preço.

    printf("+----------------------+----------------------+------+----------+------------+\n");
    printf("| %-20s | %-20s | %-4s | %-9s | %-11s |\n", "Marca", "Modelo", "Ano", "Memória", "Preço");
    printf("+----------------------+----------------------+------+----------+------------+\n");

    // Percorre a lista de smartphones e verifica se o preço está dentro da faixa fornecida
    for (int i = 0; i < n; i++) {
        // Se o preço do smartphone está dentro da faixa desejada
        if (lista[i].preco >= preco_min && lista[i].preco <= preco_max) {
            printf("| %-20s | %-20s | %4d | %5d GB | R$ %6.2f |\n",
                   lista[i].marca,
                   lista[i].modelo,
                   lista[i].ano,
                   lista[i].memoria,
                   lista[i].preco);
            encontrados++; // Incrementa o contador de smartphones encontrados
        }
    }

    if (encontrados == 0) {
        printf("| %-75s |\n", 
               "Nenhum smartphone encontrado na faixa de preço fornecida.");
    }

    printf("+----------------------+----------------------+------+----------+------------+\n");

    printf("Foram encontrados: %d dispositivos.\n", encontrados);
}

// Insere um novo smartphone, com verificação de limite máximo
void inserir_smartphone(Smartphone smartphones[], int *n) {
    if (*n >= maximo) {
        printf("Limite máximo de smartphones atingido!\n");
        return;  // Não insere se o limite for atingido
    }

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
    printf("\nSmartphone inserido com sucesso!\n");
}

// Remove smartphones com memória inferior à informada pelo usuário
void remover_por_memoria(Smartphone smartphones[], int *n, int memoria_min) {
    int i, j = 0; // i é o índice de leitura e j é o índice de escrita (onde manteremos os smartphones válidos)
    
    // Itera por todos os smartphones
    for (i = 0; i < *n; i++) {
        // Se a memória do smartphone for maior ou igual ao valor mínimo informado
        if (smartphones[i].memoria >= memoria_min) {
            // Copia o smartphone válido para a posição j
            smartphones[j++] = smartphones[i];
        }
    }

    // Calcula quantos smartphones foram removidos
    int removidos = *n - j;
    *n = j; // Atualiza o tamanho da lista com os smartphones restantes

    // Exibe a quantidade de smartphones removidos, ou mensagem caso nenhum tenha sido removido
    if (removidos > 0)
        printf("%d smartphone(s) removido(s).\n", removidos);
    else
        printf("Nenhum smartphone removido.\n");
}

int main() {
    system("chcp 65001");  // Altera o código de página para UTF-8, para acentos funcionarem´[]

    Smartphone lista[maximo];  // Declara o vetor que irá armazenar os smartphones
    int quantidade = 0;        // Variável que armazena a quantidade atual de smartphones na lista
    int opcao;                 // Variável para armazenar a opção selecionada pelo usuário

    if (ler_arquivo(lista, &quantidade)) {
        printf("\nIniciando Catalogador de Smartphones!\n");

        do {
            // Exibe o menu com as opções disponíveis
            printf("\n====== MENU ======\n");
            printf("1. Exibir todos os registros\n");
            printf("2. Exibir registros de uma marca específica\n");
            printf("3. Exibir registros por faixa de preço\n");
            printf("4. Inserir um novo registro\n");
            printf("5. Remover registros por memória\n");
            printf("6. Sair\n");
            printf("-> Digite sua opção: ");
            scanf("%d", &opcao);  // Lê a opção do usuário
            getchar();  // Limpa o buffer do teclado para evitar problemas com fgets

            // Ação a ser tomada de acordo com a opção escolhida
            switch (opcao) {
                case 1:
                    printf("\n====== TODOS OS SMARTPHONES ======\n");
                    exibir_todos(lista, quantidade);
                    break;
                case 2: {
                    char marca_procurada[maximo_marca];  // Variável para armazenar a marca buscada
                    printf("\n====== EXIBIR SMARTPHONES POR MARCA ======\n");
                    printf("Digite a marca que deseja buscar: ");
                    fgets(marca_procurada, maximo_marca, stdin);  // Lê a marca inserida pelo usuário
                    marca_procurada[strcspn(marca_procurada, "\n")] = '\0';  // Remove o '\n' do final da string
                    buscar_por_marca(lista, quantidade, marca_procurada);
                    break;
                }
                case 3: {
                    float min, max;  // Variáveis para armazenar o preço mínimo e máximo
                    printf("\n====== EXIBIR SMARTPHONES POR FAIXA DE PREÇO ======\n");
                    printf("Digite o preço mínimo: ");
                    scanf("%f", &min);  // Lê o preço mínimo
                    printf("Digite o preço máximo: ");
                    scanf("%f", &max);  // Lê o preço máximo
                    exibir_por_faixa_preco(lista, quantidade, min, max);
                    break;
                }
                case 4:
                    printf("\n====== INSERIR SMARTPHONE ======\n");
                    printf("Entre com as informações do dispositivo.\n\n");
                    inserir_smartphone(lista, &quantidade);
                    break;
                case 5: {
                    int memoria_min;  // Variável para armazenar o valor mínimo de memória
                    printf("\n====== REMOVER SMARTPHONES POR MEMÓRIA ======\n");
                    printf("Digite a memória mínima (GB): ");
                    scanf("%d", &memoria_min);  // Lê a memória mínima desejada
                    remover_por_memoria(lista, &quantidade, memoria_min);
                    break;
                }
                case 6:
                    printf("\nSaindo do programa...\n");
                    printf("\n====== AGRADECIMENTO ======\n");
                    printf("-> Obrigado por utilizar o Catalogador de Smartphones!\n");
                    printf("\n-> Desenvolvido por Abílio, Isabelly, Pedro e Samuel, graduandos de Ciência da Computação 2024.2 da PUCPR.\n");
                    printf("\n-> Projeto da disciplina de Programação Imperativa, ministrada pelo professor Alcides Calsavara.\n");
                    printf("\n");
                    break;
                default:
                    // Caso o usuário insira uma opção inválida
                    printf("Opção inválida. Tente novamente.\n");
            }

        } while (opcao != 6);  // O loop continua até o usuário escolher a opção 6 (sair)
    }

    return 0;  // Finaliza o programa
}