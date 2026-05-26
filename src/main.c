#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LINHAS 20
#define COLUNAS 30

    struct jogador{
      
      int linha;
      int coluna;
      int vida;  
      int pontos; 

    };

    struct inimigo{
        
        int linha;
        int coluna;
        int vivo;
        int direcao;
        int sobrepor;

    };
    
    void ImprimirMapa(int mapa[LINHAS][COLUNAS]){
        
        int i;
        int j;

        for(i = 0; i < LINHAS; i++){
            
            for(j = 0; j < COLUNAS; j++){
                
                if (mapa[i][j] == 1){

                    printf("# ");
                }else if(mapa[i][j] == 0){

                    printf("  ");
                }else if(mapa[i][j] == 2){

                    printf("@ ");
                }else if(mapa[i][j] == 3){

                    printf("X ");
                }else if(mapa[i][j] == 4){

                    printf(". ");
                }else if(mapa[i][j] == 6){

                    printf("G ");
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

        if(mapa[novaLinha][novaColuna] == 3){

            return 1;

        }else if(mapa[novaLinha][novaColuna] != 1 && mapa[novaLinha][novaColuna] != 3 ){

            if(mapa[novaLinha][novaColuna] == 4){
                player->pontos += 10;
            }
            
          

            mapa[player->linha][player->coluna] = 0;

            player->linha = novaLinha;
            player->coluna = novaColuna;

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

    void CopiarMapa(int mapa1[LINHAS][COLUNAS], int mapa2[LINHAS][COLUNAS]){

        int i, j;

        for(i = 0; i < LINHAS; i++){
            for(j = 0; j < COLUNAS; j++){
                 mapa1[i][j] = mapa2[i][j];
                }
            }
    }
    
    void PerderVida(struct jogador *player, struct inimigo *enemy, int mapa[LINHAS][COLUNAS], int fase){

        int InicioPL = 1;
        int InicioPC = 1;
        int inicioEL = 1;
        int InicioEC = 12;

        int InicioMapa2PL = 18;
        int InicioMapa2PC = 1;
        int InicioMapa2EL = 7;
        int InicioMapa2EC = 11;

        if(fase == 1){

            if(player->linha == enemy->linha && player->coluna == enemy->coluna){

                mapa[player->linha][player->coluna] = 0;

                player->linha = InicioPL;
                player->coluna = InicioPC;

                mapa[player->linha][player->coluna] = 2;
            
                player->vida--;
                /////////////////////////////////////////////////////////////////////
                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->linha = inicioEL;
                enemy->coluna = InicioEC;
                enemy->sobrepor = 0;
                enemy->direcao = rand() % 4;

                mapa[enemy->linha][enemy->coluna] = 6;
            
            }
        }else if(fase == 2) {
            if(player->linha == enemy->linha && player->coluna == enemy->coluna){

                mapa[player->linha][player->coluna] = 0;

                player->linha = InicioMapa2PL;
                player->coluna = InicioMapa2PC;

                mapa[player->linha][player->coluna] = 2;
                player->vida--;
            
                ////////////////////////////////////////////////////////

                mapa[enemy->linha][enemy->coluna] = enemy->sobrepor;

                enemy->linha = InicioMapa2EL;
                enemy->coluna = InicioMapa2EC;
                enemy->sobrepor = 0;
                enemy->direcao = rand() % 4;

                mapa[enemy->linha][enemy->coluna] = 6;
            
            }
        }
    }

    int VerificarGameOver(struct jogador player){

        if(player.vida <= 0){

            return 1;
        }else{

            return 0;
        }
    }
        

    int main(){

        srand(time(NULL));
        struct inimigo enemy;
        enemy.linha = 1;
        enemy.coluna = 12;
        enemy.direcao = 3;
        enemy.vivo = 1;
        enemy.sobrepor = 0;

        struct jogador player;
        player.vida = 3;
        player.pontos = 0;

    int Mapa[LINHAS][COLUNAS] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,4,4,4,4,0,0,0,0,0,1,6,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,3,1},
        {1,4,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,4,1},
        {1,4,0,0,0,4,0,1,0,0,0,4,0,0,0,0,4,0,1,0,0,0,4,0,0,0,0,4,1},
        {1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,4,0,0,0,0,1,0,0,0,6,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,4,0,1},
        {1,0,1,0,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1},
        {1,0,4,0,0,0,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,4,0,1},
        {1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,0,0,0,1,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
        {1,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,4,0,1},
        {1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,0,0,0,4,0,1,0,0,0,4,0,0,0,0,4,0,1,0,0,0,4,0,0,0,0,0,0,1},
        {1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,4,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4,0,1},
        {1,6,1,1,1,1,0,1,1,1,1,1,0,4,4,4,0,1,1,1,1,1,0,1,1,1,1,1,6,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
        
    int Mapa2[LINHAS][COLUNAS] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,3,0,0,0,0,4,4,4,4,0,1,6,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,1},
        {1,1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,1},
        {1,0,0,0,0,4,0,0,0,0,4,0,0,0,0,4,0,0,1,0,0,0,4,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,1,1,1,1,1,4,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {1,0,4,0,0,0,0,1,0,0,0,0,4,0,0,0,0,0,1,0,0,0,0,0,0,0,0,4,0,1},
        {1,0,1,0,1,1,1,1,0,1,1,1,1,1,4,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,4,0,0,0,0,0,0,0,0,6,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,4,0,1},
        {1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1},
        {1,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4,0,1,0,0,0,0,0,0,0,0,0,4,0,1},
        {1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,1,1,1,0,1,1,0,1,1},
        {1,0,4,0,0,4,0,1,0,0,0,0,0,6,0,0,0,0,1,0,0,0,4,0,0,0,0,4,0,1},
        {1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1},
        {1,0,0,0,0,4,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,4,0,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1},
        {1,0,4,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4,0,1},
        {1,2,1,1,1,1,0,1,1,1,1,1,0,4,4,4,0,1,1,1,1,1,0,1,1,1,1,1,6,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

        char tecla = ' ';

        int PassouDeFase = 0;
        int fase = 1;
            
    while(tecla != 'q'){

        system("cls");
        ImprimirMapa(Mapa);
        printf("\nVida: %d | Pontos: %d\n", player.vida, player.pontos);

        AcharJogador(&player, Mapa);

        printf("Jogador na linha %d e coluna %d\n", player.linha, player.coluna);

        scanf(" %c", &tecla);

        MoverInimigo(&enemy,Mapa);

        PassouDeFase = MoverJogador(&player, Mapa, tecla);
        
        PerderVida(&player, &enemy, Mapa, fase);

        if(VerificarGameOver(player) == 1){
            printf("Game Over!\n");
            break;
        }

        if(PassouDeFase == 1){

            if(fase == 1){

                printf("\nVoce passou para a fase 2!\n");

                CopiarMapa(Mapa, Mapa2);

                fase = 2;
                PassouDeFase = 0;
            }
            else{

                printf("\nVoce venceu o jogo!\n");
                break;
            }
        }
    }

        return 0;
    }
