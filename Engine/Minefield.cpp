#include "Minefield.h"

Minefield::Minefield(int in_nMines, Vei2 in_pos)
{
	nMines = in_nMines;
	pos = in_pos;
}

Minefield::Cell& Minefield::CellAt(const Vei2 gridPos)
{
	return field[gridPos.y * width + gridPos.x];
}

void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(pos.x, pos.y, pos.x + width * SpriteCodex::tileSize, pos.y + height * SpriteCodex::tileSize, SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			const Vei2 screenPos = gridPos * SpriteCodex::tileSize;
			SpriteCodex::DrawTileButton(screenPos, gfx);
		}
	}
}

