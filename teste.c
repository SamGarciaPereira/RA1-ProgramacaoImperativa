#include <stdio.h>

int main(){
    char nome[10]; // vetor de 10 caracteres
    
    printf("Entre com um nome: ");
    scanf("%s", nome);
    printf("nome = %s", nome);
    printf("\n");
    
    return 0;
}