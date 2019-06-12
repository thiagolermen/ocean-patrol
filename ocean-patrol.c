/*==========    BIBLIOTECAS    ==========*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <conio2.h>

/*=========     CONSTANTES     =========*/

//TECLADO
#define TECLADOAUXILIAR -32//usuario usou as setas
#define CIMA 72//seta p cima
#define BAIXO 80//seta p baixo
#define ESQUERDATECLA 75//seta p esquerda
#define DIREITATECLA 77//seta p direita
#define ESC 27//esc
#define ENTER 13//enter
#define ESPACO 32//espaco

//INTERFACE
#define LINHA1 2//borda: da espaco para colocar vidas e pontos
#define COLUNA1 1//borda: da espaco para colocar oxigenio
#define LINHA2 23//borda: final da tela
#define COLUNA2 80//borda: final da tela
#define YAGUA 4//agua: coordenada Y onde a agua sera desenhada
#define XAGUA 2//agua: coordenada X onde a agua sera desenhada
#define INICIOXOXIGENIO 41//posicao inicial do oxigenio
#define NUMRECORDES 10//numero de recordes que serao escritos no ranking

//OBSTACULO
#define NADA 0//para retirar o tipo do obstaculo para ele ser reinicializado
#define INIMIGO 1//para identificar o tipo de obstaculo
#define MERGULHADOR 2//para identificar o tipo de obstaculo
#define QTDOBSTACULOS 6//quantidade maxima de obstaculos
#define PORCENTAGEMMERGULHADORES 40//porcentagem maxima da quantidade de mergulhadores(para fazer spawn randomico)
#define PORCENTAGEMSUBMARINOSINIMIGOS 60//porcentagem maxima da quantidade de submarinos inimigos(para fazer spawn de submarinos inimigos)
#define LARGURASUBINIMIGO 10//largura do submarino inimigo
#define ALTURASUBINIMIGO 2//altura do submarino inimigo
#define LARGURAMERGULHADOR 4//largura do mergulhador
#define CAPTURADO 1//se o  mergulhador foi capturado
#define LIVRE 2//se o mergulhador esta livre na tela

//JOGADOR
#define TAMSTRING 20//ttamanho do nome do jogador
#define MAXARQ 10//capacidade maxima de nomes salvos no arquivo
#define INICIOXSUBMARINO 32//valor de X do primeiro spawn do jogador
#define INICIOYSUBMARINO 3//valor de Y do primeiro spawn do jogador
#define DIREITA 1//orientacao do submarino
#define ESQUERDA 0//oriantacao do submarino
#define LARGURASUBMARINO 12//largura do submarino jogador
#define ALTURASUBMARINO 2//altura do submarino jogador
#define INICIOVIDASJOGADOR 3//o jogador comeca com 3 vidas
#define INICIOMERGULHADORESJOGADOR 0//o jogador comeca com 0 mergulhadores capturados
#define INICIOPONTUACAOJOGADOR 0//o jogador comeca com 0 pontos
#define INICIOOXIGENIOJOGADOR 30//o jogador comeca com 30 pontos no refil do oxigenio
#define LIGADO 1//se o tiro foi disparado
#define DESLIGADO 0//se o tiro nao foi disparado

/*=========   PROTOTIPOS   =========*/
void menu();
void imprimeBordaMenu();//imprime a borda amarela do menu
void moveSeletor(char *select, int *posSX, int *posSY);//move o seletor do menu
void coloreLinhaMenu(int *posSY);//colore a linha na qual o seletor esta
void gameInterface();//imprime a interface
void desenhaAgua();//desenha a agua

/*=========    PRECISA:    =========*/
//fazer funcao para excluir ordenar ranking no arquivo texto
//ajustar bug de spawn na tela dos obstaculos em posicoes correntes


