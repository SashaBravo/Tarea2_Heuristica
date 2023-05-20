#include "Akari.h"

const int FILAS = 5;
const int COLUMNAS = 5;

using namespace std;

void print(GameState game){

    //system("clear");

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

        if(one&game.Lights&~game.robot){cout<<BLACK<<"¤"<< RESET << BG_BLACK; }
        if(one&game.robot){ cout<<RED<<"■"<< RESET << BG_BLACK; }
        if(one&game.Cell_0){ cout<<WHITE<<"0"<< RESET << BG_BLACK; }
        if(one&game.Cell_1){ cout<<WHITE<<"1"<< RESET << BG_BLACK; }
        if(one&game.Cell_2){ cout<<WHITE<<"2"<< RESET << BG_BLACK; }
        if(one&game.Cell_3){ cout<<WHITE<<"3"<< RESET << BG_BLACK; }
        if(one&game.Cell_4){ cout<<WHITE<<"4"<< RESET << BG_BLACK; }
        if(one&game.Cell_AnyBulb){ cout<<BLACK<<"."<< RESET << BG_BLACK; }

        if((one&(game.Cell_On&~game.Lights))&~game.Cells_inBlack&~game.robot){ cout<<YELLOW<<"."<< RESET << BG_BLACK; }
        if((one&~(game.Cell_On))&~game.Cells_inBlack&~game.robot){cout<<WHITE<<"."<< RESET << BG_BLACK; }  

        if(one&game.Cell_On){ cout<<BG_YELLOW<<YELLOW; }
        else { cout<<BG_WHITE; }

        if(one&game.Cells_inBlack){ cout<<BG_BLACK<<BLACK; }

        cout << "]";

        if(i%8==7){ cout<<RESET<<endl; }
        one >>= 1;
    }
    cout<<endl;
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
        else if(auxiliar[i] == 'L'){ game.FinalLights |= (firstBit>>i); }
    }

    game.Cells_inBlack = game.Cell_0|game.Cell_1|game.Cell_2|game.Cell_3|game.Cell_4|game.Cell_AnyBulb;

    print(game);
}

void playGame(GameState game){
    int opcion;
    while(1){
        
        bool up = false;
        bool down = false;
        bool right = false;
        bool left = false;
        while(1)
        {
            cout<<"Mover hacia"<<endl;
            if(neighbor(game, 1)){ cout<<"1.Arriba"<<endl;      up = true; }
            if(neighbor(game, 2)){ cout<<"2.Abajo"<<endl;       down = true; }
            if(neighbor(game, 3)){ cout<<"3.Izquierda"<<endl;   left = true; }
            if(neighbor(game, 4)){ cout<<"4.Derecha"<<endl;     right = true; }
            
            cout<<"5.Colocar/Quitar Luz"<<endl;
            cout<<"Opcion Escogida: ";
            cin>>opcion;

            if((opcion==1 && up) || (opcion==2 && down) || (opcion==3 && left) || (opcion==4 && right) || opcion==5){ break; }
            else{ cout<<"Invalido"<<endl; }
        }
        moveRobot(game, opcion);

        if(victoryCondition(game)){ break; }
        
    }
}

bool neighbor(GameState game, int move){

    switch (move)
    {
    case 1:
        if(((game.robot &~ firstRow) << 8 ) &~ game.Cells_inBlack){ return true; }
        break;
    case 2:
        if(((game.robot &~ lastRow) >> 8 ) &~ game.Cells_inBlack){ return true; }
        break;
    case 3:
        if(((game.robot &~ firstCol) << 1 ) &~ game.Cells_inBlack){ return true; }
        break;
    case 4:
        if(((game.robot &~ lastCol) >> 1 ) &~ game.Cells_inBlack){ return true; }
        break;
    
    default:
        return false;
        break;
    }

    return false;
}

