#include "Akari.h"

using namespace std;

void print(GameState& game){

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

void createBoard(GameState& game, int N_tablero){

    cout<<"Tablero Inicial - " << N_tablero +1 << "\n" <<endl;

    string auxiliar = differentsBoards[N_tablero];
    
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

void playGame(GameState& game){
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

bool neighbor(GameState& game, int move){

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

vector<GameState> getAdjacentCellsGS(GameState& game) {

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

void bfs(GameState initialState) {
    queue<GameState> q; // Cola para almacenar los estados a visitar
    unordered_set<GameState, HashFunction> visited; // Conjunto para almacenar los estados visitados
    unordered_map<GameState, GameState, HashFunction> parentMap; // Mapa para almacenar el estado padre de cada estado visitado

    // Agregar el estado inicial a la cola y marcarlo como visitado
    q.push(initialState);
    visited.insert(initialState);
    //initialState.parent = nullptr;
    while (!q.empty()) {
        GameState currentState = q.front();
        
        q.pop();

        NodosBFS++;
        //cout << NodosBFS << " - ";

        if(isLightPos(currentState)){ 
            putLight(currentState, currentState.robot); 
        }
        
        // Verificar si se alcanzó el estado objetivo
        if (currentState.Lights == currentState.FinalLights) {
            vector<GameState> path;
            // while (d > 0) 
            // {
            //     cout << "dd";
            //     path.push_back(currentState);
                
            //     currentState = *(currentState.parent); // no asigna el valor, no encontramos el porque
            //     print (currentState);
            //     d--;
            // }

            // cout << path.size() << endl;
            // for (int i = path.size() - 1; i >= 0; i--) 
            // {                     
            //     print(path[i]);
            // } 
            return;
        }

        // Generar los sucesores del estado actual
        vector<GameState> successors = getAdjacentCellsGS(currentState);

        // Recorrer los sucesores
        for (auto successor : successors) {
            if (visited.find(successor) == visited.end()) {
                
                q.push(successor);
                visited.insert(successor);
                //parentMap[successor] = currentState;
                successor.parent = new GameState(currentState);
            }
        }
    }

    // No se encontró un camino hasta el estado objetivo
    return;
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

            // Restar puntos basados en obstáculos
            if ((obstacles >> (waypoint.y * 8 + waypoint.x)) & 1) {
                distance -= 1.0; // Restar 1 punto por cada obstáculo
            }
        }
    }

    // Calcula el costo adicional basado en el número de waypoints aún no visitados
    double unvisitedWaypointsCost = static_cast<double>(countBits(~visitedWaypoints));

    // Resto de la lógica para el cálculo de la heurística
    double heuristicValue = minDistance + unvisitedWaypointsCost;

    return heuristicValue;
}

void aStar(GameState& start) {
    std::map<GameState, GameState> parents;
    std::priority_queue<std::pair<GameState, double>, std::vector<std::pair<GameState, double>>, CompareCost> open;
    
    std::set<GameState, decltype([](const GameState& a, const GameState& b) {
        return std::tie(a.Lights, a.robot, a.Cell_On) < std::tie(b.Lights, b.robot, b.Cell_On);
    })> closed;

    double initialCost = 0.0;
    auto r = 64 - __builtin_ffsll(start.robot); 
    double initialF = initialCost + heuristic(Point(r % 8, r / 8), start.FinalLights, (start.Lights&start.Cell_On), start.Cells_inBlack);

    open.push(std::make_pair(start, initialF));
    
    while (!open.empty()) {
        GameState currentState = open.top().first;
        open.pop();
        

        if (isLightPos(currentState)) {
            putLight(currentState, currentState.robot);
        }

        if (currentState.Lights == currentState.FinalLights) {
            //constructPath(parents, currentState);
            AStar_path = closed.size();
            //print(currentState);
            return;
        }

        closed.insert(currentState);
        NodosAStar++;
        std::vector<GameState> adjCells = getAdjacentCellsGS(currentState);

        for (auto adjCell : adjCells) {
            double g = initialCost + 1.0; // Peso del movimiento
            double f = g + heuristic(Point(r % 8, r / 8), adjCell.FinalLights, (adjCell.Lights&adjCell.Cell_On), adjCell.Cells_inBlack);;


            if (closed.find(adjCell) == closed.end()) {

                if (parents.find(adjCell) == parents.end() || g < parents[adjCell].g) {
                    parents[adjCell] = currentState;
                    //open.push(std::make_pair(adjCell, f));
                    
                }
                open.push(std::make_pair(adjCell, f));
            }
        }
    }

    std::cout << "Objetivo no alcanzado" << std::endl;
}


bool isLightPos(GameState& game){
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


bool idaStar(GameState& start, uint64_t goalLight) {
    auto r = 64 - __builtin_ffsll(start.robot); 
    double bound = heuristic(Point(r % 8, r / 8), start.FinalLights, (start.Lights&start.Cell_On), start.Cells_inBlack);

    std::vector<GameState> path;
    path.push_back(start);

    while (true) {
        double result = search(start, goalLight, 0.0, bound, path);


        if (result == -1.0) {

            // Solución encontrada
            return true;
        }

        if (result == numeric_limits<double>::infinity()) {
            // No se encontró la solución
            return false;
        }

        // Aumenta el límite para la siguiente iteración
        bound = result;
    }
}

double search(GameState state, uint64_t goalLight, double g, double bound, vector<GameState>& path) {
    auto r = 64 - __builtin_ffsll(state.robot);
    double f = g + heuristic(Point(r % 8, r / 8), state.FinalLights, (state.Lights & state.Cell_On), state.Cells_inBlack);

    if (f > bound) {
        return f; // Supera el límite, se realiza un corte
    }

    if (isLightPos(state)) {
        putLight(state, state.robot);
    }

    if (state.Lights == state.FinalLights) {
        return -1.0; // Se encontró la solución
    }

    double min = std::numeric_limits<double>::infinity();
    vector<GameState> adjCells = getAdjacentCellsGS(state); // Genera los sucesores

    for (auto adjCell : adjCells) {
        bool isDuplicate = false;
        for (auto visitedState : path) {
            if (visitedState.robot == adjCell.robot &&
                visitedState.Lights == adjCell.Lights &&
                visitedState.Cell_On == adjCell.Cell_On) {
                cout << "igual" << endl;
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            path.push_back(adjCell);
            double result = search(adjCell, goalLight, g + 1.0, bound, path);

            if (result == -1.0) {
                return -1.0; // Se encontró la solución en el sucesor
            }

            if (result != numeric_limits<double>::infinity() && result < min) {
                min = result; // Actualiza el nuevo mínimo
            }

            path.pop_back();
        }
    }

    if (min == numeric_limits<double>::infinity()) {
        return numeric_limits<double>::infinity(); // No se encontró la solución
    }

    return min; // Retorna el nuevo mínimo
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

bool victoryCondition(GameState& game){
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

    int option;

    cout << "Juego manual opción 1  //  Juego automatico opción 2: ";
    cin >> option;

    while (option != 1 && option != 2)
    {
        cout << "Ingrese un argumento valido: ";
        cin >> option;
    }
    


    switch (option)
    {
    case 1:
    {
        srand((unsigned) time(NULL));
        int auxRand = differentsBoards.size();
        int aux = rand() % auxRand;
    
        GameState game;
        createBoard(game, aux);
        playGame(game);
        break;
    }
    case 2:
    {
        int N_tablero = 0;
    
        for( auto Tablero : differentsBoards)
        {
            cout <<"\n"<< endl;
            GameState game;
            createBoard(game, N_tablero );

            cout<<"---------------------------------"<<endl;
            cout<<"BFS..."<<endl;
            auto startBFS = std::chrono::high_resolution_clock::now();

            bfs(game);

            auto endBFS = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationBFS = endBFS - startBFS;
            cout<<"Tiempo (segundos): "<<durationBFS.count()<<endl;
            cout<<"Nodos visitados: "<<NodosBFS<<endl;
            cout<<"Camino mas corto: "<<endl;


            cout<<"---------------------------------"<<endl;
            cout<<"A Star..."<<endl;
            auto startAStar = std::chrono::high_resolution_clock::now();

            aStar(game);

            auto endAStar = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> durationAStar = endAStar - startAStar;
            cout<<"Tiempo (segundos): "<<durationAStar.count()<<endl;
            cout<<"Nodos Visitados: "<<NodosAStar<<endl;
            cout<<"Camino mas corto: " << AStar_path <<endl;

            NodosBFS = 0;
            NodosAStar = 0;

            N_tablero++;

        cout <<"\n"<< endl;
        }
        break;
    }
    default:
        cout << "error" << endl;
        break;
    }
    
    // auto startIDAStar = std::chrono::high_resolution_clock::now();
    // cout<<"---------------------------------"<<endl;
    // cout<<"IDA Star..."<<endl;
    // auto endIDAStar = std::chrono::high_resolution_clock::now();

    // int a = idaStar(game);
    // cout << "Valor idea " << a << endl;
    // std::chrono::duration<double> durationIDAStar = endIDAStar - startIDAStar;
    // cout<<"Tiempo (segundos): "<<durationIDAStar.count()<<endl;
    // cout<<"Nodos Visitados: "<<NodosIDAStar<<endl;
    // cout<<"Camino mas corto: "<<endl;

    return 0;

}