typedef struct submarino{//estrutura do jogador
    COORD posicao;//COORD e uma estrutura com X e Y
    char nome[TAMSTRING];
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
typedef struct missil{
    COORD posicao;
    int orientacao;
    int estado;// booleano 0 ou 1
} TIRO;

void interfaceCreditos(){
    textcolor(LIGHTCYAN);
    cputsxy(15, 3, "*****************************************************");
    textcolor(YELLOW);
    cputsxy(15, 4, "                     OCEAN PATROL                    ");
    cputsxy(15, 6, "                      CREDITOS                       ");
    textcolor(LIGHTCYAN);
    cputsxy(15, 7, "*****************************************************");
    textcolor(DARKGRAY);
    cputsxy(15, 12, "               LEONARDO BARROS BILHALVA              ");
    cputsxy(15, 13, "                THIAGO SOTORIVA LERMEN               ");
    textcolor(YELLOW);
    cputsxy(15, 19, "          INF1202 - ALGORITMOS E PROGRAMACAO         ");
    textcolor(DARKGRAY);
    cputsxy(15, 21, "                PROFESSORA MARA ABEL                 ");
    cputsxy(15, 22, "             MONITOR CAUA ROCA ANTUNES               ");
    textcolor(LIGHTCYAN);
    cputsxy(15, 23, "*****************************************************");
}
void creditos(){
    char tecla;
    interfaceCreditos();
    do{
        if(kbhit()){
            tecla = getch();
        }
    }while(tecla != ESC);
    clrscr();
}
void interfaceSalvaJogador(){//desenha a interface do salva jogador
    textcolor(LIGHTCYAN);
    cputsxy(25, 10, "*********************************");
    textcolor(YELLOW);
    cputsxy(25, 11, "            FIM DE JOGO            ");
    textcolor(DARKGRAY);
    cputsxy(25, 12, "          NOME DO JOGADOR          ");
    textcolor(LIGHTCYAN);
    cputsxy(25, 16, "*********************************");
}
void gravaArquivo(FILE *ranking, char nome[][TAMSTRING], int pontuacao[], char nomeArquivo[TAMSTRING], int numJogadores){
    int i;
    ranking = fopen(nomeArquivo, "w");
    if(ranking == NULL){//verifica se o arquivo foi aberto corretamente
        interfaceSalvaJogador();
        cputsxy(28, 15, "ERRO NA LEITURA!");
        fclose(ranking);
    }else{
        for(i=0;i<numJogadores;i++){
            fprintf(ranking,"%s %d \n", nome[i], pontuacao[i]);
        }
        fclose(ranking);
    }
}
void ordenaRanking(char nome[][TAMSTRING], int pontuacao[], SUBMARINO *jogador, FILE *ranking, int numJogadores){//ordena o ranking
    int i, j, k, aux;
    char auxstring[TAMSTRING];//auxiliar para guardar a string
    for (i = 0; i < numJogadores - 1; i++){//selection sort
        k = i;//variavel para varreer os indeices
        for (j = (i+1); j < numJogadores; j++) {
            if(pontuacao[j] > pontuacao[k]){//verifica se a pontuacao do jogador e
                k = j;
            }
        }
        if (i != k) {//verifica se nao e o mesmo elemtno
            aux = pontuacao[i];//auxiliar para fazer a troca entre as posicoes
            strcpy(auxstring, nome[i]);
            pontuacao[i] = pontuacao[k];
            strcpy(nome[i], nome[k]);
            pontuacao[k] = aux;
            strcpy(nome[k], auxstring);
    }
  }
}
void gravaPontuacao(FILE *ranking, SUBMARINO *jogador, char nomeArquivo[]){
    int i = 0;
    int pontuacao[MAXARQ];
    int numJogadores = 0;
    char nome[MAXARQ][TAMSTRING];
    ranking = fopen(nomeArquivo, "r");
    if(ranking == NULL){//verifica se o arquivo foi aberto corretamente
        interfaceSalvaJogador();
        cputsxy(28, 15, "ERRO NA LEITURA!");
        fclose(ranking);
    }else{
        while(!feof(ranking)){
            fscanf(ranking, "%s", nome[i]);//escreve o nome do jogador no arquivo
            fscanf(ranking, "%d", &pontuacao[i]);//escreve a pontuacao do jogador no arquivo
            numJogadores++;
            i++;
        }
        fclose(ranking);
        ordenaRanking(nome, pontuacao, jogador, ranking, numJogadores-1);
        gravaArquivo(ranking, nome, pontuacao, nomeArquivo, numJogadores-1);
    }
}
void salvaJogador(FILE *ranking, SUBMARINO *jogador){
    int flagSalvaJogador = 0;
    char nomeArquivo[TAMSTRING] = {"ranking.txt"};
    interfaceSalvaJogador();
    do{
        ranking = fopen(nomeArquivo, "a");//arquivo aberto para append
        if(ranking == NULL){//verifica se o arquivo foi aberto corretamente
            interfaceSalvaJogador();
            cputsxy(28, 15, "ERRO NA LEITURA!");
            flagSalvaJogador = 1;
            fclose(ranking);
        }else{
            gotoxy(35, 13);
            scanf("%s", (*jogador).nome);//le o nome do jogador
            fprintf(ranking, "%s %d \n", (*jogador).nome, (*jogador).pontuacao);
            fflush(ranking);
            fclose(ranking);
            gravaPontuacao(ranking, jogador, nomeArquivo);
            flagSalvaJogador = 1;
        }
    }while(flagSalvaJogador != 1);
}
void escreveRecordes(FILE *ranking){//escreve os recordes no arquivo
    char nomeArquivo[TAMSTRING] = {"ranking.txt"};
    char nome[MAXARQ][TAMSTRING] = {};
    int pontuacao[MAXARQ] = {};
    int i = 0, j = 5;
    int numeroRecordes = 0;
    ranking = fopen(nomeArquivo, "r");//abre o arquivo para a leitura
    if (ranking != NULL){
        while(!feof(ranking)){
            numeroRecordes++;
            fscanf(ranking, "%s", nome[i]);//escreve o nome do jogador no arquivo
            fscanf(ranking, "%d", &pontuacao[i]);//escreve a pontuacao do jogador no arquivo
            i++;
        }
    }
    for(i=0;i<NUMRECORDES;i++){
        if(pontuacao[i] != 0 && nome[i]){
            cputsxy(29, j, nome[i]);//desenha o nome do jogador na tela recordes
            gotoxy(48, j);
            printf("%d", pontuacao[i]);//desenha a pontuacao do jogadorna tela
            j+=2;
        }
    }
    fclose(ranking);
}
void interfaceRecordes(){
    char cell = '*';
    int i, j = 1;
    textcolor(YELLOW);
    cputsxy(36, 2, "RECORDES");
    cputsxy(30, 4, "NOME");
    cputsxy(45, 4, "PONTUACAO");
    textcolor(LIGHTCYAN);
    for (i=2;i<25;i++){//constroi os limites vericais do ranking
            putchxy (20, i, cell);
            putchxy (60, i, cell);
    }
    for (i=20;i<61;i++){//constroi os limites horizontais do ranking
            putchxy (i, 1, cell);
            putchxy (i, 3, cell);
            putchxy (i, 24, cell);
    }
    textcolor(DARKGRAY);
    for(i=5;i<24;i+=2){
        gotoxy(22, i);
        printf("%d.", j);
        j++;
    }
}
void recordes(FILE *ranking){//imprime a funcao de rocordes no menu
    char tecla;
    interfaceRecordes();
    escreveRecordes(ranking);
    do{
        if(kbhit()){
            tecla = getch();
        }
    }while(tecla != ESC);
}
void interfaceSalvaJogo(){//desenha a interface do salvaJogo
    textcolor(LIGHTCYAN);
    cputsxy(25, 10, "*********************************");
    textcolor(YELLOW);
    cputsxy(25, 11, "            SALVAR JOGO            ");
    textcolor(DARKGRAY);
    cputsxy(25, 12, "          NOME DO JOGADOR       ");
    textcolor(LIGHTCYAN);
    cputsxy(25, 16, "*********************************");
}
void salvaJogo(FILE *jogo, SUBMARINO jogador){//salvo o jogo
    int flagSalvaJogo = 0;
    char nomeArquivo[TAMSTRING];
    interfaceSalvaJogo();
    do{
        gotoxy(35, 13);
        scanf("%s", nomeArquivo);//ne o onme do arquivo
        jogo = fopen(nomeArquivo, "wb");//abre arquivo para poder ser alterado
        if(jogo == NULL){//verifica se o arquivo nao abriu
            interfaceSalvaJogo();
            cputsxy(28, 15, "ERRO NA ESCRITA!");
            flagSalvaJogo = 1;
        }else{
            if(fwrite(&jogador, sizeof(jogador), 1, jogo) == 1){//verifica se o arquivo pode ser escrito e salva as definicoes no aruqivo
                fclose(jogo);
                flagSalvaJogo = 0;
            }else{
                interfaceSalvaJogo();
                cputsxy(28, 15, "ERRO NA ESCRITA!");
                flagSalvaJogo = 1;
            }
        }
    }while(flagSalvaJogo == 1);
}

void moveSeletorPause(char *select, int *posSX, int *posSY){
    textcolor(YELLOW);
    putchxy(*posSX, *posSY, '>');
    *select = getch();
    if(*select == TECLADOAUXILIAR){//verifica se usou as setas
        *select = getch();
        switch(*select){
        case(CIMA)://CIMA
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 13){//verifica se esta no 'resume game'
                *posSY = 15;//volta para 'sair'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)--;
                putchxy(*posSX, *posSY, '>');
            }
            break;
        case(BAIXO)://BAIXO
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 15){//verifica se esta no 'sair'
                *posSY = 13;//volta para 'resume game'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)++;
                putchxy(*posSX, *posSY, '>');
            }//else
        }//switch
    }//if select
}
void coloreLinhaPause(int posSY){
    switch(posSY){//verifica em qual posicao o seletor esta
    case(13)://resume game
        textcolor(DARKGRAY);
        cputsxy(25, 14, "          SALVAR E SAIR          ");
        cputsxy(25, 15, "              SAIR               ");
        textcolor(YELLOW);
        cputsxy(25, 13, "           RESUME GAME           ");
        break;
    case(14)://salvar e sair
        textcolor(DARKGRAY);
        cputsxy(25, 13, "           RESUME GAME           ");
        cputsxy(25, 15, "              SAIR               ");
        textcolor(YELLOW);
        cputsxy(25, 14, "          SALVAR E SAIR          ");
        break;
    case(15)://sair
        textcolor(DARKGRAY);
        cputsxy(25, 13, "           RESUME GAME           ");
        cputsxy(25, 14, "          SALVAR E SAIR          ");
        textcolor(YELLOW);
        cputsxy(25, 15, "              SAIR               ");
    }
}
void interfacePause(){
    textcolor(LIGHTCYAN);
    cputsxy(25, 10, "*********************************");
    textcolor(YELLOW);
    cputsxy(25, 11, "              PAUSE              ");
    cputsxy(25, 12, "                                 ");
    textcolor(DARKGRAY);
    cputsxy(25, 13, "           RESUME GAME           ");
    cputsxy(25, 14, "          SALVAR E SAIR          ");
    cputsxy(25, 15, "              SAIR               ");
    textcolor(LIGHTCYAN);
    cputsxy(25, 16, "*********************************");
}
void apagaOxigenio(int posO2X){
    int i;
    textbackground(0);
    for(i=posO2X;i<48;i++){//constroi a barra de oxigenio
        cputsxy(i, LINHA2 + 2, " ");

    }
}
void desenhaTiro(TIRO missil){//imprime o tiro na posicao indicada
    textcolor(YELLOW);
    putchxy(missil.posicao.X, missil.posicao.Y, '=');
}
void apagaTiro(TIRO missil){
    putchxy(missil.posicao.X, missil.posicao.Y, ' ');
}
void atualizaPosicaoTiro(TIRO *missil){//atualiza a posicao do missil de acordo com o status do missil e a orientacao do mesmo
    if((*missil).orientacao == DIREITA){//verifica a orientacao do missil
        apagaTiro(*missil);
        (*missil).posicao.X++;//incrementa a posicao
        if((*missil).posicao.Y == YAGUA){//se tiro estiver sobre a agua, deve-se imprimir a agua novamente
            desenhaAgua();
        }
        desenhaTiro(*missil);
        if((*missil).posicao.X >= COLUNA2 - 1){//verifica se o missil chegou na borda
            apagaTiro(*missil);
            (*missil).estado = DESLIGADO;//atualiza o status do tiro para desligado para poder dar outro tiro
        }
    }else{
        if((*missil).orientacao == ESQUERDA){//verifica a orientacao do tiro
            apagaTiro(*missil);
            (*missil).posicao.X--;
            if((*missil).posicao.Y == YAGUA){//verifica se o missil esta sobre a agua
                desenhaAgua();
            }
            desenhaTiro(*missil);
            if((*missil).posicao.X <= COLUNA1 + 1){//verifica se o missil chegou na borda
                apagaTiro(*missil);
                (*missil).estado = DESLIGADO;//atualiza o status do tiro para desligado para poder dar outro tiro
            }
        }
    }
}
void tiroJogador (char tecla, SUBMARINO jogador, TIRO *missil){//inicializa as caracteristicas da estrutura TIRO
    if((*missil).estado == DESLIGADO){//verifica se o missil nao foi solto
        if (tecla == ESPACO){//verifica se foi teclado espaco
            if (jogador.orientacao == DIREITA){//verifica a orientacao do jogador
                (*missil).posicao.X = jogador.posicao.X + LARGURASUBMARINO;//inicializa as coordenadas do missil
                (*missil).posicao.Y = jogador.posicao.Y + 1;
                (*missil).orientacao = DIREITA;//inicializa a orientacao do missil de acordo com a orientacao do jogador
                (*missil).estado = LIGADO;//indica que o missil foi solto
                desenhaTiro(*missil);
            }else{
                if(jogador.orientacao == ESQUERDA){//verifica a orientacao do jogador
                    (*missil).posicao.X = jogador.posicao.X - 1;//inicializa as coordenadas do missil
                    (*missil).posicao.Y = jogador.posicao.Y + 1;
                    (*missil).orientacao = ESQUERDA;//inicializa a orientacao do missil de acordo com a orientacao do jogador
                    (*missil).estado = LIGADO;//indica que o missil foi solto
                    desenhaTiro(*missil);
                }
            }
        }
    }else{
        if ((*missil).estado == LIGADO){//verifica se o missil foi solto para atualizar a posicao
            atualizaPosicaoTiro(missil);
        }
    }
}
void atualizaMergulhadores(SUBMARINO jogador){//desenha os jogadores capturados na tela
    textcolor(LIGHTGREEN);
    if(jogador.mergulhadores == 0){//se o jogador nao capturou nenhum mergulhador
        cputsxy(62, LINHA2 + 2, "                  ");
    }else{
        if(jogador.mergulhadores == 1){
            cputsxy(62, LINHA2 + 2, "\\o/              ");//se o jogador capturou um mergulhador
        }else{
            if(jogador.mergulhadores == 2){
                cputsxy(62, LINHA2 + 2, "\\o/\\o/          ");//se o jogador capturou dois mergulhadores
            }else{
                if(jogador.mergulhadores == 3){
                    cputsxy(62, LINHA2 + 2, "\\o/\\o/\\o/      ");//se o jogador capturou tres mergulhadores
                }else{
                    if(jogador.mergulhadores == 4){
                        cputsxy(62, LINHA2 + 2, "\\o/\\o/\\o/\\o/    ");
                    }else{
                        if(jogador.mergulhadores == 5){
                            cputsxy(62, LINHA2 + 2, "\\o/\\o/\\o/\\o/\\o/");
                        }
                    }
                }
            }
        }
    }
}
int atualizaOxigenio(SUBMARINO *jogador, int *posO2X){//desenha a barra de oxigenio
    textcolor(YELLOW);
    if((*jogador).oxigenio == 30){
        *posO2X = INICIOXOXIGENIO;
    }
    if((*jogador).posicao.Y != 3){//verifica se o submarino esta fora da agua
        if((*jogador).oxigenio > 0){//verificar se o oxigenio chega a 0 para para de decrementar
            (*jogador).oxigenio--;
            textbackground(0);
            cputsxy(*posO2X, LINHA2 + 2, " ");
            (*posO2X)--;
        }
    }else{
        if((*jogador).posicao.Y == 3){
            if((*jogador).oxigenio < 30){//verificar se o oxigenio chega  a 30 para para parar de incrementar
                (*jogador).oxigenio++;
                textbackground(YELLOW);
                cputsxy(*posO2X, LINHA2 + 2, " ");
                textbackground(0);
                (*posO2X)++;
            }
        }
    }
}
void pontuacao(SUBMARINO *jogador){//funcao para atualzar o valor da pontuacao
    textcolor(YELLOW);
    gotoxy(77, 1);//leva o cursor ate essa posicao
    printf("%d", (*jogador).pontuacao);//printa um decimal para ficar atualizando
}
void loopPorSegundo(SUBMARINO *jogador, int *flagGameLoop, int *posO2X){//funcao para incrementar a pontuacao de 1 em 1 de acordo com o valor do sleep
    (*flagGameLoop)++;//flag para o gameLoop
    if((*flagGameLoop)%40 == 0){//verifica se o flagGameLoop e divisivel por 40 (estimado de acordo com o valor do sleep)
        atualizaOxigenio(jogador, posO2X);
        if((*jogador).posicao.Y != 3){
            (*jogador).pontuacao++;//o jogador ganha 1 ponto a cada segundo de jogo
            pontuacao(jogador);
        }
    }
}
void pontosMergulhador(SUBMARINO *jogador){//acrescenta 20 ponto para cada mergulhador salvo
    (*jogador).pontuacao += (*jogador).mergulhadores*20;//a cada merculhador salvo adiciona 20 pontos
    pontuacao(jogador);
}
void largaMergulhadores(SUBMARINO *jogador, OBSTACULO obstaculo[]){//quando o jogador atinge a superficie, libera os mergulhadores
    int i;
    if((*jogador).posicao.Y == 3){//verifica se o submarino esta sobre a agua para poder soltar os mergulhadores
        if((*jogador).mergulhadores > 0){//verifica se o jogador capturou pelo menos 1 mergulhadores
            do{//zera  aquantidade de mergulhadores capturados
                (*jogador).pontuacao += 20;
                (*jogador).mergulhadores--;
             }while((*jogador).mergulhadores > 0);
             for(i=0;i<QTDOBSTACULOS;i++){//varre todos osbstaculos para setar para livre o status para poder ser reespawnado
                if(obstaculo[i].tipo == MERGULHADOR && obstaculo[i].status == CAPTURADO){
                    obstaculo[i].status = LIVRE;//status do mergulhador setado para livre para ele poder ser capturado novamente
                    obstaculo[i].tipo = NADA;//o tipo do obstaculo e setado para NADA para ele poder ser reespawnado
                }//if
            }//for
            pontosMergulhador(jogador);
            atualizaMergulhadores(*jogador);
        }//if
    }//if
}
void desenhaVidas(SUBMARINO jogador){
    textcolor(YELLOW);
    if(jogador.vidas == 3){//se o jogador tem 3 vidas
        cputsxy(7, 1, "<3 <3 <3");
    }else{
        if(jogador.vidas == 2){//se o jogador tem 3 vidas
            cputsxy(7, 1, "<3 <3   ");
    }else{
        if(jogador.vidas == 1){
            cputsxy(7, 1, "<3      ");
        }else{
                if(jogador.vidas == 0){//se o jogador tem 3 vidas
                    cputsxy(1, 1, "         ");
                }
            }
        }
    }

}
int fimJogo(int finalizaJogo, SUBMARINO jogador){//verifica as possibilidades de gameOver ou fechar
    if(finalizaJogo == 1 || jogador.vidas == 0 || jogador.oxigenio == 0)
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
        if(obstaculo[i].tipo == INIMIGO){
            apagaObstaculo(obstaculo, i);
            if(obstaculo[i].orientacao == DIREITA){
                obstaculo[i].posicao.X++;
                if(obstaculo[i].posicao.X >= COLUNA2 - 11){
                    obstaculo[i].posicao.X--;
                    apagaObstaculo(obstaculo, i);
                    obstaculo[i].tipo = -1;
                }else
                    desenhaObstaculo(obstaculo, i);
            }else{
                if(obstaculo[i].orientacao == ESQUERDA){
                    obstaculo[i].posicao.X--;
                    if(obstaculo[i].posicao.X <= COLUNA1 + 1){
                        obstaculo[i].posicao.X++;
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = -1;
                    }else
                        desenhaObstaculo(obstaculo, i);
            }
        }
    }else{
        if(obstaculo[i].tipo == MERGULHADOR){
                apagaObstaculo(obstaculo, i);
                if(obstaculo[i].orientacao == DIREITA){
                    obstaculo[i].posicao.X++;
                    if(obstaculo[i].posicao.X >= COLUNA2 - 5){
                        obstaculo[i].posicao.X--;
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = -1;
                    }else
                        desenhaObstaculo(obstaculo, i);
                }else{
                    if(obstaculo[i].orientacao == ESQUERDA){
                        obstaculo[i].posicao.X--;
                        if(obstaculo[i].posicao.X <= COLUNA1 + 1){
                            obstaculo[i].posicao.X++;
                            apagaObstaculo(obstaculo, i);
                            obstaculo[i].tipo = -1;
                        }else
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
                //obstaculo[i].tipo = INIMIGO;//tipo do obstaculo setado para inimigo
                obstaculo[i].status = LIVRE;
                if(rand()%2 == 1){//inicializa mergulhador no lado direito
                    obstaculo[i].tipo = INIMIGO;//tipo do obstaculo setado para inimigo
                    obstaculo[i].orientacao = ESQUERDA;
                    obstaculo[i].posicao.X = COLUNA2 - 10;
                    obstaculo[i].posicao.Y = 5 + i*3;//imprime de 3 em 3 linhas
                }else{
                    if(rand()%2 == 0){//inicializa mergulhador no lado direito
                        obstaculo[i].tipo = INIMIGO;//tipo do obstaculo setado para inimigo
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
    int i, j;
    for(i=0;i<QTDOBSTACULOS;i++){//varre todos obstaculos, checando em cada um se houve colisao
        if(obstaculo[i].tipo == INIMIGO){//verifica se o obstaculo e um inimigo
            if(((*jogador).posicao.X < obstaculo[i].posicao.X + LARGURASUBINIMIGO - 1) &&//verifica se X2(obstaculo) esta entre X1(submarino) + LARGURASUBMARINO
               ((*jogador).posicao.X + LARGURASUBMARINO - 1 > obstaculo[i].posicao.X) &&//verifica se o X1(submarino) esta entre o X2(obstaculo)
               ((*jogador).posicao.Y < obstaculo[i].posicao.Y + ALTURASUBINIMIGO) &&//verifica se Y2(obstaculo) esta entre Y1(submarino) + ALTURASUBMARINO
               ((*jogador).posicao.Y + ALTURASUBMARINO > obstaculo[i].posicao.Y)){//verifica se Y1(submarino) esta entre Y2(obstaculo)
                //houve colisao
                (*jogador).vidas--;//o jogador perde 1 vida
                (*jogador).mergulhadores = 0;//a quantidade de mergulhadores salvos e setada para 0
                atualizaMergulhadores(*jogador);
                for(j=0;j<QTDOBSTACULOS;j++){//varre todos obstaculos
                    if(obstaculo[j].tipo == MERGULHADOR && obstaculo[j].status == CAPTURADO){//verifica se o obstaculo procurado e do tipo mergulhador e o stuatus dele esta como CAPTURADO
                        obstaculo[j].status = LIVRE;//seta o status do obstaculo para LIVRE
                        obstaculo[j].tipo = NADA;//seta o tipo do obstaculo para nada para o obstaculo poder ser reespawnado
                    }
                }
                desenhaVidas(*jogador);
                apagaSubmarino(*jogador);
                //o jogador e reinicializado na posicao de respawn
                (*jogador).posicao.X = INICIOXSUBMARINO;//reinicia as coordenadas do subarino para a posicao inicial
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
                    if((*jogador).mergulhadores < 5){
                        apagaObstaculo(obstaculo, i);//o mergulhador e capturado
                        obstaculo[i].posicao.X = LINHA2+2;//a posicao onde o mergulhador e guardado
                        apagaObstaculo(obstaculo, i);
                        obstaculo[i].tipo = NADA;//o tipo do obstaculo e setado para NADA para ele poder ser reespawnado
                        obstaculo[i].status = CAPTURADO;
                        (*jogador).mergulhadores++;
                        atualizaMergulhadores(*jogador);
                    }
                    desenhaSubmarino(*jogador);
                }
            }
        }
    }
}
void testaColisaoTiro(SUBMARINO *jogador,TIRO *missil, OBSTACULO obstaculo[]){//testa se o tiro colidiu com algum obstaculo
    int i;
    for(i=0;i<QTDOBSTACULOS;i++){
        if(obstaculo[i].tipo == INIMIGO && (*missil).estado == LIGADO){//verifica se o obstaculo e um inimigo
            if(((*missil).posicao.X < obstaculo[i].posicao.X + LARGURASUBINIMIGO - 1) &&//verifica se X2(obstaculo) esta entre X1(submarino) + LARGURASUBMARINO
               ((*missil).posicao.X > obstaculo[i].posicao.X) &&//verifica se o X1(submarino) esta entre o X2(obstaculo)
               ((*missil).posicao.Y < obstaculo[i].posicao.Y + ALTURASUBINIMIGO) &&//verifica se Y2(obstaculo) esta entre Y1(submarino) + ALTURASUBMARINO
               ((*missil).posicao.Y > obstaculo[i].posicao.Y - 1)){//verifica se Y1(submarino) esta entre Y2(obstaculo)
                //houve colisao
                (*jogador).pontuacao += 10;//o jogador ganha 10 pontos ao atingir um submarino inimigo
                pontuacao(jogador);
                (*missil).estado = DESLIGADO;//o estado do tiro e setado para DESLIGADO para o jogador poder dar outro tiro
                apagaTiro(*missil);
                apagaObstaculo(obstaculo, i);
                obstaculo[i].tipo = NADA;//tipo do obstaculo e setado para NADA para poder ser reespawnado
            }
        }else{
            if(obstaculo[i].tipo == MERGULHADOR){//verifica se o obstaculo e um mergulhador
                if(((*missil).posicao.X < obstaculo[i].posicao.X + LARGURAMERGULHADOR - 1) &&//verifica se X2(obstaculo) esta entre X1(submarino) + LARGURASUBMARINO
                   ((*missil).posicao.X > obstaculo[i].posicao.X) &&//verifica se o X1(submarino) esta entre o X2(obstaculo)
                   ((*missil).posicao.Y < obstaculo[i].posicao.Y + 1) &&//verifica se Y2(obstaculo) esta entre Y1(submarino) + ALTURASUBMARINO
                   ((*missil).posicao.Y > obstaculo[i].posicao.Y)){//verifica se Y1(submarino) esta entre Y2(mergulhador)
                    //houve colisao
                    desenhaObstaculo(obstaculo, i);
                }
            }
        }
    }
}
void controlaJogador(SUBMARINO *jogador, char *flag_fim){//funcao para mover jogador(submarino)
    char tecla;
    *flag_fim = NADA;
    if(kbhit()){//vevrifica se o jogador apertou alguma tecla
        tecla = getch();
        if (tecla == TECLADOAUXILIAR){//verifica se foi utilizado o teclado auxiliar
            tecla = getch();
            switch(tecla){
            case(ESQUERDATECLA)://ESQUERDA
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
            case(DIREITATECLA)://DIREITA
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
            case(CIMA)://CIMA
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
            case(BAIXO)://BAIXO
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
        if(tecla == ESC){//verifica se foi teclado ESC para encerrar o jogo
            *flag_fim = ESC;
        }//flag para finalizar o game loop
        if(tecla == ESPACO){
            *flag_fim = ESPACO;
            fflush(stdin);
        }
    }//KBHIT
}//funcao
void inicializaJogador(SUBMARINO *jogador){//inicializa as configuracoes do jogador
    (*jogador).posicao.X = INICIOXSUBMARINO;//coordenadas para inicializar o submarino
    (*jogador).posicao.Y = INICIOYSUBMARINO;
    (*jogador).orientacao = DIREITA;//o submarino e inicializado voltado para a direira
    (*jogador).vidas = INICIOVIDASJOGADOR;//o jogador inicia com 3 vidas
    (*jogador).mergulhadores = INICIOMERGULHADORESJOGADOR;//o jogador inicia com 0 mergulhadores capturados
    (*jogador).pontuacao = INICIOPONTUACAOJOGADOR;//o jogador inicia com 0 pontos
    (*jogador).oxigenio = INICIOOXIGENIOJOGADOR;//o jogador comeca com 30 pontos de oxigenio
}
void pause(FILE *jogo, SUBMARINO *jogador, int *finalizaJogo){
    int posSX = 25, posSY = 13;//posicao inicial do seletor. comeca em 'resume game'
    int ansPause = 0;//flag para fim do loop do 'pause'
    char select;//guarda o valor do teclado para mover seletor
    clrscr();
    do{
        interfacePause();
        putchxy(posSX, posSY, '>');
        coloreLinhaPause(posSY);
        moveSeletorPause(&select, &posSX, &posSY);
        if(select == ENTER){//verifica se o usuario apertou enter
            switch(posSY){//verifica em qual posicao o seletor esta
            case(13)://resume game
                clrscr();
                gameInterface();
                desenhaSubmarino(*jogador);
                desenhaVidas(*jogador);
                atualizaMergulhadores(*jogador);
                ansPause = 1;
                break;
            case(14)://salvar e sair
                clrscr();
                salvaJogo(jogo, *jogador);
                *finalizaJogo = 1;
                ansPause = 1;
                break;
            case(15)://sair
                clrscr();
                *finalizaJogo = 1;
                ansPause = 1;
            }
        }
    }while(ansPause != 1);
}
void gameLoop(FILE *jogo, FILE *ranking, SUBMARINO *jogador){//laco do jogo
    char tecla = 0;//flag para fim do jogo
    int flagGameLoop = 0;//flag para atualizar pontuacao
    int finalizaJogo = 0;//flag para finalizar o jogo
    int posO2X = (*jogador).oxigenio + 11;//variavel da posicao inicial do OXIGENIO
    TIRO missil;//estrutura do tiro
    missil.estado = DESLIGADO;//estado do missil e inicializado como DESLIGADO para o jogador poder atirar
    OBSTACULO obstaculo[QTDOBSTACULOS];//estrutura do vetor de obstaculos
    desenhaSubmarino(*jogador);
    desenhaVidas(*jogador);
    apagaOxigenio(posO2X);
    pontuacao(jogador);
    do{
        Sleep(25);//para deixar mais lento o jogo
        if(tecla == ESC){
            pause(jogo, jogador, &finalizaJogo);
            apagaOxigenio(posO2X);
        }
        loopPorSegundo(jogador,&flagGameLoop, &posO2X);//funcao para atualizar a pontuacao
        geraObstaculo(obstaculo);
        controlaJogador(jogador, &tecla);//chama attraves de ponteiro pos alterara os valores da posicao do submarino
        tiroJogador(tecla, *jogador, &missil);
        largaMergulhadores(jogador, obstaculo);
        moveObstaculo(obstaculo);
        testaColisaoTiro(jogador, &missil, obstaculo);
        testaColisao(jogador, obstaculo);
    }while(fimJogo(finalizaJogo, *jogador) != 1);//jogo roda enquanto o jogador nao teclou ESC ou zerou as vidas
    if((*jogador).vidas == 0){
        salvaJogador(ranking, jogador);
    }
}

void interfaceProcuraArquivo(){//desenha a interface de procurar arquivo
    textcolor(LIGHTCYAN);
    cputsxy(25, 10, "*********************************");
    textcolor(YELLOW);
    cputsxy(25, 11, "           CARREGAR JOGO         ");
    textcolor(DARKGRAY);
    cputsxy(25, 12, "     DIGITE O NOME DO ARQUIVO       ");
    textcolor(LIGHTCYAN);
    cputsxy(25, 16, "*********************************");
}
int procuraJogo(FILE **jogo){//faz a busca se o arquivo do jogo existe (passado como ponteiro duplo pois acessa o ponteiro do endereco da memoria do arquivo)
    char nomeArquivo[TAMSTRING];//nome do arquivo a ser procurado deve terminar com .bin
    gotoxy(35, 13);//indica a posicao para realizar o eco
    scanf("%s", nomeArquivo);//le o nome do arquivo a ser procurado
    *jogo = fopen(nomeArquivo, "rb");//atricui o arquivo aberto para jogo
    if(*jogo == NULL){//verifica se nao teve sucesso na abertura
        return 1;
    }else{
        return 0;
    }
}
void abreJogo(FILE *jogo, FILE *ranking, SUBMARINO jogador){//faz a abertura do jogo caerregado
    if(fread(&jogador, sizeof(jogador), 1, jogo) == 1){
        clrscr();
        gameInterface();
        atualizaMergulhadores(jogador);

        gameLoop(jogo, ranking, &jogador);
        fclose(jogo);
        clrscr();
    }else{
        interfaceProcuraArquivo();
        textcolor(LIGHTMAGENTA);
        cputsxy(28, 15, "ERRO NA ABERTURA DO ARQUIVO!");
        procuraJogo(&jogo);
    }
}
void interfaceCarregaJogo(){
    textcolor(LIGHTCYAN);
    cputsxy(25, 10, "*********************************");
    textcolor(YELLOW);
    cputsxy(25, 11, "           CARREGAR JOGO         ");
    cputsxy(25, 12, "                                 ");
    textcolor(DARKGRAY);
    cputsxy(25, 13, "         BUSCAR UM ARQUIVO       ");
    cputsxy(25, 14, "               SAIR              ");
    textcolor(LIGHTCYAN);
    cputsxy(25, 16, "*********************************");
}
void moveSeletorCarrega(char *select, int *posSX, int *posSY){
    textcolor(YELLOW);
    putchxy(*posSX, *posSY, '>');
    *select = getch();
    if(*select == TECLADOAUXILIAR){//verifica se usou as setas
        *select = getch();
        switch(*select){
        case(CIMA)://CIMA
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 13){//verifica se esta no 'buscar um arquivo'
                *posSY = 14;//volta para 'sair'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)--;
                putchxy(*posSX, *posSY, '>');
            }
            break;
        case(BAIXO)://BAIXO
            putchxy(*posSX, *posSY, '\0');
            if(*posSY == 14){//verifica se esta no 'sair'
                *posSY = 13;//volta para 'buscar um arquivo'
                putchxy(*posSX, *posSY, '>');
            }else{
                (*posSY)++;
                putchxy(*posSX, *posSY, '>');
            }//else
        }//switch
    }//if select
}
void coloreLinhaCarrega(int posSY){
    switch(posSY){//verifica em qual posicao o seletor esta
    case(13)://resume game
        textcolor(DARKGRAY);
        cputsxy(25, 14, "               SAIR              ");
        textcolor(YELLOW);
        cputsxy(25, 13, "         BUSCAR UM ARQUIVO       ");
        break;
    case(14)://salvar e sair
        textcolor(DARKGRAY);
        cputsxy(25, 13, "         BUSCAR UM ARQUIVO       ");
        textcolor(YELLOW);
        cputsxy(25, 14, "               SAIR              ");
    }
}
void carregaJogo(FILE *jogo, FILE *ranking, SUBMARINO *jogador){
    int posSX = 25, posSY = 13;//posicao inicial do seletor. comeca em 'buscar um arquivo'
    int ansCarrega = 0;//flag para fim do loop do 'carregaJogo'
    char select;//guarda o valor do teclado para mover seletor
    do{
        interfaceCarregaJogo();
        putchxy(posSX, posSY, '>');
        coloreLinhaCarrega(posSY);
        moveSeletorCarrega(&select, &posSX, &posSY);
        if(select == ENTER){//verifica se o usuario apertou enter
            switch(posSY){//verifica em qual posicao o seletor esta
            case(13)://buscar um arquivo
                clrscr();
                interfaceProcuraArquivo();
                if (procuraJogo(&jogo) != 0){
                    textcolor(LIGHTMAGENTA);
                    cputsxy(28, 15, "ERRO NA ABERTURA DO ARQUIVO!");
                }else{
                    abreJogo(jogo, ranking, *jogador);
                    fclose(jogo);
                    ansCarrega = 1;
                }
                break;
            case(14)://sair
                clrscr();
                ansCarrega = 1;
            }
        }
    }while(ansCarrega != 1);
}
void interfaceMenu(){//desenha a interface do menu
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
    textcolor(YELLOW);
    cputsxy(7, 17,"                  _");
    cputsxy(7, 18,"  .         _____|___");
    cputsxy(7, 19," .      ___/  o o o  \\___");
    cputsxy(7, 20," .     /    ---------    \\ ");
    cputsxy(7, 21,"  .   |     ---------     |");
    cputsxy(7, 22,"    8-=\\_________________/");
}

int main(){
    menu();
    return 0;
}
void menu(){
    char select;//guarda o valor do teclado para mover o seletor
    int posSX = 54, posSY = 6;//posicao inicial do seletor. comeca em 'novo jogo'
    int ansMenu;//flag para fim do loop do 'menu'
    SUBMARINO jogador;
    FILE *jogo;
    FILE *ranking;
    do{
        interfaceMenu();
        imprimeBordaMenu();
        putchxy(posSX, posSY, '>');
        coloreLinhaMenu(&posSY);
        moveSeletor(&select, &posSX, &posSY);
        if(select == ENTER){//verifica se o usuario apertou enter
            switch(posSY){//verifica em qual posicao o seletor esta
            case(6)://novo jogo
                clrscr();
                gameInterface();
                inicializaJogador(&jogador);
                gameLoop(jogo, ranking, &jogador);
                clrscr();
                break;
            case(7)://carregar jogo
                clrscr();
                carregaJogo(jogo, ranking, &jogador);
                break;
            case(8)://recordes
                clrscr();
                recordes(ranking);
                clrscr();
                break;
            case(9)://creditos
                clrscr();
                creditos();
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
void moveSeletor(char *select, int *posSX, int *posSY){//move o seletor do menu
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
    char cell1 = ' ';
    char cell2 = '_';
    char cell3 = '-';
    int i,j;
    textcolor(LIGHTMAGENTA);
    cputsxy(1, 1, "VIDAS");
    cputsxy(70, 1, "PONTOS ");
    desenhaAgua();
    textcolor(DARKGRAY);
    for (i=LINHA1;i<LINHA2;i++){//constroi os limites vericais do cenario
            putchxy (COLUNA1, i, cell1);
            putchxy (COLUNA2, i, cell1);
    }
    for (i=COLUNA1;i<COLUNA2;i++){//constroi os limites horizontais do cenario
            putchxy (i, LINHA1, cell2);
            putchxy (i, LINHA2, cell2);
    }
    textcolor(LIGHTMAGENTA);
    cputsxy(1, LINHA2 + 2, "Oxigenio ");
    cputsxy(49, LINHA2 + 2, "Mergulhadores");
    textbackground(YELLOW);
    textcolor(YELLOW);
    for(i=10;i<INICIOXOXIGENIO;i ++){//constroi a barra de oxigenio
        cputsxy(i, LINHA2 + 2, " ");

    }
    textbackground(0);
}
void desenhaAgua(){
    textcolor(LIGHTCYAN);
    cputsxy(XAGUA, YAGUA, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}
