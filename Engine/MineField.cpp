#include "MineField.h"
#include <random>
#include <assert.h>

void MineField::Tile::SpawnMine()
{
    assert(!hasMine);
    hasMine = true;

}

bool MineField::Tile::HasMine() const
{
    return hasMine;
}

bool MineField::Tile::IsRevealed() const
{
    return states == State::Revealed;
}

bool MineField::Tile::IsFlagged() const
{
    return states == State::Flagged;
}

void MineField::Tile::Draw(const Vei2& screenPos, bool gameEnd, Graphics& gfx) const
{
    if (!gameEnd)
    {
        switch (states)
        {
        case State::Hidden:
            SpriteCodex::DrawTileButton(screenPos, gfx);
            break;
        case State::Flagged:
            SpriteCodex::DrawTileButton(screenPos, gfx);
            SpriteCodex::DrawTileFlag(screenPos, gfx);
            break;
        case State::Revealed:
            if (!HasMine())
            {
                SpriteCodex::DrawTileNumber(screenPos, nNeighborMine, gfx);
            }
            else
            {
                SpriteCodex::DrawTileBomb(screenPos, gfx);
            }
            break;
        }
    }
    else //Game End
    {
        switch (states)
        {
        case State::Hidden:
            if (HasMine())
            {
                SpriteCodex::DrawTileBomb(screenPos, gfx);
            }
            else
            {
                SpriteCodex::DrawTileButton(screenPos, gfx);
            }
            break;
        case State::Flagged:
            if (HasMine())
            {
                SpriteCodex::DrawTileBomb(screenPos, gfx);
                SpriteCodex::DrawTileFlag(screenPos, gfx);
            }
            else
            {
                SpriteCodex::DrawTileBomb(screenPos, gfx);
                SpriteCodex::DrawTileCross(screenPos, gfx);
            }
            break;
        case State::Revealed:
            if (!HasMine())
            {
                SpriteCodex::DrawTileNumber(screenPos, nNeighborMine, gfx);
            }
            else
            {
                SpriteCodex::DrawTileBombRed(screenPos, gfx);
            }
            break;
        }
    }
}

void MineField::Tile::Reveal()
{
    states = State::Revealed;
}

void MineField::Tile::ToggleFlag()
{
    states = State::Flagged;
}

void MineField::Tile::SetNeighborMineCount(int mineCount)
{
    assert(nNeighborMine == -1);
    nNeighborMine = mineCount;
}

MineField::MineField(const Vei2& center, int nMines)
    :
    topLeft(center - Vei2(width * SpriteCodex::tileSize, height * SpriteCodex::tileSize) / 2)
{
    assert(nMines > 0 && nMines < width* height);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);



    for (int nSpawned = 0; nSpawned < nMines; ++nSpawned)
    {
        Vei2 spawnPos;
        do
        {
            spawnPos = { xDist(rng), yDist(rng) };
        } while (TileAt(spawnPos).HasMine());

        TileAt(spawnPos).SpawnMine();
    }

    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; ++gridPos.y)
    {
        for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
        {
            TileAt(gridPos).SetNeighborMineCount(CountNeighborMines(gridPos));
        }
    }
}

void MineField::Draw(Graphics& gfx) const
{
    gfx.DrawRect(GetRect(), Colors::LightGray);
    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; ++gridPos.y)
    {
        for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
        {
            TileAt(gridPos).Draw((gridPos * SpriteCodex::tileSize) + topLeft, isBombEnd, gfx);
        }
    }
}

RectI MineField::GetRect() const
{
    return RectI(topLeft, width * SpriteCodex::tileSize, height* SpriteCodex::tileSize);
}

void MineField::DrawBoarder(Graphics& gfx) const
{

}

void MineField::OnRevealClick(const Vei2& mousePos)
{
    if (!isBombEnd)
    {
        const Vei2 gridPos = ScreenToGrid(mousePos);
        Tile& tile = TileAt(gridPos);
        if (!tile.IsRevealed() && !tile.IsFlagged())
        {
            tile.Reveal();
            if (tile.HasMine())
            {
                isBombEnd = true;
            }
        }
    }

}

void MineField::OnFlagClick(const Vei2& mousePos)
{
    if (!isBombEnd)
    {
        const Vei2 gridPos = ScreenToGrid(mousePos);
        Tile& tile = TileAt(gridPos);
        if (!tile.IsFlagged())
        {
            tile.ToggleFlag();
        }
    }
}

MineField::Tile& MineField::TileAt(const Vei2& gridPos)
{
    return field[gridPos.y * width + gridPos.x];
}

const MineField::Tile& MineField::TileAt(const Vei2& gridPos) const
{
    return field[gridPos.y * width + gridPos.x];
}

Vei2 MineField::ScreenToGrid(const Vei2& screenPos)
{
    return (screenPos - topLeft) / SpriteCodex::tileSize;
}

int MineField::CountNeighborMines(const Vei2& gridPos)
{
    const int xStart = std::max(0, gridPos.x - 1);
    const int yStart = std::max(0, gridPos.y - 1);
    const int xEnd = std::min(width - 1, gridPos.x + 1);
    const int yEnd = std::min(height - 1, gridPos.y + 1);

    int count = 0;
    for (Vei2 gridPos = { xStart, yStart }; gridPos.y <= yEnd; gridPos.y++)
    {
        for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
        {
            if (TileAt(gridPos).HasMine())
            {
                count++;
            }
        }
    }
    return count;
}


