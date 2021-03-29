#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "Imagem.h"
#include "Filtros.h"

int main(int argc, char *argv[]) {

    Imagem *imagem = NULL; //Ponteiro para a imagem
    Imagem *imagemOriginal = NULL;
    char nomeArq[100];  //Nome do arquivo

    if(argc == 2) {
        strcpy(nomeArq, argv[1]);
    }
    else {
        printf("Digite o nome do arquivo ppm: ");
        scanf("%99s[^\n]", nomeArq);
    }

    imagem = carregaImagem(nomeArq);
    
    if (imagem == NULL) {
        AVISO("Erro ao carregar a imagem");
        return 0;
    }

    imagemOriginal = copiaImagem(imagem);

    escurecerImagem(imagem);
    salvaImagem(imagem, "imagemEscurecida.ppm");
    liberaImagem(imagem);
    
    imagem = copiaImagem(imagemOriginal);

    clarearImagem(imagem);
    salvaImagem(imagem, "imagemClareada.ppm");
    liberaImagem(imagem);
    imagem = copiaImagem(imagemOriginal);

    escalaDeCinzaImagem(imagem);
    salvaImagem(imagem, "imagemEscalaDeCinza.ppm");
    liberaImagem(imagem);
    imagem = copiaImagem(imagemOriginal);

    filtroSobel(imagem);
    salvaImagem(imagem, "imagemFiltroSobel.ppm");
    liberaImagem(imagem);
    imagem = copiaImagem(imagemOriginal);

    deteccaoBordasLaplace(imagem);
    salvaImagem(imagem, "imagemBordasLaplace.ppm");
    liberaImagem(imagem);
    imagem = copiaImagem(imagemOriginal);

    meuFiltro(imagem);
    salvaImagem(imagem, "imagemMeuFiltro.ppm");
    liberaImagem(imagem);
    imagem = copiaImagem(imagemOriginal);

    salvaImagem(imagemOriginal, "imagemOriginal.ppm");

    liberaImagem(imagem);
    liberaImagem(imagemOriginal);
    return 0;
}


