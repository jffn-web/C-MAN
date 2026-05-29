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
      char nome[30];  


    };

    struct score{

        char nome[30];
        int pontos;

    };

    struct inimigo{
        
        int linha;
        int coluna;
        int direcao;
        int sobrepor;
        int linhaSpawn;
        int colunaSpawn;
        int tempoParaSair;
        int ParadoOuAndando;
        time_t inicioEspera;

        int cor;


        struct inimigo *prox;

    };
    struct inimigo enemy2;
    struct inimigo enemy3;
    struct inimigo enemy4;

    
    void ImprimirMapa(int mapa[LINHAS][COLUNAS], struct jogador *player, struct inimigo *enemy){

        int i;
        int j;

        for(i = 0; i < LINHAS; i++){

            for(j = 0; j < COLUNAS; j++){

                if(mapa[i][j] == 1){
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

                    int desenhou = 0;
                    struct inimigo *atual = enemy;

                    while(atual != NULL){

                        if(atual->linha == i && atual->coluna == j){
                            desenhou = 1;

                            if(player->poder == 0){
                                printf("\033[%dm()\033[0m", atual->cor);
                            }else{
                                if(time(NULL) - player->inicioPoder >= 5){
                                    if(time(NULL) % 2 == 0){
                                        printf("\033[97m()\033[0m");
                                    }else{
                                        printf("\033[94m()\033[0m");
                                    }
                                }else{
                                    printf("\033[94m()\033[0m");
                                }
                            }

                            break;
                        }

                        atual = atual->prox;
                    }

                    if(desenhou == 0){
                        printf("  ");
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
    void MoverInimigo(struct inimigo *enemy, int mapa[LINHAS][COLUNAS], struct jogador *player){

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

            if(novaLinha == player->linha && novaColuna == player->coluna){

                printf("COLIDIU\n");
                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->linha = novaLinha;
                enemy->coluna = novaColuna;
                enemy->sobrepor = 0;

                mapa[enemy->linha][enemy->coluna] = 6;

                return;
            }

            if(mapa[novaLinha][novaColuna] == 1 || mapa[novaLinha][novaColuna] == 6){

                enemy->direcao = rand() % 4;

            }else{

                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->sobrepor = mapa[novaLinha][novaColuna];

                enemy->linha = novaLinha;
                enemy->coluna = novaColuna;

                mapa[enemy->linha][enemy->coluna] = 6;
            }
        } 

    void AtualizarInimigo(struct inimigo *enemy, int mapa[LINHAS][COLUNAS], struct jogador *player){

            if(enemy->ParadoOuAndando == ESPERANDO){

            if(time(NULL) - enemy->inicioEspera >= enemy->tempoParaSair){

                enemy->ParadoOuAndando = ANDANDO;
                enemy->direcao = 0;

            }

            }else if(enemy->ParadoOuAndando == ANDANDO){

            MoverInimigo(enemy, mapa, player);

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

                    mapa[player->linha][player->coluna] = 2;
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
        

        int InicioMapa2PL = 1;
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

   void InicializarInimigo(struct inimigo *enemy,int linha, int coluna, int tempo, int cor){

        enemy->linha = linha;
        enemy->coluna = coluna;

        enemy->linhaSpawn = linha;
        enemy->colunaSpawn = coluna;

        enemy->tempoParaSair = tempo;

        enemy->cor = cor;

        enemy->ParadoOuAndando = ESPERANDO;

        enemy->inicioEspera = time(NULL);

        enemy->direcao = 0;
        enemy->sobrepor = 0;

   }

   void SalvarScore(struct jogador *player){

        FILE *arquivo;

        arquivo = fopen("score.txt", "a");

        if(arquivo != NULL){

            fprintf(arquivo, "%s %d\n", player->nome, player->pontos);

            fclose(arquivo);
        }
    }
        
    void HUD(int escolha){

            screenClear();

            printf("\n\n");
            printf("\033[33m========================================\033[0m\n");
            printf("\033[33m              C  -  M A N              \033[0m\n");
            printf("\033[33m========================================\033[0m\n\n");

            printf("              \033[33mC<\033[0m    \033[31m()\033[0m    \033[32m()\033[0m    \033[35m()\033[0m    \033[36m()\033[0m\n\n");

            printf("\033[34m----------------------------------------\033[0m\n\n");

            if(escolha == 1){
                printf("              \033[32m> [ JOGAR ]\033[0m\n\n");
                printf("                PONTUACAO\n\n");
                printf("                SAIR\n\n");
            }
            else if(escolha == 2){
                printf("                JOGAR\n\n");
                printf("              \033[32m> [ PONTUACAO ]\033[0m\n\n");
                printf("                SAIR\n\n");
            }
            else if(escolha == 3){
                printf("                JOGAR\n\n");
                printf("                PONTUACAO\n\n");
                printf("              \033[32m> [ SAIR ]\033[0m\n\n");
            }

            printf("\033[34m----------------------------------------\033[0m\n\n");
            printf("        Use \033[32mW/S\033[0m para mover\n");
            printf("        Use \033[32mENTER\033[0m para escolher\n\n");

            screenUpdate();
        }

    int MenuInicial(){

            char tecla = ' ';
            int escolha = 1;

            while(1){

                HUD(escolha);

                tecla = readch();
                

                if(tecla == 'w'){

                    if(escolha == 1){
                        escolha = 3;
                    }
                    else if(escolha == 2){
                        escolha = 1;
                    }
                    else{
                        escolha = 2;
                    }
                }

                else if(tecla == 's'){

                    if(escolha == 1){
                        escolha = 2;
                    }
                    else if(escolha == 2){
                        escolha = 3;
                    }
                    else{
                        escolha = 1;
                    }
                }

                if(tecla == 10 || tecla == 13){
                     return escolha;
                }
            }
        }
    
    void MostrarRanking(){

        FILE *arquivo;
        struct score *jogadores;
        int quantidade = 0;
        int i;
        char tecla = ' ';

        jogadores = malloc(100 * sizeof(struct score));

        if(jogadores == NULL){

            printf("Erro ao alocar memoria.\n");
            return;

        }

        while(tecla != 'q' && tecla != 'Q'){

            quantidade = 0;

            screenClear();

            printf("\033[33m========================================\033[0m\n");
            printf("\033[33m          PONTUACAO SALVA              \033[0m\n");
            printf("\033[33m========================================\033[0m\n\n");

            arquivo = fopen("score.txt", "r");

            if(arquivo != NULL){

                while(quantidade < 100 && fscanf(arquivo, "%29s %d", jogadores[quantidade].nome, &jogadores[quantidade].pontos) == 2){
                
                    quantidade++;

                }

                fclose(arquivo);

                for(i = 0; i < quantidade; i++){

                    printf("              %s - %d pontos\n\n",
                        jogadores[i].nome,
                        jogadores[i].pontos);

                }

            }else{

                printf("        Nenhum score salvo ainda.\n\n");

            }

            printf("\033[34m----------------------------------------\033[0m\n\n");
            printf("        Aperte \033[32mQ\033[0m para voltar\n");

            tecla = readch();
        }

        free(jogadores);
    }

    void Jogar(){

        srand(time(NULL));

        struct inimigo enemy;

        enemy.prox = &enemy2;
        enemy2.prox = &enemy3;
        enemy3.prox = &enemy4;
        enemy4.prox = NULL;

        InicializarInimigo(&enemy, 8, 14, 3, 31);
        InicializarInimigo(&enemy2, 9, 13, 6, 32);
        InicializarInimigo(&enemy3, 9, 14, 9, 35);
        InicializarInimigo(&enemy4, 9, 15, 12, 36);

        struct inimigo *atual = &enemy;

        struct jogador player;

       keyboardDestroy();

       printf("Digite seu nome: ");
       scanf("%29s", player.nome);

        keyboardInit();
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
        {1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,1},
        {1,4,1,1,1,4,1,1,1,1,4,1,1,1,4,4,1,1,1,4,1,1,1,1,4,1,1,1,4,1},
        {1,4,4,4,1,4,4,4,4,1,4,4,4,4,4,4,4,4,4,1,4,4,4,4,1,4,4,4,4,1},
        {1,4,1,4,4,4,1,1,4,4,4,1,4,4,4,4,4,1,4,4,4,1,1,4,4,4,1,4,4,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

        char tecla = ' ';
        int fase = 1;

        CopiarMapa(MapaDoMaroto, Mapa1);

        MapaDoMaroto[enemy.linha][enemy.coluna] = 6;
        MapaDoMaroto[enemy2.linha][enemy2.coluna] = 6;
        MapaDoMaroto[enemy3.linha][enemy3.coluna] = 6;
        MapaDoMaroto[enemy4.linha][enemy4.coluna] = 6;

        while(tecla != 'q'){

            if(keyhit()){
                tecla = readch();
            }

            if(timerTimeOver()){

                screenClear();

                AcharJogador(&player, MapaDoMaroto);

                MoverJogador(&player, MapaDoMaroto, tecla);

                atual = &enemy;

                while(atual != NULL){

                    PoderDaCereja(&player, atual, MapaDoMaroto, fase);
                    PerderVida(&player, atual, MapaDoMaroto, fase);

                    AtualizarInimigo(atual, MapaDoMaroto, &player);

                    PoderDaCereja(&player, atual, MapaDoMaroto, fase);
                    PerderVida(&player, atual, MapaDoMaroto, fase);

                    atual = atual->prox;
                }

                ImprimirMapa(MapaDoMaroto, &player, &enemy);

                printf("\nVida: %d | Pontos: %d\n",
                    player.vida,
                    player.pontos);

                screenUpdate();
            }

            if(VerificarGameOver(player) == 1){

                SalvarScore(&player);

                printf("Game Over!\n");

                break;
            }

            if(VerificarSePassouDeFase(MapaDoMaroto) == 0){

                if(fase == 1){

                    printf("\nVoce passou para a fase 2!\n");

                    CopiarMapa(MapaDoMaroto, Mapa2);

                    fase = 2;

                }else{

                    CopiarMapa(MapaDoMaroto, Mapa1);

                    fase = 1;
                }

                AcharJogador(&player, MapaDoMaroto);

                atual = &enemy;

                while(atual != NULL){

                    atual->linha = atual->linhaSpawn;
                    atual->coluna = atual->colunaSpawn;
                    atual->ParadoOuAndando = ESPERANDO;
                    atual->inicioEspera = time(NULL);
                    atual->sobrepor = 0;
                    atual->direcao = rand() % 4;

                    MapaDoMaroto[atual->linha][atual->coluna] = 6;

                    atual = atual->prox;
                }
            }
        }
    }
        

    int main(){

        int opcao;

        keyboardInit();
        screenInit(1);
        timerInit(125);

        while(1){

            opcao = MenuInicial();

            if(opcao == 1){
                Jogar();
            }
            else if(opcao == 2){
                MostrarRanking();
            }
            else if(opcao == 3){
                break;
            }
        }

        keyboardDestroy();
        screenDestroy();
        timerDestroy();

        return 0;
    }