#include <stdio.h>

    struct jogador{
      
      int linha;
      int coluna;
      int vida;  
    };
    
    void ImprimirMapa(int mapa[5][5]){
        
        int i;
        int j;

        for(i = 0; i < 5; i++){
            
            for(j = 0; j < 5; j++){
                
                printf("%d ",mapa[i][j]);
            } 

            printf("\n");
        }
    }    

    void AcharJogador(struct jogador *player,int mapa[5][5]){

        int i;
        int j;

        for(i = 0; i < 5; i++){

            for(j = 0; j < 5; j++){

                if(mapa[i][j] == 2){

                    player->linha = i;

                    player->coluna = j;

                }
            }
        }

    }

    void MoverJogador(struct jogador *player, int mapa[5][5], char tecla){

        if(tecla == 'd' && mapa[player->linha][player->coluna + 1] != 1){

            mapa[player->linha][player->coluna] = 0;
            player->coluna++;
            mapa[player->linha][player->coluna] = 2;

        }else if(tecla == 'a' && mapa[player->linha][player->coluna - 1] != 1){

            mapa[player->linha][player->coluna] = 0;
            player->coluna--;
            mapa[player->linha][player->coluna] = 2;

        }else if(tecla == 'w' && mapa[player->linha - 1][player->coluna] != 1){

            mapa[player->linha][player->coluna] = 0;
            player->linha--;
            mapa[player->linha][player->coluna] = 2;

        }else if(tecla == 's' && mapa[player->linha + 1][player->coluna] != 1){

            mapa[player->linha][player->coluna] = 0;
            player->linha++;
            mapa[player->linha][player->coluna] = 2;
        }
    }
        

    int main(){

        struct jogador player;

        int Mapa[5][5] = {
            {1, 1, 1, 1, 1},
            {1, 2, 0, 3, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}
            };

        ImprimirMapa(Mapa);

        AcharJogador(&player,Mapa);
        printf("Jogador na linha %d e coluna %d\n", player.linha, player.coluna);

        
        char tecla;
        scanf(" %c", &tecla);

        MoverJogador(&player, Mapa, tecla);

        ImprimirMapa(Mapa);


        return 0;
    }