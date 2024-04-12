#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <fstream>

int m, n;
int map[101][101];
int vis[101][101];
int move[4][2] = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}
};

std::ofstream f_path, f_log;   //输出文件

class Point {
public:
    int x, y, dis;
    Point *self, *parent;
    Point(int, int);
    bool operator== (const Point& other) const { 
        return x == other.x && y == other.y;
    }
    std::string to_string(); 
};

Point::Point(int _x, int _y) {x = _x; y = _y; self = this; }

std::string Point::to_string() {
    return std::to_string(x) + " " + std::to_string(y);
}

void show_path(Point* p) {
    std::stack<std::string> stk;
    do {
        stk.push(p->to_string());
        p = p->parent;
        if (p != NULL && p == p->parent) {
            std::cout << "error\n";
            break;
        }
    } while (p != NULL);
    while (!stk.empty()) {
        f_path << stk.top() << std::endl;
        stk.pop();
    }
}

std::queue<Point, std::list<Point>> q;

int main(int argc, char* argv[])
{
    // 读取迷宫
    int i, j, x_t, y_t;
    std::cin >> m >> n;          // m 对应 x, n 对应 y
    for (i = 1; i <= m; i ++) {
        for (j = 1; j <= n; j ++) {
            std::cin >> map[i][j];
        }
    }

    // 设置起点和终点
    // std::cin >> x_t >> y_t;
    Point* start = new Point(1, 1);
    // std::cin >> x_t >> y_t;
    Point target = Point(m , n);

    // 设置输出文件
    if (argc != 3) {
        std::cout << "usage: type input.txt | bfs.exe [path_output_path] [log_output_path]" << std::endl;
        return 0;
    }
    f_path.open(argv[1]);
    f_log.open(argv[2]);
 
    // BFS主算法
    start->dis = 0;
    start->parent = NULL;
    q.push(*start);

    while (!q.empty()) {
        Point now = q.front();
        q.pop();

        int x, y;
        for (i = 0; i < 4; i ++) {
            x = now.x + move[i][0];
            y = now.y + move[i][1];
            if (x > m || x < 1 || y > n || y < 1 || map[x][y] == 1 || vis[x][y] == 1) {
                continue;
            }
            vis[x][y] = 1;
            Point* p = new Point(x, y);
            p->dis = now.dis + 1;
            p->parent = now.self;
            p->self = p;

            f_log << p->to_string() << std::endl;    // 日志点

            if (*p == target) {
                // std::cout << p->dis + 1 << std::endl;
                show_path(p);
                return 0;
            }
            q.push(*p);
        }
    }

    f_path.close();
    f_log.close();

    return 0;
}