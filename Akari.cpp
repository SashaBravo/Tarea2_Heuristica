#include "Akari.h"

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

vector<GameState> getAdjacentCellsGS(GameState game) {

    vector<GameState> adjCells;
    
    if(((game.robot &~ firstRow) << 8 ) &~ game.Cells_inBlack){
        auto aux = game;
        aux.robot = aux.robot << 8 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ lastRow) >> 8 ) &~ game.Cells_inBlack){
        auto aux = game;
        aux.robot = aux.robot >> 8 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ firstCol) << 1 ) &~ game.Cells_inBlack){
        auto aux = game;
        aux.robot = aux.robot << 1 ;
        adjCells.push_back(aux);
    }

    if(((game.robot &~ lastCol) >> 1 ) &~ game.Cells_inBlack){
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
    set<pair<uint64_t, uint64_t>> visited2;
    queue<GameState> qGS;

    visited2.insert({startLight, game.robot});
    qGS.push(game);

    while (!qGS.empty()) {
        GameState actualState1 = qGS.front();
        
        qGS.pop();
        // Realizar acciones con el nodo actual
        if(isLightPos(actualState1)){ 
            putLight(actualState1, actualState1.robot); 
        }
        
        // Obtener las celdas adyacentes al robot
        vector<GameState> adjCellsGS = getAdjacentCellsGS(actualState1);

        NodosBFS++;
        if(actualState1.Lights == goalLight)
        {
            //print(actualState1);
            break;
        }
        
        for (auto adjCell : adjCellsGS) {

            if (visited2.find({adjCell.Lights, adjCell.robot}) == visited2.end()) {
                GameState newState = actualState1;
                newState.robot = adjCell.robot;

                // Realizar modificaciones en el nuevo estado
                if(victoryCondition(newState)){ break; }

                qGS.push(newState);
                visited2.insert({newState.Lights, newState.robot});
            }
        }
     }
}

int countBits(uint64_t value) {
    int count = 0;
    while (value != 0) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

double heuristic(Point start, uint64_t cellsRequired, uint64_t visitedWaypoints, uint64_t obstacles) {
    // Calcula la distancia euclidiana entre el punto de inicio y el punto de destino más cercano
    double minDistance = std::numeric_limits<double>::max();

    vector<Point> waypoints;
    
    while(cellsRequired)
    {
        auto i=64-__builtin_ffsll(cellsRequired);
        uint64_t v = firstBit>>i;

        waypoints.push_back(Point(i%8, i/8));

        cellsRequired^=v;
    }
    
    for (auto waypoint : waypoints) {
        if (!((visitedWaypoints >> (waypoint.y * 8 + waypoint.x)) & 1)) {
            // El waypoint aún no ha sido visitado
            int dx = start.x - waypoint.x;
            int dy = start.y - waypoint.y;
            double distance = std::sqrt(dx * dx + dy * dy);
            if (distance < minDistance) {
                minDistance = distance;
            }
        }
    }

    // Calcula el costo adicional basado en el número de waypoints aún no visitados
    double unvisitedWaypointsCost = static_cast<double>(countBits(~visitedWaypoints));

    // Resto de la lógica para el cálculo de la heurística
    double heuristicValue = minDistance + unvisitedWaypointsCost;

    return heuristicValue;
}

void aStar(GameState game, uint64_t goalLight) {
    set<GameState> visited;
    set<pair<uint64_t, uint64_t>> visited2;
    priority_queue<pair<GameState, double>, vector<pair<GameState, double>>, CompareCost> open;

    double initialCost = 0.0;

    auto r=64-__builtin_ffsll(game.robot);
    double initialF = initialCost + heuristic(Point(r % 8, r / 8), game.FinalLights, (game.Lights&game.Cell_On), game.Cells_inBlack);

    open.push(make_pair(game, initialF));

    while (!open.empty()) {
        GameState ActualState = open.top().first;
        open.pop();
        
        if(isLightPos(ActualState)){ 
            putLight(ActualState, ActualState.robot); 
        }

        NodosAStar++;
        if (ActualState.Lights == goalLight) {
            //print(ActualState);
            return;
        }
        
        visited2.insert({ActualState.Lights, ActualState.robot});

        vector<GameState> adjCellsGS = getAdjacentCellsGS(ActualState);

        for (auto adjCell : adjCellsGS) {

            if (visited2.find({adjCell.Lights, adjCell.robot}) == visited2.end()) {
                double g = initialCost + 1.0; // Peso del movimiento
                auto r1 =64-__builtin_ffsll(adjCell.robot);
                double f = g + heuristic(Point(r1 % 8, r1 / 8), adjCell.FinalLights, (adjCell.Lights&adjCell.Cell_On), adjCell.Cells_inBlack);


                open.push(make_pair(adjCell, f));
            }
        }
    }

    cout << "Objetivo no alcanzado" << endl;
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

    //print(game);
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
        print(game);
        return true;
    }

    else{ return false; }
}

int main(){
    GameState game;
    createBoard(game);

    cout<<"---------------------------------"<<endl;
    cout<<"BFS..."<<endl;
    auto startBFS = std::chrono::high_resolution_clock::now();

    bfs(game, game.Lights, game.FinalLights);

    auto endBFS = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationBFS = endBFS - startBFS;
    cout<<"Tiempo (segundos): "<<durationBFS.count()<<endl;
    cout<<"Nodos visitados: "<<NodosBFS<<endl;
    cout<<"Camino mas corto: "<<endl;


    cout<<"---------------------------------"<<endl;
    cout<<"A Star..."<<endl;
    auto startAStar = std::chrono::high_resolution_clock::now();

    aStar(game, game.FinalLights);

    auto endAStar = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationAStar = endAStar - startAStar;
    cout<<"Tiempo (segundos): "<<durationAStar.count()<<endl;
    cout<<"Nodos Visitados: "<<NodosAStar<<endl;
    cout<<"Camino mas corto: "<<endl;


    // auto startIDAStar = std::chrono::high_resolution_clock::now();
    // cout<<"---------------------------------"<<endl;
    // cout<<"IDA Star..."<<endl;
    // auto endIDAStar = std::chrono::high_resolution_clock::now();

    // //Aqui va el IDAStar 

    // std::chrono::duration<double> durationIDAStar = endIDAStar - startIDAStar;
    // cout<<"Tiempo (segundos): "<<durationIDAStar.count()<<endl;
    // cout<<"Nodos Visitados: "<<NodosIDAStar<<endl;
    // cout<<"Camino mas corto: "<<endl;

    return 0;

}