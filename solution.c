#include <stdio.h>

#define MAX_REQUESTS 500000

// Min-heap for upward requests
typedef struct {
    int arr[MAX_REQUESTS];
    int cnt;
} MinHeap;

void minHeapInit(MinHeap* heap) {
    heap->cnt = 0;
}

void minHeapPush(MinHeap* heap, int x) {
    heap->arr[++heap->cnt] = x;
    for (int i = heap->cnt, j = heap->cnt >> 1; j; i = j, j >>= 1) {
        if (heap->arr[j] > heap->arr[i]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

int minHeapTop(MinHeap* heap) {
    return heap->arr[1];
}

void minHeapPop(MinHeap* heap) {
    heap->arr[1] = heap->arr[heap->cnt--];
    for (int i = 1, j = i << 1; j <= heap->cnt; i = j, j <<= 1) {
        if (j + 1 <= heap->cnt && heap->arr[j + 1] < heap->arr[j]) j++;
        if (heap->arr[i] > heap->arr[j]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

int minHeapEmpty(MinHeap* heap) {
    return heap->cnt == 0;
}

void minHeapRemove(MinHeap* heap, int x) {
    int pos = -1;
    for (int i = 1; i <= heap->cnt; i++) {
        if (heap->arr[i] == x) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    
    heap->arr[pos] = heap->arr[heap->cnt--];
    for (int i = pos, j = i << 1; j <= heap->cnt; i = j, j <<= 1) {
        if (j + 1 <= heap->cnt && heap->arr[j + 1] < heap->arr[j]) j++;
        if (heap->arr[i] > heap->arr[j]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
    for (int i = pos, j = i >> 1; j; i = j, j >>= 1) {
        if (heap->arr[j] > heap->arr[i]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

// Max-heap for downward requests
typedef struct {
    int arr[MAX_REQUESTS];
    int cnt;
} MaxHeap;

void maxHeapInit(MaxHeap* heap) {
    heap->cnt = 0;
}

void maxHeapPush(MaxHeap* heap, int x) {
    heap->arr[++heap->cnt] = x;
    for (int i = heap->cnt, j = heap->cnt >> 1; j; i = j, j >>= 1) {
        if (heap->arr[j] < heap->arr[i]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

int maxHeapTop(MaxHeap* heap) {
    return heap->arr[1];
}

void maxHeapPop(MaxHeap* heap) {
    heap->arr[1] = heap->arr[heap->cnt--];
    for (int i = 1, j = i << 1; j <= heap->cnt; i = j, j <<= 1) {
        if (j + 1 <= heap->cnt && heap->arr[j + 1] > heap->arr[j]) j++;
        if (heap->arr[i] < heap->arr[j]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

int maxHeapEmpty(MaxHeap* heap) {
    return heap->cnt == 0;
}

void maxHeapRemove(MaxHeap* heap, int x) {
    int pos = -1;
    for (int i = 1; i <= heap->cnt; i++) {
        if (heap->arr[i] == x) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    
    heap->arr[pos] = heap->arr[heap->cnt--];
    for (int i = pos, j = i << 1; j <= heap->cnt; i = j, j <<= 1) {
        if (j + 1 <= heap->cnt && heap->arr[j + 1] > heap->arr[j]) j++;
        if (heap->arr[i] < heap->arr[j]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
    for (int i = pos, j = i >> 1; j; i = j, j >>= 1) {
        if (heap->arr[j] < heap->arr[i]) {
            int tmp = heap->arr[i];
            heap->arr[i] = heap->arr[j];
            heap->arr[j] = tmp;
        } else break;
    }
}

typedef struct {
    int current_floor;
    int direction_up;  // 1 for up, 0 for down
    MinHeap up_heap;    // requests above current floor
    MaxHeap down_heap;  // requests below current floor
    
    // Temporary arrays for reorganization
    int temp_up[MAX_REQUESTS], temp_down[MAX_REQUESTS];
    int temp_up_count, temp_down_count;
} ElevatorSystem;

void elevatorInit(ElevatorSystem* elevator) {
    elevator->current_floor = 0;
    elevator->direction_up = 1;
    minHeapInit(&elevator->up_heap);
    maxHeapInit(&elevator->down_heap);
    elevator->temp_up_count = 0;
    elevator->temp_down_count = 0;
}

void elevatorAddRequest(ElevatorSystem* elevator, int floor) {
    if (floor > elevator->current_floor) {
        minHeapPush(&elevator->up_heap, floor);
    } else {
        maxHeapPush(&elevator->down_heap, floor);
    }
}

void elevatorCancelRequest(ElevatorSystem* elevator, int floor) {
    if (floor > elevator->current_floor) {
        minHeapRemove(&elevator->up_heap, floor);
    } else {
        maxHeapRemove(&elevator->down_heap, floor);
    }
}

void elevatorAction(ElevatorSystem* elevator) {
    if (minHeapEmpty(&elevator->up_heap) && maxHeapEmpty(&elevator->down_heap)) {
        // No requests, stay still
        return;
    }
    
    if (elevator->direction_up) {
        // Moving up
        if (!minHeapEmpty(&elevator->up_heap)) {
            // Complete nearest upward request
            elevator->current_floor = minHeapTop(&elevator->up_heap);
            minHeapPop(&elevator->up_heap);
            
            // Reorganize heaps based on new current floor
            elevator->temp_up_count = elevator->temp_down_count = 0;
            
            // Move elements from up_heap that are now below current floor
            while (!minHeapEmpty(&elevator->up_heap)) {
                int val = minHeapTop(&elevator->up_heap);
                minHeapPop(&elevator->up_heap);
                if (val > elevator->current_floor) {
                    elevator->temp_up[elevator->temp_up_count++] = val;
                } else {
                    elevator->temp_down[elevator->temp_down_count++] = val;
                }
            }
            
            // Move elements from down_heap that are now above current floor
            while (!maxHeapEmpty(&elevator->down_heap)) {
                int val = maxHeapTop(&elevator->down_heap);
                maxHeapPop(&elevator->down_heap);
                if (val > elevator->current_floor) {
                    elevator->temp_up[elevator->temp_up_count++] = val;
                } else {
                    elevator->temp_down[elevator->temp_down_count++] = val;
                }
            }
            
            // Rebuild heaps
            for (int i = 0; i < elevator->temp_up_count; i++) {
                minHeapPush(&elevator->up_heap, elevator->temp_up[i]);
            }
            for (int i = 0; i < elevator->temp_down_count; i++) {
                maxHeapPush(&elevator->down_heap, elevator->temp_down[i]);
            }
        } else {
            // No more upward requests, reverse direction
            elevator->direction_up = 0;
            if (!maxHeapEmpty(&elevator->down_heap)) {
                // Complete nearest downward request
                elevator->current_floor = maxHeapTop(&elevator->down_heap);
                maxHeapPop(&elevator->down_heap);
                
                // Reorganize heaps based on new current floor
                elevator->temp_up_count = elevator->temp_down_count = 0;
                
                while (!minHeapEmpty(&elevator->up_heap)) {
                    int val = minHeapTop(&elevator->up_heap);
                    minHeapPop(&elevator->up_heap);
                    if (val > elevator->current_floor) {
                        elevator->temp_up[elevator->temp_up_count++] = val;
                    } else {
                        elevator->temp_down[elevator->temp_down_count++] = val;
                    }
                }
                
                while (!maxHeapEmpty(&elevator->down_heap)) {
                    int val = maxHeapTop(&elevator->down_heap);
                    maxHeapPop(&elevator->down_heap);
                    if (val > elevator->current_floor) {
                        elevator->temp_up[elevator->temp_up_count++] = val;
                    } else {
                        elevator->temp_down[elevator->temp_down_count++] = val;
                    }
                }
                
                // Rebuild heaps
                for (int i = 0; i < elevator->temp_up_count; i++) {
                    minHeapPush(&elevator->up_heap, elevator->temp_up[i]);
                }
                for (int i = 0; i < elevator->temp_down_count; i++) {
                    maxHeapPush(&elevator->down_heap, elevator->temp_down[i]);
                }
            }
        }
    } else {
        // Moving down
        if (!maxHeapEmpty(&elevator->down_heap)) {
            // Complete nearest downward request
            elevator->current_floor = maxHeapTop(&elevator->down_heap);
            maxHeapPop(&elevator->down_heap);
            
            // Reorganize heaps based on new current floor
            elevator->temp_up_count = elevator->temp_down_count = 0;
            
            while (!minHeapEmpty(&elevator->up_heap)) {
                int val = minHeapTop(&elevator->up_heap);
                minHeapPop(&elevator->up_heap);
                if (val > elevator->current_floor) {
                    elevator->temp_up[elevator->temp_up_count++] = val;
                } else {
                    elevator->temp_down[elevator->temp_down_count++] = val;
                }
            }
            
            while (!maxHeapEmpty(&elevator->down_heap)) {
                int val = maxHeapTop(&elevator->down_heap);
                maxHeapPop(&elevator->down_heap);
                if (val > elevator->current_floor) {
                    elevator->temp_up[elevator->temp_up_count++] = val;
                } else {
                    elevator->temp_down[elevator->temp_down_count++] = val;
                }
            }
            
            // Rebuild heaps
            for (int i = 0; i < elevator->temp_up_count; i++) {
                minHeapPush(&elevator->up_heap, elevator->temp_up[i]);
            }
            for (int i = 0; i < elevator->temp_down_count; i++) {
                maxHeapPush(&elevator->down_heap, elevator->temp_down[i]);
            }
        } else {
            // No more downward requests, reverse direction
            elevator->direction_up = 1;
            if (!minHeapEmpty(&elevator->up_heap)) {
                // Complete nearest upward request
                elevator->current_floor = minHeapTop(&elevator->up_heap);
                minHeapPop(&elevator->up_heap);
                
                // Reorganize heaps based on new current floor
                elevator->temp_up_count = elevator->temp_down_count = 0;
                
                while (!minHeapEmpty(&elevator->up_heap)) {
                    int val = minHeapTop(&elevator->up_heap);
                    minHeapPop(&elevator->up_heap);
                    if (val > elevator->current_floor) {
                        elevator->temp_up[elevator->temp_up_count++] = val;
                    } else {
                        elevator->temp_down[elevator->temp_down_count++] = val;
                    }
                }
                
                while (!maxHeapEmpty(&elevator->down_heap)) {
                    int val = maxHeapTop(&elevator->down_heap);
                    maxHeapPop(&elevator->down_heap);
                    if (val > elevator->current_floor) {
                        elevator->temp_up[elevator->temp_up_count++] = val;
                    } else {
                        elevator->temp_down[elevator->temp_down_count++] = val;
                    }
                }
                
                // Rebuild heaps
                for (int i = 0; i < elevator->temp_up_count; i++) {
                    minHeapPush(&elevator->up_heap, elevator->temp_up[i]);
                }
                for (int i = 0; i < elevator->temp_down_count; i++) {
                    maxHeapPush(&elevator->down_heap, elevator->temp_down[i]);
                }
            }
        }
    }
}

int elevatorLocate(ElevatorSystem* elevator) {
    return elevator->current_floor;
}

int main() {
    int n;
    scanf("%d", &n);
    
    ElevatorSystem elevator;
    elevatorInit(&elevator);
    
    for (int i = 0; i < n; i++) {
        char command[20];
        scanf("%s", command);
        
        if (command[0] == 'a' && command[1] == 'c') {
            // action
            elevatorAction(&elevator);
        } else if (command[0] == 'a' && command[1] == 'd') {
            // add
            int x;
            scanf("%d", &x);
            elevatorAddRequest(&elevator, x);
        } else if (command[0] == 'c') {
            // cancel
            int x;
            scanf("%d", &x);
            elevatorCancelRequest(&elevator, x);
        } else if (command[0] == 'l') {
            // locate
            printf("%d\n", elevatorLocate(&elevator));
        }
    }
    
    return 0;
}