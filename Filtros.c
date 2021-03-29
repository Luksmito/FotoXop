#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Imagem.h"
#include "Filtros.h"

typedef struct pesos {
    int red;
    int green;
    int blue;
} Pesos;

void escurecerImagem(Imagem *img){
    int v;
    printf("Digite o fator de escurecimento: ");
    scanf("%d", &v);
    int altura = obtemAltura(img);
    int largura = obtemLargura(img);
    for (int h = 0; h < altura; h++) {
        for (int w = 0; w < largura; w++) {
            Pixel pixel = obtemPixel(img, h, w);
            pixel.cor[RED]   = (((int)pixel.cor[RED]   - v) >= 0 ? (pixel.cor[RED]   - v) : 0);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] - v) >= 0 ? (pixel.cor[GREEN] - v) : 0);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE]  - v) >= 0 ? (pixel.cor[BLUE]  - v) : 0);
            recolorePixel(img, h, w, pixel);
        }
    }
}

void clarearImagem(Imagem *img){
   int v;
    printf("Digite o fator de clareamento: ");
    scanf("%d", &v);
    int altura = obtemAltura(img);
    int largura = obtemLargura(img);
    for (int h = 0; h < altura; h++) {
        for (int w = 0; w < largura; w++) {
            Pixel pixel = obtemPixel(img, h, w);
            pixel.cor[RED]   = (((int)pixel.cor[RED] + v) <= 255 ? (pixel.cor[RED] + v) : 255);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] + v) <= 255 ? (pixel.cor[GREEN] + v) : 255);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE] + v) <= 255 ? (pixel.cor[BLUE] + v) : 255);
            recolorePixel(img, h, w, pixel);
        }
    }

}

void escalaDeCinzaImagem(Imagem *img){
    int altura = obtemAltura(img);
    int largura = obtemLargura(img);

    for (int h = 0; h < altura; h++) {
        for (int w = 0; w < largura; w++) {
            Pixel pixel = obtemPixel(img, h, w);
            Byte escala = (pixel.cor[RED] + pixel.cor[GREEN] + pixel.cor[BLUE])/3;
            pixel.cor[RED] = escala;
            pixel.cor[GREEN] = escala;
            pixel.cor[BLUE] = escala;
            recolorePixel(img, h, w, pixel);
        }
    }
}

