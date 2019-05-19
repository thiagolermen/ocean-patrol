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
#define COLUNA1 1//borda: da espaco para colocar oxigenio
#define LINHA2 23//borda: final da tela
#define COLUNA2 80//borda: final da tela

#define INIMIGO 1//para identificar o tipo de obstaculo
#define MERGULHADOR 2//para identificar o tipo de obstaculo
#define QTDOBSTACULOS 6
#define QTDSUBINIMIGOS 9
#define QTDMERGULHADORES 5
#define CAPTURADO 1//se o submarino inimigo foi atingido ou o mergulhador capturado
#define LIVRE 2//se o submarino inimigo ou o mergulhador esta livre na tela
#define PORCENTAGEMMERGULHADORES 40//porcentagem maxima da quantidade de mergulhadores(para fazer spawn randomico)
#define PORCENTAGEMSUBMARINOSINIMIGOS 60//porcentagem maxima da quantidade de submarinos inimigos(para fazer spawn de submarinos inimigos)

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


typedef struct submarino{//estrutura do jogador
    COORD posicao;//COORD e uma estrutura com X e Y
    int orientacao;//orientacao do submarino(direita = 1 ou esquerda = 0)
    int vidas;//vidas do jogador
    int oxigenio;//oxigenio do jogador
    int pontuacao;//pontuacao do jogador
    int mergulhadores;//quantidade d emergulhadores capturados
} SUBMARINO;

typedef struct obstaculo{//estrutura dos obstaculos (submarinos inimgos ou mergulhadores)
    int tipo;//tipo de cada obstaculo (1: SUBMARINO INIMIGO e 2: MERGULHADOR)
    COORD posicao;// posicao X e Y dos obstaculos na tela
    int orientacao;//e a orientacao (1 - direita ou 0 - esquerda) dos obstaculos
    int status;//status do obstaculo(1 - capturado 2 - livre)
} OBSTACULO;

