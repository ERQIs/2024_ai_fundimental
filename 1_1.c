#include <stdio.h>
#include <stdlib.h>
#define MAX 1000005
#define MOD 100003
#define QSIZE 1000003
typedef struct node {
    int idx;
    struct node* next;
}* NODE;

int q[QSIZE], out, in, m, n, reach[MAX], solved[MAX];

struct node G[MAX];

void q_in(int n) {
    in = (in + 1) % QSIZE;
    q[in] = n;
}

int q_out() {
    int ret = q[out];
    out = (out + 1) % QSIZE;
    return ret;
}

NODE NODE_new (int idx) {
    NODE ret = (NODE) malloc (sizeof(struct node));
    ret->idx = idx;
    ret->next = NULL;
    return ret;
}

void list_insert(int from, int to) {
        int doubled = 0;
        NODE end = &G[from];
        while (end->next != NULL) {
            end = end->next;
        }
        if (!doubled)
            end->next = NODE_new(to);
}

void read() {
    int i, a, b;
    scanf("%d%d", &n, &m);
    for (i = 1; i <= m; i ++) {
        scanf("%d%d", &a, &b);
        list_insert(a, b);
    }
}

int main()
{
    read();

    int pre_layer = 1, next_layer = 0, i;
    q[in ++] = 1;
    reach[1] = 0;

    for (i = 2; i <= n; i ++){
        reach[i] = MAX;
    }

    while (in != out) {
        int node, dst;
        NODE iter_p;
        while (pre_layer --) {
            node = q_out();
            if (solved[node] == 1) {
                continue;
            }
            for(iter_p = G[node].next; iter_p != NULL; iter_p = iter_p->next) {
                dst = iter_p->idx;
                if (reach[dst] >= reach[node] + 1) {
                    reach[dst] = reach[node] + 1;
                    q_in(dst);
                    next_layer ++;
                }
            }
            solved[node] = 1;
        }
        pre_layer = next_layer; 
        next_layer = 0;
    }
    if(reach[n] == MAX) printf("-1\n");
    else printf("%d\n", reach[n]);
    return 0;
}
