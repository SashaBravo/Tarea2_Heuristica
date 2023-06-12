#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <set>
#include <chrono>
#include <ctime>
#include <stddef.h>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <map>

using namespace std;

/*Colores a Utilizar*/
#define RESET "\033[0m"
#define WHITE "\033[37m"
#define BLACK "\x1B[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BG_BLACK "\033[40m"
#define BG_YELLOW "\033[43m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

#define win 10000

//tablero de 7x7, 64 bits
uint64_t empty=0x0;
uint64_t full=0xffffffffffffffff;

uint64_t firstBit=0x8000000000000000;

uint64_t firstRow=0xff00000000000000;
uint64_t lastRow=0x00000000000000ff;
uint64_t borde=0xff818181818181ff;
uint64_t firstCol=0x8080808080808080;
uint64_t lastCol=0x0101010101010101;

//Temporizadores
chrono::duration<double> BFS_Time;
chrono::duration<double> AStar_Time;
chrono::duration<double> IDAStar_Time;

//Nodos
int NodosBFS = 0;
int NodosAStar = 0;
int NodosIDAStar = 0;
int AStar_path = 0;

//Tableros de la pagina /* krazydad.com/play/akari */
vector<string> differentsBoards = {
    "L1x.L12L......L..xx..1x......L........L..x1L.x2.......L..xx..xx.", //  Tablero 1   1-1-1
    ".L..xxL.L4L...2LxLx.L2..x....L....L....x..3L.xLxL3L...3L.Lxx..L.", //  Tablero 2   1-1-2
    "Lxxx1xxL....L....L.0x.L.x1..L.1xL2L...1L...L.........L...L1xx2L.", //  Tablero 3   2-50-16
    "1Lx1x.Lx...L....L..x....x..xL.1LL2L.2L.x...Lx..L....L...1L.x1xL1", //  Tablero 4   2-50-17
    "L.....xL1xx.L.0...L.xL1..L3L........L3L.L2Lx.L...x.L.xx1.x.....L", //  Tablero 5   2-53-17
    };

struct GameState {
    uint64_t Board = full;
    uint64_t Lights = 0x0;
    uint64_t FinalLights = 0x0;
    uint64_t Cell_On = 0x0;
    uint64_t Cells_inBlack = 0x0;
    uint64_t Cell_AnyBulb = 0x0;
    uint64_t Cell_0 = 0x0;
    uint64_t Cell_1 = 0x0;
    uint64_t Cell_2 = 0x0;
    uint64_t Cell_3 = 0x0;
    uint64_t Cell_4 = 0x0;

    uint64_t robot = 0x0000000400000000;

    GameState* parent = nullptr;
    double g;
    double h;
    double f;

    bool operator<(const GameState& other) const {
        return Board < other.Board;
    }

    bool operator==(const GameState& other) const {
        return (Board == other.Board &&
                Lights == other.Lights &&
                FinalLights == other.FinalLights &&
                Cell_On == other.Cell_On &&
                Cells_inBlack == other.Cells_inBlack &&
                Cell_AnyBulb == other.Cell_AnyBulb &&
                Cell_0 == other.Cell_0 &&
                Cell_1 == other.Cell_1 &&
                Cell_2 == other.Cell_2 &&
                Cell_3 == other.Cell_3 &&
                Cell_4 == other.Cell_4 &&
                robot == other.robot &&
                //parent == other.parent &&
                g == other.g &&
                h == other.h &&
                f == other.f);
    }

    bool operator!=(const GameState& other) const {
        return !(*this == other);
    }
};

struct HashFunction {
    std::size_t operator()(const GameState& state) const {
        std::size_t hash = 0;
        hash_combine(hash, std::hash<uint64_t>{}(state.Board));
        hash_combine(hash, std::hash<uint64_t>{}(state.Lights));
        hash_combine(hash, std::hash<uint64_t>{}(state.FinalLights));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_On));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cells_inBlack));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_AnyBulb));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_0));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_1));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_2));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_3));
        hash_combine(hash, std::hash<uint64_t>{}(state.Cell_4));
        hash_combine(hash, std::hash<uint64_t>{}(state.robot));
        return hash;
    }

    private:
    // Función auxiliar para combinar hashes
    template <typename T>
    void hash_combine(std::size_t& seed, const T& value) const {
        seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
};

struct CompareCost {
    bool operator()(const pair<GameState, double>& a, const pair<GameState, double>& b) {
        return a.second > b.second;
    }
};

void print(GameState& game);
void createBoard(GameState& game, int N_tablero);

void putLight(GameState& game, uint64_t newLight);
void illumBoard(GameState& game, bool turnOn);

void playGame(GameState& game);
void moveRobot(GameState& game, int move);
bool neighbor(GameState& game, int move);

bool victoryCondition(GameState& game);
bool isLightPos(GameState& game);

void bfs(GameState initialState);

void aStar(GameState& game);
double heuristic(Point start, uint64_t cellsRequired, uint64_t visitedWaypoints, uint64_t obstacles);
int countBits(uint64_t value);

bool idaStar(GameState& start, uint64_t goalLight);
double search(GameState state, uint64_t goalLight, double g, double bound, std::vector<GameState>& path);
vector<GameState> getAdjacentCellsGS(GameState& game);