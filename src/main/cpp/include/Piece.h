#pragma once

enum class PieceType {
    NORMAL,
    KING
};

enum class PieceColor {
    BLACK,
    RED,
    NONE
};

struct Piece {
    PieceType type;
    PieceColor color;
    
    Piece() : type(PieceType::NORMAL), color(PieceColor::NONE) {}
    Piece(PieceColor c) : type(PieceType::NORMAL), color(c) {}
};