void apagaObstaculo(OBSTACULO obstaculo[], int i){
    if(obstaculo[i].tipo == INIMIGO){
        if(obstaculo[i].orientacao == DIREITA){
            cputsxy(obstaculo[i].posicao.X + 3, obstaculo[i].posicao.Y, "      ");
            cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "          ");
        }else{
            if(obstaculo[i].orientacao == ESQUERDA){
                cputsxy(obstaculo[i].posicao.X + 1, obstaculo[i].posicao.Y, "      ");
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "          ");
            }
        }
    }else{
        if(obstaculo[i].tipo == MERGULHADOR){
            if(obstaculo[i].orientacao == DIREITA){
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "    ");
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){
                    cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "    ");
                }
            }
        }
    }
}
void desenhaObstaculo(OBSTACULO obstaculo[], int i){
    if(obstaculo[i].tipo == MERGULHADOR){
        textcolor(LIGHTGREEN);
        if(obstaculo[i].orientacao == DIREITA){
            cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, ">->o");
        }else{
            if(obstaculo[i].orientacao == ESQUERDA){
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "o<-<");
            }
        }
    }else{
        if(obstaculo[i].tipo == INIMIGO){
            textcolor(LIGHTGRAY);
            if(obstaculo[i].orientacao == DIREITA){
                cputsxy(obstaculo[i].posicao.X + 3, obstaculo[i].posicao.Y, "_|o|__");
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "[+(______)");
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){
                    cputsxy(obstaculo[i].posicao.X + 1, obstaculo[i].posicao.Y, "__|o|_");
                    cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "(______)+]");
                }
            }
        }
    }
}
void moveObstaculo(OBSTACULO obstaculo[]){
    int i;
    for(i=0;i<QTDOBSTACULOS;i++){
        Sleep(2.6);
        if(obstaculo[i].tipo == INIMIGO){
            apagaObstaculo(obstaculo, i);
            if(obstaculo[i].orientacao == DIREITA){
                obstaculo[i].posicao.X++;
                if(obstaculo[i].posicao.X >= COLUNA2 - 10){
                    obstaculo[i].posicao.X--;
                    apagaObstaculo(obstaculo, i);
                    obstaculo[i].tipo = -1;
                }
                desenhaObstaculo(obstaculo, i);
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){
                    obstaculo[i].posicao.X--;
                    if(obstaculo[i].posicao.X <= COLUNA1){
                        obstaculo[i].posicao.X++;
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = -1;
                    }
                    desenhaObstaculo(obstaculo, i);
            }
        }
    }else{
        if(obstaculo[i].tipo == MERGULHADOR){
                apagaObstaculo(obstaculo, i);
                if(obstaculo[i].orientacao == DIREITA){
                    obstaculo[i].posicao.X++;
                    if(obstaculo[i].posicao.X >= COLUNA2 - 4){
                        obstaculo[i].posicao.X--;
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = -1;
                    }
                    desenhaObstaculo(obstaculo, i);
                }else{
                    if(obstaculo[i].orientacao == ESQUERDA){
                        obstaculo[i].posicao.X--;
                        if(obstaculo[i].posicao.X <= COLUNA1){
                            obstaculo[i].posicao.X++;
                            apagaObstaculo(obstaculo, i);
                            obstaculo[i].tipo = -1;
                        }
                        desenhaObstaculo(obstaculo, i);
                    }
                }
            }
        }
    }
}
void geraObstaculo(OBSTACULO obstaculo[]){
    int i;
    srand(time(0));
    for(i=0;i<QTDOBSTACULOS;i++){
        if(rand()%100 <= PORCENTAGEMMERGULHADORES){//checa probabilidade para inicializar o mergulhador
            if(obstaculo[i].tipo != MERGULHADOR && obstaculo[i].tipo != INIMIGO){
                obstaculo[i].tipo = MERGULHADOR;
                if(rand()%2 == 1){//inicializa mergulhador no lado direito
                    obstaculo[i].orientacao = ESQUERDA;
                    obstaculo[i].posicao.X = COLUNA2 - 4;
                    obstaculo[i].posicao.Y = 5 + i*3;
                }else{
                    if(rand()%2 == 0){//inicializa mergulhador no lado direito
                        obstaculo[i].orientacao = DIREITA;
                        obstaculo[i].posicao.X = COLUNA1 + 1;
                        obstaculo[i].posicao.Y = 5 + i*3;
                    }
                }
            }
        }
        if(rand()%100 > PORCENTAGEMMERGULHADORES){
            if(obstaculo[i].tipo != MERGULHADOR && obstaculo[i].tipo != INIMIGO){
                obstaculo[i].tipo = INIMIGO;
                if(rand()%2 == 1){//inicializa mergulhador no lado direito
                    obstaculo[i].orientacao = ESQUERDA;
                    obstaculo[i].posicao.X = COLUNA2 - 10;
                    obstaculo[i].posicao.Y = 5 + i*3;
                }else{
                    if(rand()%2 == 0){//inicializa mergulhador no lado direito
                        obstaculo[i].orientacao = DIREITA;
                        obstaculo[i].posicao.X = COLUNA1 + 1;
                        obstaculo[i].posicao.Y = 5 + i*3;
                    }
                }
            }
        }
    }
}
/*typedef struct obstaculo{//estrutura dos obstaculos (submarinos inimgos ou mergulhadores)
    int tipo;//tipo de cada obstaculo (1: SUBMARINO INIMIGO e 2: MERGULHADOR)
    COORD posicao;// posicao X e Y dos obstaculos na tela
    int orientacao;//e a orientacao (1 - direita ou 0 - esquerda) dos obstaculos
    int status;//status do obstaculo(1 - capturado 2 - livre)
} SUB_INIMIGOS, MERGULHADORES;

void apagaObstaculo(SUB_INIMIGOS submarinoInimigo[], MERGULHADORES mergulhador[], int indice){
    if(mergulhador[indice].tipo == MERGULHADOR){
        textcolor(LIGHTGREEN);
        if(mergulhador[indice].orientacao == DIREITA){
            cputsxy(mergulhador[indice].posicao.X, mergulhador[indice].posicao.Y, "    ");
        }else{
            if(mergulhador[indice].orientacao == ESQUERDA){
                cputsxy(mergulhador[indice].posicao.X, mergulhador[indice].posicao.Y, "    ");
            }
        }
    }else{
        if(submarinoInimigo[indice].tipo == INIMIGO){
            textcolor(LIGHTGRAY);
            if(submarinoInimigo[indice].orientacao == DIREITA){
                cputsxy(submarinoInimigo[indice].posicao.X + 3, submarinoInimigo[indice].posicao.Y, "      ");
                cputsxy(submarinoInimigo[indice].posicao.X, submarinoInimigo[indice].posicao.Y + 1, "          ");
            }else{
                if(submarinoInimigo[indice].orientacao == ESQUERDA){
                    cputsxy(submarinoInimigo[indice].posicao.X + 1, submarinoInimigo[indice].posicao.Y, "      ");
                    cputsxy(submarinoInimigo[indice].posicao.X, submarinoInimigo[indice].posicao.Y + 1, "          ");
                }
            }
        }
    }
}
void desenhaObstaculo(SUB_INIMIGOS submarinoInimigo[], MERGULHADORES mergulhador[], int indice){
    if(mergulhador[indice].tipo == MERGULHADOR){
        textcolor(LIGHTGREEN);
        if(mergulhador[indice].orientacao == DIREITA){
            cputsxy(mergulhador[indice].posicao.X, mergulhador[indice].posicao.Y, ">->o");
        }else{
            if(mergulhador[indice].orientacao == ESQUERDA){
                cputsxy(mergulhador[indice].posicao.X, mergulhador[indice].posicao.Y, "o<-<");
            }
        }
    }else{
        if(submarinoInimigo[indice].tipo == INIMIGO){
            textcolor(LIGHTGRAY);
            if(submarinoInimigo[indice].orientacao == DIREITA){
                cputsxy(submarinoInimigo[indice].posicao.X + 3, submarinoInimigo[indice].posicao.Y, "_|o|__");
                cputsxy(submarinoInimigo[indice].posicao.X, submarinoInimigo[indice].posicao.Y + 1, "[+(______)");
            }else{
                if(submarinoInimigo[indice].orientacao == ESQUERDA){
                    cputsxy(submarinoInimigo[indice].posicao.X + 1, submarinoInimigo[indice].posicao.Y, "__|o|_");
                    cputsxy(submarinoInimigo[indice].posicao.X, submarinoInimigo[indice].posicao.Y + 1, "(______)+]");
                }
            }
        }
    }
}
void moveObstaculo(SUB_INIMIGOS submarinoInimigo[], MERGULHADORES mergulhador[]){
    int i;
    for(i=0;i<QTDOBSTACULOS;i++){
        if(submarinoInimigo[i].tipo == INIMIGO){
            apagaObstaculo(submarinoInimigo, mergulhador, i);
            if(submarinoInimigo[i].orientacao == DIREITA){
                submarinoInimigo[i].posicao.X++;
                if(submarinoInimigo[i].posicao.X >= COLUNA2)
                    apagaObstaculo(submarinoInimigo, mergulhador, i);
                desenhaObstaculo(submarinoInimigo, mergulhador, i);
                }else{
             else       if(submarinoInimigo[i].orientacao == ESQUERDA){
                        submarinoInimigo[i].posicao.X--;
                        if(submarinoInimigo[i].posicao.X <= COLUNA1)
                            apagaObstaculo(submarinoInimigo, mergulhador, i);
                        desenhaObstaculo(submarinoInimigo, mergulhador, i);
                    }//if orientacao submarino inimigo
                }//else
        }
        if(mergulhador[i].tipo == MERGULHADOR){
            apagaObstaculo(submarinoInimigo, mergulhador, i);
            if(mergulhador[i].orientacao == DIREITA){
                mergulhador[i].posicao.X++;
                if(mergulhador[i].posicao.X <= COLUNA1)
                    apagaObstaculo(submarinoInimigo, mergulhador, i);
                desenhaObstaculo(submarinoInimigo, mergulhador, i);
            }else{
                if(mergulhador[i].orientacao == ESQUERDA){
                    mergulhador[i].posicao.X--;
                    if(mergulhador[i].posicao.X >= COLUNA2)
                        apagaObstaculo(submarinoInimigo, mergulhador, i);
                    desenhaObstaculo(submarinoInimigo, mergulhador, i);
                }//if orientacao mergulhador
            }//else
        }//if tipo mergulhador
    }//for
}
void inicializaObstaculos(SUB_INIMIGOS submarinoInimigo[], MERGULHADORES mergulhador[]){//inicializa os obstaculos randomicamente
    int i;
    srand(time(0));
    for(i=0;i<QTDOBSTACULOS;i++){//varre todos os elementos dos vetores mergulhador ou submarinoInimigo
        if(rand()%100 <= PORCENTAGEMMERGULHADORES){//inicializa mergulhadores
            if(mergulhador[i].tipo != MERGULHADOR && submarinoInimigo[i].tipo != INIMIGO){
                mergulhador[i].tipo = MERGULHADOR;
                if(rand()%2 == 1){//se rand == 1 inicializa mergulhador no lado direito
                    mergulhador[i].orientacao = ESQUERDA;
                    mergulhador[i].posicao.X = COLUNA2 - 4;
                    mergulhador[i].posicao.Y = 5 + i*3;//imprime o mergulhador a cada 3 linhas
                    desenhaObstaculo(submarinoInimigo, mergulhador, i);
                }else{
                    if(rand()%2 == 0){//se rand == 0 inicializa mergulhador no lado esquerdo
                        mergulhador[i].orientacao = DIREITA;
                        mergulhador[i].posicao.X = COLUNA1 + 1;
                        mergulhador[i].posicao.Y = 5 + i*3;
                        desenhaObstaculo(submarinoInimigo, mergulhador, i);
                    }
                }
            }
        }
        if(rand()%100 > PORCENTAGEMMERGULHADORES){//inicializa submarinos inimigos
            if(submarinoInimigo[i].tipo != INIMIGO && mergulhador[i].tipo != MERGULHADOR){
                submarinoInimigo[i].tipo = INIMIGO;
                if(rand()%2 == 1){//se rand == 1 inicializa submarino inimigo no lado direito
                    submarinoInimigo[i].orientacao = ESQUERDA;
                    submarinoInimigo[i].posicao.X = COLUNA2 - 10;
                    submarinoInimigo[i].posicao.Y = 5 + i*3;
                    desenhaObstaculo(submarinoInimigo, mergulhador, i);
                }else{
                    if(rand()%2 == 0){
                        submarinoInimigo[i].orientacao = DIREITA;
                        submarinoInimigo[i].posicao.X = COLUNA1 + 1;
                        submarinoInimigo[i].posicao.Y = 5 + i*3;
                        desenhaObstaculo(submarinoInimigo, mergulhador, i);
                    }
                }
            }
        }
    }
}*/
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
void moveJogador(SUBMARINO *jogador, char *flag_fim){//funcao para mover jogador(submarino)
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
        if(tecla == 27)
            *flag_fim = 27;
    }//KBHIT
}
void gameLoop(){//laco do jogo
    char tecla;//flag para fim do jogo
    SUBMARINO jogador;//estrutura do submarino
    OBSTACULO obstaculo[QTDOBSTACULOS];
    jogador.posicao.X = INICIOXSUBMARINO;//coordenadas para inicializar o submarino
    jogador.posicao.Y = INICIOYSUBMARINO;
    jogador.orientacao = DIREITA;//o submarino e inicializado voltado para a direira
    desenhaSubmarino(jogador);
    do{
        geraObstaculo(obstaculo);
        moveJogador(&jogador, &tecla);//chama attraves de ponteiro pos alterara os valores da posicao do submarino
        moveObstaculo(obstaculo);
    }while(tecla != 27);//jogo roda enquanto o jogador nao teclou ESC
}



