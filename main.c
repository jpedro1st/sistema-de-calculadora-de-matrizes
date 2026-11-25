
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>

#define MAX_MATRIZES 10
#define MAX_DIM 10
#define TAM_BUFFER 256
#define EPSILON (100.0 * DBL_EPSILON)

typedef struct {
    int linhas;
    int colunas;
    double dados[MAX_DIM][MAX_DIM];
} Matriz;

// PROTOTIPOS
void lerLinha(char mensagem[], char buffer[], size_t tamanho);
int lerInteiro(char mensagem[], int minimo, int maximo);
double lerDouble(char mensagem[]);
void lerMatriz(Matriz *m, int numero);
void exibirMatriz(Matriz m);
int matrizFinita(Matriz m);

int somar(Matriz a, Matriz b, Matriz *r);
int subtrair(Matriz a, Matriz b, Matriz *r);
int multiplicar(Matriz a, Matriz b, Matriz *r);
int inverter(Matriz original, Matriz *inversa);
int dividir(Matriz a, Matriz b, Matriz *r);

void executarOperacao(Matriz matrizes[],
                      int quantidade, int opcao);
void menu(void);

// LEITURA SEGURA DE TEXTO
void lerLinha(char mensagem[], char buffer[], size_t tamanho) {
    while (1) {
        printf("%s", mensagem);
        fflush(stdout);

        if (fgets(buffer, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(EXIT_FAILURE);
        }

        size_t n = strlen(buffer);

        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
            return;
        }

        if (feof(stdin))
            return;

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Entrada muito longa. Tente novamente.\n");
    }
}

// LEITURA E VALIDACAO DE INTEIROS
int lerInteiro(char mensagem[], int minimo, int maximo) {
    char buffer[TAM_BUFFER];
    char *fim;
    long valor;

    while (1) {
        lerLinha(mensagem, buffer, sizeof(buffer));

        errno = 0;
        valor = strtol(buffer, &fim, 10);

        while (isspace((unsigned char)*fim))
            fim++;

        if (buffer[0] != '\0' &&
            *fim == '\0' &&
            errno != ERANGE &&
            valor >= minimo &&
            valor <= maximo) {

            return (int)valor;
        }

        printf("Entrada invalida! Digite um inteiro "
               "entre %d e %d.\n", minimo, maximo);
    }
}

// LEITURA E VALIDACAO DE NUMEROS REAIS
double lerDouble(char mensagem[]) {
    char buffer[TAM_BUFFER];
    char *fim;
    double valor;

    while (1) {
        lerLinha(mensagem, buffer, sizeof(buffer));

        errno = 0;
        valor = strtod(buffer, &fim);

        while (isspace((unsigned char)*fim))
            fim++;

        if (buffer[0] != '\0' &&
            *fim == '\0' &&
            errno != ERANGE &&
            isfinite(valor)) {

            return valor;
        }

        printf("Numero invalido! Digite um valor real valido.\n");
    }
}

// CADASTRO DE MATRIZES
void lerMatriz(Matriz *m, int numero) {
    char mensagem[100];

    printf("\n====================================\n");
    printf("       CADASTRO DA MATRIZ %d\n", numero);
    printf("====================================\n");

    m->linhas = lerInteiro(
        "Quantidade de linhas: ", 1, MAX_DIM);

    m->colunas = lerInteiro(
        "Quantidade de colunas: ", 1, MAX_DIM);

    for (int i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {

            snprintf(mensagem, sizeof(mensagem),
                     "Elemento [%d][%d]: ", i + 1, j + 1);

            m->dados[i][j] = lerDouble(mensagem);
        }
    }

    printf("Matriz %d cadastrada com sucesso!\n", numero);
}

// EXIBICAO DE MATRIZES
void exibirMatriz(Matriz m) {
    printf("\nMatriz resultante (%d x %d):\n",
           m.linhas, m.colunas);

    for (int i = 0; i < m.linhas; i++) {
        printf("| ");

        for (int j = 0; j < m.colunas; j++) {
            printf("%12.5g ", m.dados[i][j]);
        }

        printf("|\n");
    }
}

// VERIFICA VALORES FINITOS
int matrizFinita(Matriz m) {
    for (int i = 0; i < m.linhas; i++) {
        for (int j = 0; j < m.colunas; j++) {
            if (!isfinite(m.dados[i][j]))
                return 0;
        }
    }

    return 1;
}

