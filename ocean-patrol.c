#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <conio2.h>

/*CONSTANTES*/
#define LINHA1 2//borda: da espaco para colocar vidas e pontos
#define COLUNA1 4//borda: da espaco para colocar oxigenio
#define LINHA2 25//borda: final da tela
#define COLUNA2 80//borda: final da tela

#define INIMIGO 1//para identificar o tipo de obstaculo
#define MERGULHADOR 2//para identificar o tipo de obstaculo
#define QTDMERGULHADORES 6//quantidade maxima de mergulhadores que aparecerao na tela
#define QTDSUBINIMIGOS 15//quantidade maxima de inimigos que aparecerao na tela
#define PORCENTAGEMMERGULHADORES 20//porcentagem maxima da quantidade de mergulhadores(para fazer spawn randomico)
#define PORCENTAGEMSUBMARINOSINIMIGOS 50//porcentagem maxima da quantidade de submarinos inimigos(para fazer spawn de submarinos inimigos)

#define INICIOXSUBMARINO 5//valor de X do primeiro spawn do jogador
#define INICIOYSUBMARINO 3//valor de Y do primeiro spawn do jogador
#define DIREITA 1//orientacao do submarino
#define ESQUERDA 0//oriantacao do submarino

/* PROTOTIPOS */
void menu();
void imprimeBordaMenu();//imprime a borda amarela do menu
void moveSeletor(char *select, int *posSX, int *posSY);//move o seletor do menu
void coloreLinhaMenu(int *posSY);//colore a linha na qual o seletor esta
void gameInterface();//imprime a interface
void desenhaAgua();//desenha a agua


typedef struct submarino{
    COORD posicao;//COORD e uma estrutura com X e Y
    int orientacao;//orientacao do submarino(direita = 1 ou esquerda = 0)
    int vidas;//vidas do jogador
    int oxigenio;//oxigenio do jogador
    int pontuacao;//pontuacao do jogador
    int mergulhadores;//quantidade d emergulhadores capturados
} SUBMARINO;
void apagaSubmarino(SUBMARINO jogador){//funcao para apagar o rastro do jogador
    if(jogador.orientacao == DIREITA){
        cputsxy(jogador.posicao.X + 3, jogador.posicao.Y, "        ");
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "            ");
    }
    if(jogador.orientacao == ESQUERDA){
        cputsxy(jogador.posicao.X + 1, jogador.posicao.Y, "        ");
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "            ");
    }
}
void desenhaSubmarino(SUBMARINO jogador){//funcao para desenhar o jogador
    textcolor(YELLOW);
    if(jogador.orientacao == DIREITA){
        cputsxy(jogador.posicao.X + 3, jogador.posicao.Y, "__|o|___");
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "[+(________)");
    }
    if(jogador.orientacao == ESQUERDA){
        cputsxy(jogador.posicao.X + 1, jogador.posicao.Y, "___|o|__");
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "(________)+]");
    }
}
void moveJogador(SUBMARINO *jogador){//funcao para mover jogador(submarino)
    char tecla;
    if(kbhit()){//vevrifica se o jogador apertou alguma tecla
        tecla = getch();
        if (tecla == -32){
            tecla = getch();
            switch(tecla){
            case(75)://ESQUERDA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.X)--;
                if((*jogador).posicao.X <= COLUNA1){
                    ((*jogador).posicao.X)++;
                }
                if((*jogador).posicao.Y == 3 || (*jogador).posicao.Y == 4){
                    desenhaAgua();
                }
                (*jogador).orientacao = ESQUERDA;//atualiza orientacao para a funcao desenhaSubmarino
                desenhaSubmarino(*jogador);
                break;
            case(77)://DIREITA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.X)++;
                if((*jogador).posicao.X >= COLUNA2-11){
                    ((*jogador).posicao.X)--;
                }
                if((*jogador).posicao.Y == 3 || (*jogador).posicao.Y == 4){
                    desenhaAgua();
                }
                (*jogador).orientacao = DIREITA;
                desenhaSubmarino(*jogador);
                break;
            case(72)://CIMA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.Y)--;
                if((*jogador).posicao.Y <= LINHA1){
                    ((*jogador).posicao.Y)++;
                }
                if((*jogador).posicao.Y == 4){
                    desenhaAgua();
                }
                desenhaSubmarino(*jogador);
                break;
            case(80)://BAIXO
                apagaSubmarino(*jogador);
                ((*jogador).posicao.Y)++;
                if((*jogador).posicao.Y >= LINHA2-1){
                    ((*jogador).posicao.Y)--;
                }
                if((*jogador).posicao.Y == 5){
                    desenhaAgua();
                }
                desenhaSubmarino(*jogador);
            }//switch
        }//if
    }//KBHIT
}
void gameLoop(){//laco do jogo
    char tecla;
    SUBMARINO jogador;//estrutura do submarino
    jogador.posicao.X = INICIOXSUBMARINO;//coordenadas para inicializar o submarino
    jogador.posicao.Y = INICIOYSUBMARINO;
    jogador.orientacao = DIREITA;//o submarino e inicializado voltado para a direira
    desenhaSubmarino(jogador);
    do{
        //tecla = getch();
        moveJogador(&jogador);//chama attraves de ponteiro pos alterara os valores da posicao do submarino
    }while(tecla != 27);//jogo roda enquanto o jogador nao teclou ESC
}



