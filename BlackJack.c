#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BlackJack.h"

#define NUM_CARTAS 52

const char *nipes[] = {"Copas", "Ouros", "Paus", "Espadas"};
const char *valores[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

typedef struct {
    const char *valor;
    const char *naipe;
    int pontos;
} Carta;

void inicializarBaralho(Carta baralho[]) {
    int i = 0;
    for (int n = 0; n < 4; n++) {
        for (int v = 0; v < 13; v++) {
            baralho[i].naipe = nipes[n];
            baralho[i].valor = valores[v];
            if (v >= 10) baralho[i].pontos = 10;
            else if (v == 0) baralho[i].pontos = 11;
            else baralho[i].pontos = v + 1;
            i++;
        }
    }
}

void embaralhar(Carta baralho[]) {
    for (int i = 0; i < NUM_CARTAS; i++) {
        int r = rand() % NUM_CARTAS;
        Carta temp = baralho[i];
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
}

void mostrarCarta(Carta c) {
    printf("%s de %s (%d pontos)\n", c.valor, c.naipe, c.pontos);
}

int calcularPontos(Carta mao[], int num_cartas) {
    int soma = 0, ases = 0;
    for (int i = 0; i < num_cartas; i++) {
        soma += mao[i].pontos;
        if (mao[i].pontos == 11) ases++;
    }
    while (soma > 21 && ases > 0) {
        soma -= 10;
        ases--;
    }
    return soma;
}

void jogarBlackJack() {
    Carta baralho[NUM_CARTAS];
    Carta jogador[12], dealer[12];
    int num_jogador = 0, num_dealer = 0;

    srand(time(NULL));
    inicializarBaralho(baralho);
    embaralhar(baralho);

    jogador[num_jogador++] = baralho[0];
    dealer[num_dealer++] = baralho[1];
    jogador[num_jogador++] = baralho[2];
    dealer[num_dealer++] = baralho[3];

    int topo = 4;

    printf("\nSuas cartas:\n");
    for (int i = 0; i < num_jogador; i++) mostrarCarta(jogador[i]);
    printf("Total: %d\n", calcularPontos(jogador, num_jogador));

    printf("\nCarta visível do dealer:\n");
    mostrarCarta(dealer[0]);

    char escolha;
    while (1) {
        printf("\nDeseja comprar outra carta? (s/n): ");
        scanf(" %c", &escolha);
        if (escolha == 's') {
            jogador[num_jogador++] = baralho[topo++];
            printf("\nSua nova carta:\n");
            mostrarCarta(jogador[num_jogador - 1]);
            int pontos = calcularPontos(jogador, num_jogador);
            printf("Total: %d\n", pontos);
            if (pontos > 21) {
                printf("Estourou! Dealer venceu.\n");
                return;
            }
        } else {
            break;
        }
    }

    printf("\nTurno do dealer:\n");
    while (calcularPontos(dealer, num_dealer) < 17) {
        dealer[num_dealer++] = baralho[topo++];
    }

    printf("\nCartas do dealer:\n");
    for (int i = 0; i < num_dealer; i++) mostrarCarta(dealer[i]);
    int pontos_jogador = calcularPontos(jogador, num_jogador);
    int pontos_dealer = calcularPontos(dealer, num_dealer);

    printf("\nTotal jogador: %d\n", pontos_jogador);
    printf("Total dealer: %d\n", pontos_dealer);

    if (pontos_dealer > 21 || pontos_jogador > pontos_dealer)
        printf("Você venceu!\n");
    else if (pontos_jogador < pontos_dealer)
        printf("Dealer venceu!\n");
    else
        printf("Empate!\n");
}