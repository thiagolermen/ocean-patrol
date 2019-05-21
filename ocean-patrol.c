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

#define NADA 0//para retirar o tipo do obstaculo para ele ser reinicializado
#define INIMIGO 1//para identificar o tipo de obstaculo
#define MERGULHADOR 2//para identificar o tipo de obstaculo

#define QTDOBSTACULOS 6
#define QTDSUBINIMIGOS 9
#define QTDMERGULHADORES 5
#define CAPTURADO 1//se o submarino inimigo foi atingido ou o mergulhador capturado
#define LIVRE 2//se o submarino inimigo ou o mergulhador esta livre na tela
#define PORCENTAGEMMERGULHADORES 40//porcentagem maxima da quantidade de mergulhadores(para fazer spawn randomico)
#define PORCENTAGEMSUBMARINOSINIMIGOS 60//porcentagem maxima da quantidade de submarinos inimigos(para fazer spawn de submarinos inimigos)
#define LARGURASUBINIMIGO 10
#define ALTURASUBINIMIGO 2
#define LARGURAMERGULHADOR 4

#define INICIOXSUBMARINO 5//valor de X do primeiro spawn do jogador
#define INICIOYSUBMARINO 3//valor de Y do primeiro spawn do jogador
#define DIREITA 1//orientacao do submarino
#define ESQUERDA 0//oriantacao do submarino
#define LARGURASUBMARINO 12
#define ALTURASUBMARINO 2

/* PROTOTIPOS */
void menu();
void imprimeBordaMenu();//imprime a borda amarela do menu
void moveSeletor(char *select, int *posSX, int *posSY);//move o seletor do menu
void coloreLinhaMenu(int *posSY);//colore a linha na qual o seletor esta
void gameInterface();//imprime a interface
void desenhaAgua();//desenha a agua

