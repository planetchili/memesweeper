#include "Minefield.h"
#include <assert.h>

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

			switch ( CellAt(gridPos).GetStatus() )
			{
			case Cell::Status::Hidden:
				SpriteCodex::DrawTileButton(screenPos, gfx);
				break;
			case Cell::Status::Revealed:
				SpriteCodex::DrawTile0(screenPos, gfx);
				break;
			case Cell::Status::Flagged:
				SpriteCodex::DrawTileButton(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
				break;
			}
		}
	}
}

Vei2 Minefield::ScreenToGrid(const Vei2& screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

void Minefield::RevealOnClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	if (gridPos.x < width && gridPos.x >= 0 && gridPos.y < height && gridPos.y >= 0)
	{
		Cell& cell = CellAt(gridPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		cell.Reveal();
	}
}

void Minefield::ToggleFlagOnClick(const Vei2& screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	if (gridPos.x < width && gridPos.x >= 0 && gridPos.y < height && gridPos.y >= 0)
	{
		Cell& cell = CellAt(gridPos);
		assert(gridPos.x > 0 && gridPos.x < width&& gridPos.y > 0 && gridPos.y < height);
		cell.ToggleFlag();
	}
}

void Minefield::Cell::Reveal()
{
	if (!IsRevealed() && !IsFlagged())
	{
		status = Status::Revealed;
	}
}

void Minefield::Cell::ToggleFlag()
{
	if (!IsRevealed())
	{
		if (!IsFlagged())
		{
			status = Status::Flagged;
		}
		else
		{
			status = Status::Hidden;
		}
	}
}

bool Minefield::Cell::IsRevealed()
{
	return status == Status::Revealed;
}

bool Minefield::Cell::IsFlagged()
{
	return status == Status::Flagged;
}

Minefield::Cell::Status& Minefield::Cell::GetStatus()
{
	return status;
}