vector<uint64_t> getAdjacentCells(GameState game) {

    vector<uint64_t> adjCells;
    
    if(((game.robot &~ firstRow) << 8 ) &~ game.Cells_inBlack){ adjCells.push_back(game.robot << 8); }
    if(((game.robot &~ lastRow) >> 8 ) &~ game.Cells_inBlack){ adjCells.push_back(game.robot >> 8); }
    if(((game.robot &~ firstCol) << 1 ) &~ game.Cells_inBlack){ adjCells.push_back(game.robot << 1); }
    if(((game.robot &~ lastCol) >> 1 ) &~ game.Cells_inBlack){ adjCells.push_back(game.robot >> 1); }

    return adjCells;
}

vector<GameState> getAdjacentCellsGS(GameState game) {

    vector<GameState> adjCells;
    
    if(((game.robot &~ firstRow) << 8 ) &~ game.Cells_inBlack){
        cout<<"puede arriba"<<endl;
        auto aux = game;
        aux.robot = aux.robot << 8 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ lastRow) >> 8 ) &~ game.Cells_inBlack){
        cout<<"puede abajo"<<endl;
        auto aux = game;
        aux.robot = aux.robot >> 8 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ firstCol) << 1 ) &~ game.Cells_inBlack){
        cout<<"puede izq"<<endl;
        auto aux = game;
        aux.robot = aux.robot << 1 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ lastCol) >> 1 ) &~ game.Cells_inBlack){
        cout<<"puede der"<<endl;
        auto aux = game;
        aux.robot = aux.robot >> 1 ;
        adjCells.push_back(aux);
    }

    return adjCells;
}

void moveRobot(GameState& game, int move){
    
    switch (move)
    {
    case 1:
        game.robot = game.robot<<8;
        break;

    case 2:
        game.robot = game.robot>>8;
        break;
    
    case 3:
        game.robot = game.robot<<1;
        break;
    
    case 4:
        game.robot = game.robot>>1;
        break;
    
    case 5:
        putLight(game, game.robot);
        break;
    
    default:
        break;
    }

    print(game);

}

void bfs(GameState game, uint64_t startLight, uint64_t goalLight) {
    set<uint64_t> visited;
    set<GameState> visitedStates;
    queue<uint64_t> q;
    queue<GameState> qGS;

    q.push(startLight);
    visited.insert(startLight);
    qGS.push(game);
    visitedStates.insert(game);
    

    while (!qGS.empty()) {
        GameState actualState = qGS.front();
        qGS.pop();
        // Realizar acciones con el nodo actual
        if(isLightPos(actualState)){ 
            putLight(actualState, actualState.robot); 
        }
        
        
        // Obtener las celdas adyacentes al robot
        vector<GameState> adjCellsGS = getAdjacentCellsGS(actualState);
        
        print(actualState);
        
        if(actualState.Lights == goalLight)
        {
            cout<<"termine waxin"<<endl;
            break;
        }
        
        for (auto adjCell : adjCellsGS) {
            if (visitedStates.find(adjCell) == visitedStates.end()) {
                GameState newState = actualState;
                newState.robot = adjCell.robot;
                // Realizar modificaciones en el nuevo estado

                if(victoryCondition(newState)){ break; }

                qGS.push(newState);
                visitedStates.insert(adjCell);
            }
        }

        // vector<uint64_t> adjCells = getAdjacentCells(actualState);
        // for (auto adjCell : adjCells) {
        //     if (visited.find(adjCell) == visited.end()) {
        //         GameState newState = actualState;
        //         newState.robot = adjCell;
        //         // Realizar modificaciones en el nuevo estado

        //         //if(victoryCondition(newState)){ break; }

        //         q.push(newState.Lights);
        //         visited.insert(adjCell);
        //         actualState = newState;
        //     }
        // }
     }
}

bool isLightPos(GameState game){
    if(game.robot & game.FinalLights){ return true; }
    return false;
}

