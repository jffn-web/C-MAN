#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../cli-lib/include/keyboard.h"
#include "../cli-lib/include/screen.h"
#include "../cli-lib/include/timer.h"
#define LINHAS 20
#define COLUNAS 30
#define ESPERANDO 0
#define ANDANDO 1

    struct jogador{
      
      int linha;
      int coluna;
      int vida;  
      int pontos; 
      int poder;
      time_t inicioPoder;

    };

    struct inimigo{
        
        int linha;
        int coluna;
        int vivo;
        int direcao;
        int sobrepor;
        int linhaSpawn;
        int colunaSpawn;
        int tempoParaSair;
        int ParadoOuAndando;
        time_t inicioEspera;


        struct inimigo *prox;

    };
    struct inimigo enemy2;
    struct inimigo enemy3;
    struct inimigo enemy4;

    
    void ImprimirMapa(int mapa[LINHAS][COLUNAS],struct jogador *player){
        
        int i;
        int j;

        for(i = 0; i < LINHAS; i++){
            
            for(j = 0; j < COLUNAS; j++){
                
            if (mapa[i][j] == 1){

                printf("\033[34m[]\033[0m");

            }else if(mapa[i][j] == 0){

                printf("  ");

            }else if(mapa[i][j] == 2){

                printf("\033[33mC<\033[0m");

            }else if(mapa[i][j] == 4){

                printf("\033[37m..\033[0m");

            }else if(mapa[i][j] == 5){

                printf("\033[31m◉ \033[0m");

            }else if(mapa[i][j] == 6){
                
                if (player->poder == 0){
                    
                    printf("\033[35m()\033[0m");

                }else{

                    if( time(NULL) - player->inicioPoder >= 5){
                        
                        if(time(NULL) % 2 == 0){

                            printf("\033[97m()\033[0m");

                        }else{

                            printf("\033[94m()\033[0m");

                            }
                        }else{

                            printf("\033[94m()\033[0m");
                        }
                    }
                }
            } 

            printf("\n");
        }
    }    

    void AcharJogador(struct jogador *player,int mapa[LINHAS][COLUNAS]){

        int i;
        int j;

        for(i = 0; i < LINHAS; i++){

            for(j = 0; j < COLUNAS; j++){

                if(mapa[i][j] == 2){

                    player->linha = i;

                    player->coluna = j;

                }
            }
        }

    }

    int MoverJogador(struct jogador *player, int mapa[LINHAS][COLUNAS], char tecla){

        int novaLinha = player->linha;
        int novaColuna = player->coluna;

        if(tecla == 'd'){

            novaColuna ++;

        }
        else if(tecla == 'a'){

            novaColuna--;

        }
        else if(tecla == 'w'){

            novaLinha--;

        }
        else if(tecla == 's'){

            novaLinha++;

        }

        if(mapa[novaLinha][novaColuna] != 1 ){

            if(mapa[novaLinha][novaColuna] == 4){
                player->pontos += 10;
            }
            if(mapa[novaLinha][novaColuna] == 5){

                player->poder = 1;
                player->inicioPoder = time(NULL);

            }
            

            mapa[player->linha][player->coluna] = 0;

            player->linha = novaLinha;
            player->coluna = novaColuna;

            if(player->linha == 7 && player->coluna == 0 && tecla == 'a'){

                mapa[player->linha][player->coluna] = 0;

                player->coluna = 28;

            }

            if(player->linha == 7 && player->coluna == 29 && tecla == 'd'){

                mapa[player->linha][player->coluna] = 0;

                player->coluna = 1;

            }
            mapa[player->linha][player->coluna] = 2;


        }

        return 0;

        }

    void MoverInimigo(struct inimigo *enemy, int mapa[LINHAS][COLUNAS]){

        int novaLinha = enemy->linha;
        int novaColuna = enemy->coluna;


        if(enemy->direcao == 0){
            novaLinha--;
        }else if(enemy->direcao == 1){
            novaColuna++;
        }else if(enemy->direcao == 2){
            novaLinha++;
        }else if(enemy->direcao == 3){
            novaColuna--;
        }

      if(novaLinha == 7 && novaColuna < 0){
            
            novaColuna = 28;
            
        }

    if(novaLinha == 7 && novaColuna > 29){
            
            novaColuna = 1;

        }


        if(mapa[novaLinha][novaColuna] == 1 || mapa[novaLinha][novaColuna] == 2){

            enemy->direcao = rand() % 4;
        
        }else{


            mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

            enemy->sobrepor = mapa[novaLinha][novaColuna];

            enemy->linha = novaLinha;
            enemy->coluna = novaColuna;

            mapa[enemy->linha][enemy->coluna] = 6;

            }
    }   

    void AtualizarInimigo(struct inimigo *enemy, int mapa[LINHAS][COLUNAS]){

            if(enemy->ParadoOuAndando == ESPERANDO){

            if(time(NULL) - enemy->inicioEspera >= enemy->tempoParaSair){

                enemy->ParadoOuAndando = ANDANDO;
                enemy->direcao = 0;

            }

            }else if(enemy->ParadoOuAndando == ANDANDO){

            MoverInimigo(enemy, mapa);

            }
    }

    void CopiarMapa(int mapa1[LINHAS][COLUNAS], int mapa2[LINHAS][COLUNAS]){

        int i, j;

        for(i = 0; i < LINHAS; i++){
            for(j = 0; j < COLUNAS; j++){
                 mapa1[i][j] = mapa2[i][j];
                }
            }
    }

    void PoderDaCereja(struct jogador *player, struct inimigo *enemy,int mapa[LINHAS][COLUNAS], int fase ){

        
        if (player->poder == 1){


            if(time(NULL) - player->inicioPoder >= 10){

            player->poder = 0;

        }
    }

        if(fase == 1){
            if(player->linha == enemy->linha && player->coluna == enemy->coluna && player->poder == 1){

                    player->pontos += 50;
                    mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                    enemy->linha = enemy->linhaSpawn;
                    enemy->coluna = enemy->colunaSpawn;
                    enemy->sobrepor = 0;
                    enemy->ParadoOuAndando = ESPERANDO;
                    enemy->inicioEspera = time(NULL);
                    enemy->direcao = rand() % 4;

                    mapa[enemy->linha][enemy->coluna] = 6;
            }
        }else if(fase == 2){

                if(player->linha == enemy->linha && player->coluna == enemy->coluna && player->poder == 1){
                    player->pontos += 50;
                    mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                    enemy->linha = enemy->linhaSpawn;
                    enemy->coluna = enemy->colunaSpawn;
                    enemy->sobrepor = 0;
                    enemy->ParadoOuAndando = ESPERANDO;
                    enemy->inicioEspera = time(NULL);
                    enemy->direcao = rand() % 4;

                    mapa[enemy->linha][enemy->coluna] = 6;
                }
        }

    }
    
    void PerderVida(struct jogador *player, struct inimigo *enemy, int mapa[LINHAS][COLUNAS], int fase){

        int InicioPL = 1;
        int InicioPC = 1;
        

        int InicioMapa2PL = 18;
        int InicioMapa2PC = 1;
        

        if(fase == 1){

            if(player->linha == enemy->linha && player->coluna == enemy->coluna && player->poder == 0){

                mapa[player->linha][player->coluna] = 0;

                player->linha = InicioPL;
                player->coluna = InicioPC;

                mapa[player->linha][player->coluna] = 2;
                
                player->vida--;
                
                /////////////////////////////////////////////////////////////////////
                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->linha = enemy->linhaSpawn;
                enemy->coluna = enemy->colunaSpawn;
                enemy->sobrepor = 0;
                enemy->ParadoOuAndando = ESPERANDO;
                enemy->inicioEspera = time(NULL);
                enemy->direcao = rand() % 4;

                mapa[enemy->linha][enemy->coluna] = 6;
            
            }
        }else if(fase == 2) {
            if(player->linha == enemy->linha && player->coluna == enemy->coluna && player->poder == 0){

                mapa[player->linha][player->coluna] = 0;

                player->linha = InicioMapa2PL;
                player->coluna = InicioMapa2PC;

                mapa[player->linha][player->coluna] = 2;

                player->vida--;
            
                ////////////////////////////////////////////////////////

                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->linha = enemy->linhaSpawn;
                enemy->coluna = enemy->colunaSpawn;
                enemy->sobrepor = 0;
                enemy->ParadoOuAndando = ESPERANDO;
                enemy->inicioEspera = time(NULL);   
                enemy->direcao = rand() % 4;

                mapa[enemy->linha][enemy->coluna] = 6;
            
            }
        }
    }

    int VerificarSePassouDeFase(int mapa[LINHAS][COLUNAS]){

        int i, j;

        for(i = 0; i < LINHAS; i++){

            for(j = 0; j < COLUNAS; j++){

                if(mapa[i][j] == 4){

                    return 1;
                
                }
            }
        }   

        return 0;

    }

    int VerificarGameOver(struct jogador player){

        if(player.vida <= 0){

            return 1;
        }else{

            return 0;
        }
    }

    
        

    int main(){

        system("chcp 65001");

        srand(time(NULL));
        struct inimigo enemy;
        enemy.linha = 8;
        enemy.coluna = 14;

        enemy.linhaSpawn = 8;
        enemy.colunaSpawn = 14;

        enemy.tempoParaSair = 3;

        enemy.ParadoOuAndando = ESPERANDO;

        enemy.inicioEspera = time(NULL);

        enemy.direcao = 3;
        enemy.vivo = 1;
        enemy.sobrepor = 0;
        struct inimigo *atual;

        atual = &enemy;

        struct jogador player;
        player.vida = 3;
        player.pontos = 0;
        player.poder = 0;


    int MapaDoMaroto[LINHAS][COLUNAS];    

    int Mapa1[LINHAS][COLUNAS] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,4,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,4,4,4,4,4,4,5,1},
        {1,4,1,1,1,1,4,1,1,1,1,4,1,4,1,1,4,1,4,1,1,1,1,4,1,1,1,1,4,1},
        {1,4,1,0,0,1,4,1,0,0,1,4,4,4,1,1,4,4,4,1,0,0,1,4,1,0,0,1,4,1},
        {1,4,1,1,1,1,4,1,1,1,1,4,1,4,4,4,4,1,4,1,1,1,1,4,1,1,1,1,4,1},
        {1,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,1},
        {1,4,1,1,1,1,4,1,1,1,1,4,4,4,4,4,4,4,4,1,1,1,1,4,1,1,1,1,4,1},
        {0,0,4,4,4,4,4,1,4,4,4,4,1,4,0,4,1,4,4,4,4,1,4,4,4,4,4,4,0,0},
        {1,1,1,1,4,1,4,1,4,1,1,4,1,0,0,0,1,4,1,1,4,1,4,1,4,1,1,1,1,1},
        {1,4,4,4,4,1,4,4,4,1,0,4,1,6,6,6,1,4,0,1,4,4,4,1,4,4,4,4,4,1},
        {1,1,1,1,4,1,4,1,4,1,1,4,1,1,0,1,1,4,1,1,4,1,4,1,4,1,1,1,1,1},
        {1,4,4,4,4,4,4,1,4,4,4,4,4,4,0,4,4,4,4,4,4,1,4,4,4,4,4,4,4,1},
        {1,4,1,1,1,1,4,1,1,1,1,4,1,1,1,1,1,1,4,1,1,1,1,4,1,1,1,1,4,1},
        {1,4,4,4,5,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,5,4,4,4,4,1},
        {1,1,1,1,4,1,4,1,1,1,1,1,1,4,1,1,4,1,1,1,1,1,4,1,4,1,1,1,1,1},
        {1,4,4,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,4,4,4,4,4,4,4,1},
        {1,4,1,1,1,1,4,1,1,1,1,4,1,1,1,1,1,1,4,1,1,1,1,4,1,1,1,1,4,1},
        {1,4,1,0,0,1,4,1,0,0,1,4,4,4,4,4,4,4,4,1,0,0,1,4,1,0,0,1,4,1},
        {1,4,4,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,4,4,4,4,4,4,4,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };
    int Mapa2[LINHAS][COLUNAS] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,4,4,4,4,4,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,4,4,4,4,4,5,1},
        {1,4,1,1,1,1,4,1,4,1,1,1,1,4,1,1,4,1,1,1,1,4,1,4,1,1,1,1,4,1},
        {1,4,4,4,4,1,4,4,4,1,0,0,1,4,4,4,4,1,0,0,1,4,4,4,1,4,4,4,4,1},
        {1,1,1,1,4,1,1,1,4,1,1,1,1,1,4,4,1,1,1,1,1,4,1,1,1,4,1,1,1,1},
        {1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1},
        {1,4,1,1,1,4,1,1,1,1,4,1,1,1,4,4,1,1,1,4,1,1,1,1,4,1,1,1,4,1},
        {0,0,4,4,1,4,4,4,4,1,4,4,1,4,0,4,1,4,4,1,4,4,4,4,1,4,4,4,0,0},
        {1,1,1,4,1,1,1,1,4,1,1,4,1,0,0,0,1,4,1,1,4,1,1,1,1,4,1,1,1,1},
        {1,4,4,4,4,4,4,1,4,4,4,4,1,6,6,6,1,4,4,4,4,1,4,4,4,4,4,4,4,1},
        {1,1,1,4,1,1,4,1,1,1,1,4,1,1,0,1,1,4,1,1,1,1,4,1,1,4,1,1,1,1},
        {1,4,4,4,1,4,4,4,4,4,4,4,4,4,0,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1},
        {1,4,1,1,1,4,1,1,1,4,1,1,1,1,4,4,1,1,1,1,4,1,1,1,1,4,1,1,4,1},
        {1,4,4,4,4,4,4,4,1,4,4,4,4,4,5,4,4,4,4,4,4,1,4,4,4,4,4,4,4,1},
        {1,1,1,1,4,1,1,4,1,1,1,1,4,1,1,1,1,4,1,1,1,1,4,1,1,4,1,1,1,1},
        {1,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,4,5,1},
        {1,4,1,1,1,4,1,1,1,1,4,1,1,1,4,4,1,1,1,4,1,1,1,1,4,1,1,1,4,1},
        {1,4,4,4,1,4,4,4,4,1,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,1},
        {1,4,1,4,4,4,1,1,4,4,4,1,4,4,4,4,4,1,4,4,4,1,1,4,4,4,1,4,4,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

    char tecla = ' ';

        
    int fase = 1;
            
    CopiarMapa(MapaDoMaroto, Mapa1);
    MapaDoMaroto[enemy.linha][enemy.coluna] = 6;

    keyboardInit();
    screenInit(1);
    timerInit(200);

    while(tecla != 'q'){

        if (keyhit()) {
        tecla = readch();
    }

        if (timerTimeOver()) {

            screenClear();

            AcharJogador(&player, MapaDoMaroto);

            AtualizarInimigo(&enemy, MapaDoMaroto);
            MoverJogador(&player, MapaDoMaroto, tecla);

            PoderDaCereja(&player, &enemy, MapaDoMaroto, fase);
            PerderVida(&player, &enemy, MapaDoMaroto, fase);

            ImprimirMapa(MapaDoMaroto, &player);
            printf("\nVida: %d | Pontos: %d\n", player.vida, player.pontos);

            screenUpdate();
    }
        if(VerificarGameOver(player) == 1){
            printf("Game Over!\n");
            break;
        }

        if(VerificarSePassouDeFase(MapaDoMaroto) == 0){

            if(fase == 1){

                printf("\nVoce passou para a fase 2!\n");

                CopiarMapa(MapaDoMaroto, Mapa2);

                fase = 2;
                
            }
            else{
                CopiarMapa(MapaDoMaroto, Mapa1);
                fase = 1;
                
            }
             AcharJogador(&player, MapaDoMaroto);

        enemy.linha = enemy.linhaSpawn;
        enemy.coluna = enemy.colunaSpawn;
        enemy.ParadoOuAndando = ESPERANDO;
        enemy.inicioEspera = time(NULL);
        enemy.sobrepor = 0;
        enemy.direcao = rand() % 4;
        MapaDoMaroto[enemy.linha][enemy.coluna] = 6;
    }
        
    }
        keyboardDestroy();
        screenDestroy();
        timerDestroy();
        return 0;
    }
