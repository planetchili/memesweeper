#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "RectI.h"

class MemeField
{
public:
	class Cell {
	public:
		enum class CellType {
			Closed,
			Flagged,
			Opened
		};

	public:
		bool HasMeme() const;
		void DropMeme();
		void Draw(Vei2 pos, Graphics& gfx, bool isGameOver) const;
		void IncreaseNeighborCount();
		void FlagToggle();
		void OpenCell();
		CellType GetCellType();
		bool IsFlagged() const;
		bool IsOpened() const;
		void Reset();

	private:
		bool hasMeme=false;
		CellType cellType=CellType::Closed;
		int neighBorCount = 0;
	};

public:
	MemeField(int nBombCount);
	void Draw(Graphics& gfx) const;
	void FlagCell(Vei2 pos);
	void OpenCell(Vei2 pos);
	bool IsGameOver() const;
	void SpawnBombs(Vei2 pos);
	bool IsMemesSpawned() const;
	bool IsWon() const;
	void Reset();

private:
	int ConvertRowColumnPairToIndex(int row, int column) const;
	Vei2 FromGridPosToPosition(int row, int column) const;
	Vei2 FromPositiontoGridPos(Vei2 pos) const;
	bool IsInBoard(int row, int col) const;
	RectI GetRect() const;
	RectI GetRectWithBorder() const;

private:
	Vei2 startingGridPos;
	bool isGameOver = false;

	int memeCount;
	bool isMemesSpawned=false;

	static constexpr int rowCount = 2;
	static constexpr int columnCount = 3;

	static constexpr int border = 12;
	static constexpr Color borderColor = Colors::Blue;

	Cell memes[rowCount*columnCount];

};