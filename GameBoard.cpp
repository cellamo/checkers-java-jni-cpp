#include "GameBoard.hpp"

GameBoard::GameBoard() : board(BOARD_SIZE, std::vector<PieceType>(BOARD_SIZE, PieceType::EMPTY)) {
    initializeBoard();
}

void GameBoard::initializeBoard() {
    // Initialize black pieces (top of board)
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {  // Dark squares
                board[row][col] = PieceType::BLACK;
            }
        }
    }

    // Middle rows are empty by default

    // Initialize red pieces (bottom of board)
    for (int row = 5; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {  // Dark squares
                board[row][col] = PieceType::RED;
            }
        }
    }

    // Set initial game status
    status = GameStatus::IN_PROGRESS;
    currentPlayer = Player::BLACK;  // Black moves first
}

PieceType GameBoard::getPiece(int x, int y) const {
    if (!isValidPosition(x, y)) return PieceType::EMPTY;
    return board[x][y];
}

bool GameBoard::isValidPosition(int x, int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

bool GameBoard::isKing(PieceType piece) const {
    return piece == PieceType::BLACK_KING || piece == PieceType::RED_KING;
}

bool GameBoard::isPieceOwnedBy(Player player, PieceType piece) const {
    if (player == Player::BLACK) {
        return piece == PieceType::BLACK || piece == PieceType::BLACK_KING;
    }
    return piece == PieceType::RED || piece == PieceType::RED_KING;
}

std::vector<std::pair<int, int>> GameBoard::getValidMoves(int x, int y) const {
    std::vector<std::pair<int, int>> moves;
    PieceType piece = getPiece(x, y);
    
    if (piece == PieceType::EMPTY) return moves;

    int directions[] = {-1, 1};
    int forward = (piece == PieceType::BLACK || piece == PieceType::BLACK_KING) ? 1 : -1;

    for (int dx : directions) {
        int newX = x + dx;
        int newY = y + forward;
        
        if (isValidPosition(newX, newY) && getPiece(newX, newY) == PieceType::EMPTY) {
            moves.push_back({newX, newY});
        }
        
        // Kings can move backwards
        if (isKing(piece)) {
            newY = y - forward;
            if (isValidPosition(newX, newY) && getPiece(newX, newY) == PieceType::EMPTY) {
                moves.push_back({newX, newY});
            }
        }
    }

    return moves;
}

void GameBoard::switchPlayer() {
    currentPlayer = (currentPlayer == Player::BLACK) ? Player::RED : Player::BLACK;
}

Player GameBoard::getCurrentPlayer() const {
    return currentPlayer;
}


std::vector<std::pair<int, int>> GameBoard::getValidJumps(int x, int y) const {
    std::vector<std::pair<int, int>> jumps;
    PieceType piece = getPiece(x, y);
    
    if (piece == PieceType::EMPTY) return jumps;

    int directions[] = {-1, 1};
    int forward = (piece == PieceType::BLACK || piece == PieceType::BLACK_KING) ? 1 : -1;

    for (int dx : directions) {
        // Forward jumps
        int midX = x + dx;
        int midY = y + forward;
        int jumpX = midX + dx;
        int jumpY = midY + forward;
        
        if (isValidPosition(jumpX, jumpY) && 
            getPiece(jumpX, jumpY) == PieceType::EMPTY &&
            isValidPosition(midX, midY)) {
            PieceType midPiece = getPiece(midX, midY);
            if (isPieceOwnedBy(currentPlayer == Player::BLACK ? Player::RED : Player::BLACK, midPiece)) {
                jumps.push_back({jumpX, jumpY});
            }
        }
        
        // Backward jumps for kings
        if (isKing(piece)) {
            midY = y - forward;
            jumpY = midY - forward;
            
            if (isValidPosition(jumpX, jumpY) && 
                getPiece(jumpX, jumpY) == PieceType::EMPTY &&
                isValidPosition(midX, midY)) {
                PieceType midPiece = getPiece(midX, midY);
                if (isPieceOwnedBy(currentPlayer == Player::BLACK ? Player::RED : Player::BLACK, midPiece)) {
                    jumps.push_back({jumpX, jumpY});
                }
            }
        }
    }

    return jumps;
}

bool GameBoard::hasValidJumps() const {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (isPieceOwnedBy(currentPlayer, getPiece(x, y))) {
                if (!getValidJumps(x, y).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameBoard::removeCapturedPiece(int fromX, int fromY, int toX, int toY) {
    int midX = (fromX + toX) / 2;
    int midY = (fromY + toY) / 2;
    board[midX][midY] = PieceType::EMPTY;
}

bool GameBoard::makeMove(int fromX, int fromY, int toX, int toY) {
    if (!isValidPosition(fromX, fromY) || !isValidPosition(toX, toY)) {
        return false;
    }

    PieceType piece = getPiece(fromX, fromY);
    if (!isPieceOwnedBy(currentPlayer, piece)) {
        return false;
    }

    bool isJump = false;
    auto jumps = getValidJumps(fromX, fromY);
    
    // If jumps are available, only allow jumps
    if (hasValidJumps()) {
        for (const auto& jump : jumps) {
            if (jump.first == toX && jump.second == toY) {
                isJump = true;
                break;
            }
        }
        if (!isJump) return false;
    } else {
        // Regular moves only if no jumps are available
        auto moves = getValidMoves(fromX, fromY);
        bool isValidMove = false;
        for (const auto& move : moves) {
            if (move.first == toX && move.second == toY) {
                isValidMove = true;
                break;
            }
        }
        if (!isValidMove) return false;
    }

    // Perform the move
    board[toX][toY] = piece;
    board[fromX][fromY] = PieceType::EMPTY;

    if (isJump) {
        removeCapturedPiece(fromX, fromY, toX, toY);
    }

    // Check for promotion
    if (!isKing(piece)) {
        if (piece == PieceType::BLACK && toY == BOARD_SIZE - 1) {
            board[toX][toY] = PieceType::BLACK_KING;
        } else if (piece == PieceType::RED && toY == 0) {
            board[toX][toY] = PieceType::RED_KING;
        }
    }

    updateGameStatus();
    if (status == GameStatus::IN_PROGRESS) {
        switchPlayer();
    }
    
    return true;
}

void GameBoard::updateGameStatus() {
    int blackPieces = getPieceCount(Player::BLACK);
    int redPieces = getPieceCount(Player::RED);

    if (blackPieces == 0) {
        status = GameStatus::RED_WINS;
    } else if (redPieces == 0) {
        status = GameStatus::BLACK_WINS;
    } else if (!hasValidMoves(currentPlayer)) {
        status = currentPlayer == Player::BLACK ? GameStatus::RED_WINS : GameStatus::BLACK_WINS;
    } else {
        status = GameStatus::IN_PROGRESS;
    }
}

int GameBoard::getPieceCount(Player player) const {
    int count = 0;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (isPieceOwnedBy(player, getPiece(x, y))) {
                count++;
            }
        }
    }
    return count;
}

GameStatus GameBoard::getGameStatus() const {
    return status;
}

bool GameBoard::hasValidMoves(Player player) const {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (isPieceOwnedBy(player, getPiece(x, y))) {
                if (!getValidMoves(x, y).empty() || !getValidJumps(x, y).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}