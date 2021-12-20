#pragma once
#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class MineField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMine();
		bool HasMine() const;
		bool IsRevealed() const;
		bool IsFlagged() const;
		void Draw(const Vei2& screenPos, bool gameEnd, Graphics& gfx) const;
		void Reveal();
		void ToggleFlag();
		void SetNeighborMineCount(int mineCount);
	private:
		State states = State::Hidden;
		bool hasMine = false;
		int nNeighborMine = -1;
	};

public:
	MineField(const Vei2& center, int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void DrawBoarder(Graphics& gfx) const;
	void OnRevealClick(const Vei2& mousePos);
	void OnFlagClick(const Vei2& mousePos);
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighborMines(const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;

	Tile field[width * height];
	Vei2 topLeft;
	static constexpr int boardOffset = 10;
	static constexpr Color boardColor = Colors::Blue;
	
	bool isBombEnd = false;

};