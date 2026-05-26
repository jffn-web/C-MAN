#include <stdio.h>
#include <stdlib.h>

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

    };
    
    void ImprimirMapa(int mapa[5][5]){
        
        int i;
        int j;

        for(i = 0; i < 5; i++){
            
            for(j = 0; j < 5; j++){
                
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

    int MoverJogador(struct jogador *player, int mapa[5][5], char tecla){

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
            
            if(mapa[novaLinha][novaColuna] == 6){

                player->vida--;

            }

            mapa[player->linha][player->coluna] = 0;

            player->linha = novaLinha;
            player->coluna = novaColuna;

            mapa[player->linha][player->coluna] = 2;

        }

        return 0;

        }

    void MoverInimigo(struct inimigo *enemy, int mapa[5][5]){

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

        if(mapa[novaLinha][novaColuna] == 1){

            enemy->direcao++;

            if(enemy->direcao > 3){

                enemy->direcao = 0;

            }
        
        }else{

                mapa[enemy->linha][enemy->coluna] = 0;

                enemy->linha = novaLinha;
                enemy->coluna = novaColuna;

                mapa[enemy->linha][enemy->coluna] = 6;

            }
    }   

    void CopiarMapa(int mapa1[5][5], int mapa2[5][5]){

        int i, j;

        for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++){
                 mapa1[i][j] = mapa2[i][j];
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

        struct inimigo enemy;
        enemy.linha = 2;
        enemy.coluna = 1;
        enemy.vivo = 1;
        enemy.direcao = 1;

        struct jogador player;
        player.vida = 3;
        player.pontos = 0;

        int Mapa[5][5] = {
            {1, 1, 1, 1, 1},
            {1, 2, 0, 3, 1},
            {1, 6, 1, 6, 1},
            {1, 4, 0, 0, 1},
            {1, 1, 1, 1, 1}
            };
        
        int Mapa2[5][5] = {
            {1, 3, 1, 1, 1},
            {1, 0, 6, 0, 1},
            {1, 0, 1, 6, 1},
            {1, 4, 0, 2, 1},
            {1, 1, 1, 1, 1}
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