/**********************************
* Nome dos(as) estudantes: Gustavo Henrique Florentin Pires Pereira
* Trabalho 2
* Professor: Marco Aurelio
*********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
  int dado;
  struct No *proximo;
} No;

typedef struct NumGrande {
  No *cabeca;
  No *cauda;
} NumGrande;

// Função para criar um novo nó
No *criarNo(int dado) {
  No *novoNo = (No *)malloc(sizeof(No));
  novoNo->dado = dado;
  novoNo->proximo = NULL;
  return novoNo;
}

// Função para inicializar um NumGrande
void inicializarNumGrande(NumGrande *num) {
  num->cabeca = NULL;
  num->cauda = NULL;
}

// Função para adicionar um dígito ao NumGrande
void adicionarDigito(NumGrande *num, int digito) {
  No *novoNo = criarNo(digito);
  if (num->cabeca == NULL) {
    num->cabeca = novoNo;
    num->cauda = novoNo;
  } else {
    num->cauda->proximo = novoNo;
    num->cauda = novoNo;
  }
}

// Função para criar um NumGrande a partir de uma string
NumGrande criarNumGrande(const char *str) {
  NumGrande num;
  inicializarNumGrande(&num);
  int len = strlen(str);
  for (int i = len - 1; i >= 0; i--) {
    adicionarDigito(&num, str[i] - '0');
  }
  return num;
}

// Função para imprimir um NumGrande
void imprimirNumGrande(NumGrande num) {
  No *atual = num.cabeca;
  char *str = (char *)malloc(1000 * sizeof(char));
  int indice = 0;
  while (atual != NULL) {
    str[indice++] = '0' + atual->dado;
    atual = atual->proximo;
  }
  printf("O resultado da operacao e igual a:\n");
  for (int i = indice - 1; i >= 0; i--) {
    printf("%c", str[i]);
  }
  printf("\n\n");
  free(str);
}

// Função para adicionar dois NumGrande
NumGrande adicionarNumGrande(NumGrande a, NumGrande b) {
  NumGrande resultado;
  inicializarNumGrande(&resultado);
  No *p1 = a.cabeca;
  No *p2 = b.cabeca;
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

// Função para subtrair dois NumGrande
NumGrande subtrairNumGrande(NumGrande a, NumGrande b) {
  NumGrande resultado;
  inicializarNumGrande(&resultado);
  No *p1 = a.cabeca;
  No *p2 = b.cabeca;
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
    if (p2)
      p2 = p2->proximo;
  }
  // Remover zeros à esquerda no resultado
  while (resultado.cauda != NULL && resultado.cauda->dado == 0) {
    No *temp = resultado.cauda;
    if (resultado.cauda == resultado.cabeca) {
      resultado.cabeca = resultado.cauda = NULL;
    } else {
      No *atual = resultado.cabeca;
      while (atual->proximo != resultado.cauda) {
        atual = atual->proximo;
      }
      atual->proximo = NULL;
      resultado.cauda = atual;
    }
    free(temp);
  }
  // Se o resultado é zero, adicionar um dígito 0
  if (resultado.cabeca == NULL) {
    adicionarDigito(&resultado, 0);
  }

  return resultado;
}

// Função para comparar dois NumGrande
int compararNumGrande(NumGrande a, NumGrande b) {
  // Conta o número de dígitos de cada NumGrande
  int len1 = 0, len2 = 0;
  No *p1 = a.cabeca;
  No *p2 = b.cabeca;
  while (p1) {
    len1++;
    p1 = p1->proximo;
  }
  while (p2) {
    len2++;
    p2 = p2->proximo;
  }

  // Se um número tem mais dígitos, ele é maior
  if (len1 > len2)
    return 1;
  if (len1 < len2)
    return -1;

  // Se o número de dígitos é igual, compara os dígitos do mais significativo ao
  // menos significativo
  p1 = a.cabeca;
  p2 = b.cabeca;
  int *digitos1 = (int *)malloc(len1 * sizeof(int));
  int *digitos2 = (int *)malloc(len2 * sizeof(int));
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
// Função para multiplicar dois NumGrande
NumGrande multiplicarNumGrande(NumGrande a, NumGrande b) {
  NumGrande resultado;
  inicializarNumGrande(&resultado);
  No *p1 = a.cabeca;
  No *p2;
  int deslocamento = 0;

  while (p1 != NULL) {
    NumGrande temp;
    inicializarNumGrande(&temp);
    for (int i = 0; i < deslocamento; i++) {
      adicionarDigito(&temp, 0);
    }
    int carry = 0;
    p2 = b.cabeca;
    while (p2 != NULL || carry) {
      int mul = carry + (p2 ? p2->dado : 0) * p1->dado;
      adicionarDigito(&temp, mul % 10);
      carry = mul / 10;
      if (p2)
        p2 = p2->proximo;
    }
    resultado = adicionarNumGrande(resultado, temp);
    p1 = p1->proximo;
    deslocamento++;
  }
  return resultado;
}

// Função para copiar um BigInt
NumGrande copiarNumGrande(NumGrande num) {
  NumGrande resultado;
  inicializarNumGrande(&resultado);
  No *atual = num.cabeca;
  while (atual != NULL) {
    adicionarDigito(&resultado, atual->dado);
    atual = atual->proximo;
  }
  return resultado;
}

void dividirNumGrande(NumGrande a, NumGrande b, NumGrande *quociente, NumGrande *resto) {
  inicializarNumGrande(quociente);
  inicializarNumGrande(resto);

  // Copiar o valor inicial do dividendo (a) para o resto
  No *atual = a.cabeca;
  while (atual != NULL) {
    adicionarDigito(resto, atual->dado);
    atual = atual->proximo;
  }

  // Inicializar um NumGrande temporário para armazenar o divisor original (b)
  NumGrande tempB;
  inicializarNumGrande(&tempB);
  atual = b.cabeca;
  while (atual != NULL) {
    adicionarDigito(&tempB, atual->dado);
    atual = atual->proximo;
  }

  // Verificar se o divisor (b) é zero
  if (compararNumGrande(tempB, criarNumGrande("0")) == 0) {
    printf("Erro: Divisão por zero.\n");
    return;
  }

  // Inicializar NumGrande temporários para o quociente e para o valor
  // multiplicador
  NumGrande tempQuociente;
  inicializarNumGrande(&tempQuociente);
  NumGrande multiplicador;
  inicializarNumGrande(&multiplicador);
  adicionarDigito(&multiplicador, 1);

  // Loop principal para a divisão
  while (compararNumGrande(*resto, tempB) >= 0) {
    NumGrande tempMultiplicador = copiarNumGrande(multiplicador);
    NumGrande tempResultado = multiplicarNumGrande(tempB, tempMultiplicador);

    while (compararNumGrande(*resto, tempResultado) >= 0) {
      *resto = subtrairNumGrande(*resto, tempResultado);
      *quociente = adicionarNumGrande(*quociente, tempMultiplicador);
    }

    // Ajustar o multiplicador para a próxima iteração
    multiplicador = multiplicarNumGrande(multiplicador, criarNumGrande("10"));
  }

  // Limpar zeros à esquerda no quociente, se houver
  while (quociente->cauda != NULL && quociente->cauda->dado == 0) {
    No *temp = quociente->cauda;
    if (quociente->cauda == quociente->cabeca) {
      quociente->cabeca = quociente->cauda = NULL;
    } else {
      No *atual = quociente->cabeca;
      while (atual->proximo != quociente->cauda) {
        atual = atual->proximo;
      }
      atual->proximo = NULL;
      quociente->cauda = atual;
    }
    free(temp);
  }

  // Liberar memória utilizada pelos BigInts temporários
  No *tempNode = tempB.cabeca;
  while (tempNode != NULL) {
    No *temp = tempNode;
    tempNode = tempNode->proximo;
    free(temp);
  }

  tempNode = tempQuociente.cabeca;
  while (tempNode != NULL) {
    No *temp = tempNode;
    tempNode = tempNode->proximo;
    free(temp);
  }

  tempNode = multiplicador.cabeca;
  while (tempNode != NULL) {
    No *temp = tempNode;
    tempNode = tempNode->proximo;
    free(temp);
  }
}

int main() {
  int N;
  printf("Digite a quantidade de Operacoes:\n");
  if (scanf("%d", &N) != 1) {
    printf("Erro ao ler N.\n");
    return 1;
  }
  getchar(); // Para consumir a linha em branco
  printf("Inicio das operacoes a serem realizadas");
  printf("\n\n");
  for (int i = 0; i < N; i++) {
    char num1[1001], num2[1001], op;
    printf("Informe o primeiro numero da Operacao:\n");
    if (scanf("%s", num1) != 1) {
      printf("Erro ao ler num1.\n");
      return 1;
    }
    printf("---------------------------------------\n");
    getchar(); // Para consumir a linha em branco
    printf("Informe o segundo numero da Operacao:\n");
    if (scanf("%s", num2) != 1) {
      printf("Erro ao ler num2.\n");
      return 1;
    }
    printf("-------------------------------------\n");
    getchar(); // Para consumir a linha em branco
    printf("Informe a operacao a ser realizada:\n");
    if (scanf(" %c", &op) != 1) {
      printf("Erro ao ler operador.\n");
      return 1;
    }

    NumGrande a = criarNumGrande(num1);
    NumGrande b = criarNumGrande(num2);
    NumGrande resultado, resto;
    switch (op) {
    case '+':
      resultado = adicionarNumGrande(a, b);
      imprimirNumGrande(resultado);
      break;
    case '-':
      resultado = subtrairNumGrande(a, b);
      imprimirNumGrande(resultado);
      break;
    case '*':
      resultado = multiplicarNumGrande(a, b);
      imprimirNumGrande(resultado);
      break;
    case '/':
      if (compararNumGrande(b, criarNumGrande("0")) == 0) {
        printf("Erro: Divisão por zero.\n");
      } else {
        dividirNumGrande(a, b, &resultado, &resto);
        imprimirNumGrande(resultado);
      }
      break;
    default:
      printf("Operador inválido.\n");
      break;
    }
  }

  return 0;
}
