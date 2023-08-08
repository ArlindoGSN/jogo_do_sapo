#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TAMANHO 10
#define AZUL 'B'
#define VERM 'R'
#define VAZIO '.'
#define LAGO 'J'
#define SAPO_MORTO 'X'

int ultimoPontoAZUL = 0;
int ultimoPontoVERM = 0;

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void imprimirMatriz(char matriz[TAMANHO][TAMANHO], bool mostrarJacares, int pontoAzul, int pontoVerm) {
    printf("  0 1 2 3 4 5 6 7 8 9   Placar:\n");
    for (int i = 0; i < TAMANHO; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAMANHO; j++) {
            if (matriz[i][j] == LAGO && !mostrarJacares) {
                printf("%c ", VAZIO);
            } else {
                printf("%c ", matriz[i][j]);
            }
        }
        if (i == 4) {
            printf("   Jogador Azul (B): %d", pontoAzul);
        } else if (i == 5) {
            printf("   Jogador Vermelho (R): %d", pontoVerm);
        }
        printf("\n");
    }
}

bool moveSapo(char matriz[TAMANHO][TAMANHO], int player, int x, int y, int dx, int dy, int *pontoAzul, int *pontoVerm) {
    int novoX = x + dx;
    int novoY = y + dy;

    if (novoX < 0 || novoX >= TAMANHO || novoY < 0 || novoY >= TAMANHO) {
        printf("Movimento inválido. Fora dos limites do tabuleiro.\n");
        return false;
    }

    if (matriz[novoX][novoY] == LAGO) {
        matriz[x][y] = LAGO;
        matriz[novoX][novoY] = SAPO_MORTO;
        printf("Sapo morreu ao entrar na lagoa!\n");
        return false;
    }

    if (matriz[novoX][novoY] != VAZIO) {
        printf("Movimento inválido. Posição ocupada.\n");
        return false;
    }

    if (matriz[x][y] != (player == 1 ? AZUL : VERM)) {
        printf("Movimento inválido. Escolha um sapo válido pertencente ao jogador atual.\n");
        return false;
    }

    if (player == 1 && novoX == TAMANHO - 1) {
        (*pontoAzul)++;
        printf("Sapo Azul marcou um ponto!\n");
        matriz[novoX][novoY] = VAZIO;
    } else if (player == 2 && novoX == 0) {
        (*pontoVerm)++;
        printf("Sapo Vermelho marcou um ponto!\n");
        matriz[novoX][novoY] = VAZIO;
    }

    matriz[novoX][novoY] = (player == 1) ? AZUL : VERM;
    matriz[x][y] = VAZIO;

    return true;
}

void matrizInicial(char matriz[TAMANHO][TAMANHO]) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            matriz[i][j] = VAZIO;
        }
    }

    // Colocar lagos de jacaré
    matriz[3][3] = LAGO;
    matriz[6][7] = LAGO;
    matriz[8][1] = LAGO;
    matriz[8][3] = LAGO;
    matriz[8][5] = LAGO;

    // Colocar sapos azuis
    matriz[8][0] = AZUL;
    matriz[8][2] = AZUL;
    matriz[8][4] = AZUL;

    // Colocar sapos vermelhos
    matriz[1][1] = VERM;
    matriz[1][3] = VERM;
    matriz[1][5] = VERM;
    matriz[1][7] = VERM;
}

