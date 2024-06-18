/**********************************
* Nome dos(as) estudantes: [Seu Nome]
* Trabalho 2
* Professor: Nome do professor: [Professor Higa]
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} BigInt;

// Função para criar um novo nó
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Função para inicializar um BigInt
void initBigInt(BigInt* num) {
    num->head = NULL;
    num->tail = NULL;
}

// Função para adicionar um dígito ao BigInt
void appendBigInt(BigInt* num, int digit) {
    Node* newNode = createNode(digit);
    if (num->head == NULL) {
        num->head = newNode;
        num->tail = newNode;
    } else {
        num->tail->next = newNode;
        num->tail = newNode;
    }
}

// Função para criar um BigInt a partir de uma string
BigInt createBigInt(char* str) {
    BigInt num;
    initBigInt(&num);
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        appendBigInt(&num, str[i] - '0');
    }
    return num;
}

// Função para imprimir um BigInt
void printBigInt(BigInt num) {
    Node* current = num.head;
    char* str = (char*)malloc(1000 * sizeof(char));
    int index = 0;
    while (current != NULL) {
        str[index++] = '0' + current->data;
        current = current->next;
    }
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", str[i]);
    }
    printf("\n");
    free(str);
}

// Função para adicionar dois BigInts
BigInt addBigInt(BigInt a, BigInt b) {
    BigInt result;
    initBigInt(&result);
    Node* p1 = a.head;
    Node* p2 = b.head;
    int carry = 0;
    while (p1 != NULL || p2 != NULL || carry) {
        int sum = carry;
        if (p1 != NULL) {
            sum += p1->data;
            p1 = p1->next;
        }
        if (p2 != NULL) {
            sum += p2->data;
            p2 = p2->next;
        }
        appendBigInt(&result, sum % 10);
        carry = sum / 10;
    }
    return result;
}

// Função para subtrair dois BigInts
BigInt subtractBigInt(BigInt a, BigInt b) {
    BigInt result;
    initBigInt(&result);
    Node* p1 = a.head;
    Node* p2 = b.head;
    int borrow = 0;
    while (p1 != NULL) {
        int sub = p1->data - (p2 ? p2->data : 0) - borrow;
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        appendBigInt(&result, sub);
        p1 = p1->next;
        if (p2) p2 = p2->next;
    }
    return result;
}

// Função para multiplicar dois BigInts
BigInt multiplyBigInt(BigInt a, BigInt b) {
    BigInt result;
    initBigInt(&result);
    Node* p1 = a.head;
    Node* p2;
    int shift = 0;

    while (p1 != NULL) {
        BigInt temp;
        initBigInt(&temp);
        for (int i = 0; i < shift; i++) {
            appendBigInt(&temp, 0);
        }
        int carry = 0;
        p2 = b.head;
        while (p2 != NULL || carry) {
            int mul = carry + (p2 ? p2->data : 0) * p1->data;
            appendBigInt(&temp, mul % 10);
            carry = mul / 10;
            if (p2) p2 = p2->next;
        }
        result = addBigInt(result, temp);
        p1 = p1->next;
        shift++;
    }
    return result;
}

// Função para comparar dois BigInts
int compareBigInt(BigInt a, BigInt b) {
    Node* p1 = a.head;
    Node* p2 = b.head;
    int len1 = 0, len2 = 0;
    
    // Calcular o tamanho dos dois BigInts
    while (p1 != NULL) {
        len1++;
        p1 = p1->next;
    }
    while (p2 != NULL) {
        len2++;
        p2 = p2->next;
    }
    
    // Comparar os tamanhos
    if (len1 > len2) return 1;
    if (len2 > len1) return -1;
    
    // Se os tamanhos são iguais, comparar os dígitos
    p1 = a.head;
    p2 = b.head;
    while (p1 != NULL) {
        if (p1->data > p2->data) return 1;
        if (p1->data < p2->data) return -1;
        p1 = p1->next;
        p2 = p2->next;
    }
    return 0;
}

// Função para dividir dois BigInts
BigInt divideBigInt(BigInt a, BigInt b) {
    BigInt result;
    initBigInt(&result);
    BigInt current;
    initBigInt(&current);
    Node* p = a.head;
    while (p != NULL) {
        appendBigInt(&current, p->data);
        p = p->next;
        int count = 0;
        while (compareBigInt(current, b) >= 0) {
            current = subtractBigInt(current, b);
            count++;
        }
        appendBigInt(&result, count);
    }
    return result;
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
        
        BigInt a = createBigInt(num1);
        BigInt b = createBigInt(num2);
        BigInt result;
        switch (op) {
            case '+':
                result = addBigInt(a, b);
                break;
            case '-':
                result = subtractBigInt(a, b);
                break;
            case '*':
                result = multiplyBigInt(a, b);
                break;
            case '/':
                result = divideBigInt(a, b);
                break;
        }
        printBigInt(result);
    }

    return 0;
}
