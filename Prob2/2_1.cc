#include <bits/stdc++.h>
#define HASH_LIST_SIZE 4000

struct state {
    int hash_val, z_pos, p; //p是下一个需要看的子状态
    int puz[9];
    long long id;
};

char debug_hash[1000000000];

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

const int check_arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};

// 生成状态对应的哈希值
long long hash_f (int* arr) {
    int i;
    long long ret = 0;
    for (i = 0; i < 9; i ++) {
        ret <<= 1;
        ret += arr[i];
    }
    return ret;
}

// 生成状态对应的唯一标识
long long id_f (int* arr) {
    int i;
    long long ret = 0;
    for (i = 0; i < 9; i ++) {
        ret *= 10;
        ret += arr[i];
    }
    return ret;
}

// 检测是否是目标状态
bool check_tar(struct state* s) {
    int* arr = s->puz;
    int i;
    for (i = 0; i < 9; i ++) {
        if (arr[i] !=check_arr[i]) {
            return false;
        }
    }
    return true;
}


void show_state(struct state* s) {
    int* arr = s->puz;
    int i;
    for (i = 0; i < 9; i ++) {
        std::cout << arr[i] << " \n"[i%3 == 2];
    }
    std::cout << "id: " << s->id << "\nz_pos: " << s->z_pos << "\nhash_val: " << s->hash_val << "\n";
}

// 判断状态是否已经冲突，如果没有冲突则将状态记录到哈希表中。
bool check_and_register_hash(struct state* s) {
    int i, to, hv;
    long long id;
    hv = s->hash_val;
    id = s->id;
    to = hash_t[hv].size();
    for (i = 0; i < to; i ++) {
        if (hash_t[hv][i] == id) {
            return true;
        }
    }
    hash_t[hv].push_back(id);
    return false;
}

void swap_int(int* _a, int* _b) {
    int a = *_a;
    int b = *_b;
    *_b = a;
    *_a = b;
}

struct state* move (struct state* s, int p) {
    struct state* ret = new state;
    return ret;
}


std::stack<struct state*> stk;

bool dfs(struct state* s) {
    int i, z_pos, nxt_pos;
    z_pos = s->z_pos;
    show_state(s);
    for (i = 0, nxt_pos = dir[z_pos][0]; nxt_pos != -1; nxt_pos = dir[z_pos][++ i]) {

        // 创建新的state
        struct state new_state = *s;
        swap_int(&(new_state.puz)[nxt_pos], &(new_state.puz)[z_pos]);
        new_state.id = id_f(new_state.puz);
        new_state.hash_val = hash_f(new_state.puz);
        new_state.z_pos = nxt_pos;

        // 查看新的状态是否是目标状态
        if (check_tar(&new_state)) {
            printf("1\n");
            return true;
        }
        
        // 查看状态是否已经被记录，如果没有则记录状态
        if (check_and_register_hash(&new_state)) {
            continue;
        }

        // 继续查询下一个状态
        if (dfs(&new_state) == true) {
            return true;
        }

    }
    return false;
}


int main() {
    int i;
    struct state init;
    for (i = 0; i < 9; i ++) {
        init.puz[i] = getchar() - '0';
        if (init.puz[i] == 'x' - '0'){
            init.puz[i] = 0;
            init.z_pos = i;
        }
        if (i != 8) getchar();
    }
    init.hash_val = hash_f(init.puz);
    init.id = id_f(init.puz);
    init.p = 0;


    stk.push(&init);
    debug_hash[init.id] = 1;
    check_and_register_hash(&init);

    while (!stk.empty()) {

        struct state* now = stk.top();
        stk.pop();

        int i, z_pos = now->z_pos , nxt_pos = dir[z_pos][now->p];

        // 查看新的状态是否是目标状态
        if (check_tar(now)) {
            printf("1\n");
            return 0;
        }
        
        for (i = 0, nxt_pos = dir[z_pos][0]; nxt_pos != -1; nxt_pos = dir[z_pos][++ i]) {

            // 创建新的state
            struct state* new_state = new struct state;
            *new_state = *now;
            swap_int(&(new_state->puz)[nxt_pos], &(new_state->puz)[z_pos]);
            new_state->id = id_f(new_state->puz);
            new_state->hash_val = hash_f(new_state->puz);
            new_state->z_pos = nxt_pos;


            // 查看状态是否已经被记录，如果没有则记录状态
            if (check_and_register_hash(new_state)) {
                delete new_state;
                continue;
            }

            // 将新状态加入到frontier中
            stk.push(new_state);
        }
    }

    printf("0\n");

    return 0;
}