// ADICAO
int somar(Matriz a, Matriz b, Matriz *r) {
    if (a.linhas != b.linhas ||
        a.colunas != b.colunas) {

        printf("\nERRO: as matrizes precisam "
               "ter as mesmas dimensoes.\n");
        return 0;
    }

    r->linhas = a.linhas;
    r->colunas = a.colunas;

    for (int i = 0; i < a.linhas; i++) {
        for (int j = 0; j < a.colunas; j++) {
            r->dados[i][j] =
                a.dados[i][j] + b.dados[i][j];
        }
    }

    if (!matrizFinita(*r)) {
        printf("ERRO: resultado numerico invalido na adicao.\n");
        return 0;
    }

    return 1;
}

// SUBTRACAO
int subtrair(Matriz a, Matriz b, Matriz *r) {
    if (a.linhas != b.linhas ||
        a.colunas != b.colunas) {

        printf("\nERRO: as matrizes precisam "
               "ter as mesmas dimensoes.\n");
        return 0;
    }

    r->linhas = a.linhas;
    r->colunas = a.colunas;

    for (int i = 0; i < a.linhas; i++) {
        for (int j = 0; j < a.colunas; j++) {
            r->dados[i][j] =
                a.dados[i][j] - b.dados[i][j];
        }
    }

    if (!matrizFinita(*r)) {
        printf("ERRO: resultado numerico invalido na subtracao.\n");
        return 0;
    }

    return 1;
}

// MULTIPLICACAO
int multiplicar(Matriz a, Matriz b, Matriz *r) {
    if (a.colunas != b.linhas) {
        printf("\nERRO: dimensoes incompativeis para multiplicacao.\n");
        printf("Colunas da primeira: %d\n", a.colunas);
        printf("Linhas da segunda: %d\n", b.linhas);
        return 0;
    }

    r->linhas = a.linhas;
    r->colunas = b.colunas;

    for (int i = 0; i < r->linhas; i++) {
        for (int j = 0; j < r->colunas; j++) {
            double soma = 0.0;

            for (int k = 0; k < a.colunas; k++) {
                double produto =
                    a.dados[i][k] * b.dados[k][j];

                if (!isfinite(produto)) {
                    printf("ERRO: estouro numerico na multiplicacao.\n");
                    return 0;
                }

                soma += produto;

                if (!isfinite(soma)) {
                    printf("ERRO: resultado fora do limite numerico.\n");
                    return 0;
                }
            }

            r->dados[i][j] = soma;
        }
    }

    return 1;
}

// INVERSAO PELO METODO DE GAUSS-JORDAN
int inverter(Matriz original, Matriz *inversa) {
    if (original.linhas != original.colunas) {
        printf("ERRO: a matriz divisora nao e quadrada.\n");
        return 0;
    }

    if (!matrizFinita(original)) {
        printf("ERRO: matriz contem valores invalidos.\n");
        return 0;
    }

    int n = original.linhas;

    double auxiliar[MAX_DIM][2 * MAX_DIM];
    double escala = 0.0;

    // Encontra a maior magnitude
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double valor = fabs(original.dados[i][j]);

            if (valor > escala)
                escala = valor;
        }
    }

    if (escala == 0.0) {
        printf("ERRO: matriz singular.\n");
        return 0;
    }

    // Monta a matriz aumentada [A/escala | I]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auxiliar[i][j] =
                original.dados[i][j] / escala;

            auxiliar[i][j + n] =
                (i == j) ? 1.0 : 0.0;
        }
    }

    // Gauss-Jordan com pivotamento parcial
    for (int i = 0; i < n; i++) {
        int pivo = i;

        for (int k = i + 1; k < n; k++) {
            if (fabs(auxiliar[k][i]) >
                fabs(auxiliar[pivo][i])) {

                pivo = k;
            }
        }

        if (fabs(auxiliar[pivo][i]) < EPSILON) {
            printf("ERRO: matriz singular ou numericamente "
                   "singular. Nao possui inversa confiavel.\n");
            return 0;
        }

        // Troca as linhas
        if (pivo != i) {
            for (int j = 0; j < 2 * n; j++) {
                double temp = auxiliar[i][j];

                auxiliar[i][j] = auxiliar[pivo][j];
                auxiliar[pivo][j] = temp;
            }
        }

        // Normaliza o pivo
        double divisor = auxiliar[i][i];

        for (int j = 0; j < 2 * n; j++) {
            auxiliar[i][j] /= divisor;

            if (!isfinite(auxiliar[i][j])) {
                printf("ERRO: falha numerica na inversao.\n");
                return 0;
            }
        }

        // Zera os outros elementos da coluna
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double fator = auxiliar[k][i];

                for (int j = 0; j < 2 * n; j++) {
                    auxiliar[k][j] -=
                        fator * auxiliar[i][j];

                    if (!isfinite(auxiliar[k][j])) {
                        printf("ERRO: falha numerica na inversao.\n");
                        return 0;
                    }
                }
            }
        }
    }

    // Copia a inversa e desfaz a escala
    inversa->linhas = n;
    inversa->colunas = n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inversa->dados[i][j] =
                auxiliar[i][j + n] / escala;

            if (!isfinite(inversa->dados[i][j])) {
                printf("ERRO: inversa fora do limite numerico.\n");
                return 0;
            }
        }
    }

    return 1;
}

