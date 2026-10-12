#include <cstdio>
#include <iostream>

using namespace std;

const int MAX_REQUESTS = 500000;

// Min-heap for upward requests
class MinHeap {
private:
    int arr[MAX_REQUESTS];
    int cnt;
    
public:
    MinHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[j] > arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        arr[1] = arr[cnt--];
        for (int i = 1, j = i << 1; j <= cnt; i = j, j <<= 1) {
            if (j + 1 <= cnt && arr[j + 1] < arr[j]) j++;
            if (arr[i] > arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
    
    void remove(int x) {
        int pos = -1;
        for (int i = 1; i <= cnt; i++) {
            if (arr[i] == x) {
                pos = i;
                break;
            }
        }
        if (pos == -1) return;
        
        arr[pos] = arr[cnt--];
        for (int i = pos, j = i << 1; j <= cnt; i = j, j <<= 1) {
            if (j + 1 <= cnt && arr[j + 1] < arr[j]) j++;
            if (arr[i] > arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
        for (int i = pos, j = i >> 1; j; i = j, j >>= 1) {
            if (arr[j] > arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
};

// Max-heap for downward requests
class MaxHeap {
private:
    int arr[MAX_REQUESTS];
    int cnt;
    
public:
    MaxHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[j] < arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        arr[1] = arr[cnt--];
        for (int i = 1, j = i << 1; j <= cnt; i = j, j <<= 1) {
            if (j + 1 <= cnt && arr[j + 1] > arr[j]) j++;
            if (arr[i] < arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
    
    void remove(int x) {
        int pos = -1;
        for (int i = 1; i <= cnt; i++) {
            if (arr[i] == x) {
                pos = i;
                break;
            }
        }
        if (pos == -1) return;
        
        arr[pos] = arr[cnt--];
        for (int i = pos, j = i << 1; j <= cnt; i = j, j <<= 1) {
            if (j + 1 <= cnt && arr[j + 1] > arr[j]) j++;
            if (arr[i] < arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
        for (int i = pos, j = i >> 1; j; i = j, j >>= 1) {
            if (arr[j] < arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
};

class ElevatorSystem {
private:
    int current_floor;
    bool direction_up;  // true for up, false for down
    MinHeap up_heap;    // requests above current floor
    MaxHeap down_heap;  // requests below current floor
    
    // Temporary arrays for reorganization
    int temp_up[MAX_REQUESTS], temp_down[MAX_REQUESTS];
    int temp_up_count, temp_down_count;
    
public:
    ElevatorSystem() : current_floor(0), direction_up(true) {}
    
    void add_request(int floor) {
        if (floor > current_floor) {
            up_heap.push(floor);
        } else {
            down_heap.push(floor);
        }
    }
    
    void cancel_request(int floor) {
        if (floor > current_floor) {
            up_heap.remove(floor);
        } else {
            down_heap.remove(floor);
        }
    }
    
    void action() {
        if (up_heap.empty() && down_heap.empty()) {
            // No requests, stay still
            return;
        }
        
        if (direction_up) {
            // Moving up
            if (!up_heap.empty()) {
                // Complete nearest upward request
                current_floor = up_heap.top();
                up_heap.pop();
                
                // Reorganize heaps based on new current floor
                temp_up_count = temp_down_count = 0;
                
                // Move elements from up_heap that are now below current floor
                while (!up_heap.empty()) {
                    int val = up_heap.top();
                    up_heap.pop();
                    if (val > current_floor) {
                        temp_up[temp_up_count++] = val;
                    } else {
                        temp_down[temp_down_count++] = val;
                    }
                }
                
                // Move elements from down_heap that are now above current floor
                while (!down_heap.empty()) {
                    int val = down_heap.top();
                    down_heap.pop();
                    if (val > current_floor) {
                        temp_up[temp_up_count++] = val;
                    } else {
                        temp_down[temp_down_count++] = val;
                    }
                }
                
                // Rebuild heaps
                for (int i = 0; i < temp_up_count; i++) {
                    up_heap.push(temp_up[i]);
                }
                for (int i = 0; i < temp_down_count; i++) {
                    down_heap.push(temp_down[i]);
                }
            } else {
                // No more upward requests, reverse direction
                direction_up = false;
                if (!down_heap.empty()) {
                    // Complete nearest downward request
                    current_floor = down_heap.top();
                    down_heap.pop();
                    
                    // Reorganize heaps based on new current floor
                    temp_up_count = temp_down_count = 0;
                    
                    while (!up_heap.empty()) {
                        int val = up_heap.top();
                        up_heap.pop();
                        if (val > current_floor) {
                            temp_up[temp_up_count++] = val;
                        } else {
                            temp_down[temp_down_count++] = val;
                        }
                    }
                    
                    while (!down_heap.empty()) {
                        int val = down_heap.top();
                        down_heap.pop();
                        if (val > current_floor) {
                            temp_up[temp_up_count++] = val;
                        } else {
                            temp_down[temp_down_count++] = val;
                        }
                    }
                    
                    // Rebuild heaps
                    for (int i = 0; i < temp_up_count; i++) {
                        up_heap.push(temp_up[i]);
                    }
                    for (int i = 0; i < temp_down_count; i++) {
                        down_heap.push(temp_down[i]);
                    }
                }
            }
        } else {
            // Moving down
            if (!down_heap.empty()) {
                // Complete nearest downward request
                current_floor = down_heap.top();
                down_heap.pop();
                
                // Reorganize heaps based on new current floor
                temp_up_count = temp_down_count = 0;
                
                while (!up_heap.empty()) {
                    int val = up_heap.top();
                    up_heap.pop();
                    if (val > current_floor) {
                        temp_up[temp_up_count++] = val;
                    } else {
                        temp_down[temp_down_count++] = val;
                    }
                }
                
                while (!down_heap.empty()) {
                    int val = down_heap.top();
                    down_heap.pop();
                    if (val > current_floor) {
                        temp_up[temp_up_count++] = val;
                    } else {
                        temp_down[temp_down_count++] = val;
                    }
                }
                
                // Rebuild heaps
                for (int i = 0; i < temp_up_count; i++) {
                    up_heap.push(temp_up[i]);
                }
                for (int i = 0; i < temp_down_count; i++) {
                    down_heap.push(temp_down[i]);
                }
            } else {
                // No more downward requests, reverse direction
                direction_up = true;
                if (!up_heap.empty()) {
                    // Complete nearest upward request
                    current_floor = up_heap.top();
                    up_heap.pop();
                    
                    // Reorganize heaps based on new current floor
                    temp_up_count = temp_down_count = 0;
                    
                    while (!up_heap.empty()) {
                        int val = up_heap.top();
                        up_heap.pop();
                        if (val > current_floor) {
                            temp_up[temp_up_count++] = val;
                        } else {
                            temp_down[temp_down_count++] = val;
                        }
                    }
                    
                    while (!down_heap.empty()) {
                        int val = down_heap.top();
                        down_heap.pop();
                        if (val > current_floor) {
                            temp_up[temp_up_count++] = val;
                        } else {
                            temp_down[temp_down_count++] = val;
                        }
                    }
                    
                    // Rebuild heaps
                    for (int i = 0; i < temp_up_count; i++) {
                        up_heap.push(temp_up[i]);
                    }
                    for (int i = 0; i < temp_down_count; i++) {
                        down_heap.push(temp_down[i]);
                    }
                }
            }
        }
    }
    
    int locate() {
        return current_floor;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        char command[20];
        scanf("%s", command);
        
        if (command[0] == 'a' && command[1] == 'c') {
            // action
            elevator.action();
        } else if (command[0] == 'a' && command[1] == 'd') {
            // add
            int x;
            scanf("%d", &x);
            elevator.add_request(x);
        } else if (command[0] == 'c') {
            // cancel
            int x;
            scanf("%d", &x);
            elevator.cancel_request(x);
        } else if (command[0] == 'l') {
            // locate
            printf("%d\n", elevator.locate());
        }
    }
    
    return 0;
}