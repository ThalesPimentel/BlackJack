#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pegarCarta() {
    return rand() % 13 + 1;
}

int valorCarta(int carta, int somaAtual) {
    if (carta >= 11) {
        return 10; // J, Q, K
    } else if (carta == 1) {
        if (somaAtual + 11 <= 21) {
            return 11; 
        } else {
            return 1; 
        }
    } else {
        return carta; 
    }
}

int main(void) {
    srand(time(NULL)); 

    int controlador = 0;
    int somaJogador = 0, somaBanca = 0;

    int num1 = pegarCarta();
    int valor1 = valorCarta(num1, somaJogador);
    somaJogador += valor1;

    int num2 = pegarCarta();
    int valor2 = valorCarta(num2, somaJogador);
    somaJogador += valor2;

    printf("Suas cartas: %d e %d\n", num1, num2);
    printf("Sua soma: %d\n", somaJogador);

    if (somaJogador > 21) {
        printf("Você perdeu! Estourou 21.\n");
        return 0;
    }

    printf("Digite 1 para continuar ou 0 para parar: ");
    scanf("%d", &controlador);

    while (somaJogador <= 21 && controlador == 1) {
        int novaCarta = pegarCarta();
        int valor = valorCarta(novaCarta, somaJogador);
        somaJogador += valor;

        printf("Você tirou: %d (vale %d)\n", novaCarta, valor);
        printf("Nova soma: %d\n", somaJogador);

        if (somaJogador > 21) {
            printf("Você perdeu! Estourou 21.\n");
            return 0;
        }

        printf("Digite 1 para continuar ou 0 para parar: ");
        scanf("%d", &controlador);
    }

    printf("\nVocê parou com a soma: %d\n", somaJogador);

    printf("\n--- Turno da Banca ---\n");

    while (somaBanca <= somaJogador && somaBanca < 21) {
        int cartaBanca = pegarCarta();
        int valorBanca = valorCarta(cartaBanca, somaBanca);
        somaBanca += valorBanca;
        printf("Banca tirou: %d (vale %d), soma: %d\n", cartaBanca, valorBanca, somaBanca);
    }

    printf("\nResultado final:\n");
    printf("Sua soma: %d\n", somaJogador);
    printf("Soma da banca: %d\n", somaBanca);

    if (somaBanca > 21) {
        printf("Banca estourou! Você venceu!\n");
    } else if (somaBanca > somaJogador) {
        printf("A banca venceu!\n");
    } else if (somaBanca < somaJogador) {
        printf("Você venceu!\n");
    } else {
        printf("Empate!\n");
    }

    return 0;
}