void filtroSobel(Imagem *img){
    
    int altura = obtemAltura(img);
    int largura = obtemLargura(img);

    Pesos **pesos = (Pesos **) calloc(altura, sizeof(Pesos *));
    for (int i = 0; i < altura; i++) {
        pesos[i] = (Pesos *) calloc(largura, sizeof(Pesos));
    }


    for (int i = 1, pesox = 0, pesoy = 0; i < altura - 1; i++, pesox = 0, pesoy = 0) {
        for (int j = 1; j < largura - 1; j++) {
            Pixel pixels[3][3];
            pixels[0][0] = obtemPixel(img, i-1, j-1), pixels[0][1] = obtemPixel(img, i-1, j), pixels[0][2] = obtemPixel(img, i-1, j+1);
            pixels[1][0] = obtemPixel(img, i, j-1), pixels[1][1] = obtemPixel(img, i,j), pixels[1][2] = obtemPixel(img, i, j+1);
            pixels[2][0] = obtemPixel(img, i+1, j-1), pixels[2][1] = obtemPixel(img, i+1, j), pixels[2][2] = obtemPixel(img, i+1, j+1);
        
            pesoy = (pixels[0][0].cor[RED]) + 
                    (pixels[0][1].cor[RED] * 2) + 
                    (pixels[0][2].cor[RED]) + 
                    (pixels[2][0].cor[RED] * -1) +
                    (pixels[2][1].cor[RED] * -2) + 
                    (pixels[2][2].cor[RED] * -1);

            pesox = (pixels[0][0].cor[RED]) + 
                    (pixels[1][0].cor[RED] * 2) + 
                    (pixels[2][0].cor[RED]) + 
                    (pixels[0][2].cor[RED] * -1) + 
                    (pixels[1][2].cor[RED] * -2) +
                    (pixels[2][2].cor[RED] * -1);

            pesos[i][j].red = sqrt(pow(pesox,2) + pow(pesoy,2));

            pesoy = (pixels[0][0].cor[GREEN]) + 
                    (pixels[0][1].cor[GREEN] * 2) + 
                    (pixels[0][2].cor[GREEN]) + 
                    (pixels[2][0].cor[GREEN] * -1) +
                    (pixels[2][1].cor[GREEN] * -2) + 
                    (pixels[2][2].cor[GREEN] * -1);

            pesox = (pixels[0][0].cor[GREEN]) + 
                    (pixels[1][0].cor[GREEN] * 2) + 
                    (pixels[2][0].cor[GREEN]) + 
                    (pixels[0][2].cor[GREEN] * -1) + 
                    (pixels[1][2].cor[GREEN] * -2) +
                    (pixels[2][2].cor[GREEN] * -1);

            pesos[i][j].green = sqrt(pow(pesox,2) + pow(pesoy,2));
            
            pesoy = (pixels[0][0].cor[BLUE]) + 
                    (pixels[0][1].cor[BLUE] * 2) + 
                    (pixels[0][2].cor[BLUE]) + 
                    (pixels[2][0].cor[BLUE] * -1) +
                    (pixels[2][1].cor[BLUE] * -2) + 
                    (pixels[2][2].cor[BLUE] * -1);

            pesox = (pixels[0][0].cor[BLUE]) + 
                    (pixels[1][0].cor[BLUE] * 2) + 
                    (pixels[2][0].cor[BLUE]) + 
                    (pixels[0][2].cor[BLUE] * -1) + 
                    (pixels[1][2].cor[BLUE] * -2) +
                    (pixels[2][2].cor[BLUE] * -1);

            pesos[i][j].blue = sqrt(pow(pesox,2) + pow(pesoy,2));

            
        
        } 
    }
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (j == 0 || j == altura || i == 0 || i == altura) {
                Pixel pixel = obtemPixel(img, i, j);
                pixel.cor[RED]   = 0;
                pixel.cor[GREEN] = 0;
                pixel.cor[BLUE]  = 0;
                recolorePixel(img, i, j, pixel);
            } else {
                Pixel pixel = obtemPixel(img, i, j);
                pixel.cor[RED]   = (pesos[i][j].red > 255) ? 255 : pesos[i][j].red;
                pixel.cor[GREEN] = (pesos[i][j].green > 255) ? 255 : pesos[i][j].green;
                pixel.cor[BLUE]  = (pesos[i][j].blue > 255) ? 255 : pesos[i][j].blue;
                recolorePixel(img, i, j, pixel);
            }
        }
    }
    for(int i = 0; i < altura; i++) {
        free(pesos[i]);
    }
    free(pesos);
    pesos = NULL;
}

void deteccaoBordasLaplace(Imagem *img) {
    //Retire esssa mensagem ao implementar a fução
    /* 0 -1  0
      -1  4 -1
       0 -1  0*/
    /* Siga as mesmas dicas do filtro de Sobel */

    int altura = obtemAltura(img);
    int largura = obtemLargura(img);
    Pixel **pesos = (Pixel **) calloc(altura, sizeof(Pixel *));
    for (int i = 0; i < altura; i++) {
        pesos[i] = (Pixel *) calloc(largura, sizeof(Pixel));
    }


    for (int i = 1, pesoRed = 0, pesoGreen = 0, pesoBlue = 0; i < altura - 1; i++, pesoRed = 0, pesoGreen = 0, pesoBlue = 0) {
        for (int j = 1; j < largura - 1; j++) {
            Pixel pixel[5] = {obtemPixel(img,i-1,j), obtemPixel(img, i, j-1), obtemPixel(img,i, j), 
            obtemPixel(img, i, j+1), obtemPixel(img, i+1, j)};
            
            pesoRed = (pixel[0].cor[RED] + pixel[1].cor[RED] + pixel[3].cor[RED] + pixel[4].cor[RED]) * -1
            + pixel[2].cor[RED]*4;

            pesoGreen = (pixel[0].cor[GREEN] + pixel[1].cor[GREEN] + pixel[3].cor[GREEN] + pixel[4].cor[GREEN]) * -1
            + pixel[2].cor[GREEN]*4;

            pesoBlue = (pixel[0].cor[BLUE] + pixel[1].cor[BLUE] + pixel[3].cor[BLUE] + pixel[4].cor[BLUE]) * -1
            + pixel[2].cor[BLUE]*4;

            pesoRed = (pesoRed < 0) ? 0 : (pesoRed > 255) ? 255 : pesoRed; 

            pesoGreen = (pesoGreen < 0) ? 0 : (pesoGreen > 255) ? 255 : pesoGreen; 

            pesoBlue = (pesoBlue < 0) ? 0 : (pesoBlue > 255) ? 255 : pesoBlue; 

            pesos[i][j].cor[RED] = pesoRed;
            pesos[i][j].cor[GREEN] = pesoGreen;
            pesos[i][j].cor[BLUE] = pesoBlue;
        }
    }
    for (int i = 0; i < altura - 1; i++) {
        for (int j = 0; j < largura - 1; j++) {
            if (j == 0 || j == altura || i == 0 || i == altura) {
                Pixel pixel = obtemPixel(img, i, j);
                pixel.cor[RED]   = 0;
                pixel.cor[GREEN] = 0;
                pixel.cor[BLUE]  = 0;
                recolorePixel(img, i, j, pixel);
            } else {
                recolorePixel(img, i, j, pesos[i][j]);
            }
            recolorePixel(img, i, j, pesos[i][j]);
        }
    }
    for(int i = 0; i < altura; i++) {
        free(pesos[i]);
    }
    free(pesos);
    pesos = NULL;
}

