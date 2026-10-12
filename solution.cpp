#include <cstdio>

#define MAX_REQUESTS 500000

int up_requests[MAX_REQUESTS];
int down_requests[MAX_REQUESTS];
int up_count = 0;
int down_count = 0;
int current_floor = 0;
bool direction_up = true;

void add_request(int floor) {
    if (floor > current_floor) {
        up_requests[up_count++] = floor;
    } else {
        down_requests[down_count++] = floor;
    }
}

void cancel_request(int floor) {
    if (floor > current_floor) {
        for (int i = 0; i < up_count; i++) {
            if (up_requests[i] == floor) {
                for (int j = i; j < up_count - 1; j++) {
                    up_requests[j] = up_requests[j + 1];
                }
                up_count--;
                break;
            }
        }
    } else {
        for (int i = 0; i < down_count; i++) {
            if (down_requests[i] == floor) {
                for (int j = i; j < down_count - 1; j++) {
                    down_requests[j] = down_requests[j + 1];
                }
                down_count--;
                break;
            }
        }
    }
}

int find_min_up() {
    if (up_count == 0) return -1;
    int min_idx = 0;
    for (int i = 1; i < up_count; i++) {
        if (up_requests[i] < up_requests[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

int find_max_down() {
    if (down_count == 0) return -1;
    int max_idx = 0;
    for (int i = 1; i < down_count; i++) {
        if (down_requests[i] > down_requests[max_idx]) {
            max_idx = i;
        }
    }
    return max_idx;
}

void action() {
    if (up_count == 0 && down_count == 0) {
        return;
    }
    
    if (direction_up) {
        int min_idx = find_min_up();
        if (min_idx != -1) {
            current_floor = up_requests[min_idx];
            for (int i = min_idx; i < up_count - 1; i++) {
                up_requests[i] = up_requests[i + 1];
            }
            up_count--;
        } else {
            direction_up = false;
            int max_idx = find_max_down();
            if (max_idx != -1) {
                current_floor = down_requests[max_idx];
                for (int i = max_idx; i < down_count - 1; i++) {
                    down_requests[i] = down_requests[i + 1];
                }
                down_count--;
            }
        }
    } else {
        int max_idx = find_max_down();
        if (max_idx != -1) {
            current_floor = down_requests[max_idx];
            for (int i = max_idx; i < down_count - 1; i++) {
                down_requests[i] = down_requests[i + 1];
            }
            down_count--;
        } else {
            direction_up = true;
            int min_idx = find_min_up();
            if (min_idx != -1) {
                current_floor = up_requests[min_idx];
                for (int i = min_idx; i < up_count - 1; i++) {
                    up_requests[i] = up_requests[i + 1];
                }
                up_count--;
            }
        }
    }
    
    // Reorganize requests based on new current floor
    int temp_up[MAX_REQUESTS], temp_down[MAX_REQUESTS];
    int temp_up_count = 0, temp_down_count = 0;
    
    for (int i = 0; i < up_count; i++) {
        if (up_requests[i] > current_floor) {
            temp_up[temp_up_count++] = up_requests[i];
        } else {
            temp_down[temp_down_count++] = up_requests[i];
        }
    }
    
    for (int i = 0; i < down_count; i++) {
        if (down_requests[i] > current_floor) {
            temp_up[temp_up_count++] = down_requests[i];
        } else {
            temp_down[temp_down_count++] = down_requests[i];
        }
    }
    
    up_count = temp_up_count;
    down_count = temp_down_count;
    
    for (int i = 0; i < up_count; i++) {
        up_requests[i] = temp_up[i];
    }
    for (int i = 0; i < down_count; i++) {
        down_requests[i] = temp_down[i];
    }
}

int locate() {
    return current_floor;
}

int main() {
    int n;
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        char command[20];
        scanf("%s", command);
        
        if (command[0] == 'a' && command[1] == 'c') {
            action();
        } else if (command[0] == 'a' && command[1] == 'd') {
            int x;
            scanf("%d", &x);
            add_request(x);
        } else if (command[0] == 'c') {
            int x;
            scanf("%d", &x);
            cancel_request(x);
        } else if (command[0] == 'l') {
            printf("%d\n", locate());
        }
    }
    
    return 0;
}