//PRECISA:
//CASO O STATUS DO MARINHEIRO SEJA 'CAPTURADO' NAO INICIALIZAR
//FUNCAO PARA SOLTAR MERGULHADORES CAPTURADOS ALTERANDO PORTANTO O STATUS DELES PARA 'LIVRE' E SETAR JOGADOR.MERGULHADORES PARA 0;

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
void desenhaVidas(SUBMARINO jogador){
    textcolor(LIGHTMAGENTA);
    if(jogador.vidas == 2){
        cputsxy(1, 1, "VIDAS <3 <3   ");
    }else{
        if(jogador.vidas == 1){
            cputsxy(1, 1, "VIDAS <3      ");
        }else{
            if(jogador.vidas == 0){
                cputsxy(1, 1, "VIDAS         ");
            }
        }
    }
}
int fimJogo(char tecla, SUBMARINO jogador){//verifica as possibilidades de gameOver ou fechar
    if(tecla == 27 || jogador.vidas == 0)
        return 1;
    else
        return 0;
}
void apagaSubmarino(SUBMARINO jogador){//funcao para apagar o rastro do jogador
    if(jogador.orientacao == DIREITA){//verifica se o submarino esta direcionado para a direita
        cputsxy(jogador.posicao.X + 3, jogador.posicao.Y, "        ");//imprime espacos para apagar
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "            ");
    }
    if(jogador.orientacao == ESQUERDA){//verifica se o submarino esta direcionado para a esquerda
        cputsxy(jogador.posicao.X + 1, jogador.posicao.Y, "        ");
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "            ");
    }
}
void desenhaSubmarino(SUBMARINO jogador){//funcao para desenhar o jogador
    textcolor(YELLOW);
    if(jogador.orientacao == DIREITA){//verifica se o submarino esta direcionado para a direita
        cputsxy(jogador.posicao.X + 3, jogador.posicao.Y, "__|o|___");//imprime a parte de cima do submarino com +3 no x para acompanhar o movimento
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "[+(________)");
    }
    if(jogador.orientacao == ESQUERDA){//verifica se o submarino esta direcionado para a esquerda
        cputsxy(jogador.posicao.X + 1, jogador.posicao.Y, "___|o|__");//imprime a parte de cima com +1 no x para acompanhar o movimento
        cputsxy(jogador.posicao.X, jogador.posicao.Y + 1, "(________)+]");
    }
}
void apagaObstaculo(OBSTACULO obstaculo[], int i){//apaga o rastro dos obstaculos
    if(obstaculo[i].tipo == INIMIGO){//verifica se o tipo do obstaculo e do tipo inimigo
        if(obstaculo[i].orientacao == DIREITA){//verifica se o obstaculo esta direcionado para a direita
            cputsxy(obstaculo[i].posicao.X + 3, obstaculo[i].posicao.Y, "      ");
            cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "          ");
        }else{
            if(obstaculo[i].orientacao == ESQUERDA){//verifica se o obstaculo esta direcionado para a esquerda
                cputsxy(obstaculo[i].posicao.X + 1, obstaculo[i].posicao.Y, "      ");
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "          ");
            }
        }
    }else{
        if(obstaculo[i].tipo == MERGULHADOR){//verifica se o obstaculo e do tipo mergulhador
            if(obstaculo[i].orientacao == DIREITA){//verifica se o obstaculo esta direcionado para a direita
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "    ");
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){//verifica se o obstaculo esta direcionado para a esquerda
                    cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "    ");
                }
            }
        }
    }
}
void desenhaObstaculo(OBSTACULO obstaculo[], int i){//desenha os obstaculos nas posicoes randomicas
    if(obstaculo[i].tipo == MERGULHADOR){//verifica se o obstaculo e do tipo mergulhador
        textcolor(LIGHTGREEN);
        if(obstaculo[i].orientacao == DIREITA){//verifica se o obsyaculo esta direcionado para a esquerda
            cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, ">->o");
        }else{
            if(obstaculo[i].orientacao == ESQUERDA){//verifica se o obstaculo estta direcionado para a direita
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y, "o<-<");
            }
        }
    }else{
        if(obstaculo[i].tipo == INIMIGO){//verifica se o obstaculo e do tipo inimigo
            textcolor(LIGHTGRAY);
            if(obstaculo[i].orientacao == DIREITA){//verifica se o obstaculo esta redirecionado para a direita
                cputsxy(obstaculo[i].posicao.X + 3, obstaculo[i].posicao.Y, "_|o|__");
                cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "[+(______)");
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){//verifica se o obstaculo eesta redirecionado para a esquerda
                    cputsxy(obstaculo[i].posicao.X + 1, obstaculo[i].posicao.Y, "__|o|_");
                    cputsxy(obstaculo[i].posicao.X, obstaculo[i].posicao.Y + 1, "(______)+]");
                }
            }
        }
    }
}
void moveObstaculo(OBSTACULO obstaculo[]){//move os obstaculos
    int i;
    for(i=0;i<QTDOBSTACULOS;i++){
        Sleep(5);
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
void geraObstaculo(OBSTACULO obstaculo[]){//inicializa os obstaculos randomicamente
    int i;
    srand(time(0));//iniciar o random
    for(i=0;i<QTDOBSTACULOS;i++){
        if(rand()%100 <= PORCENTAGEMMERGULHADORES){//checa probabilidade para inicializar o mergulhador
            if(obstaculo[i].tipo != MERGULHADOR && obstaculo[i].tipo != INIMIGO && obstaculo[i].status != CAPTURADO){//verifica se o obstaculo ja nao esta inicializado
                obstaculo[i].tipo = MERGULHADOR;//o tipo do obstaculo e setado para mergulhador
                obstaculo[i].status = LIVRE;
                if(rand()%2 == 1){//inicializa mergulhador no lado direito
                    obstaculo[i].orientacao = ESQUERDA;
                    obstaculo[i].posicao.X = COLUNA2 - 4;
                    obstaculo[i].posicao.Y = 5 + i*3;//imprime de 3 em 3 linhas
                }else{
                    if(rand()%2 == 0){//inicializa mergulhador no lado direito
                        obstaculo[i].orientacao = DIREITA;
                        obstaculo[i].posicao.X = COLUNA1 + 1;
                        obstaculo[i].posicao.Y = 5 + i*3;//imprime de 3 em 3 linhas
                    }
                }
            }
        }
        if(rand()%100 > PORCENTAGEMMERGULHADORES){//checa a probabilidade para iniciar submarinos inimigos
            if(obstaculo[i].tipo != MERGULHADOR && obstaculo[i].tipo != INIMIGO){//verifica se o obstaculo ja nao esta inicializado
                obstaculo[i].tipo = INIMIGO;//tipo do obstaculo setado para inimigo
                obstaculo[i].status = LIVRE;
                if(rand()%2 == 1){//inicializa mergulhador no lado direito
                    obstaculo[i].orientacao = ESQUERDA;
                    obstaculo[i].posicao.X = COLUNA2 - 10;
                    obstaculo[i].posicao.Y = 5 + i*3;//imprime de 3 em 3 linhas
                }else{
                    if(rand()%2 == 0){//inicializa mergulhador no lado direito
                        obstaculo[i].orientacao = DIREITA;
                        obstaculo[i].posicao.X = COLUNA1 + 1;
                        obstaculo[i].posicao.Y = 5 + i*3;//imprime de 3 em tres linhas
                    }
                }
            }
        }
    }
}

void testaColisao(SUBMARINO *jogador, OBSTACULO obstaculo[]){//verifica se houve colisao entre o jogador e os obstaculos
    int i;
    for(i=0;i<QTDOBSTACULOS;i++){//varre todos obstaculos, checando em cada um se houve colisao
        if(obstaculo[i].tipo == INIMIGO){//verifica se o obstaculo e um inimigo
            if(((*jogador).posicao.X < obstaculo[i].posicao.X + LARGURASUBINIMIGO - 1) &&//verifica se X2(obstaculo) esta entre X1(submarino) + LARGURASUBMARINO
               ((*jogador).posicao.X + LARGURASUBMARINO - 1 > obstaculo[i].posicao.X) &&//verifica se o X1(submarino) esta entre o X2(obstaculo)
               ((*jogador).posicao.Y < obstaculo[i].posicao.Y + ALTURASUBINIMIGO) &&//verifica se Y2(obstaculo) esta entre Y1(submarino) + ALTURASUBMARINO
               ((*jogador).posicao.Y + ALTURASUBMARINO > obstaculo[i].posicao.Y)){//verifica se Y1(submarino) esta entre Y2(obstaculo)
                //houve colisao
                (*jogador).vidas--;
                desenhaVidas(*jogador);
                apagaSubmarino(*jogador);
                //o jogador e reinicializado na posicao de respawn
                (*jogador).posicao.X = INICIOXSUBMARINO;
                (*jogador).posicao.Y = INICIOYSUBMARINO;
                desenhaSubmarino(*jogador);
            }
        }else{
            if(obstaculo[i].tipo == MERGULHADOR){//verifica se o obstaculo e um mergulhador
                if(((*jogador).posicao.X < obstaculo[i].posicao.X + LARGURAMERGULHADOR - 1) &&//verifica se X2(obstaculo) esta entre X1(submarino) + LARGURASUBMARINO
                   ((*jogador).posicao.X + LARGURASUBMARINO - 1 > obstaculo[i].posicao.X) &&//verifica se o X1(submarino) esta entre o X2(obstaculo)
                   ((*jogador).posicao.Y < obstaculo[i].posicao.Y + 1) &&//verifica se Y2(obstaculo) esta entre Y1(submarino) + ALTURASUBMARINO
                   ((*jogador).posicao.Y + ALTURASUBMARINO > obstaculo[i].posicao.Y)){//verifica se Y1(submarino) esta entre Y2(mergulhador)
                    //houve colisao
                    if((*jogador).mergulhadores < 3){
                        apagaObstaculo(obstaculo, i);//o mergulhador e capturado
                        obstaculo[i].posicao.X = LINHA1;//a posicao onde o mergulhador e guardado
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = NADA;//o tipo do obstaculo e setado para NADA para ele poder ser reespawnado
                        obstaculo[i].status = CAPTURADO;
                        (*jogador).mergulhadores++;
                    }
                    desenhaSubmarino(*jogador);
                }
            }
        }
    }
}
void moveJogador(SUBMARINO *jogador, char *flag_fim){//funcao para mover jogador(submarino)
    char tecla;
    if(kbhit()){//vevrifica se o jogador apertou alguma tecla
        tecla = getch();
        if (tecla == -32){//verifica se foi utilizado o teclado auxiliar
            tecla = getch();
            switch(tecla){
            case(75)://ESQUERDA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.X)--;
                if((*jogador).posicao.X <= COLUNA1){//verifica se o jogador esta atingindo o limite da area de jogo
                    ((*jogador).posicao.X)++;
                }
                if((*jogador).posicao.Y == 3 || (*jogador).posicao.Y == 4){//verifica se o submarino esta sobre a agua
                    desenhaAgua();
                }
                (*jogador).orientacao = ESQUERDA;//atualiza orientacao para a funcao desenhaSubmarino
                desenhaSubmarino(*jogador);
                break;
            case(77)://DIREITA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.X)++;
                if((*jogador).posicao.X >= COLUNA2-11){//verifica se o jogador esta atingindo o limite da area de jogo
                    ((*jogador).posicao.X)--;
                }
                if((*jogador).posicao.Y == 3 || (*jogador).posicao.Y == 4){//verifica s eo submarino esta sobre a agua
                    desenhaAgua();
                }
                (*jogador).orientacao = DIREITA;//atualiza orientacao para a funcao desenhaSubmarino
                desenhaSubmarino(*jogador);
                break;
            case(72)://CIMA
                apagaSubmarino(*jogador);
                ((*jogador).posicao.Y)--;
                if((*jogador).posicao.Y <= LINHA1){//verifica se o jogador esta atingindo o limite da area de jogo
                    ((*jogador).posicao.Y)++;
                }
                if((*jogador).posicao.Y == 4){//verifica se o submarino esta sobre a agua
                    desenhaAgua();
                }
                desenhaSubmarino(*jogador);
                break;
            case(80)://BAIXO
                apagaSubmarino(*jogador);
                ((*jogador).posicao.Y)++;
                if((*jogador).posicao.Y >= LINHA2-1){//verifica se o jogador esta atingindo o limite da area de jogo
                    ((*jogador).posicao.Y)--;
                }
                if((*jogador).posicao.Y == 5){//verifica se o submarino esta sobre a agua
                    desenhaAgua();
                }
                desenhaSubmarino(*jogador);
            }//switch
        }//if
        if(tecla == 27){//verifica se foi teclado ESC para encerrar o jogo
            *flag_fim = 27;
        }//flag para finalizar o game loop
        fimJogo(*flag_fim, *jogador);
    }//KBHIT
}//funcao
void gameLoop(){//laco do jogo
    char tecla;//flag para fim do jogo
    SUBMARINO jogador;//estrutura do submarino
    OBSTACULO obstaculo[QTDOBSTACULOS];
    jogador.posicao.X = INICIOXSUBMARINO;//coordenadas para inicializar o submarino
    jogador.posicao.Y = INICIOYSUBMARINO;
    jogador.orientacao = DIREITA;//o submarino e inicializado voltado para a direira
    jogador.vidas = 3;
    jogador.mergulhadores = 0;
    desenhaSubmarino(jogador);
    do{
        geraObstaculo(obstaculo);
        moveJogador(&jogador, &tecla);//chama attraves de ponteiro pos alterara os valores da posicao do submarino
        moveObstaculo(obstaculo);
        testaColisao(&jogador, obstaculo);
    }while(fimJogo(tecla, jogador) != 1);//jogo roda enquanto o jogador nao teclou ESC
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
    cputsxy(1, 1, "VIDAS <3 <3 <3");
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
   for(i=4;i<COLUNA2 - 50;i ++){//constroi a barra de oxigenio
        cputsxy(i, LINHA2 + 1, "|");

   }
   cputsxy(i, LINHA2 + 1, "]");
}
void desenhaAgua(){
    textcolor(LIGHTCYAN);
    cputsxy(2, 4, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}


