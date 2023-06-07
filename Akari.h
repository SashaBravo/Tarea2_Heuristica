#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <ctime>
#include <stddef.h>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <unordered_set>

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

//Tableros de la pagina /* krazydad.com/play/akari */
vector<string> differentsBoards = {
    "L1x.L12L......L..xx..1x......L........L..x1L.x2.......L..xx..xx.", //  Tablero 1   1-1-1
    ".L..xxL.L4L...2LxLx.L2..x....L....L....x..3L.xLxL3L...3L.Lxx..L.", //  Tablero 2   1-1-2
    "Lxxx1xxL....L....L.0x.L.x1..L.1xL2L...1L...L.........L...L1xx2L.", //  Tablero 3   2-50-16
    "1Lx1x.Lx...L....L..x....x..xL.1LL2L.2L.x...Lx..L....L...1L.x1xL1", //  Tablero 4   2-50-17
    "L.....xL1xx.L.0...L.xL1..L3L........L3L.L2Lx.L...x.L.xx1.x.....L", //  Tablero 5   2-53-17
    ".Lx.Lx.LL.xL2.L.2....L3xLx....L..L....xLx3L....x.L.xL1.LL.1L.xL.", //  Tablero 6   1-53-17
    "..LxLx2LxL2...L.x..L..x.L..1xL.x2.L11..LLx..L..x.L...xL2.1xLx..L", //  Tablero 7   2-67-15
    "LxxxL..x...L3L..0.L..1.L.L.xL...L.xL2.L.1.L.xL3.....L4L.L.0..L.."  //  Tablero 8   2-67-16
    };

struct GameState{
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

    uint64_t robot = 0x0000000400000000; //la casilla 30 es blanca para todos los tableros

    bool operator<(const GameState& other) const {
        return Board < other.Board;
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

void print(GameState game);
void createBoard(GameState& game);

void putLight(GameState& game, uint64_t newLight);
void illumBoard(GameState& game, bool turnOn);

void playGame(GameState game);
void moveRobot(GameState& game, int move);
bool neighbor(GameState game, int move);

bool victoryCondition(GameState game);
bool isLightPos(GameState game);

void bfs(GameState game, uint64_t startLight, uint64_t goalLight);

void aStar(GameState game, uint64_t goalLight);
double heuristic(Point start, uint64_t cellsRequired, uint64_t visitedWaypoints, uint64_t obstacles);
int countBits(uint64_t value);

void idaStar(GameState game);
int Search(unordered_set<GameState>& path, int costo, int limit);

vector<GameState> getAdjacentCellsGS(GameState game);