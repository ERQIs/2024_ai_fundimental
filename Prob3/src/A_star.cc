#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <fstream>
#include <vector>
#include <cmath>
#define N_MAX 150
#define M_MAX 150
#define INFI 10000


int m, n;
int map[101][101];
bool vis[101][101];
int dis[101][101];
int huriestic[101][101];
int move[4][2] = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}
};

std::ofstream f_path, f_log;   //输出文件

class Point {
public:
    int x, y;
    // Point *self, *parent;
    Point(int, int);
    Point();
    bool operator== (const Point& other) const { return x == other.x && y == other.y;}
    bool operator!= (const Point& other) const { return x != other.x || y != other.y;}
    std::string to_string(); 
    int get_dis();
    void set_dis(int);
    Point get_parent();
    void set_parent(Point);
    void set_visit();
    bool get_visit();
    int get_huriestic();
    void set_huriestic(int);
};

Point null_point;
Point parent[101][101];

// Point::Point(int _x, int _y) {x = _x; y = _y; self = this; }
Point::Point(int _x, int _y) {x = _x; y = _y;}
Point::Point() {x = -1; y = -1;}
Point Point::get_parent() {return parent[x][y]; }
void Point::set_parent(Point p) {parent[x][y] = p; } 
bool Point::get_visit() {return vis[x][y]; }
void Point::set_visit() {vis[x][y] = true; } 
int Point::get_dis() {return dis[x][y]; }
int Point::get_huriestic() {return huriestic[x][y]; }
void Point::set_huriestic(int h) {huriestic[x][y] = h; } 
void Point::set_dis(int d) {dis[x][y] = d; }
std::string Point::to_string() {
    return std::to_string(x) + " " + std::to_string(y);
}

void show_path(Point p) {
    std::stack<std::string> stk;
    do {
        stk.push(p.to_string());
        p = p.get_parent();
        if (p != null_point && p == p.get_parent()) {
            std::cout << "error\n";
            break;
        }
    } while (p != null_point);
    while (!stk.empty()) {
        f_path << stk.top() << std::endl;
        // std::cout << stk.top() << std::endl;
        stk.pop();
    }
}

// Function to compute the n-norm of a vector
double computeNorm(const std::vector<double>& vec, int n) {
    double norm = 0.0;
    // Calculate the sum of the nth powers of each element
    for (double elem : vec) {
        norm += std::pow(std::abs(elem), n);
    }
    // Take the nth root of the sum
    norm = std::pow(norm, 1.0 / n);
    return norm;
}

int huriestic_f(Point p) {
    std::vector<double> vec = {static_cast<double>(m - p.x), static_cast<double>(n - p.y)};
    double n_norm = computeNorm(vec, 2);
    return static_cast<int>(n_norm);
}

Point get_min_point() {
    Point ret = Point(0, 0);
    ret.set_dis(INFI);
    for (int i = 1; i <= m; i ++) {
        for (int j = 1; j <= n; j ++) {
            Point iter = Point(i, j);
            if ((iter.get_visit() == false)) {
                if (ret.get_dis() + ret.get_huriestic() > iter.get_dis() + iter.get_huriestic() ) {
                    ret = Point(i, j);
                }
            }
        }
    }
    return ret;
}
 
int main(int argc, char* argv[])
{
    // 读取迷宫
    int i, j, x_t, y_t;
    std::cin >> m >> n;          // m 对应 x, n 对应 y
    for (i = 1; i <= m; i ++) {
        for (j = 1; j <= n; j ++) {
            Point iter = Point(i, j);
            std::cin >> map[i][j];
            iter.set_dis(INFI);
            // 提前计算 估值函数
            iter.set_huriestic(huriestic_f(iter)); 
        }
    }

    
    // 设置起点和终点
    // std::cin >> x_t >> y_t;
    Point start = Point(1, 1);
    // std::cin >> x_t >> y_t;
    Point target = Point(m , n);

    // 设置输出文件
    if (argc != 3) {
        std::cout << "usage: type input.txt | dijkstra.exe [path_output_path] [log_output_path]" << std::endl;
        return 0;
    }
    f_path.open(argv[1]);
    f_log.open(argv[2]);
 

    // A_star 主算法
    int reachable_grids = 1;
    start.set_parent(null_point);
    start.set_dis(0);

    while (reachable_grids != 0) {
        Point now_p = get_min_point();
        f_log << now_p.to_string() << std::endl;    // 日志点
        // printf("yet the min is %d ", now_p.get_dis() + now_p.get_huriestic());
        // std::cout << "at " << now_p.to_string() << std::endl;
        reachable_grids --;
        if (now_p == target) {
            show_path(now_p);
            break;
        }
        now_p.set_visit();
        int x, y;
        for (i = 0; i < 4; i ++) {
            x = now_p.x + move[i][0];
            y = now_p.y + move[i][1];
            if (x > m || x < 1 || y > n || y < 1 || map[x][y] == 1 || vis[x][y] == 1) {
                continue;
            }
            Point adjecent_p = Point(x, y);
            if (now_p.get_dis() + 1 < adjecent_p.get_dis()) {
                reachable_grids ++;
                adjecent_p.set_dis(now_p.get_dis() + 1);
                adjecent_p.set_parent(now_p);
            }
        }
    }

    f_path.close();
    f_log.close();
    // printf("tag");
    return 0;
}
