#include <iostream>
#include <vector>
#include <cstdlib>  // 랜덤 함수 사용을 위해 필요
#include <ctime>    // 시간 기반 랜덤 시드 설정

using namespace std;

// 방의 방향을 나타내는 비트 플래그
enum Direction {
    UP = 1,    // U
    RIGHT = 2,  // R
    DOWN = 4, // D
    LEFT = 8   // L
};

// 방 구조체
struct Room {
    int identity;  // 방의 정체성 (비트 플래그 합산 값)
    int x, y;  // 방의 위치
    bool isStartRoom;  // 시작 방 여부
    bool isEndRoom;  // 마지막 방 여부
    bool isEdge;
    // 생성자

    Room(int id = 0) : identity(id), isEndRoom(false) {}
    Room(int id, bool isEnd) : identity(id), isEndRoom(isEnd) {}

};

vector <Room*> rooms;

// 발 뻗기 최대 길이
int maxDepth = 7;

// 월드의 한 변의 길이 = 최대 깊이 * 2 + 1
int worldSize = (maxDepth * 2) +1;

// 월드 맵을 정사각형 그리드로 초기화 (기본 정체성 0)
std::vector<std::vector<Room*>> world(worldSize, std::vector<Room*>(worldSize));

// 랜덤 방향 선택
Direction getRandomDirection(Direction currentDir) {
    float randValue = static_cast<float>(rand()) / RAND_MAX;
    if (randValue < 0.5) {
        return currentDir;  // 기존 진행 방향을 선택할 확률 0.4
    } else if (randValue < 0.75) {
        return static_cast<Direction>((currentDir*2) % 15);  // 나머지 방향 0.3 확률
    } else {
        return static_cast<Direction>((currentDir*8) % 15);  // 나머지 방향 0.3 확률
    }
}

// 방 생성 재귀 함수
void createRooms(Room* currentRoom, int depth, Direction currentDir) {
    cout << "currentRoom: " << currentRoom->x << " " << currentRoom->y << " " << currentDir << endl;
    cout << endl;
    if (depth == 0) {
        currentRoom->identity = (currentDir*4)%15;  // 마지막 방일 경우 역 진행코드 설정
        currentRoom->isEndRoom = true;
        currentRoom->isEdge = true;
        return;
    }
    if (depth != 7 && currentRoom->isStartRoom){
        return;
    }

    // 새로운 방향 선택
    Direction newDir = getRandomDirection(currentDir);
    
    // 새로운 방 생성


    Room* nextRoom = new Room();
    if(currentDir == UP) {
        if(world[currentRoom->x][currentRoom->y - 1]->identity == 15){return;}
        nextRoom->x = currentRoom->x;
        nextRoom->y = currentRoom->y - 1;
    } else if(currentDir == DOWN) {
        if(world[currentRoom->x][currentRoom->y + 1]->identity == 15){return;}
        nextRoom->x = currentRoom->x;
        nextRoom->y = currentRoom->y + 1;
    } else if(currentDir == RIGHT) {
        if(world[currentRoom->x + 1][currentRoom->y]->identity == 15){return;}
        nextRoom->x = currentRoom->x + 1;
        nextRoom->y = currentRoom->y;
    } else if(currentDir == LEFT) {
        if(world[currentRoom->x - 1][currentRoom->y]->identity == 15){return;}
        nextRoom->x = currentRoom->x - 1;
        nextRoom->y = currentRoom->y;
    }
    
    //새로운 방 ID
    nextRoom->identity = (currentDir*4)%15 + newDir;
    nextRoom->isStartRoom = false;


    rooms.push_back(nextRoom);

    // 다음 방 재귀 호출
    if(depth == 1) {
        newDir = currentDir;
    }
    createRooms(nextRoom, depth - 1, newDir);
}

// 월드 맵 그리드 입력
void getMapIntoWorld(std::vector<std::vector<Room*>>& world, const std::vector<Room*>& rooms) {
    cout << "getMapIntoWorld\n\n\n" << endl;

    for (int i = 0; i < rooms.size(); i++) {
        cout << "room condition : " << rooms[i]->x << " " << rooms[i]->y << endl;

        Room* room = rooms[i];
        int newIdentity = room->identity | world[rooms[i]->x][rooms[i]->y]->identity;
        world[rooms[i]->x][rooms[i]->y] = room;
        world[rooms[i]->x][rooms[i]->y]->identity = newIdentity;
    }
}


// 월드 맵 그리드 출력
void printWorldMap(const std::vector<std::vector<Room*>>& world) {
    cout << "printWorldMap" << endl;

    for (int i = 0; i < world.size(); ++i) {
        for (int j = 0; j < world[i].size(); ++j) {
            if(world[j][i]->identity == 0){
                std::cout << " " << "\t";  // 방이 없는 경우 공백 출력
            }else{
                if(world[j][i]->isEdge){
                    
                }
                std::cout << world[j][i]->identity << "\t";  // 각 방의 정체성 출력
            }
        }
        std::cout << std::endl;  // 한 줄 출력 후 줄바꿈
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // 랜덤 시드 설정


    // 모든 방을 초기화
    for (int i = 0; i < worldSize; ++i) {
        for (int j = 0; j < worldSize; ++j) {
            world[i][j] = new Room(0);  // 기본적으로 방의 정체성을 0으로 설정
        }
    }

    // 시작 방을 월드의 중앙에 설정
    int startX = worldSize / 2 ;
    int startY = worldSize / 2 ;
    Room* startRoom = new Room(99, false);// 중앙 방을 시작 방으로 설정
    startRoom->isStartRoom = true;
    startRoom->x = startX;
    startRoom->y = startY;
    world[startX][startY] = startRoom;
    rooms.push_back(startRoom);

    // 방 확장 실행 (시작 방에서 발 뻗기 시작)
    createRooms(startRoom, maxDepth, UP);  // UP 방향으로 발 뻗기 시작
    createRooms(startRoom, maxDepth, DOWN);  // DOWN 방향으로 발 뻗기 시작
    createRooms(startRoom, maxDepth, RIGHT);  // RIGHT 방향으로 발 뻗기 시작
    createRooms(startRoom, maxDepth, LEFT);  // LEFT 방향으로 발 뻗기 시작
    
    
    getMapIntoWorld(world, rooms);  // 방 정보를 월드에 입력

    // 월드 출력
     
    printWorldMap(world);

    return 0;
}
