#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    int dado;
    struct No* proximo;
} No;

typedef struct BigInt{
    No* cabeca;
    No* cauda;
} BigInt;

// Função para criar um novo nó
No* criarNo(int dado) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->dado = dado;
    novoNo->proximo = NULL;
    return novoNo;
}

// Função para inicializar um BigInt
void inicializarBigInt(BigInt* num) {
    num->cabeca = NULL;
    num->cauda = NULL;
}

// Função para adicionar um dígito ao BigInt
void adicionarDigito(BigInt* num, int digito) {
    No* novoNo = criarNo(digito);
    if (num->cabeca == NULL) {
        num->cabeca = novoNo;
        num->cauda = novoNo;
    } else {
        num->cauda->proximo = novoNo;
        num->cauda = novoNo;
    }
}

// Função para criar um BigInt a partir de uma string
BigInt criarBigInt(char* str) {
    BigInt num;
    inicializarBigInt(&num);
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        adicionarDigito(&num, str[i] - '0');
    }
    return num;
}

// Função para imprimir um BigInt
void imprimirBigInt(BigInt num) {
    No* atual = num.cabeca;
    char* str = (char*)malloc(1000 * sizeof(char));
    int indice = 0;
    while (atual != NULL) {
        str[indice++] = '0' + atual->dado;
        atual = atual->proximo;
    }
    for (int i = indice - 1; i >= 0; i--) {
        printf("%c", str[i]);
    }
    printf("\n");
    free(str);
}

// Função para adicionar dois BigInts
BigInt adicionarBigInt(BigInt a, BigInt b) {
    BigInt resultado;
    inicializarBigInt(&resultado);
    No* p1 = a.cabeca;
    No* p2 = b.cabeca;
    int carry = 0;
    while (p1 != NULL || p2 != NULL || carry) {
        int soma = carry;
        if (p1 != NULL) {
            soma += p1->dado;
            p1 = p1->proximo;
        }
        if (p2 != NULL) {
            soma += p2->dado;
            p2 = p2->proximo;
        }
        adicionarDigito(&resultado, soma % 10);
        carry = soma / 10;
    }
    return resultado;
}

// Função para subtrair dois BigInts
BigInt subtrairBigInt(BigInt a, BigInt b) {
    BigInt resultado;
    inicializarBigInt(&resultado);
    No* p1 = a.cabeca;
    No* p2 = b.cabeca;
    int emprestimo = 0;
    while (p1 != NULL) {
        int sub = p1->dado - (p2 ? p2->dado : 0) - emprestimo;
        if (sub < 0) {
            sub += 10;
            emprestimo = 1;
        } else {
            emprestimo = 0;
        }
        adicionarDigito(&resultado, sub);
        p1 = p1->proximo;
        if (p2) p2 = p2->proximo;
    }
    // Remover zeros à esquerda no resultado
    while (resultado.cauda != NULL && resultado.cauda->dado == 0) {
        No* temp = resultado.cauda;
        if (resultado.cauda == resultado.cabeca) {
            resultado.cabeca = resultado.cauda = NULL;
        } else {
            No* atual = resultado.cabeca;
            while (atual->proximo != resultado.cauda) {
                atual = atual->proximo;
            }
            atual->proximo = NULL;
            resultado.cauda = atual;
        }
        free(temp);
    }
    return resultado;
}