void meuFiltro(Imagem *img) {
    /*
    |1 |4 |7 |4 |1 |
    |4 |16|26|16|4 |
    |7 |26|41|26|7 |
    |4 |16|26|16|4 |
    |1 |4 |7 |4 |1 |
    */
    int altura = obtemAltura(img);
    int largura = obtemLargura(img);
    Pixel **pesos = (Pixel **) calloc(altura - 2, sizeof(Pixel *));
    for (int i = 0; i < altura - 2; i++) {
        pesos[i] = (Pixel *) calloc(largura - 2, sizeof(Pixel));
    }


    for (int i = 2, pesoRed = 0, pesoGreen = 0, pesoBlue = 0; i < altura - 2; i++, pesoRed = 0, pesoGreen = 0, pesoBlue = 0) {
        for (int j = 2; j < largura - 2; j++) {
            Pixel pixels[5][5] = {
                {obtemPixel(img,i-2,j-2), obtemPixel(img,i-2,j-1),obtemPixel(img, i-2, j), obtemPixel(img, i-2, j+1), obtemPixel(img, i-2, j+2)},
                {obtemPixel(img, i-1,j-2), obtemPixel(img, i-1,j-1), obtemPixel(img, i-1, j), obtemPixel(img, i-1, j+1), obtemPixel(img, i-1, j+2)},
                {obtemPixel(img, i,j-2), obtemPixel(img, i,j-1), obtemPixel(img, i, j), obtemPixel(img, i, j+1), obtemPixel(img, i, j+2)},
                {obtemPixel(img, i+1, j-2), obtemPixel(img, i+1,j-1), obtemPixel(img, i+1, j), obtemPixel(img, i+1, j+1), obtemPixel(img, i+1, j+2)},
                {obtemPixel(img, i+2,j-2), obtemPixel(img, i+2,j-1), obtemPixel(img, i+2, j), obtemPixel(img, i+2, j+1), obtemPixel(img, i+2, j+2)}
            };

            pesoRed = 
             (pixels[0][0].cor[RED]      + (pixels[0][1].cor[RED] * 4)  + (pixels[0][2].cor[RED] * 7)  + (pixels[0][3].cor[RED] * 4) +    pixels[0][4].cor[RED] +
            (pixels[1][0].cor[RED] * 4) + (pixels[1][1].cor[RED] * 16) + (pixels[1][2].cor[RED] * 26) + (pixels[1][3].cor[RED] * 16)   +(pixels[1][4].cor[RED] * 4) +
            (pixels[2][0].cor[RED] * 7) + (pixels[2][1].cor[RED] * 26) + (pixels[2][2].cor[RED] * 41) + (pixels[2][3].cor[RED] * 26)   +(pixels[2][4].cor[RED] * 7) +
            (pixels[3][0].cor[RED] * 4) + (pixels[3][1].cor[RED] * 16) + (pixels[3][2].cor[RED] * 26) + (pixels[3][3].cor[RED] * 16)   +(pixels[3][4].cor[RED] * 4) +
             pixels[4][0].cor[RED]      + (pixels[4][1].cor[RED] * 4)  + (pixels[4][2].cor[RED] * 7) +  (pixels[4][3].cor[RED] * 4)+     pixels[4][4].cor[RED]) * 1/273;

            pesoGreen = 
             (pixels[0][0].cor[GREEN]      + (pixels[0][1].cor[GREEN] * 4)  + (pixels[0][2].cor[GREEN] * 7)  + (pixels[0][3].cor[GREEN] * 4) +    pixels[0][4].cor[GREEN] +
            (pixels[1][0].cor[GREEN] * 4) + (pixels[1][1].cor[GREEN] * 16) + (pixels[1][2].cor[GREEN] * 26) + (pixels[1][3].cor[GREEN] * 16)   +(pixels[1][4].cor[GREEN] * 4) +
            (pixels[2][0].cor[GREEN] * 7) + (pixels[2][1].cor[GREEN] * 26) + (pixels[2][2].cor[GREEN] * 41) + (pixels[2][3].cor[GREEN] * 26)   +(pixels[2][4].cor[GREEN] * 7) +
            (pixels[3][0].cor[GREEN] * 4) + (pixels[3][1].cor[GREEN] * 16) + (pixels[3][2].cor[GREEN] * 26) + (pixels[3][3].cor[GREEN] * 16)   +(pixels[3][4].cor[GREEN] * 4) +
             pixels[4][0].cor[GREEN]      + (pixels[4][1].cor[GREEN] * 4)  + (pixels[4][2].cor[GREEN] * 7) +  (pixels[4][3].cor[GREEN] * 4)+     pixels[4][4].cor[GREEN]) * 1/273;

            pesoBlue = 
             (pixels[0][0].cor[BLUE]      + (pixels[0][1].cor[BLUE] * 4)  + (pixels[0][2].cor[BLUE] * 7)  + (pixels[0][3].cor[BLUE] * 4) +    pixels[0][4].cor[BLUE] +
            (pixels[1][0].cor[BLUE] * 4) + (pixels[1][1].cor[BLUE] * 16) + (pixels[1][2].cor[BLUE] * 26) + (pixels[1][3].cor[BLUE] * 16)   +(pixels[1][4].cor[BLUE] * 4) +
            (pixels[2][0].cor[BLUE] * 7) + (pixels[2][1].cor[BLUE] * 26) + (pixels[2][2].cor[BLUE] * 41) + (pixels[2][3].cor[BLUE] * 26)   +(pixels[2][4].cor[BLUE] * 7) +
            (pixels[3][0].cor[BLUE] * 4) + (pixels[3][1].cor[BLUE] * 16) + (pixels[3][2].cor[BLUE] * 26) + (pixels[3][3].cor[BLUE] * 16)   +(pixels[3][4].cor[BLUE] * 4) +
             pixels[4][0].cor[BLUE]      + (pixels[4][1].cor[BLUE] * 4)  + (pixels[4][2].cor[BLUE] * 7) +  (pixels[4][3].cor[BLUE] * 4)+     pixels[4][4].cor[BLUE]) * 1/273;
            
            pesos[i-2][j-2].cor[RED] = pesoRed;
            pesos[i-2][j-2].cor[GREEN] = pesoGreen;
            pesos[i-2][j-2].cor[BLUE] = pesoBlue;
        }
    }
    for (int i = 0; i < altura - 2; i++) {
        for (int j = 0; j < largura - 2; j++) {
            recolorePixel(img, i, j, pesos[i][j]);
        }
    }
    for(int i = 0; i < altura - 2; i++) {
        free(pesos[i]);
    }
    free(pesos);
    pesos = NULL;
}

