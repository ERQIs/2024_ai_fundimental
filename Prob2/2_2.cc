#include <bits/stdc++.h>
#define HASH_LIST_SIZE 4000

std::vector<long long> hash_t[HASH_LIST_SIZE];

// 移动方法：
/* 
0  1  2
3  4  5
6  7  8

dir[0] 表示0号位置可以移动到的位置，数组结尾以-1标识。
*/
const int dir[9][10] = {
    {1, 3, -1}, 
    {0, 2, 4, -1},
    {1, 5, -1}, 
    {0, 4, 6, -1}, 
    {1, 3, 5, 7, -1},
    {2, 4, 8, -1}, 
    {3, 7, -1}, 
    {4, 6, 8, -1},
    {5, 7, -1}
};

class state {
  private:
    int hash_val, z_pos, dis;
    int puz[9];
    long long id;
  public:
    state(int arr[]);
    state();
    void make_id();
    long long get_id();
    int get_z_pos();
    int get_distance();
    void set_distance(int);
    void make_hash();
    int get_hash();
    void show();
    bool hash_check();
    void hash_register();
    void swap_zero_to(int);
    bool operator==(state&);
};

state::state(int arr[]) {
    int i;
    for (i = 0; i < 9; i ++) {
        puz[i] = arr[i];
        if (arr[i] == 0) {
            z_pos = i;
        }
    }
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
    make_id();
    make_hash(); 
}

int state::get_distance() { return dis; }
void state::set_distance(int d) {dis = d; } 
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


std::queue<state*> qq;

int main() {
    int i, read[9];
    int goal_arr[9] = {1, 2, 3, 4, 5 ,6 ,7, 8, 0};
    for (i = 0; i < 9; i ++) {
        read[i] = getchar() - '0';
        if (read[i] == 'x' - '0') read[i] = 0;
        if (i != 8) getchar();
    }
    state init = state(read);
    state goal = state(goal_arr);

    qq.push(&init);

    init.set_distance(0);

    if (init == goal) {
        printf("0\n");
        return 0;
    }

    while (!qq.empty()) {
        state* now = qq.front();
        qq.pop();
        // now->show();
        int i, z_pos = now->get_z_pos() , nxt_pos;

        if (now->hash_check()) {
            delete now;
            continue;
        }
        now->hash_register();

        // 查看新的状态是否是目标状态

        for (i = 0, nxt_pos = dir[z_pos][0]; nxt_pos != -1; nxt_pos = dir[z_pos][++ i]) {

            state* new_state = new state();
            *new_state = *now;
            new_state->swap_zero_to(nxt_pos);

            // 查看状态是否已经被记录，如果没有则记录状态
            if (new_state->hash_check()) {
                delete new_state;
                continue;
            }

            // new_state->hash_register();
            new_state->set_distance( new_state->get_distance() + 1 );

            if (*new_state == goal) {
                printf("%d\n", new_state->get_distance());
                return 0;
            }

            // 将新状态加入到frontier中
            qq.push(new_state);
        }
    }
    printf("-1\n");
    return 0;
}