// DIVISAO MATRICIAL: A / B = A * B^-1
int dividir(Matriz a, Matriz b, Matriz *r) {
    Matriz inversa;

    if (b.linhas != b.colunas) {
        printf("ERRO: matriz divisora precisa ser quadrada.\n");
        return 0;
    }

    if (a.colunas != b.linhas) {
        printf("ERRO: dimensoes incompativeis para divisao.\n");
        return 0;
    }

    printf("Calculando a inversa da matriz divisora...\n");

    if (!inverter(b, &inversa)) {
        printf("Divisao cancelada: matriz sem inversa valida.\n");
        return 0;
    }

    printf("Inversa calculada com sucesso!\n");

    return multiplicar(a, inversa, r);
}

// EXECUCAO DAS OPERACOES
void executarOperacao(Matriz matrizes[],
                      int quantidade, int opcao) {

    Matriz resultado = matrizes[0];
    Matriz temporaria;

    printf("\n====================================\n");
    printf("       EXECUTANDO OPERACAO\n");
    printf("====================================\n");

    for (int i = 1; i < quantidade; i++) {
        int sucesso = 0;

        printf("\nEtapa %d: matriz %d com matriz %d\n",
               i, i, i + 1);

        switch (opcao) {
            case 1:
                printf("Realizando adicao...\n");
                sucesso = somar(resultado, matrizes[i],
                                &temporaria);
                break;

            case 2:
                printf("Realizando subtracao...\n");
                sucesso = subtrair(resultado, matrizes[i],
                                   &temporaria);
                break;

            case 3:
                printf("Realizando multiplicacao...\n");
                sucesso = multiplicar(resultado, matrizes[i],
                                      &temporaria);
                break;

            case 4:
                printf("Realizando divisao matricial...\n");
                sucesso = dividir(resultado, matrizes[i],
                                  &temporaria);
                break;

            default:
                printf("Opcao invalida!\n");
                return;
        }

        if (!sucesso) {
            printf("\nOPERACAO INTERROMPIDA NA ETAPA %d.\n", i);
            printf("Verifique as dimensoes e os valores das matrizes.\n");
            return;
        }

        resultado = temporaria;

        printf("Etapa concluida com sucesso!\n");
    }

    printf("\n====================================\n");
    printf("          RESULTADO FINAL\n");
    printf("====================================\n");

    exibirMatriz(resultado);
}

// MENU
void menu(void) {
    printf("\n====================================\n");
    printf("       CALCULADORA DE MATRIZES\n");
    printf("====================================\n");
    printf("1 - Adicao\n");
    printf("2 - Subtracao\n");
    printf("3 - Multiplicacao\n");
    printf("4 - Divisao matricial\n");
    printf("5 - Encerrar programa\n");
    printf("====================================\n");
}

// FUNCAO PRINCIPAL
int main(void) {
    Matriz matrizes[MAX_MATRIZES];

    int quantidade;
    int opcao;

    printf("\n====================================\n");
    printf("    SISTEMA CALCULADORA DE MATRIZES\n");
    printf("====================================\n");

    quantidade = lerInteiro(
        "\nQuantas matrizes deseja cadastrar (2 a 10)? ",
        2, MAX_MATRIZES
    );

    // Cadastro de todas as matrizes
    for (int i = 0; i < quantidade; i++) {
        lerMatriz(&matrizes[i], i + 1);
    }

    printf("\nTodas as matrizes foram cadastradas!\n");

    // Menu interativo
    do {
        menu();

        opcao = lerInteiro("Escolha uma operacao: ", 1, 5);

        switch (opcao) {
            case 1:
            case 2:
            case 3:
            case 4:
                executarOperacao(matrizes, quantidade, opcao);
                break;

            case 5:
                printf("\nPrograma encerrado com sucesso!\n");
                break;
        }

    } while (opcao != 5);

    return 0;
}
