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

void MineField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
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
            SpriteCodex::DrawTile0(screenPos, gfx);
        }
        else
        {
            SpriteCodex::DrawTileBomb(screenPos, gfx);
        }
        break;
    }
}

MineField::MineField(int nMines)
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
    }
}

void MineField::Draw(Graphics& gfx) const
{
    gfx.DrawRect(boardLeft, boardTop, boardRight, boardBottom, Colors::LightGray);
    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; ++gridPos.y)
    {
        for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
        {
            TileAt(gridPos).Draw((gridPos * SpriteCodex::tileSize) + Vei2(boardLeft, boardTop), gfx);
        }
    }
}

RectI MineField::GetRect() const
{
    return RectI(0, width, 0, height);
}

void MineField::DrawBoarder(Graphics& gfx) const
{
    // Left, Right, Top, Bottom Rect
    gfx.DrawRect(boardLeft - boardOffset, boardTop, boardLeft, boardBottom, boardColor);
    gfx.DrawRect(boardRight, boardTop, boardRight + boardOffset, boardBottom, boardColor);
    gfx.DrawRect(boardLeft - boardOffset, boardTop - boardOffset, boardRight + boardOffset, boardTop, boardColor);
    gfx.DrawRect(boardLeft - boardOffset, boardBottom, boardRight + boardOffset, boardBottom + boardOffset, boardColor);
}

MineField::Tile& MineField::TileAt(const Vei2& gridPos)
{
    return field[gridPos.y * width + gridPos.x];
}

const MineField::Tile& MineField::TileAt(const Vei2& gridPos) const
{
    return field[gridPos.y * width + gridPos.x];
}


