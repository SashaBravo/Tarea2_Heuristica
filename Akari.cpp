#include "Akari.h"

const int FILAS = 5;
const int COLUMNAS = 5;

using namespace std;

void print(GameState game){
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
        if(one&game.Cell_On){ cout<<BG_YELLOW<<YELLOW; }
        else { cout<<BG_WHITE<<WHITE; }

        if(one&game.Cells_inBlack){ cout<<BG_BLACK<<BLACK; }

        cout << "[";

        if(one&game.Lights){ cout<<BLACK<<"¤"<< RESET << BG_BLACK; }
        if(one&game.Cell_0){ cout<<WHITE<<"0"<< RESET << BG_BLACK; }
        if(one&game.Cell_1){ cout<<WHITE<<"1"<< RESET << BG_BLACK; }
        if(one&game.Cell_2){ cout<<WHITE<<"2"<< RESET << BG_BLACK; }
        if(one&game.Cell_3){ cout<<WHITE<<"3"<< RESET << BG_BLACK; }
        if(one&game.Cell_4){ cout<<WHITE<<"4"<< RESET << BG_BLACK; }
        if(one&game.Cell_AnyBulb){ cout<<BLACK<<"."<< RESET << BG_BLACK; }

        if((one&(game.Cell_On&~game.Lights))&~game.Cells_inBlack){ cout<<YELLOW<<"1"<< RESET << BG_BLACK; }
        if((one&~(game.Cell_On))&~game.Cells_inBlack){cout<<WHITE<<"2"<< RESET << BG_BLACK; }  

        if(one&game.Cell_On){ cout<<BG_YELLOW<<YELLOW; }
        else { cout<<BG_WHITE; }

        if(one&game.Cells_inBlack){ cout<<BG_BLACK<<BLACK; }

        cout << "]";

        if(i%8==7){ cout<<RESET<<endl; }
        one >>= 1;
    }
    cout<<endl;
}

uint64_t cellsWLight(GameState game){

    uint64_t cells = game.Cell_AnyBulb|game.Cell_0|game.Cell_1|game.Cell_2|game.Cell_3|game.Cell_4;

    return cells;
} 

void createBoard(GameState& game){

    cout<<"Tablero Inicial"<<endl;

    srand((unsigned) time(NULL));

    int auxRand = differentsBoards.size();

    int aux = rand() % auxRand;

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

    game.Cells_inBlack = game.Cell_0|game.Cell_1|game.Cell_2|game.Cell_3|game.Cell_4|game.Cell_AnyBulb;

    print(game);

}

void putLight(GameState& game){

    string pos;
    int row,col;
    while(1){
        cout<<"Ingrese un cordenada (5d) :";
        cin>>pos;
        cout<<endl;

        row = pos[0]-49;
        col = pos[1]-97;

        if((0 <= row && row <= 7) && (0 <= col && col <= 8) && ((firstBit>>((row*8)+col))&~game.Cells_inBlack)){ 

            if((firstBit>>((row*8)+col)) &~ game.Lights){
                game.Lights |= firstBit>>((row*8)+col);
                illumBoard(game, true);
            }
            else{
                game.Lights ^= firstBit>>((row*8)+col);
                illumBoard(game, false);
            }
            
            if(victoryCondition(game)){
                break;
            }
        }
        else{ cout<<"Posicion Invalida"<<endl; }
    }
}

