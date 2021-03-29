#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>   

#include "Imagem.h"

struct imagem {
    int largura;   /* número de colunas (largura) da imagem em pixels */
    int altura;    /* número de linhas (altura) da imagem em pixels */
    Pixel **pixel; /* matriz altura x largura de pixels com os níveis RGB */
};

void * mallocSafe(size_t nbytes);

Imagem *alocaImagem(int largura, int altura){
    Imagem *img = (Imagem *) mallocSafe(sizeof(Imagem)); 
    img->altura = altura;
    img->largura = largura;
    img->pixel = (Pixel **) mallocSafe(altura * sizeof(Pixel*));
    for (int i = 0; i < altura; i++) {
        img->pixel[i] = (Pixel *) mallocSafe(largura * sizeof(Pixel));
    }
    return img;
}

void liberaImagem(Imagem *img){
    if (img != NULL) {
        for (int i = 0; i <img->altura; i++) {
            free(img->pixel[i]);
        }
        free(img->pixel);
        free(img);
    } else {
        printf("Espaço nao alocado");
    }
    /* Lembre-se sempre de verificar se o dado foi alocado antes de fazer a desalocação */
}

int obtemLargura(Imagem *img){
    return img->largura;
}

int obtemAltura(Imagem *img){
    return img->altura;
}

Pixel obtemPixel(Imagem *img, int l, int c){
    return img->pixel[l][c];
}

Imagem *copiaImagem(Imagem *origem){
   if (origem != NULL) {
       Imagem *dest = (Imagem *) mallocSafe(sizeof(Imagem)); 
        dest->altura = origem->altura;
        dest->largura = origem->largura;
        dest->pixel = (Pixel **) mallocSafe(origem->altura * sizeof(Pixel*));
        for (int i = 0; i < origem->altura; i++) {
            dest->pixel[i] = (Pixel *) mallocSafe(origem->largura * sizeof(Pixel));
        }
       for (int i = 0; i < origem->altura; i++) {
           for (int j = 0; j < origem->largura; j++) {
               dest->pixel[i][j].cor[RED] = origem->pixel[i][j].cor[RED];
               dest->pixel[i][j].cor[GREEN] = origem->pixel[i][j].cor[GREEN];
               dest->pixel[i][j].cor[BLUE] = origem->pixel[i][j].cor[BLUE];
           }
       }
       return dest;
   } else {
       printf("VALOR NULO PASSADO COMO PARAMETRO");
       return NULL;
   }

    
}

void recolorePixel(Imagem *img, int l, int c, Pixel pixel){
    img->pixel[l][c].cor[RED] = pixel.cor[RED];
    img->pixel[l][c].cor[GREEN] = pixel.cor[GREEN];
    img->pixel[l][c].cor[BLUE] = pixel.cor[BLUE];
}

Imagem *carregaImagem(const char *nomeArquivo){
    char buff[16];
    Imagem *img;
    FILE *fp;
    int c;
    fp = fopen(nomeArquivo, "r");

    if (fp == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    if (!fgets(buff, sizeof(buff), fp)) {
        perror("stdin");
        exit(1);
    }

    if (buff[0] != 'P' || buff[1] != '3') {
        fprintf(stderr, "Formato da imagem invalido (deve ser 'P3')\n");
        exit(1);
    }
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp); /* coloca ultimo caractere devolta no buffer */
    int largura, altura, threshold;
    if (fscanf(fp, "%d %d", &largura, &altura) != 2) {
        fprintf(stderr, "Tamanho da imagem invalido\n");
        exit(1);
    }

    if (fscanf(fp, "%d", &threshold) != 1) {
        fprintf(stderr, "Componente rgb invalido\n");
        exit(1);
    }

    while (fgetc(fp) != '\n') ;

    img = alocaImagem(largura, altura);
    if (!img) {
        fprintf(stderr, "Erro na alocacao da memoria\n");
        exit(1);
    }
    img->altura = altura;
    img->largura = largura;

    int r, g, b;
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            c = fscanf(fp, "%d %d %d", &r, &g, &b);
            img->pixel[i][j].cor[RED]   = (Byte)r; /* intensidade vermelho */
            img->pixel[i][j].cor[GREEN] = (Byte)g; /* intensidade verde */
            img->pixel[i][j].cor[BLUE]  = (Byte)b; /* intensidade azul */
        }
    }
    fclose(fp);

    return img;
}

void salvaImagem(Imagem *img, const char *nomeArquivo){
    FILE *arquivo;
    int c, rgb_comp_color;
    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "salvaImagem: ERRO: arquivo '%s' nao pode ser criado\n", nomeArquivo);
        return;
    }
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "#EP1 - Estrutura de Dados 1 - Lucas Raniere\n");
    fprintf(arquivo, "%d %d %d\n", img->largura, img->altura, 255);
    for (int i = 0; i< img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            fprintf(arquivo, "%d\n%d\n%d\n", img->pixel[i][j].cor[RED],
            img->pixel[i][j].cor[GREEN],  img->pixel[i][j].cor[BLUE]);
        }
    }

    fclose(arquivo);
    printf("salvaImagem: A imagem foi salva no arquivo: '%s'\n", nomeArquivo);
}

/*  
   mallocSafe 
   Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
void *mallocSafe(size_t nbytes) {
    void *ptr = malloc(nbytes);
    if (ptr == NULL) {
        fprintf(stderr, "Sem memória!\n");
        exit(1);
    }
    return ptr;
}
