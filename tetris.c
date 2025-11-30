
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "peca.h"

#define FILA_CAP 5
#define PILHA_CAP 3

typedef struct {
    Peca data[FILA_CAP];
    int head;
    int tail;
    int count;
    int next_id;
} Fila;

typedef struct {
    Peca data[PILHA_CAP];
    int top;
    int count;
} Pilha;

void iniciar_fila(Fila *f) {
    f->head = 0;
    f->tail = 0;
    f->count = 0;
    f->next_id = 1;

    while (f->count < FILA_CAP) {
        char tipos[] = {'I','O','T','L'};
        Peca p;
        p.tipo = tipos[rand() % 4];
        p.id = f->next_id++;
        f->data[f->tail] = p;
        f->tail = (f->tail + 1) % FILA_CAP;
        f->count++;
    }
}

int fila_vazia(Fila *f) { return f->count == 0; }
int fila_cheia(Fila *f) { return f->count == FILA_CAP; }

void enqueue(Fila *f, Peca p) {
    if (fila_cheia(f)) return;
    f->data[f->tail] = p;
    f->tail = (f->tail + 1) % FILA_CAP;
    f->count++;
}

Peca dequeue(Fila *f) {
    Peca empty = {'?', -1};
    if (fila_vazia(f)) return empty;
    Peca p = f->data[f->head];
    f->head = (f->head + 1) % FILA_CAP;
    f->count--;
    return p;
}

Peca gerar_peca(Fila *f) {
    char tipos[] = {'I','O','T','L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = f->next_id++;
    return p;
}

void iniciar_pilha(Pilha *s) {
    s->top = -1;
    s->count = 0;
}

int pilha_vazia(Pilha *s) { return s->top == -1; }
int pilha_cheia(Pilha *s) { return s->top == PILHA_CAP - 1; }

int push(Pilha *s, Peca p) {
    if (pilha_cheia(s)) return 0;
    s->data[++s->top] = p;
    s->count++;
    return 1;
}

Peca pop(Pilha *s) {
    Peca empty = {'?', -1};
    if (pilha_vazia(s)) return empty;
    Peca p = s->data[s->top--];
    s->count--;
    return p;
}

void mostrar_fila(Fila *f) {
    printf("\n[FILA] count=%d head=%d tail=%d\n", f->count, f->head, f->tail);
    int idx = f->head;
    for (int i = 0; i < f->count; i++) {
        Peca p = f->data[idx];
        printf(" pos %d -> Tipo:%c ID:%d\n", i + 1, p.tipo, p.id);
        idx = (idx + 1) % FILA_CAP;
    }
}

void mostrar_pilha(Pilha *s) {
    printf("\n[PILHA] count=%d\n", s->count);
    for (int i = s->top; i >= 0; i--) {
        Peca p = s->data[i];
        printf(" top-%d -> Tipo:%c ID:%d\n", i, p.tipo, p.id);
    }
}

int trocar_frente_topo(Fila *f, Pilha *s) {
    if (fila_vazia(f) || pilha_vazia(s)) return 0;
    int front_idx = f->head;
    Peca tmp = f->data[front_idx];
    f->data[front_idx] = s->data[s->top];
    s->data[s->top] = tmp;
    return 1;
}

int trocar_3(Fila *f, Pilha *s) {
    if (s->count != 3 || f->count < 3) return 0;

    int idxs[3];
    int idx = f->head;
    for (int i = 0; i < 3; i++) {
        idxs[i] = idx;
        idx = (idx + 1) % FILA_CAP;
    }

    for (int i = 0; i < 3; i++) {
        Peca tmp = f->data[idxs[i]];
        f->data[idxs[i]] = s->data[i];
        s->data[i] = tmp;
    }

    return 1;
}

int main(void) {
    srand((unsigned int) time(NULL));

    Fila fila;
    Pilha pilha;

    iniciar_fila(&fila);
    iniciar_pilha(&pilha);

    char buf[32];

    while (1) {
        mostrar_fila(&fila);
        mostrar_pilha(&pilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\nEscolha: ");

        if (!fgets(buf, sizeof(buf), stdin)) break;
        int op = atoi(buf);

        if (op == 0) break;

        if (op == 1) {
            Peca p = dequeue(&fila);
            if (p.id == -1) { printf("Fila vazia.\n"); continue; }
            printf("Jogou peca: Tipo %c ID %d\n", p.tipo, p.id);
            enqueue(&fila, gerar_peca(&fila));
        }
        else if (op == 2) {
            if (pilha_cheia(&pilha)) { printf("Pilha cheia.\n"); continue; }
            Peca p = dequeue(&fila);
            if (p.id == -1) { printf("Fila vazia.\n"); continue; }
            push(&pilha, p);
            enqueue(&fila, gerar_peca(&fila));
            printf("Reservou peca: Tipo %c ID %d\n", p.tipo, p.id);
        }
        else if (op == 3) {
            Peca p = pop(&pilha);
            if (p.id == -1) { printf("Pilha vazia.\n"); continue; }
            printf("Usou peca: Tipo %c ID %d\n", p.tipo, p.id);
        }
        else if (op == 4) {
            if (trocar_frente_topo(&fila, &pilha)) printf("Troca realizada.\n");
            else printf("Nao foi possivel trocar.\n");
        }
        else if (op == 5) {
            if (trocar_3(&fila, &pilha)) printf("Troca realizada.\n");
            else printf("Nao foi possivel trocar.\n");
        }
        else {
            printf("Opcao invalida.\n");
        }
    }

    printf("Encerrando.\n");
    return 0;
}
