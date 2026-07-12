#pragma once
#include <vector>
#include <memory>
#include "Piece.hpp"
#include "Position.hpp"

class Board {
private:
    int rows, cols;
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;

public:
    Board(int r, int c) : rows(r), cols(c), grid(r, std::vector<std::shared_ptr<Piece>>(c, nullptr)) {}
    
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
    std::shared_ptr<Piece> getPieceAt(const Position& pos) const {
        return (pos.getRow() >= 0 && pos.getRow() < rows && pos.getCol() >= 0 && pos.getCol() < cols) 
               ? grid[pos.getRow()][pos.getCol()] : nullptr;
    }
    
    void setPieceAt(int r, int c, std::shared_ptr<Piece> p) {
        if(r >= 0 && r < rows && c >= 0 && c < cols) {
            grid[r][c] = p;
            if(p) p->setPosition(Position(r, c));
        }
    }
    bool isInsideBoard(const Position& to) const
    {
        return to.getRow() < rows && to.getRow() >= 0 && to.getCol() < cols && to.getCol() >=0;
    }
    /**
     * @brief מעבירה כלי ממיקום אחד לאחר.
     * @param from המיקום הנוכחי של הכלי.
     * @param to המיקום שאליו רוצים להזיז.
     */
    void movePiece(const Position& from, const Position& to) {
        if (!isInsideBoard(from) || !isInsideBoard(to)) return;

        std::shared_ptr<Piece> piece = grid[from.getRow()][from.getCol()];
        
        if (piece) {
            // 1. עדכון המיקום החדש בלוח
            grid[to.getRow()][to.getCol()] = piece;
            
            // 2. עדכון המיקום הפנימי בתוך אובייקט הכלי
            piece->setPosition(to);
            
            // 3. ניקוי המיקום הישן בלוח (רק אם לא הזזנו לאותו המקום)
            if (!(from.getRow() == to.getRow() && from.getCol() == to.getCol())) {
                grid[from.getRow()][from.getCol()] = nullptr;
            }
        }
    }

    /**
     * @brief מסירה כלי ממיקום ספציפי בלוח.
     * @param pos המיקום ממנו רוצים להסיר את הכלי.
     */
    void removePiece(const Position& pos) {
        if (isInsideBoard(pos)) {
            grid[pos.getRow()][pos.getCol()] = nullptr;
        }
    }
};