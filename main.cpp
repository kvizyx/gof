#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define DEF_W 20
#define DEF_H 10
#define ALIVE '#'
#define DEAD '.'
#define SPEED std::chrono::milliseconds(100)

class GoF {
public:
    GoF(const int height, const int width) {
        this->height = height;
        this->width = width;

        cells = new bool *[height];
        for (int i = 0; i < this->height; i++) {
            cells[i] = new bool[this->width];
        }
    }

    ~GoF() {
        for (int i = 0; i < height; i++) {
            delete[] cells[i];
        }
        delete[] cells;
    }

    void seed() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cells[i][j] = std::rand() % 2 == 0;
            }
        }
    }

    void print_state() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (!cells[i][j]) {
                    std::cout << DEAD;
                } else {
                    std::cout << ALIVE;
                }
            }
            std::cout << "\n";
        }
    }

    void next_state() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cells[i][j] = next(j, i);
            }
        }
    }

private:
    int neighbors(int x, int y) {
        int cnt = 0;

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;

                int nx = x + i;
                int ny = y + j;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    if (cells[ny][nx]) cnt++;
                }
            }
        }

        return cnt;
    }

    bool next(int x, int y) {
        bool alive = cells[y][x];
        int ngb = neighbors(x, y);

        if (alive && (ngb == 2 || ngb == 3)) {
            return true;
        }

        if (!alive && ngb == 3) {
            return true;
        }

        return false;
    }

    int height;
    int width;
    bool **cells;
};

struct args {
    int height = DEF_H;
    int width = DEF_W;
};

args parse_args(int argc, char *argv[]) {
    args arg;

    for (int i = 1; i < argc; i++) {
        std::string row_arg = argv[i];

        if (row_arg == "--h" && i + 1 < argc) {
            int h = std::stoi(argv[i + 1]);
            if (h < 0) {
                continue;
            }
            arg.height = h;
        }

        if (row_arg == "--w" && i + 1 < argc) {
            int w = std::stoi(argv[i + 1]);
            if (w < 0) {
                continue;
            }
            arg.width = w;
        }
    }

    return arg;
}

int main(int argc, char *argv[]) {
    auto arg = parse_args(argc, argv);

    GoF gof(arg.height, arg.width);
    gof.seed();

    while (true) {
        gof.print_state();
        gof.next_state();

        std::this_thread::sleep_for(SPEED);

        std::cout << "\033[2J\033[1;1H";
    }
}