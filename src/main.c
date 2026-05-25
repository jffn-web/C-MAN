#include <stdio.h>

    struct jogador{
      
      int linha;
      int coluna;
      int vida;  
      int pontos;
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

        int novaLinha = player->linha;
        int novaColuna = player->coluna;

        if(tecla == 'd'){
            novaColuna++;
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

        if(mapa[novaLinha][novaColuna] != 1){

            if(mapa[novaLinha][novaColuna] == 4){
                player->pontos += 10;
        }

            mapa[player->linha][player->coluna] = 0;

            player->linha = novaLinha;
            player->coluna = novaColuna;

            mapa[player->linha][player->coluna] = 2;
    }
}
        

    int main(){

        struct jogador player;
        player.vida = 3;
        player.pontos = 0;

        int Mapa[5][5] = {
            {1, 1, 1, 1, 1},
            {1, 2, 0, 3, 1},
            {1, 0, 1, 0, 1},
            {1, 4, 0, 0, 1},
            {1, 1, 1, 1, 1}
            };
        char tecla = ' ';
            
    while(tecla != 'q'){

        ImprimirMapa(Mapa);
        printf("\nVida: %d | Pontos: %d\n", player.vida, player.pontos);

        AcharJogador(&player, Mapa);

        printf("Jogador na linha %d e coluna %d\n", player.linha, player.coluna);

        scanf(" %c", &tecla);

        MoverJogador(&player, Mapa, tecla);
    }   
        return 0;
    }