// Função para comparar dois BigInts
int compararBigInt(BigInt a, BigInt b) {
    // Conta o número de dígitos de cada BigInt
    int len1 = 0, len2 = 0;
    No* p1 = a.cabeca;
    No* p2 = b.cabeca;
    while (p1) {
        len1++;
        p1 = p1->proximo;
    }
    while (p2) {
        len2++;
        p2 = p2->proximo;
    }

    // Se um número tem mais dígitos, ele é maior
    if (len1 > len2) return 1;
    if (len1 < len2) return -1;

    // Se o número de dígitos é igual, compara os dígitos do mais significativo ao menos significativo
    p1 = a.cabeca;
    p2 = b.cabeca;
    int* digitos1 = (int*)malloc(len1 * sizeof(int));
    int* digitos2 = (int*)malloc(len2 * sizeof(int));
    for (int i = 0; i < len1; i++) {
        digitos1[i] = p1->dado;
        p1 = p1->proximo;
    }
    for (int i = 0; i < len2; i++) {
        digitos2[i] = p2->dado;
        p2 = p2->proximo;
    }
    for (int i = len1 - 1; i >= 0; i--) {
        if (digitos1[i] > digitos2[i]) {
            free(digitos1);
            free(digitos2);
            return 1;
        }
        if (digitos1[i] < digitos2[i]) {
            free(digitos1);
            free(digitos2);
            return -1;
        }
    }

    free(digitos1);
    free(digitos2);
    return 0;
}
// Função para multiplicar dois BigInts
BigInt multiplicarBigInt(BigInt a, BigInt b) {
    BigInt resultado;
    inicializarBigInt(&resultado);
    No* p1 = a.cabeca;
    No* p2;
    int deslocamento = 0;

    while (p1 != NULL) {
        BigInt temp;
        inicializarBigInt(&temp);
        for (int i = 0; i < deslocamento; i++) {
            adicionarDigito(&temp, 0);
        }
        int carry = 0;
        p2 = b.cabeca;
        while (p2 != NULL || carry) {
            int mul = carry + (p2 ? p2->dado : 0) * p1->dado;
            adicionarDigito(&temp, mul % 10);
            carry = mul / 10;
            if (p2) p2 = p2->proximo;
        }
        resultado = adicionarBigInt(resultado, temp);
        p1 = p1->proximo;
        deslocamento++;
    }
    return resultado;
}


// Função para copiar um BigInt
BigInt copiarBigInt(BigInt num) {
    BigInt resultado;
    inicializarBigInt(&resultado);
    No* atual = num.cabeca;
    while (atual != NULL) {
        adicionarDigito(&resultado, atual->dado);
        atual = atual->proximo;
    }
    return resultado;
}

void dividirBigInt(BigInt a, BigInt b, BigInt* quociente, BigInt* resto) {
    inicializarBigInt(quociente);
    inicializarBigInt(resto);

    // Copiar o valor inicial do dividendo (a) para o resto
    No* atual = a.cabeca;
    while (atual != NULL) {
        adicionarDigito(resto, atual->dado);
        atual = atual->proximo;
    }

    // Inicializar um BigInt temporário para armazenar o divisor original (b)
    BigInt tempB;
    inicializarBigInt(&tempB);
    atual = b.cabeca;
    while (atual != NULL) {
        adicionarDigito(&tempB, atual->dado);
        atual = atual->proximo;
    }

    // Loop principal para a divisão
    while (compararBigInt(*resto, tempB) >= 0) {
        int count = 0;
        while (compararBigInt(*resto, tempB) >= 0) {
            *resto = subtrairBigInt(*resto, tempB);
            count++;
        }
        adicionarDigito(quociente, count);
    }

    // Limpar zeros à esquerda no quociente, se houver
    while (quociente->cauda != NULL && quociente->cauda->dado == 0) {
        No* temp = quociente->cauda;
        if (quociente->cauda == quociente->cabeca) {
            quociente->cabeca = quociente->cauda = NULL;
        } else {
            No* atual = quociente->cabeca;
            while (atual->proximo != quociente->cauda) {
                atual = atual->proximo;
            }
            atual->proximo = NULL;
            quociente->cauda = atual;
        }
        free(temp);
    }

    // Liberar memória utilizada pelo BigInt temporário para o divisor (tempB)
    No* tempNode = tempB.cabeca;
    while (tempNode != NULL) {
        No* temp = tempNode;
        tempNode = tempNode->proximo;
        free(temp);
    }
}

int main() {
    int N;
    scanf("%d", &N);
    getchar(); // Para consumir a linha em branco

    for (int i = 0; i < N; i++) {
        char num1[1001], num2[1001], op;
        scanf("%s", num1);
        getchar(); // Para consumir a linha em branco
        scanf("%s", num2);
        getchar(); // Para consumir a linha em branco
        scanf(" %c", &op);

        BigInt a = criarBigInt(num1);
        BigInt b = criarBigInt(num2);
        BigInt resultado, resto;
        switch (op) {
            case '+':
                resultado = adicionarBigInt(a, b);
                imprimirBigInt(resultado);
                break;
            case '-':
                resultado = subtrairBigInt(a, b);
                imprimirBigInt(resultado);
                break;
            case '*':
                resultado = multiplicarBigInt(a, b);
                imprimirBigInt(resultado);
                break;
            case '/':
                if (compararBigInt(b, criarBigInt("0")) == 0) {
                    printf("Erro: Divisão por zero.\n");
                } else {
                    dividirBigInt(a, b, &resultado, &resto);
                    imprimirBigInt(resultado);
                }
                break;
        }
    }

    return 0;
}
