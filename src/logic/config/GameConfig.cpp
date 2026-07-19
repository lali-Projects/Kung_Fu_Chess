#include "GameConfig.hpp"

// אתחול המשתנים הקיימים
int GameConfig::CELL_SIZE_PX = 100;
int GameConfig::DEFAULT_TRAVEL_TIME_MS = 1000;
int GameConfig::INITIAL_TIME_MS = 0;

int GameConfig::LONG_REST_DURATION_MS = 2000;
int GameConfig::SHORT_REST_DURATION_MS = 1000;

// אתחול משתני התווים (ניתנים לשינוי במהלך המשחק)
char GameConfig::EMPTY_CELL = '.';
char GameConfig::SIDE_WHITE = 'w';
char GameConfig::SIDE_BLACK = 'b';

// אתחול מפת הכלים הדינמית
std::unordered_map<char, PieceType> GameConfig::TYPE_MAP = {
    {'K', PieceType::KING}, 
    {'Q', PieceType::QUEEN}, 
    {'R', PieceType::ROOK},
    {'B', PieceType::BISHOP}, 
    {'N', PieceType::KNIGHT}, 
    {'P', PieceType::PAWN}
};