void illumBoard(GameState& game, bool turnOn){
    
    if(turnOn){
        auto one = firstBit;
        for(int i=0;i<64;i++){
            bool up = true;
            bool down = true;
            bool left = true;
            bool right = true;

            if(one&game.Lights){
                for(int j = 0; j < 8; j++){
                    if(((one>>j)&~game.Cells_inBlack) && right){
                        game.Cell_On |= one>>j;
                        if((one>>j)&lastCol) { right = false; }
                    }
                    else{ right = false; }

                    if(((one<<j)&~game.Cells_inBlack) && left){
                        game.Cell_On |= one<<j;
                        if((one<<j)&firstCol) { left = false; }
                    }
                    else{ left = false; }

                    if(((one>>(j*8))&~game.Cells_inBlack) && down){
                        game.Cell_On |= (one>>(j*8));
                        if((one>>(j*8))&lastRow) { down = false; }
                    }
                    else{ down = false; }

                    if(((one<<(j*8))&~game.Cells_inBlack) && up){
                        game.Cell_On |= (one<<(j*8));
                        if((one<<(j*8))&firstRow) { up = false; }
                    }
                    else{ up = false; }
                }
            }
            one >>= 1;
        }
    }
    else{
        auto one = firstBit;
        for(int i=0;i<64;i++){
            if((one&game.Cell_On)&~game.Cells_inBlack){
                bool up = true;
                bool down = true;
                bool left = true;
                bool right = true;
                bool turnOff = true;

                for(int j = 0; j < 8; j++){

                    if(((one>>j)&~game.Cells_inBlack) && right){
                        if((one>>j)&game.Lights) { turnOff = false; }
                        if((one>>j)&lastCol) { right = false; }
                    }
                    else{ right = false; }

                    if(((one<<j)&~game.Cells_inBlack) && left){
                        if((one<<j)&game.Lights) { turnOff = false; }
                        if((one<<j)&firstCol) { left = false; }
                    }
                    else{ left = false; }

                    if(((one>>(j*8))&~game.Cells_inBlack) && down){
                        if((one>>(j*8))&game.Lights) { turnOff = false; }
                        if((one>>(j*8))&lastCol) { down = false; }
                    }
                    else{ down = false; }

                    if(((one<<(j*8))&~game.Cells_inBlack) && up){
                        if((one<<(j*8))&game.Lights) { turnOff = false; }
                        if((one<<(j*8))&lastCol) { up = false; }
                    }
                    else{ up = false; }
                }
                if(turnOff){ game.Cell_On^= one; }
            }
            one >>= 1;
        }
    }

    print(game);
}

bool victoryCondition(GameState game){
    auto one = firstBit;

    if((game.Cells_inBlack | game.Cell_On) == game.Board)
    {
        for(int i=0;i<64;i++){

            if(one&game.Cell_0){
                if((one>>1)&game.Lights){ return false; }
                if((one<<1)&game.Lights){ return false; }
                if((one>>8)&game.Lights){ return false; }
                if((one<<8)&game.Lights){ return false; }

                cout<<"casilla de 0 ok"<<endl;
            }
            if(one&game.Cell_1){
                int count = 0;
                if((one>>1)&game.Lights){ count++; }
                if((one<<1)&game.Lights){ count++; }
                if((one>>8)&game.Lights){ count++; }
                if((one<<8)&game.Lights){ count++; }

                if(count>1){ return false; }
                else if(count<1){ return false; }
                else{ cout<<"casilla de 1 ok"<<endl; }
            }
            if(one&game.Cell_2){
                int count = 0;
                if((one>>1)&game.Lights){ count++; }
                if((one<<1)&game.Lights){ count++; }
                if((one>>8)&game.Lights){ count++; }
                if((one<<8)&game.Lights){ count++; }

                if(count>2){ return false; }
                else if(count<2){ return false; }
                else{ cout<<"casilla de 2 ok"<<endl; }
            }
            if(one&game.Cell_3){
                int count = 0;
                if((one>>1)&game.Lights){ count++; }
                if((one<<1)&game.Lights){ count++; }
                if((one>>8)&game.Lights){ count++; }
                if((one<<8)&game.Lights){ count++; }

                if(count>3){ return false; }
                else if(count<3){ return false; }
                else{ cout<<"casilla de 3 ok"<<endl; }
            }
            if(one&game.Cell_4){
                int count = 0;
                if((one>>1)&game.Lights){ count++; }
                if((one<<1)&game.Lights){ count++; }
                if((one>>8)&game.Lights){ count++; }
                if((one<<8)&game.Lights){ count++; }

                if(count>4){ return false; }
                else if(count<4){ return false; }
                else if(count==4){ cout<<"casilla de 4 ok"<<endl; }
            }
            one >>= 1;
        }
        return true;
    }

    else{ return false;}
}

int main(){
    GameState game;

    createBoard(game);
    putLight(game);

    return 0;

}