#include <iostream>
#include <queue>
#define N_MAX 10004
#define M_MAX 500005
#define WEIGHT 0
#define TO 1
#define NEXT 2
#define INFI ((1 << 30) - 1)

unsigned m, n, s;
unsigned list_heads[N_MAX];          //用链表存储边，对应各点的链表头
unsigned edges[M_MAX][3];            //[0]: 边的权重 [1]: 指向的节点 [2]:链表中的下一条边

unsigned dis[N_MAX]/*达到节点的距离*/, vis[N_MAX] /*已达集合*/;

void read()
{
    unsigned i, from, to, weight;
    std::cin >> n >> m;
    for (i = 1; i <= m; i ++) {
        std::cin >> from >> to >> weight;
        edges[i][WEIGHT] = weight;
        edges[i][TO] = to;
        edges[i][NEXT] = list_heads[from];
        list_heads[from] = i;
    }
}

unsigned find_min_node() {
    unsigned i, min = INFI, min_idx = 0;
    for (i = 1; i <= n; i ++ ) {
        if (vis[i] == 0 && dis[i] < min) {
            min_idx = i;
            min = dis[i];
        }
    }
    return min_idx;
}

int main()
{
    unsigned i, reachable_nodes;
    read();
    for (i = 1; i <= n; i ++) {
        dis[i] = INFI;
    }


    dis [1] = 0; 
    reachable_nodes = 1;        //记录可达但是没有加入到已达集合中的点的个数

    while (reachable_nodes != 0) {
        unsigned min_idx = find_min_node();     // 剔除并记录最近的可达点（记位已达）
        reachable_nodes --;
        vis[min_idx] = 1;
        unsigned edge_p;
        for (edge_p = list_heads[min_idx]; edge_p != 0; edge_p = edges[edge_p][NEXT]) {
            // 更新子节点
            unsigned to = edges[edge_p][TO];
            unsigned weight = edges[edge_p][WEIGHT];
            if (dis[min_idx] + weight < dis[to]) {
                reachable_nodes ++;
                dis[to] = dis[min_idx] + weight;
            }
        }
    }
    
    if (dis[n] == INFI) std::cout << "-1\n";
    else std::cout << dis[n] << "\n";

    return 0;
}
