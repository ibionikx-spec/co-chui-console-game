#pragma once
#include <vector>


enum Player {
    EMPTY = 0,
    PLAYER_A = 1,
    PLAYER_B = 2  // Có thể là Người chơi 2 hoặc Bot
};

// Định nghĩa các trạng thái của Vòng lặp Game 
enum GameState {
    MENU,
    PHASE_1_PLACING,  // Giai đoạn đặt quân (Mỗi bên 9 quân)
    PHASE_2_MOVING,   // Giai đoạn di chuyển quân sang ô kề
    FLYING_MODE,      // Giai đoạn "bay" (khi chỉ còn 3 quân - tùy chọn mở rộng)
    MILL_STRIKE,      // Trạng thái chờ ăn 1 quân của đối thủ
    GAME_OVER
};


// Định nghĩa danh sách kề 
const std::vector<std::vector<int>> ADJACENCY_LIST = {
    {1, 9},        
    {0, 2, 4},     
    {1, 14}, // Hoàn thiện 24 dòng
};

// Định nghĩa 8 dòng để kiểm tra luật ăn quân (Mill)
const int MILL_LINES[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},        
    {21, 22, 23}, {18, 19, 20}, {15, 16, 17}, 
    {0, 9, 21}, {3, 10, 18} //(bổ sung đủ các đường dọc,ngang)
};