int main(){
    menu();
}
void menu(){
    char select;//guarda o valor do teclado para mover o seletor
    int posSX = 54, posSY = 6;//posicao inicial do seletor. comeca em 'novo jogo'
    int ansMenu = 0;//flag para fim do loop do 'menu'
    clrscr();
    textcolor(LIGHTCYAN);
    cputsxy(5, 2,"     ___     ___      __      __       ___");
    cputsxy(5, 3,"   / __`\\  /'___\\  /'__`\\  /'__`\\   /' _ `\\");
    cputsxy(5, 4,"  /\\ \\L\\ \\/\\ \\__/ /\\  __/ /\\ \\L\\.\\_ /\\ \\/\\ \\");
    cputsxy(5, 5,"  \\ \\____/\\ \\____\\\\ \\____\\\\ \\__/.\\_\\\\ \\_\\ \\_\\");
    cputsxy(5, 6,"   \\/___/  \\/____/ \\/____/ \\/__/\\/_/ \\/_/\\/_/");
    cputsxy(5, 7,"                  __                 ___");
    cputsxy(5, 8,"                 /\\ \\__             /\\_ \\");
    cputsxy(5, 9,"  _____      __  \\ \\ ,_\\  _ __   ___\\//\\ \\");
    cputsxy(5, 10, " /\\ '__`\\  /'__`\\ \\ \\ \\/ /\\`'__\\/ __`\\\\ \\ \\");
    cputsxy(5, 11," \\ \\ \\L\\ \\/\\ \\L\\.\\_\\ \\ \\_\\ \\ \\//\\ \\L\\ \\\\_\\ \\_");
    cputsxy(5, 12,"  \\ \\ ,__/\\ \\__/.\\_\\\\ \\__\\\\ \\_\\\\ \\____//\\____\\");
    cputsxy(5, 13,"   \\ \\ \\/  \\/__/\\/_/ \\/__/ \\/_/ \\/___/ \\/____/");
    cputsxy(5, 14,"    \\ \\_\\");
    cputsxy(5, 15,"     \\/_/");
    imprimeBordaMenu();
    putchxy(posSX, posSY, '>');
    do{
        coloreLinhaMenu(&posSY);
        moveSeletor(&select, &posSX, &posSY);
        if(select == 13){//verifica se o usuario apertou enter
            switch(posSY){//verifica em qual posicao o seletor esta
            case(6)://novo jogo
                clrscr();
                gameInterface();
                gameLoop();
                break;
            case(7)://carregar jogo
                clrscr();
                break;
            case(8)://recordes
                clrscr();
                break;
            case(9)://creditos
                clrscr();
                break;
            case(10)://sair
                ansMenu = 1;
            }
        }
    }while(ansMenu != 1);
    textcolor(WHITE);
    clrscr();
}
void imprimeBordaMenu(){
    textcolor(LIGHTCYAN);
    cputsxy(53, 5,"*****************");
    cputsxy(53, 11,"*****************");

}
void moveSeletor(char *select, int *posSX, int *posSY){
    textcolor(LIGHTMAGENTA);
    putchxy(*posSX, *posSY, '>');
    *select = getch();
    if(*select == -32){//verifica se usou as setas
        *select = getch();
        switch(*select){
        case(72)://CIMA
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 6){//verifica se esta no 'novo jogo'
                *posSY = 10;//volta para 'sair'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)--;
                putchxy(*posSX, *posSY, '>');
            }
            break;
        case(80)://BAIXO
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 10){//verifica se esta no 'sair'
                *posSY = 6;//volta para 'novo jogo'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)++;
                putchxy(*posSX, *posSY, '>');
            }
        }
    }
}
void coloreLinhaMenu(int *posSY){
    switch(*posSY){//verifica em qual posicao o seletor esta
    case(6)://novo jogo
        textcolor(DARKGRAY);
        cputsxy(56, 7,"CARREGAR JOGO");
        cputsxy(56, 8,"RECORDES");
        cputsxy(56, 9,"CREDITOS");
        cputsxy(56, 10,"SAIR");
        textcolor(LIGHTMAGENTA);
        cputsxy(56, 6,"NOVO JOGO");
        break;
    case(7)://carregar jogo
        textcolor(DARKGRAY);
        cputsxy(56, 6,"NOVO JOGO");
        cputsxy(56, 8,"RECORDES");
        cputsxy(56, 9,"CREDITOS");
        cputsxy(56, 10,"SAIR");
        textcolor(LIGHTMAGENTA);
        cputsxy(56, 7,"CARREGAR JOGO");
        break;
    case(8)://recordes
        textcolor(DARKGRAY);
        cputsxy(56, 6,"NOVO JOGO");
        cputsxy(56, 7,"CARREGAR JOGO");
        cputsxy(56, 9,"CREDITOS");
        cputsxy(56, 10,"SAIR");
        textcolor(LIGHTMAGENTA);
        cputsxy(56, 8,"RECORDES");
        break;
    case(9)://creditos
        textcolor(DARKGRAY);
        cputsxy(56, 6,"NOVO JOGO");
        cputsxy(56, 7,"CARREGAR JOGO");
        cputsxy(56, 8,"RECORDES");
        cputsxy(56, 10,"SAIR");
        textcolor(LIGHTMAGENTA);
        cputsxy(56, 9,"CREDITOS");
        break;
    case(10)://sair
        textcolor(DARKGRAY);
        cputsxy(56, 6,"NOVO JOGO");
        cputsxy(56, 7,"CARREGAR JOGO");
        cputsxy(56, 8,"RECORDES");
        cputsxy(56, 9,"CREDITOS");
        textcolor(LIGHTMAGENTA);
        cputsxy(56, 10,"SAIR");
    }
}
void gameInterface(){
    clrscr();
    char cell = '*';
    int i,j;
    textcolor(LIGHTMAGENTA);
    cputsxy(1, 1, "LIFE <3 <3 <3");
    cputsxy(68, 1, "PONTOS XX");
    desenhaAgua();
    textcolor(DARKGRAY);
    for (i=LINHA1;i<LINHA2;i++){//constroi os limites vericais do cenario
            putchxy (COLUNA1, i, cell);
            putchxy (COLUNA2, i, cell);
    }
    for (i=COLUNA1;i<COLUNA2;i++){//constroi os limites horizontais do cenario
            putchxy (i, LINHA1, cell);
            putchxy (i, LINHA2, cell);
   }
}
void desenhaAgua(){
    textcolor(LIGHTCYAN);
    cputsxy(5, 4, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}


