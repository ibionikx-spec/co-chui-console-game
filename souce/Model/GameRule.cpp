//  GameRules.cpp  –  Game Logic cho Cờ Chùi (Nine Men's Morris)
//  Game Logic Specialist
//  Branch: feature/model

#include "GameRules.h"
#include <algorithm>   

//  1. KHỞI TẠO DỮ LIỆU TĨNH

// ----------------------------------------------------------
//  Danh sách cạnh kề (Adjacency List)
//  Mỗi hàng là một cặp {a, b} cho biết ô a và ô b liền kề nhau.
//
//  Sơ đồ bàn cờ (Nine Men's Morris chuẩn – 24 ô):
//
//   0 ─────────── 1 ─────────── 2
//   │             │             │
//   │   3 ─────── 4 ─────── 5   │
//   │   │         │         │   │
//   │   │   6 ─── 7 ─── 8   │   │
//   9 ─10 ──11         12 ──13 ─14
//   │   │   15 ──16 ──17    │   │
//   │   │         │         │   │
//   │   18 ───── 19 ───── 20    │
//   │             │             │
//   21 ────────  22 ─────────── 23
//
// ----------------------------------------------------------
const int GameRules::ADJACENCY[][2] = {
    // Vòng ngoài (vòng số 1) – ngang
    {0, 1}, {1, 2},
    {3, 4}, {4, 5},
    {6, 7}, {7, 8},
    {15,16},{16,17},
    {18,19},{19,20},
    {21,22},{22,23},
    // Vòng ngoài – dọc
    {0, 9}, {9,21},
    {3,10},{10,18},
    {6,11},{11,15},
    {8,12},{12,17},
    {5,13},{13,20},
    {2,14},{14,23},
    // Nối giữa các vòng – cạnh giữa bàn
    {1, 4}, {4, 7},
    {9,10},{10,11},
    {12,13},{13,14},
    {16,19},{19,22}
};
const int GameRules::ADJACENCY_COUNT =
    sizeof(GameRules::ADJACENCY) / sizeof(GameRules::ADJACENCY[0]);

// ----------------------------------------------------------
//  Tất cả 16 bộ ba tạo Mill (hàng 3 thẳng hàng)
// ----------------------------------------------------------
const int GameRules::MILLS[][3] = {
    // Hàng ngang – vòng ngoài
    { 0,  1,  2},
    { 3,  4,  5},
    { 6,  7,  8},
    {15, 16, 17},
    {18, 19, 20},
    {21, 22, 23},
    // Hàng dọc – vòng ngoài
    { 0,  9, 21},
    { 3, 10, 18},
    { 6, 11, 15},
    { 8, 12, 17},
    { 5, 13, 20},
    { 2, 14, 23},
    // Hàng ngang – vòng giữa (nối các vòng)
    { 1,  4,  7},
    {16, 19, 22},
    // Hàng dọc – vòng giữa
    { 9, 10, 11},
    {12, 13, 14}
};
const int GameRules::MILLS_COUNT =
    sizeof(GameRules::MILLS) / sizeof(GameRules::MILLS[0]);


//  2. HÀM NỘI BỘ (PRIVATE HELPERS)

// Trả về true nếu pos1 và pos2 có cạnh nối trực tiếp
bool GameRules::areAdjacent(int pos1, int pos2) {
    for (int i = 0; i < ADJACENCY_COUNT; ++i) {
        if ((ADJACENCY[i][0] == pos1 && ADJACENCY[i][1] == pos2) ||
            (ADJACENCY[i][0] == pos2 && ADJACENCY[i][1] == pos1)) {
            return true;
        }
    }
    return false;
}

// Trả về true nếu bộ ba {a,b,c} đều là quân của player
bool GameRules::isMillTriple(const int board[BOARD_SIZE],
                              int a, int b, int c, int player) {
    return (board[a] == player &&
            board[b] == player &&
            board[c] == player);
}

//  3. PHASE 1 – KIỂM TRA ĐẶT QUÂN

MoveResult GameRules::isValidPlacement(const int board[BOARD_SIZE], int pos) {
    // Kiểm tra chỉ số nằm trong phạm vi hợp lệ
    if (pos < 0 || pos >= BOARD_SIZE)
        return MoveResult::INVALID_OUT_OF_RANGE;

    // Ô phải trống mới được đặt
    if (board[pos] != EMPTY)
        return MoveResult::INVALID_OCCUPIED;

    return MoveResult::VALID;
}

//  4. PHASE 2 – KIỂM TRA DI CHUYỂN QUÂN

