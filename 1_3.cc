#include <iostream>
#include <queue>
#define N_MAX 100004
#define M_MAX 200005
#define WEIGHT 0
#define TO 1
#define NEXT 2
#define INFI ((1 << 31) - 1)

int m, n, s;
int list_heads[N_MAX];          //用链表存储边，对应各点的链表头
int edges[M_MAX][3];            //[0]: 边的权重 [1]: 指向的节点 [2]:链表中的下一条边

int dis[N_MAX]/*达到节点的距离*/ , vis[N_MAX] /*已达集合*/;


struct node {
    int idx, distance;
    friend bool operator < (node a, node b) {
        return a.distance < b.distance;
    }
    friend bool operator > (node a, node b) {
        return a.distance > b.distance;
    }
};


std::priority_queue<node, std::vector<node>, std::greater<node>> pq;

void read()
{
    unsigned i, from, to, weight;
    std::cin >> n >> m;
    s = 1;
    for (i = 1; i <= m; i ++) {
        std::cin >> from >> to >> weight;
        edges[i][WEIGHT] = weight;
        edges[i][TO] = to;
        edges[i][NEXT] = list_heads[from];
        list_heads[from] = i;
    }
}

int main()
{
    int i, reachable_nodes;
    node nd;
    read();
    for (i = 1; i <= n; i ++) {
        dis[i] = INFI;
    }


    nd.distance = 0;
    nd.idx = s;
    pq.push(nd);
    dis[s] = 0;

    while (!pq.empty()) {
        node min_node = pq.top();
        int min_idx = min_node.idx;
        pq.pop();
        if (vis[min_idx]) continue;
        vis[min_idx] = 1;
        int edge_p;
        for (edge_p = list_heads[min_idx]; edge_p != 0; edge_p = edges[edge_p][NEXT]) {
            // 更新子节点
            int to = edges[edge_p][TO];
            int weight = edges[edge_p][WEIGHT];
            if (min_node.distance + weight < dis[to]) {
                // reachable_nodes ++;
                dis[to] = min_node.distance + weight;
                nd.idx = to;
                nd.distance = min_node.distance + weight;
                pq.push(nd);
            }
        }
    }

    if (dis[n] == INFI) std::cout << "-1\n";
    else std::cout << dis[n] << "\n";

    return 0;
}
