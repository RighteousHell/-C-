#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <future>
#include <mutex>
#include <unordered_set>
#include <thread>

struct Coordinate {
    int x;
    int y;

    struct Hash {
        std::size_t operator()(const Coordinate& c) const {
            return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
        }
    };

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
};

class ChessBoard {
public:
    ChessBoard() : board_lock(), occupied_positions() {}

    bool acquire(int x, int y) {
        std::lock_guard<std::mutex> lock(board_lock);
        if (!occupied_positions.count(Coordinate{x, y})) {
            occupied_positions.insert(Coordinate{x, y});
            return true;
        }
        return false;
    }

    void release(int x, int y) {
        std::lock_guard<std::mutex> lock(board_lock);
        occupied_positions.erase(Coordinate{x, y});
    }

private:
    std::mutex board_lock;
    std::unordered_set<Coordinate, Coordinate::Hash> occupied_positions;
};

class Rook {
public:
    Rook(int id, ChessBoard& board) : id(id), chess_board(board), moves(0) {}

    void run() {
        while (moves < 50) {
            int new_x = get_random_position();
            int new_y = get_random_position();
            if (move(new_x, new_y)) {
                moves++;
            }
        }
    }

private:
    int id;
    ChessBoard& chess_board;
    int moves;

    int get_random_position() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(1, 8);
        return dis(gen);
    }

    bool check_collisions(int new_x, int new_y) {
        for (int x = 1; x <= 8; ++x) {
            if (x != new_x) {
                if (chess_board.acquire(x, new_y)) {
                    chess_board.release(x, new_y);
                } else {
                    return true;
                }
            }
        }

        for (int y = 1; y <= 8; ++y) {
            if (y != new_y) {
                if (chess_board.acquire(new_x, y)) {
                    chess_board.release(new_x, y);
                } else {
                    return true;
                }
            }
        }

        return false;
    }

    bool move(int new_x, int new_y) {
        if (check_collisions(new_x, new_y)) {
            std::cout << "Rook-" << id << " is waiting since there is another rook on the path to (" << new_x << ", " << new_y << ")." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));

            if (chess_board.acquire(new_x, new_y)) {
                std::cout << "Rook-" << id << " Now path is free! moves to (" << new_x << ", " << new_y << ")" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200 + rand() % 101));
                chess_board.release(new_x, new_y);
                return true;
            } else {
                std::cout << "Rook-" << id << " changed direction since the path to (" << new_x << ", " << new_y << ") is still occupied." << std::endl;
                return false;
            }
        }

        if (chess_board.acquire(new_x, new_y)) {
            std::cout << "Rook-" << id << " moves to (" << new_x << ", " << new_y << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200 + rand() % 101));
            chess_board.release(new_x, new_y);
            return true;
        } else {
            std::cout << "Rook-" << id << " failed to move to (" << new_x << ", " << new_y << "), choosing another position." << std::endl;
            return false;
        }
    }
};

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> num_rooks_dis(4, 6);
    
    const int num_rooks = num_rooks_dis(gen);

    std::cout << "Total number of rocks is " << num_rooks << std::endl;
    ChessBoard chess_board;
    std::vector<Rook> rooks;

    for (int i = 0; i < num_rooks; ++i) {
        rooks.emplace_back(i + 1, chess_board);
    }

    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_rooks; ++i) {
        futures.emplace_back(std::async(std::launch::async, &Rook::run, &rooks[i]));
    }

    for (auto& future : futures) {
        future.wait();
    }

    std::cout << "All rooks have completed 50 moves." << std::endl;

    return 0;
}




