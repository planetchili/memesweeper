#pragma once

#include "Graphics.h"

class MineField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden = 0,
			Flagged,
			Revealed
		};
	public:
		void SpawnMine();
		bool HasMine() const;
		//besides Draw in MineField, we need a Draw for a tile
		//besides gfx, we need to know the pixel coords to put them on the screen
		//Vei2 will represent 2 concepts - grid pos and pixel pos
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
	private:
		State state = State::Hidden;
		bool hasMine = false;
	};
public:
	MineField(int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2& screenPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	static constexpr int nFields = width * height;
	Tile field[width * height];
private:
	//if the fn is const you cannot return a non-const reference!!
	//you can create another version for this fn - the have identical code, but one of them is a const version
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	const Vei2 ScreenToGrid(const Vei2& screenPos);
};

