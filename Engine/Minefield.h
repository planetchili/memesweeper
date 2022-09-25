#pragma once
#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class Minefield
{
private:
	class Cell {
	private:
		enum class Status {
			Hidden,
			Revealed,
			Flagged
		};
	public:

	private:
		Status status = Status::Hidden;
		bool hasMine = false;
	};
public:
	Minefield(int in_nMines, Vei2 in_pos);
	Cell& CellAt(const Vei2 gridPos);
	void Draw(Graphics& gfx);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	Cell field[width * height];
	int nMines;
	Vei2 pos;
};

