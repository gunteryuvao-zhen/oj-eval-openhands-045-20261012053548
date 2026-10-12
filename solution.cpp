#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class ElevatorSystem {
private:
    int current_floor;
    bool direction_up;  // true for up, false for down
    multiset<int> up_requests;   // requests above current floor
    multiset<int> down_requests; // requests below current floor
    
public:
    ElevatorSystem() : current_floor(0), direction_up(true) {}
    
    void add_request(int floor) {
        if (floor > current_floor) {
            up_requests.insert(floor);
        } else {
            down_requests.insert(floor);
        }
    }
    
    void cancel_request(int floor) {
        if (floor > current_floor) {
            up_requests.erase(up_requests.find(floor));
        } else {
            down_requests.erase(down_requests.find(floor));
        }
    }
    
    void action() {
        if (up_requests.empty() && down_requests.empty()) {
            // No requests, stay still
            return;
        }
        
        if (direction_up) {
            // Moving up
            if (!up_requests.empty()) {
                // Complete nearest upward request
                auto it = up_requests.begin();
                current_floor = *it;
                up_requests.erase(it);
                
                // Move remaining down_requests to up_requests if they're now above current floor
                vector<int> to_move;
                for (int floor : down_requests) {
                    if (floor > current_floor) {
                        to_move.push_back(floor);
                    }
                }
                for (int floor : to_move) {
                    down_requests.erase(down_requests.find(floor));
                    up_requests.insert(floor);
                }
            } else {
                // No more upward requests, reverse direction
                direction_up = false;
                if (!down_requests.empty()) {
                    // Complete nearest downward request
                    auto it = prev(down_requests.end());
                    current_floor = *it;
                    down_requests.erase(it);
                }
            }
        } else {
            // Moving down
            if (!down_requests.empty()) {
                // Complete nearest downward request
                auto it = prev(down_requests.end());
                current_floor = *it;
                down_requests.erase(it);
                
                // Move remaining up_requests to down_requests if they're now below current floor
                vector<int> to_move;
                for (int floor : up_requests) {
                    if (floor < current_floor) {
                        to_move.push_back(floor);
                    }
                }
                for (int floor : to_move) {
                    up_requests.erase(up_requests.find(floor));
                    down_requests.insert(floor);
                }
            } else {
                // No more downward requests, reverse direction
                direction_up = true;
                if (!up_requests.empty()) {
                    // Complete nearest upward request
                    auto it = up_requests.begin();
                    current_floor = *it;
                    up_requests.erase(it);
                }
            }
        }
    }
    
    int locate() const {
        return current_floor;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;
        
        if (command == "action") {
            elevator.action();
        } else if (command == "add") {
            int x;
            cin >> x;
            elevator.add_request(x);
        } else if (command == "cancel") {
            int x;
            cin >> x;
            elevator.cancel_request(x);
        } else if (command == "locate") {
            cout << elevator.locate() << "\n";
        }
    }
    
    return 0;
}