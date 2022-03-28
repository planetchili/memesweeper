
#include "MemeField.h"
#include "SpriteCodex.h"

#include <random>
#include <cassert>


bool MemeField::Cell::HasMeme() const
{
	return hasMeme;
}

void MemeField::Cell::DropMeme()
{
	assert(!HasMeme());
	hasMeme = true;
}

void MemeField::Cell::Draw(Vei2 pos, Graphics& gfx, bool isGameOver) const
{
	if (isGameOver) 
	{
		switch (cellType)
		{
		case CellType::Closed:
			SpriteCodex::DrawTileButton(pos, gfx);
			if (HasMeme()) {
				SpriteCodex::DrawTileBomb(pos, gfx);
			}
			break;
		case CellType::Flagged:
			SpriteCodex::DrawTileButton(pos, gfx);
			
			if (HasMeme()) {
				SpriteCodex::DrawTileBomb(pos, gfx);
				SpriteCodex::DrawTileFlag(pos, gfx);
			}
			else {
				SpriteCodex::DrawTileFlag(pos, gfx);
				SpriteCodex::DrawTileCross(pos, gfx);
			}
			break;
		case CellType::Opened:
			if (HasMeme()) {
				SpriteCodex::DrawTileButton(pos, gfx);
				SpriteCodex::DrawTileBombRed(pos, gfx);
			}
			else {
				SpriteCodex::DrawTileButton(pos, gfx);
				SpriteCodex::DrawTileNum(pos, gfx, neighBorCount);
			}
		}
	}
	else 
	{
		switch (cellType)
		{
		case CellType::Closed:
			SpriteCodex::DrawTileButton(pos, gfx);
			break;
		case CellType::Flagged:
			SpriteCodex::DrawTileButton(pos, gfx);
			SpriteCodex::DrawTileFlag(pos, gfx);
			break;
		case CellType::Opened:
			if (HasMeme()) {
				SpriteCodex::DrawTileButton(pos, gfx);
				SpriteCodex::DrawTileBomb(pos, gfx);
			}
			else {
				SpriteCodex::DrawTileButton(pos, gfx);
				SpriteCodex::DrawTileNum(pos, gfx, neighBorCount);
			}
		}
	}
}

void MemeField::Cell::IncreaseNeighborCount()
{
	assert(neighBorCount <= 7);
	++neighBorCount;
}

void MemeField::Cell::FlagToggle()
{
	assert(cellType != CellType::Opened);
	cellType = cellType == CellType::Flagged ? CellType::Closed : CellType::Flagged;
}

void MemeField::Cell::OpenCell()
{
	assert(cellType != CellType::Flagged && cellType != CellType::Opened);
	cellType = CellType::Opened;
}

MemeField::Cell::CellType MemeField::Cell::GetCellType()
{
	return cellType;
}

bool MemeField::Cell::IsFlagged() const
{
	return cellType == CellType::Flagged;
}

bool MemeField::Cell::IsOpened() const
{
	return cellType == CellType::Opened;
}

void MemeField::Cell::Reset()
{
	hasMeme = false;
	cellType = CellType::Closed;
	neighBorCount = 0;
}

MemeField::MemeField(int nBombCount)	
	:
	memeCount(nBombCount)
{
	startingGridPos.x = (Graphics::ScreenWidth - rowCount * SpriteCodex::tileSize) / 2;
	startingGridPos.y = (Graphics::ScreenHeight - columnCount * SpriteCodex::tileSize) / 2;

	
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRectWithBorder(), borderColor);
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (int i = 0; i < rowCount; ++i) {
		for (int j = 0; j < columnCount; j++)
		{
			memes[ConvertRowColumnPairToIndex(i, j)].Draw(FromGridPosToPosition(i, j), gfx, isGameOver);
		}
	}
}