void putLight(GameState& game, uint64_t newLight){

    if(newLight &~ game.Lights){
        game.Lights |= newLight;
        illumBoard(game, true);
    }
    else{
        game.Lights ^= newLight;
        illumBoard(game, false);
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
                        if((one>>(j*8))&lastRow) { down = false; }
                    }
                    else{ down = false; }

                    if(((one<<(j*8))&~game.Cells_inBlack) && up){
                        if((one<<(j*8))&game.Lights) { turnOff = false; }
                        if((one<<(j*8))&firstRow) { up = false; }
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
                if(((one&~lastCol)>>1)&game.Lights){ return false; }
                if(((one&~firstCol)<<1)&game.Lights){ return false; }
                if(((one&~lastRow)>>8)&game.Lights){ return false; }
                if(((one&~firstRow)<<8)&game.Lights){ return false; }

            }
            if(one&game.Cell_1){
                int count = 0;
                if(((one&~lastCol)>>1)&game.Lights){ count++; }
                if(((one&~firstCol)<<1)&game.Lights){ count++; }
                if(((one&~lastRow)>>8)&game.Lights){ count++; }
                if(((one&~firstRow)<<8)&game.Lights){ count++; }

                if(count!=1){ return false; }
            }
            if(one&game.Cell_2){
                int count = 0;
                if(((one&~lastCol)>>1)&game.Lights){ count++; }
                if(((one&~firstCol)<<1)&game.Lights){ count++; }
                if(((one&~lastRow)>>8)&game.Lights){ count++; }
                if(((one&~firstRow)<<8)&game.Lights){ count++; }

                if(count!=2){ return false; }
            }
            if(one&game.Cell_3){
                int count = 0;
                if(((one&~lastCol)>>1)&game.Lights){ count++; }
                if(((one&~firstCol)<<1)&game.Lights){ count++; }
                if(((one&~lastRow)>>8)&game.Lights){ count++; }
                if(((one&~firstRow)<<8)&game.Lights){ count++; }

                if(count!=3){ return false; }
            }
            if(one&game.Cell_4){
                int count = 0;
                if(((one&~lastCol)>>1)&game.Lights){ count++; }
                if(((one&~firstCol)<<1)&game.Lights){ count++; }
                if(((one&~lastRow)>>8)&game.Lights){ count++; }
                if(((one&~firstRow)<<8)&game.Lights){ count++; }

                if(count!=4){ return false; }
            }

            bool up = true;
            bool down = true;
            bool left = true;
            bool right = true;
            bool haveLight = false;

            if(one&game.Lights){
                for(int j = 1; j < 8; j++){
                    if(((one>>j)&~game.Cells_inBlack) && ((one>>j)&~firstCol) && (one&~lastCol) && right){
                        if((one>>j)&game.Lights) { haveLight = true; }
                    }
                    else{ right = false; }

                    if(((one<<j)&~game.Cells_inBlack) && ((one<<j)&~lastCol) && (one&~firstCol) && left){
                        if((one<<j)&game.Lights) { haveLight = true; }
                    }
                    else{ left = false; }

                    if(((one>>(j*8))&~game.Cells_inBlack) && ((one>>(j*8))&~firstRow) && (one&~lastRow) && down){
                        if((one>>(j*8))&game.Lights) { haveLight = true; }
                    }
                    else{ down = false; }

                    if(((one<<(j*8))&~game.Cells_inBlack) && ((one<<(j*8))&~lastRow) && (one&firstRow) && up){
                        if((one<<(j*8))&game.Lights) { haveLight = true; }
                    }
                    else{ up = false; }
                }
                if(haveLight){ return false; }
            }
            one >>= 1;
        }
        cout<<"Ganaste"<<endl<<endl;
        return true;
    }

    else{ return false; }
}

int main(){
    GameState game;

    createBoard(game);

    //playGame(game);

    bfs(game, game.Lights, game.FinalLights);

    //putLight(game, 0x0);

    return 0;

}