MoveResult GameRules::isValidMove(const int board[BOARD_SIZE],
                                   int from, int to, int player) {
    // Kiểm tra phạm vi cả hai chỉ số
    if (from < 0 || from >= BOARD_SIZE ||
        to   < 0 || to   >= BOARD_SIZE)
        return MoveResult::INVALID_OUT_OF_RANGE;

    // Ô nguồn phải có quân
    if (board[from] == EMPTY)
        return MoveResult::INVALID_NO_PIECE;

    // Quân tại ô nguồn phải là của player đang đi
    if (board[from] != player)
        return MoveResult::INVALID_WRONG_PLAYER;

    // Ô đích phải trống
    if (board[to] != EMPTY)
        return MoveResult::INVALID_OCCUPIED;

    // Ô đích phải liền kề ô nguồn
    if (!areAdjacent(from, to))
        return MoveResult::INVALID_NOT_ADJACENT;

    return MoveResult::VALID;
}

//  5. KIỂM TRA MILL (HÀNG 3)

bool GameRules::checkMill(const int board[BOARD_SIZE], int pos, int player) {
    // Duyệt tất cả 16 bộ ba Mill, xem pos có thuộc bộ nào không
    for (int i = 0; i < MILLS_COUNT; ++i) {
        int a = MILLS[i][0];
        int b = MILLS[i][1];
        int c = MILLS[i][2];

        // Bộ ba này có chứa pos không?
        if (pos == a || pos == b || pos == c) {
            // Cả 3 ô đều là quân của player → Mill!
            if (isMillTriple(board, a, b, c, player))
                return true;
        }
    }
    return false;
}

//  6. KIỂM TRA ĂN QUÂN (REMOVE PIECE)

bool GameRules::canRemovePiece(const int board[BOARD_SIZE],
                                int pos, int opponent) {
    // Ô pos phải có quân của đối thủ
    if (board[pos] != opponent)
        return false;

    // Nếu quân đang nằm trong Mill → mặc định KHÔNG được ăn
    bool inMill = checkMill(board, pos, opponent);
    if (!inMill)
        return true; // Không trong Mill → ăn được ngay

    // Ngoại lệ: Nếu TẤT CẢ quân đối thủ đều trong Mill
    // → được phép ăn quân trong Mill (không còn lựa chọn nào khác)
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == opponent && !checkMill(board, i, opponent)) {
            // Còn ít nhất 1 quân đối thủ ngoài Mill → không ăn pos
            return false;
        }
    }

    // Toàn bộ quân đối thủ đều trong Mill → được phép ăn
    return true;
}

//  7. KIỂM TRA ĐIỀU KIỆN THUA

bool GameRules::isLoser(const int board[BOARD_SIZE],
                         int player,
                         int piecesOnBoard,
                         int piecesInHand,
                         GamePhase phase) {

    // Điều kiện 1: Phase 2 mà còn dưới 3 quân → thua
    if (phase == GamePhase::PHASE2_MOVING && piecesOnBoard < 3)
        return true;

    // Điều kiện 2: Hết quân hoàn toàn → thua
    if (piecesOnBoard == 0 && piecesInHand == 0)
        return true;

    // Điều kiện 3: Phase 2 mà không còn nước đi hợp lệ nào → thua
    if (phase == GamePhase::PHASE2_MOVING) {
        auto moves = getAllValidMoves(board, player);
        if (moves.empty())
            return true;
    }

    return false;
}

//  8. CÁC HÀM TIỆN ÍCH (UTILITY)

// Trả về danh sách các ô liền kề với pos
std::vector<int> GameRules::getAdjacentPositions(int pos) {
    std::vector<int> result;
    if (pos < 0 || pos >= BOARD_SIZE) return result;

    for (int i = 0; i < ADJACENCY_COUNT; ++i) {
        if (ADJACENCY[i][0] == pos) result.push_back(ADJACENCY[i][1]);
        if (ADJACENCY[i][1] == pos) result.push_back(ADJACENCY[i][0]);
    }
    return result;
}

// Trả về tất cả nước đi hợp lệ Phase 2 dưới dạng {from, to}
std::vector<std::pair<int,int>> GameRules::getAllValidMoves(
    const int board[BOARD_SIZE], int player) {

    std::vector<std::pair<int,int>> moves;

    for (int from = 0; from < BOARD_SIZE; ++from) {
        if (board[from] != player) continue;

        // Thử di chuyển sang từng ô liền kề
        auto neighbors = getAdjacentPositions(from);
        for (int to : neighbors) {
            if (board[to] == EMPTY) {
                moves.push_back({from, to});
            }
        }
    }
    return moves;
}

// Trả về tất cả ô đối thủ có thể bị ăn
std::vector<int> GameRules::getRemovablePieces(
    const int board[BOARD_SIZE], int opponent) {

    std::vector<int> removable;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (canRemovePiece(board, i, opponent))
            removable.push_back(i);
    }
    return removable;
}

// Đếm số quân của player còn trên bàn
int GameRules::countPieces(const int board[BOARD_SIZE], int player) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i] == player) ++count;
    }
    return count;
}