void funcao_de_Canny(Imagem *img) {
    meuFiltro(img);

    int altura = obtemAltura(img);
    int largura = obtemLargura(img);

    Pesos **pesos = (Pesos **) calloc(altura, sizeof(Pesos *));
    for (int i = 0; i < altura; i++) {
        pesos[i] = (Pesos*) calloc(largura, sizeof(Pesos));
    }


    for (int i = 1, pesox = 0, pesoy = 0; i < altura - 1; i++, pesox = 0, pesoy = 0) {
        for (int j = 1; j < largura - 1; j++) {
            Pixel pixels[3][3];
            pixels[0][0] = obtemPixel(img, i-1, j-1), pixels[0][1] = obtemPixel(img, i-1, j), pixels[0][2] = obtemPixel(img, i-1, j+1);
            pixels[1][0] = obtemPixel(img, i, j-1), pixels[1][1] = obtemPixel(img, i,j), pixels[1][2] = obtemPixel(img, i, j+1);
            pixels[2][0] = obtemPixel(img, i+1, j-1), pixels[2][1] = obtemPixel(img, i+1, j), pixels[2][2] = obtemPixel(img, i+1, j+1);
        
            pesoy = (pixels[0][0].cor[RED]) + 
                    (pixels[0][1].cor[RED] * 2) + 
                    (pixels[0][2].cor[RED]) + 
                    (pixels[2][0].cor[RED] * -1) +
                    (pixels[2][1].cor[RED] * -2) + 
                    (pixels[2][2].cor[RED] * -1);

            pesox = (pixels[0][0].cor[RED]) + 
                    (pixels[1][0].cor[RED] * 2) + 
                    (pixels[2][0].cor[RED]) + 
                    (pixels[0][2].cor[RED] * -1) + 
                    (pixels[1][2].cor[RED] * -2) +
                    (pixels[2][2].cor[RED] * -1);

            pesos[i][j].red = sqrt(pow(pesox,2) + pow(pesoy,2));

            pesoy = (pixels[0][0].cor[GREEN]) + 
                    (pixels[0][1].cor[GREEN] * 2) + 
                    (pixels[0][2].cor[GREEN]) + 
                    (pixels[2][0].cor[GREEN] * -1) +
                    (pixels[2][1].cor[GREEN] * -2) + 
                    (pixels[2][2].cor[GREEN] * -1);

            pesox = (pixels[0][0].cor[GREEN]) + 
                    (pixels[1][0].cor[GREEN] * 2) + 
                    (pixels[2][0].cor[GREEN]) + 
                    (pixels[0][2].cor[GREEN] * -1) + 
                    (pixels[1][2].cor[GREEN] * -2) +
                    (pixels[2][2].cor[GREEN] * -1);

            pesos[i][j].green = sqrt(pow(pesox,2) + pow(pesoy,2));
            
            pesoy = (pixels[0][0].cor[BLUE]) + 
                    (pixels[0][1].cor[BLUE] * 2) + 
                    (pixels[0][2].cor[BLUE]) + 
                    (pixels[2][0].cor[BLUE] * -1) +
                    (pixels[2][1].cor[BLUE] * -2) + 
                    (pixels[2][2].cor[BLUE] * -1);

            pesox = (pixels[0][0].cor[BLUE]) + 
                    (pixels[1][0].cor[BLUE] * 2) + 
                    (pixels[2][0].cor[BLUE]) + 
                    (pixels[0][2].cor[BLUE] * -1) + 
                    (pixels[1][2].cor[BLUE] * -2) +
                    (pixels[2][2].cor[BLUE] * -1);

            pesos[i][j].blue = sqrt(pow(pesox,2) + pow(pesoy,2));

            
        
        } 
    }
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (j == 0 || j == altura || i == 0 || i == altura) {
                Pixel pixel = obtemPixel(img, i, j);
                pixel.cor[RED]   = 0;
                pixel.cor[GREEN] = 0;
                pixel.cor[BLUE]  = 0;
                recolorePixel(img, i, j, pixel);
            } else {
                Pixel pixel = obtemPixel(img, i, j);
                pixel.cor[RED]   = (pesos[i][j].red > 100) ? pesos[i][j].red : 0;
                pixel.cor[GREEN] = (pesos[i][j].green > 100) ? pesos[i][j].green : 0;
                pixel.cor[BLUE]  = (pesos[i][j].blue > 100) ? pesos[i][j].blue : 0;
                recolorePixel(img, i, j, pixel);
            }
        }
    }
    for(int i = 0; i < altura; i++) {
        free(pesos[i]);
    }
    free(pesos);
    pesos = NULL;
}