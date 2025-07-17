#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MAX_CARTAS 12
#define NUM_CARTAS 13

const char *valores[] = {"ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king"};

typedef struct {
    const char *valor;
    int pontos;
    Texture2D imagem;
} CartaG;

CartaG criarCarta(const char *valor) {
    CartaG c;
    c.valor = valor;

    if (strcmp(valor, "ace") == 0) c.pontos = 11;
    else if (strcmp(valor, "jack") == 0 || strcmp(valor, "queen") == 0 || strcmp(valor, "king") == 0)
        c.pontos = 10;
    else c.pontos = atoi(valor);

    char path[64];
    snprintf(path, sizeof(path), "assets/%s_of_spades.png", valor);
    c.imagem = LoadTexture(path);
    return c;
}

void descarregarCartas(CartaG baralho[]) {
    for (int i = 0; i < NUM_CARTAS; i++) {
        UnloadTexture(baralho[i].imagem);
    }
}

int calcularPontos(CartaG mao[], int total) {
    int soma = 0, ases = 0;
    for (int i = 0; i < total; i++) {
        soma += mao[i].pontos;
        if (mao[i].pontos == 11) ases++;
    }
    while (soma > 21 && ases > 0) {
        soma -= 10;
        ases--;
    }
    return soma;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BlackJack com Raylib");
    SetTargetFPS(60);
    srand(time(NULL));

    CartaG baralho[NUM_CARTAS];
    for (int i = 0; i < NUM_CARTAS; i++) {
        baralho[i] = criarCarta(valores[i]);
    }

    CartaG jogador[MAX_CARTAS], dealer[MAX_CARTAS];
    int numJogador = 0, numDealer = 0, jogoAtivo = 1, fimDeJogo = 0;
    const char *mensagem = "Clique em COMPRAR para iniciar.";

    Rectangle btnComprar = { 100, 600, 200, 50 };
    Rectangle btnParar   = { 350, 600, 200, 50 };
    Rectangle btnReiniciar = { 600, 600, 200, 50 };

    // Compra inicial
    jogador[numJogador++] = baralho[rand() % NUM_CARTAS];
    dealer[numDealer++] = baralho[rand() % NUM_CARTAS];
    jogador[numJogador++] = baralho[rand() % NUM_CARTAS];
    dealer[numDealer++] = baralho[rand() % NUM_CARTAS];

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawText("BLACKJACK", 20, 20, 40, GOLD);
        DrawText(mensagem, 20, 70, 24, RAYWHITE);

        // Cartas jogador
        DrawText("Jogador:", 20, 120, 20, WHITE);
        for (int i = 0; i < numJogador; i++) {
            DrawTexture(jogador[i].imagem, 20 + i * 80, 150, WHITE);
        }
        DrawText(TextFormat("Total: %d", calcularPontos(jogador, numJogador)), 20, 250, 20, WHITE);

        // Cartas dealer
        DrawText("Dealer:", 20, 300, 20, WHITE);
        for (int i = 0; i < numDealer; i++) {
            DrawTexture(dealer[i].imagem, 20 + i * 80, 330, WHITE);
        }
        DrawText(TextFormat("Total: %d", calcularPontos(dealer, numDealer)), 20, 430, 20, WHITE);

        // Botões
        DrawRectangleRec(btnComprar, BLUE);
        DrawText("COMPRAR", btnComprar.x + 50, btnComprar.y + 15, 20, WHITE);

        DrawRectangleRec(btnParar, RED);
        DrawText("PARAR", btnParar.x + 60, btnParar.y + 15, 20, WHITE);

        DrawRectangleRec(btnReiniciar, DARKGRAY);
        DrawText("REINICIAR", btnReiniciar.x + 40, btnReiniciar.y + 15, 20, WHITE);

        // Ações
        if (jogoAtivo) {
            if (CheckCollisionPointRec(GetMousePosition(), btnComprar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                jogador[numJogador++] = baralho[rand() % NUM_CARTAS];
                int total = calcularPontos(jogador, numJogador);
                if (total > 21) {
                    mensagem = "Você estourou! Dealer vence.";
                    jogoAtivo = 0;
                    fimDeJogo = 1;
                }
            }
            else if (CheckCollisionPointRec(GetMousePosition(), btnParar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                while (calcularPontos(dealer, numDealer) < 17 && numDealer < MAX_CARTAS) {
                    dealer[numDealer++] = baralho[rand() % NUM_CARTAS];
                }

                int pJog = calcularPontos(jogador, numJogador);
                int pDeal = calcularPontos(dealer, numDealer);
                if (pDeal > 21 || pJog > pDeal)
                    mensagem = "Você venceu!";
                else if (pJog < pDeal)
                    mensagem = "Dealer venceu!";
                else
                    mensagem = "Empate!";

                jogoAtivo = 0;
                fimDeJogo = 1;
            }
        } else if (fimDeJogo) {
            if (CheckCollisionPointRec(GetMousePosition(), btnReiniciar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                numJogador = numDealer = 0;
                jogador[numJogador++] = baralho[rand() % NUM_CARTAS];
                dealer[numDealer++] = baralho[rand() % NUM_CARTAS];
                jogador[numJogador++] = baralho[rand() % NUM_CARTAS];
                dealer[numDealer++] = baralho[rand() % NUM_CARTAS];
                jogoAtivo = 1;
                fimDeJogo = 0;
                mensagem = "Clique em COMPRAR para jogar.";
            }
        }

        EndDrawing();
    }

    descarregarCartas(baralho);
    CloseWindow();
    return 0;
}