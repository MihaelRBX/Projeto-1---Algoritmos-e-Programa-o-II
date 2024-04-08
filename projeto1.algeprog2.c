//Projeto 1 - Alg e Prog 2 - Aluno: Mhael Rommel Barbosa Xavier - RA: 10239617

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PALAVRAS 6621
#define COMPRIMENTO_PALAVRA 5
#define MAX_TENTATIVAS 6

// como decidi imprimir a tentativa em maiusculo, criei essa funcao para padronizar a entrada, evitando problemas 
void converter_para_minuscula(char *palavra) {
    for (int i = 0; palavra[i]; i++) {
        palavra[i] = tolower(palavra[i]);
    }
}

int carregar_dicionario(const char *nome_arquivo, char palavras[MAX_PALAVRAS][COMPRIMENTO_PALAVRA + 1]) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir dicionário");
        return -1;
    }

    char buffer[100];
    int contador = 0;

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        size_t tamanho = strlen(buffer);
        if (buffer[tamanho - 1] == '\n') {
            buffer[tamanho - 1] = '\0'; 
        }
        if (strlen(buffer) == COMPRIMENTO_PALAVRA) {
            converter_para_minuscula(buffer);
            strcpy(palavras[contador++], buffer);
            if (contador >= MAX_PALAVRAS) {
                break; 
            }
        }
    }

    fclose(arquivo);
    return contador;
}

void imprimir_feedback(const char *tentativa, const char *palavra_alvo) {
    char tentativa_min[COMPRIMENTO_PALAVRA + 1];
    strcpy(tentativa_min, tentativa);
    converter_para_minuscula(tentativa_min); 

    // imprime a tentativa com os carecteres separados por espaco.
    for (int i = 0; i < COMPRIMENTO_PALAVRA; i++) {
        printf("%c ", toupper(tentativa[i]));
    }
    printf("\n");

    for (int i = 0; i < COMPRIMENTO_PALAVRA; i++) {
        if (tentativa_min[i] == palavra_alvo[i]) {
            printf("^ "); // letra correta na posicao certa.
        } else {
            int letraEncontrada = 0;
            for (int j = 0; j < COMPRIMENTO_PALAVRA; j++) {
                if (tentativa_min[i] == palavra_alvo[j]) {
                    letraEncontrada = 1;
                    break; // letra correta, posicao errada
                }
            }
            printf("%c ", letraEncontrada ? '!' : 'X'); // letra incorreta
        }
    }
    printf("\n");
}

int main() {
    char dicionario[MAX_PALAVRAS][COMPRIMENTO_PALAVRA + 1];
    char palavra_alvo[COMPRIMENTO_PALAVRA + 1];
    char tentativa[COMPRIMENTO_PALAVRA + 1];
    int qtd_palavras = carregar_dicionario("sem_acentos.txt", dicionario);
    time_t inicio = time(NULL), fim;
    double tempo_gasto;
    int tentativas = 0;

    if (qtd_palavras == -1) {
        fprintf(stderr, "Falha ao carregar o dicionário.\n");
        return 1;
    }

    srand(time(NULL));
    strcpy(palavra_alvo, dicionario[rand() % qtd_palavras]);

    printf("Bem-vindo ao jogo MACK-Advinha!\n");
    printf("Adivinhe a palavra de 5 letras.\n");

    while (tentativas < MAX_TENTATIVAS) {
        printf("Tentativa %d de %d. Digite sua tentativa: ", tentativas + 1, MAX_TENTATIVAS);
        scanf("%s", tentativa);

        if (strlen(tentativa) != COMPRIMENTO_PALAVRA) {
            printf("Por favor, digite uma palavra com exatamente %d letras.\n", COMPRIMENTO_PALAVRA);
            continue;
        }

        imprimir_feedback(tentativa, palavra_alvo);

        if (strcasecmp(tentativa, palavra_alvo) == 0) {
            fim = time(NULL);
            tempo_gasto = difftime(fim, inicio); // calcula o tempo gasto em segundos

        char nome_jogador[50]; // aqui podemos aumentar o numero conforme a necessidade
        printf("Parabéns! Você adivinhou a palavra corretamente!\n");
        printf("Por favor, digite seu nome: ");
        scanf("%s", nome_jogador);

        FILE *arquivo_scores = fopen("scores.txt", "a"); 
        if (arquivo_scores == NULL) {
            perror("Erro ao abrir o arquivo de scores");
            return 1;
        }

        fprintf(arquivo_scores, "%s %s %d %.2f\n", nome_jogador, palavra_alvo, tentativas + 1, tempo_gasto);
        fclose(arquivo_scores); 

        printf("Sua pontuação foi salva!\n");
        break;
    }

        tentativas++;
    }

    if (tentativas == MAX_TENTATIVAS) {
        printf("Você não conseguiu adivinhar. A palavra correta era '%s'.\n", palavra_alvo);
    }

    return 0;
}