bool verificaTodosOsSapos(char matriz[TAMANHO][TAMANHO], int playerAtual) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (matriz[i][j] == (playerAtual == 1 ? AZUL : VERM)) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int novoX = i + dx;
                        int novoY = j + dy;

                        if (novoX >= 0 && novoX < TAMANHO && novoY >= 0 && novoY < TAMANHO) {
                            if (matriz[novoX][novoY] == VAZIO) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool verificaVencedor(int pontoAzul, int pontoVerm) {
    if (pontoAzul >= 5) {
        printf("Jogador Azul venceu com mais pontos!\n");
        return true;
    } else if (pontoVerm >= 5) {
        printf("Jogador Vermelho venceu com mais pontos!\n");
        return true;
    }
    return false;
}

int main() {
    char matriz[TAMANHO][TAMANHO];
    int playerAtual, playerX, playerY;
    int pontoAzul, pontoVerm;
    char escolha;

    while (true) {
        printf("\n-----------------\n");
        printf("Jogo do Sapo\n");
        printf("-----------------\n");
        printf("Menu:\n");
        printf("1 - Jogar\n");
        printf("2 - Ver Placar do Último Jogo\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == '1') {
            matrizInicial(matriz);

            playerAtual = 1;
            pontoAzul = 0;
            pontoVerm = 0;

            limparTela();
            printf("Placar:\n");
            printf("Jogador Azul: %d\n", pontoAzul);
            printf("Jogador Vermelho: %d\n", pontoVerm);

            while (true) {
                bool avaliaSapo = verificaTodosOsSapos(matriz, playerAtual);

                if (!avaliaSapo) {
                    printf("Não há sapos disponíveis para mover. A vez do jogador %c será pulada.\n", (playerAtual == 1) ? 'B' : 'R');
                    playerAtual = (playerAtual == 1) ? 2 : 1;

                    if (!verificaTodosOsSapos(matriz, playerAtual)) {
                        limparTela();
                        printf("Placar final:\n");
                        printf("Jogador Azul: %d\n", pontoAzul);
                        printf("Jogador Vermelho: %d\n", pontoVerm);

                        if (pontoAzul > pontoVerm) {
                            printf("Jogador Azul venceu com mais pontos!\n");
                        } else if (pontoVerm > pontoAzul) {
                            printf("Jogador Vermelho venceu com mais pontos!\n");
                        } else {
                            printf("Empate! Ambos os jogadores têm a mesma quantidade de pontos.\n");
                        }
                        break;
                    }
                    continue;
                }

                limparTela();
                imprimirMatriz(matriz, false, pontoAzul, pontoVerm);

                if (playerAtual == 1) {
                    playerY = 0;
                } else {
                    playerY = TAMANHO - 1;
                }

                printf("Jogador %c, informe a linha do sapo que deseja mover: ", (playerAtual == 1) ? 'B' : 'R');
                scanf("%d", &playerX);

                printf("Informe a coluna do sapo que deseja mover: ");
                scanf("%d", &playerY);

                if (matriz[playerX][playerY] == (playerAtual == 1 ? AZUL : VERM)) {
                    printf("Deseja fazer um movimento diagonal? (s/n): ");
                    char escolhaDiagonal;
                    scanf(" %c", &escolhaDiagonal);

                    if (escolhaDiagonal == 's') {
                        printf("Informe a direção do movimento (q-e-z-c): ");
                        char direcao;
                        scanf(" %c", &direcao);

                        int dx = 0, dy = 0;

                        switch (direcao) {
                            case 'q':
                                dx = -1;
                                dy = -1;
                                break;
                            case 'e':
                                dx = -1;
                                dy = 1;
                                break;
                            case 'z':
                                dx = 1;
                                dy = -1;
                                break;
                            case 'c':
                                dx = 1;
                                dy = 1;
                                break;
                            default:
                                printf("Direção inválida. Tente novamente.\n");
                                continue;
                        }

                        if (moveSapo(matriz, playerAtual, playerX, playerY, dx, dy, &pontoAzul, &pontoVerm)) {
                            playerAtual = (playerAtual == 1) ? 2 : 1;
                        }

                        if (verificaVencedor(pontoAzul, pontoVerm)) {
                            break;
                        }
                    } else {
                        printf("Informe a direção do movimento (w-a-s-d): ");
                        char direcao;
                        scanf(" %c", &direcao);

                        int dx = 0, dy = 0;

                        switch (direcao) {
                            case 'w':
                                dx = -1;
                                break;
                            case 's':
                                dx = 1;
                                break;
                            case 'a':
                                dy = -1;
                                break;
                            case 'd':
                                dy = 1;
                                break;
                            default:
                                printf("Direção inválida. Tente novamente.\n");
                                continue;
                        }

                        if (moveSapo(matriz, playerAtual, playerX, playerY, dx, dy, &pontoAzul, &pontoVerm)) {
                            playerAtual = (playerAtual == 1) ? 2 : 1;
                        }

                        if (verificaVencedor(pontoAzul, pontoVerm)) {
                            break;
                        }
                    }
                } else {
                    printf("Movimento inválido. Escolha um sapo válido pertencente ao jogador atual.\n");
                    playerAtual = (playerAtual == 1) ? 2 : 1;
                }
            }

            ultimoPontoAZUL = pontoAzul;
            ultimoPontoVERM = pontoVerm;
        } else if (escolha == '2') {
            limparTela();
            printf("Placar do Último Jogo:\n");
            printf("Jogador Azul: %d\n", ultimoPontoAZUL);
            printf("Jogador Vermelho: %d\n", ultimoPontoVERM);
        } else if (escolha == '3') {
            printf("Obrigado por jogar! Até a próxima!\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
