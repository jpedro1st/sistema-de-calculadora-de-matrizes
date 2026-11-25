
# Calculadora de Matrizes em C

## Objetivo

Desenvolver uma aplicação em linguagem C capaz de cadastrar
e realizar operações matemáticas entre duas ou mais matrizes.

O sistema foi desenvolvido como atividade acadêmica de
programação, utilizando funções, estruturas de repetição,
decisão e armazenamento de dados.

## Funcionalidades

- Cadastro de 2 a 10 matrizes.
- Definição da quantidade de linhas e colunas.
- Leitura individual dos elementos.
- Adição de matrizes.
- Subtração de matrizes.
- Multiplicação de matrizes.
- Divisão matricial utilizando matriz inversa.
- Identificação de matrizes singulares.
- Validação de dimensões.
- Menu interativo.
- Exibição organizada dos resultados.

## Tecnologias utilizadas

- Linguagem C.
- Biblioteca stdio.h.
- Biblioteca stdlib.h.
- Biblioteca math.h.

## Execução

No Linux:

```bash
./calculadora
```

No Windows:

```bash
calculadora.exe
```

## Funções implementadas

| Função | Responsabilidade |
|---|---|
| limparBuffer() | Limpar caracteres da entrada |
| lerInteiro() | Ler e validar números inteiros |
| lerMatriz() | Cadastrar os elementos |
| exibirMatriz() | Exibir os resultados |
| somar() | Realizar adição |
| subtrair() | Realizar subtração |
| multiplicar() | Realizar multiplicação |
| inverter() | Calcular a matriz inversa |
| dividir() | Realizar divisão matricial |
| executarOperacao() | Coordenar operações sequenciais |
| menu() | Apresentar opções do sistema |
| main() | Coordenar a execução principal |

## Regra da divisão matricial

A divisão é definida por:

A / B = A * B⁻¹

A matriz divisora precisa ser quadrada e possuir inversa.

O cálculo da inversa utiliza o método de Gauss-Jordan
com pivotamento parcial.

## Exemplo de utilização

Cadastro da primeira matriz:

1 2
3 4

Cadastro da segunda matriz:

5 6
7 8

Escolha a opção 1 (Adição).

Resultado:

6  8
10 12

## Limitações

- O sistema permite matrizes de até 10 linhas e 10 colunas.
- O cadastro permite até 10 matrizes.
- A inversão utiliza uma tolerância numérica para identificar
  matrizes singulares.
- Os cálculos utilizam o tipo double.

## Autor

Projeto acadêmico desenvolvido em linguagem C.