int main(){
    COORD coordinate = {COLUNA2+2, LINHA1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
    menu();
    return 0;
}
void menu(){
    char select;//guarda o valor do teclado para mover o seletor
    int posSX = 54, posSY = 6;//posicao inicial do seletor. comeca em 'novo jogo'
    int ansMenu;//flag para fim do loop do 'menu'
    do{
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

        cputsxy(7, 17,"                  _");
        cputsxy(7, 18,"  .         _____|___");
        cputsxy(7, 19," .      ___/  o o o  \\___");
        cputsxy(7, 20," .     /    ---------    \\ ");
        cputsxy(7, 21,"  .   |     ---------     |");
        cputsxy(7, 22,"    8-=\\_________________/");

        putchxy(posSX, posSY, '>');
        imprimeBordaMenu();
        coloreLinhaMenu(&posSY);
        moveSeletor(&select, &posSX, &posSY);
        if(select == 13){//verifica se o usuario apertou enter
            switch(posSY){//verifica em qual posicao o seletor esta
            case(6)://novo jogo
                clrscr();
                gameInterface();
                gameLoop();
                clrscr();
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
                clrscr();
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
            }//else
        }//switch
    }//if select
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
    cputsxy(70, 1, "PONTOS XX");
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
   textcolor(LIGHTMAGENTA);
   cputsxy(1, LINHA2 + 1, "O2 ");
   for(i=4;i<COLUNA2 - 50;i ++){
        cputsxy(i, LINHA2 + 1, "|");

   }
   cputsxy(i, LINHA2 + 1, "]");
}
void desenhaAgua(){
    textcolor(LIGHTCYAN);
    cputsxy(2, 4, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}


