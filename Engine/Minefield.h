#pragma once
#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"
#include "Mouse.h"
#include <random>

class Minefield
{
private:
	class Cell {
	public:
		enum class Status {
			Hidden,
			Revealed,
			Flagged
		};
		void Reveal();
		void ToggleFlag();
		bool IsRevealed();
		bool IsFlagged();
		void SpawnMine();
		bool HasMine();
		Status& GetStatus();
	private:
		Status status = Status::Hidden;
		bool hasMine = false;
	};
public:
	Minefield(int in_nMines, Vei2 in_pos);
	Cell& CellAt(const Vei2 gridPos);
	void Draw(Graphics& gfx);
	Vei2 ScreenToGrid(const Vei2& screenPos);
	void RevealOnClick(const Vei2& screenPos);
	void ToggleFlagOnClick(const Vei2& screenPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Cell field[width * height];
	int nMines;
	Vei2 pos;
	bool isDead = false;
};

