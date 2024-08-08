#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]) {
    FILE *fpointer;
    int n, m, fsize = 0;
    char filename[30];
    char c;
    if (argc != 4) {
        printf("\n please use 4 arguments - file name, text name, number of fragments and fragment size");
        return 0;
    }
    strcpy(filename, argv[1]); //copia o que se encontra na posição 1 dos argumentos para o filename
    sscanf(argv[2], "%i", &n); //guarda o que está na posição 2 na variável n (número do fragmento)
    sscanf(argv[3], "%i", &m); //guarda o que está na posição 2 na variável m (tamanho de cada fragmento)
    fpointer = fopen(filename, "r"); //abre o ficheiro para leitura
    if (fpointer == NULL) {
        printf("could not open file %s", filename);
        return 0;
    }
    for (c = fgetc(fpointer); c != EOF; c = fgetc(fpointer))
        fsize++; //contar o número de caracteres do ficheiro
    srand(0);
    int idx,j;
    for(int i=0;i<n;i++) {
        //printf("im in the loop");
        idx = rand() % (fsize - m); // número entre 0 e filesize
        //printf("%d\n", idx);
        fseek(fpointer, idx, SEEK_SET); //indíce do número aleatório
        j = 0;
        printf(">");
        // imprime caracter a caracter até atingir o final do ficheiro ou o tamanho do fragmento m
        while(j!=m){
            c=fgetc(fpointer);
            printf("%c",c);
            j++;
        }
        printf("<\n");
    }
    fclose(fpointer);
}