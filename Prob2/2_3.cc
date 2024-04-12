#include <bits/stdc++.h>
#define HASH_LIST_SIZE 4000

std::vector<long long> hash_t[HASH_LIST_SIZE];

// 移动方法：
/* 
0  1  2
3  4  5
6  7  8

direc 表明上一个状态来到这个状态时选择的方向
*/
const char dir[9][10] = {
    {'R', 'D', 'e'},                        // 0
    {'L', 'R', 'D', 'e'},                   // 1
    {'L', 'D', 'e'},                        // 2
    {'U', 'R', 'D', 'e'},                   // 3
    {'U', 'L', 'R', 'D', 'e'},              // 4
    {'U', 'L', 'D', 'e'},                   // 5
    {'U', 'R', 'e'},                        // 6
    {'U', 'L', 'R', 'e'},                   // 7
    {'U', 'L', 'e'}                         // 8
};

class state {
  private:
    int hash_val, z_pos, dis, d_left, dir_idx;
    char direc;
    int puz[9];
    long long id;
  public:
    state* parent, *self;
    state(int arr[]);
    state();
    void make_id();
    long long get_id();
    int get_z_pos();
    int get_distance();
    void set_distance(int);
    int get_d_left();
    void set_d_left(int);
    void make_hash();
    int get_hash();
    void show();
    bool hash_check();
    void hash_register();
    void swap_zero_to(int);
    char get_direc();
    void set_direc(char);
    bool operator==(state&);
    friend bool operator < (state a, state b) {
        return a.d_left < b.d_left;
    }
    friend bool operator > (state a, state b) {
        return a.d_left > b.d_left;
    }
};

state::state(int arr[]) {
    int i;
    for (i = 0; i < 9; i ++) {
        puz[i] = arr[i];
        if (arr[i] == 0) {
            z_pos = i;
        }
    }
    dir_idx = 0;
    make_id();
    make_hash();
}


state::state() {
    int i;
    for (i = 0; i < 9; i ++) {
        puz[i] = i;
        if (puz[i] == 0) {
            z_pos = i;
        }
    }
    dir_idx = 0;
    make_id();
    make_hash(); 
}

int state::get_distance() { return dis; }
void state::set_distance(int d) {dis = d; } 
char state::get_direc() { return direc; }
void state::set_direc(char d) {direc = d; } 
int state::get_d_left() { return d_left; }
void state::set_d_left(int d) {d_left = d; } 
int state::get_z_pos() { return z_pos; }
bool state::operator==(state &t) { return id == t.id; }
int state::get_hash() { return hash_val; }
long long state::get_id() { return id; }

void swap_int(int* _a, int* _b) {
    int a = *_a;
    int b = *_b;
    *_b = a;
    *_a = b;
}

void state::swap_zero_to(int new_z_pos) {
    swap_int(&puz[z_pos], &puz[new_z_pos]);
    this->make_id();
    this->make_hash();
    this->z_pos = new_z_pos;
}

void state::make_id() {
    int i;
    long long ret = 0;
    for (i = 0; i < 9; i ++) {
        ret *= 10;
        ret += puz[i];
    }
    id = ret;
}

void state::make_hash() {
    int i;
    long long ret = 0;
    for (i = 0; i < 9; i ++) {
        ret <<= 1;
        ret += puz[i];
    }
    hash_val = ret;
}

void state::show() {
    int i;
    for (i = 0; i < 9; i ++) {
        std::cout << puz[i] << " \n"[i%3 == 2];
    }
    std::cout << "id: " << id << "\nz_pos: " << z_pos << "\nhash_val: " << hash_val << "\n";
}

bool state::hash_check() {
    int i, to, hv;
    hv = hash_val;
    to = hash_t[hv].size();
    for (i = 0; i < to; i ++) {
        if (hash_t[hv][i] == id) {
            return true;
        }
    }
    return false;
}

void state::hash_register() { hash_t[hash_val].push_back(id); }

int move_to_pos (int pos, char mv) {
    int ret = -1;
    switch(mv) {
        case 'U':
            ret = pos - 3;
            break;
        case 'R':
            ret = pos + 1;
            break;
        case 'L':
            ret = pos - 1;
            break;
        case 'D':
            ret = pos + 3;
            break;
        default:
            break;
    }
    return ret;
}

void print_path(state* s) {
    char out;
    std::stack<char> stk;
    for (; out != 'e'; out = s->get_direc() , s = s->parent) {
        
        stk.push(out - 'A' + 'a');
    }
    while (!stk.empty()) {
        std::cout << stk.top();
        stk.pop();
    }
    std::cout << std::endl;

}

std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

int main() {
    int i, read[9];
    int goal_arr[9] = {1, 2, 3, 4, 5 ,6 ,7, 8, 0};
    int huristic[9];

    for (i = 0; i < 9; i ++) {
        read[i] = getchar() - '0';
        if (read[i] == 'x' - '0') read[i] = 0;
        if (i != 8) getchar();
    }
    
    state init = state(read);
    state goal = state(goal_arr);

    int z_pos = goal.get_z_pos();
    for (i = 0; i < 9; i ++) {
        huristic[i] = abs((i % 3) - (z_pos % 3)) + abs(i/3 - z_pos/3);
    }

    init.set_distance(0);
    init.set_direc('e');
    init.self = &init;
    pq.push(init);

    if (init == goal) {
        printf("0\n");
        return 0;
    }

    while (!pq.empty()) {
        state now = pq.top();
        pq.pop();
        // printf("%d\n", now.get_distance());
        // now->show();
        int i, z_pos = now.get_z_pos() , nxt_pos;
        char nxt_move;

        if (now.hash_check()) {
            // delete &now;
            continue;
        }
        now.hash_register();

        // 查看新的状态是否是目标状态

        for (i = 0, nxt_move = dir[z_pos][0]; nxt_move != 'e'; nxt_move = dir[z_pos][++ i]) {

            state* new_state = new state();
            *new_state = now;
            nxt_pos = move_to_pos(z_pos, nxt_move);
            // printf("%d ", nxt_pos);       /* TAG */
            new_state->swap_zero_to(nxt_pos);

            // 查看状态是否已经被记录
            if (new_state->hash_check()) {
                delete new_state;
                continue;
            }

            // new_state->hash_register();
            new_state->set_distance( new_state->get_distance() + 1 );
            new_state->set_d_left( new_state->get_distance() + huristic[new_state->get_z_pos()] );
            new_state->set_direc(nxt_move);
            new_state->parent = now.self;
            new_state->self = new_state;

            if (*new_state == goal) {
                // printf("%d\n", new_state->get_distance());
                print_path(new_state);
                return 0;
            }

            // 将新状态加入到frontier中
            pq.push(*new_state);
        }
    }
    printf("unsolvable\n");
    return 0;
}
