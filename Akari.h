#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <stddef.h>
#include <limits>
#include <cstdlib>

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

//Tableros de la pagina /* krazydad.com/play/akari */
vector<string> differentsBoards = {
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 1   1-1-1
    "....xx...4....2.x.x..2..x..............x..3..x.x.3....3...xx....", //  Tablero 2   1-1-2
    ".xxx1xx............0x...x1....1x.2....1...................1xx2..", //  Tablero 3   2-50-16
    "1.x1x..x...........x....x..x..1..2..2..x....x...........1..x1x.1", //  Tablero 4   2-50-17
    "......x.1xx...0.....x.1...3..........3...2.x.....x...xx1.x......", //  Tablero 5   2-53-17
    "..x..x....x.2...2.....3x.x............x.x3.....x...x.1....1..x..", //  Tablero 6   1-53-17
    "...x.x2.x.2.....x.....x....1x..x2..11....x.....x.....x.2.1x.x...", //  Tablero 7   2-67-15
    ".xxx...x....3...0....1.....x......x.2...1...x.3......4....0....."  //  Tablero 8   2-67-16
    };

struct GameState{
    uint64_t Board = full;

    uint64_t Lights = 0x0;
    uint64_t Cell_On = 0x0;

    uint64_t Cells_inBlack = 0x0;
    
    uint64_t Cell_AnyBulb = 0x0;
    uint64_t Cell_0 = 0x0;
    uint64_t Cell_1 = 0x0;
    uint64_t Cell_2 = 0x0;
    uint64_t Cell_3 = 0x0;
    uint64_t Cell_4 = 0x0;
};

uint64_t cellsWLight(GameState game);

void print(GameState game, uint64_t board);
void createBoard(GameState& game);

void putLight(GameState& game);
void illumBoard(GameState& game, bool turnOn);

bool victoryCondition(GameState game);