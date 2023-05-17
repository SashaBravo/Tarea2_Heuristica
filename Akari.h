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

vector<string> differentsBoards = {
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 1   Listo
    "....xx...4....2.x.x..2..x..............x..3..x.x.3....3...xx....", //  Tablero 2   Listo
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 3   Cambiar
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 4   Cambiar
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 5   Cambiar
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 6   Cambiar
    ".1x..12..........xx..1x..................x1..x2..........xx..xx.", //  Tablero 7   Cambiar
    ".1x..12..........xx..1x..................x1..x2..........xx..xx."  //  Tablero 8   Cambiar
    };

struct GameState{
    uint64_t Board = 0x0;

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

uint64_t CellsWLight(GameState game);

void print(GameState game, uint64_t board);
void CreateBoard(GameState& game);