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
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
	private:
		State states = State::Hidden;
		bool hasMine = false;
	};

public:
	MineField(int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void DrawBoarder(Graphics& gfx) const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 16;

	static constexpr int boardOffset = 10;
	static constexpr Color boardColor = Colors::Blue;
	static constexpr int boardLeft = (Graphics::ScreenWidth / 2) - ((width / 2) * SpriteCodex::tileSize);
	static constexpr int boardRight = (Graphics::ScreenWidth / 2) + ((width / 2) * SpriteCodex::tileSize);
	static constexpr int boardTop = (Graphics::ScreenHeight / 2) - ((height / 2) * SpriteCodex::tileSize);
	static constexpr int boardBottom = (Graphics::ScreenHeight / 2) + ((height / 2) * SpriteCodex::tileSize);

	Tile field[width * height];

};