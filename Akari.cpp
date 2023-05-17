#include "Akari.h"

const int FILAS = 5;
const int COLUMNAS = 5;

using namespace std;

void print(GameState game){

    uint64_t cells = CellsWLight(game);
    uint64_t one = firstBit;
    char a = 'A';
    cout<< "  ";
    for(int i=0;i<8;i++){
        cout << "[" << a << "]";
        a++;
    }

    cout<<endl;

    for(int i=0;i<64;i++){

        int aux = (i+8)/8;
        if((i+8)%8 == 0) { cout<< aux <<" "; }

        cout << BG_BLACK;
        if(one&game.Cell_On){ cout<<BG_YELLOW; }
        else { cout<<BG_WHITE<<WHITE; }

        

        if(one&cells){ cout<<BG_BLACK<<BLACK; }

        cout << "[";

        if(one&game.Lights){ cout<<BLACK<<"¤"<< RESET << BG_BLACK; }
        if(one&game.Cell_0){ cout<<WHITE<<"0"<< RESET << BG_BLACK; }
        if(one&game.Cell_1){ cout<<WHITE<<"1"<< RESET << BG_BLACK; }
        if(one&game.Cell_2){ cout<<WHITE<<"2"<< RESET << BG_BLACK; }
        if(one&game.Cell_3){ cout<<WHITE<<"3"<< RESET << BG_BLACK; }
        if(one&game.Cell_4){ cout<<WHITE<<"4"<< RESET << BG_BLACK; }
        if(one&game.Cell_AnyBulb){ cout<<BLACK<<"."<< RESET << BG_BLACK; }

        if((one&game.Cell_On)&~game.Lights){ cout<<YELLOW<<"."<< RESET << BG_BLACK; }
        if((one^game.Cell_On)&~cells){cout<<WHITE<<"."<< RESET << BG_BLACK;};

        if(one&game.Cell_On){ cout<<BG_YELLOW; }
        else { cout<<BG_WHITE; }

        if(one&cells){ cout<<BG_BLACK<<BLACK; }

        cout << "]";

        if(i%8==7){ cout<<RESET<<endl; }
        one >>= 1;
    }
    cout<<endl;
}

uint64_t CellsWLight(GameState game){

    uint64_t cells = game.Cell_AnyBulb|game.Cell_0|game.Cell_1|game.Cell_2|game.Cell_3|game.Cell_4;

    return cells;
} 

void CreateBoard(GameState& game){

    srand((unsigned) time(NULL));

    int aux = rand() % 2;

    string auxiliar = differentsBoards[aux];
    int a = auxiliar.length();
    for(int i = 0; i < a; i++)
    {
        if(auxiliar[i] == 'x'){ game.Cell_AnyBulb |= (firstBit>>i); }
        else if(auxiliar[i] == '0'){ game.Cell_0 |= (firstBit>>i); }
        else if(auxiliar[i] == '1'){ game.Cell_1 |= (firstBit>>i); }
        else if(auxiliar[i] == '2'){ game.Cell_2 |= (firstBit>>i); }
        else if(auxiliar[i] == '3'){ game.Cell_3 |= (firstBit>>i); }
        else if(auxiliar[i] == '4'){ game.Cell_4 |= (firstBit>>i); }
    }
}

int main(){
    GameState game;

    cout<<"Tablero Inicial"<<endl;

    CreateBoard(game);

    print(game);

    return 0;

}