void MemeField::FlagCell(Vei2 pos)
{
	Vei2 gridPos = FromPositiontoGridPos(pos);
	assert(IsInBoard(gridPos.x, gridPos.y));
	int index = ConvertRowColumnPairToIndex(gridPos.x, gridPos.y);

	if (memes[index].GetCellType()!=MemeField::Cell::CellType::Opened)
	{
		memes[index].FlagToggle();
	}
}

void MemeField::OpenCell(Vei2 pos)
{
	Vei2 gridPos = FromPositiontoGridPos(pos);
	assert(IsInBoard(gridPos.x, gridPos.y));

	int index = ConvertRowColumnPairToIndex(gridPos.x, gridPos.y);
	if (memes[index].GetCellType() != MemeField::Cell::CellType::Opened &&
		memes[index].GetCellType() != MemeField::Cell::CellType::Flagged)
	{
		memes[index].OpenCell();
		if (memes[index].HasMeme()) {
			isGameOver = true;
		}
	}
	
}

bool MemeField::IsGameOver() const
{
	return isGameOver;
}

void MemeField::SpawnBombs(Vei2 pos)
{
	if (!isMemesSpawned) {
		Vei2 gridPos = FromPositiontoGridPos(pos);
		if (IsInBoard(gridPos.x, gridPos.y)) {

			int firstIndex = ConvertRowColumnPairToIndex(gridPos.x, gridPos.y);

			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<int> distRow(0, rowCount - 1);
			std::uniform_int_distribution<int> distColumn(0, columnCount - 1);



			for (int i = 0; i < memeCount; i++)
			{
				int index;
				int rowIndex;
				int columnIndex;

				do {
					rowIndex = distRow(rng);
					columnIndex = distColumn(rng);
					index = ConvertRowColumnPairToIndex(rowIndex, columnIndex);
				} while (memes[index].HasMeme() || index == firstIndex);

				memes[index].DropMeme();

				for (int k_row = -1; k_row <= 1; ++k_row) {
					for (int k_col = -1; k_col <= 1; ++k_col) {
						if ((k_row != 0 || k_col != 0) && MemeField::IsInBoard(rowIndex + k_row, columnIndex + k_col)) {
							memes[ConvertRowColumnPairToIndex(rowIndex + k_row, columnIndex + k_col)].IncreaseNeighborCount();
						}
					}
				}

			}

			isMemesSpawned = true;
		}
	}
}


int MemeField::ConvertRowColumnPairToIndex(int row, int column) const
{
	return row * columnCount + column;
}

Vei2 MemeField::FromGridPosToPosition(int row, int column) const
{
	return startingGridPos + Vei2{row, column}*SpriteCodex::tileSize;
}

Vei2 MemeField::FromPositiontoGridPos(Vei2 pos) const
{
	return (pos - startingGridPos)/SpriteCodex::tileSize;
}

bool MemeField::IsInBoard(int row, int col) const
{
	return row >= 0 && row < rowCount && col >= 0 && col < columnCount;
}

RectI MemeField::GetRect() const
{
	return RectI{ startingGridPos, startingGridPos + Vei2{ rowCount, columnCount } * SpriteCodex::tileSize };
}

RectI MemeField::GetRectWithBorder() const
{
	return RectI{ startingGridPos - Vei2(border,border), startingGridPos + Vei2{ rowCount, columnCount } *SpriteCodex::tileSize + Vei2(border,border) };
}

bool MemeField::IsMemesSpawned() const
{
	return isMemesSpawned;
}

bool MemeField::IsWon() const
{
	bool isAllOccupied = true;
	bool isWon = true;

	for (int i = 0; isWon && isAllOccupied && i < rowCount*columnCount; i++)
	{
		if (!memes[i].IsFlagged() && !memes[i].IsOpened()) {
			isAllOccupied = false;
		}

		if (memes[i].IsFlagged() && !memes[i].HasMeme()) {
			isWon = false;
		}
	}

	return isWon && isAllOccupied;
}

void MemeField::Reset()
{
	isMemesSpawned = false;
	for (int i = 0; i < rowCount * columnCount; ++i) {
		memes[i].Reset();
	}

}


