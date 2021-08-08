#include "MineField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"

void MineField::Tile::SpawnMine()
{
    assert(!hasMine);//only spawn on tiles without a bomb
    hasMine = true;
}

bool MineField::Tile::HasMine() const
{
    return hasMine;
}

void MineField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
    switch (state)
    {
    case State::Hidden: //btn looking thing
        SpriteCodex::DrawTileButton(screenPos, gfx);
        break;
    case State::Flagged:
        SpriteCodex::DrawTileButton(screenPos, gfx);
        SpriteCodex::DrawTileFlag(screenPos, gfx);
        break;
    case State::Revealed:
        //has a mine or doesn't have a mine
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

void MineField::Tile::Reveal()
{
    assert(state == State::Hidden);
    state = State::Revealed;
}

bool MineField::Tile::IsRevealed() const
{
    return state == State::Revealed;
}

void MineField::OnRevealClick(const Vei2& screenPos)
{
    //convert screenPos to gridPos
    const Vei2 gridPos = ScreenToGrid(screenPos);
    assert(gridPos.x > 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);

    Tile& tile = TileAt(gridPos);
    if (!tile.IsRevealed())
        tile.Reveal();
}

MineField::MineField(int nMines)
{
    assert(nMines > 0 && nMines < nFields);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    for (int nSpawned = 0; nSpawned < nMines; ++nSpawned)
    {
        Vei2 spawnPos;
        do
        {
            spawnPos = { xDist(rng), yDist(rng) }; //randomly generated position
        }
        while (TileAt(spawnPos).HasMine()); //check if there is already a bomb in that position - you need to map the grid to 1D array - separete fn for that

        TileAt(spawnPos).SpawnMine();
    }

    //reveal test
    for (int i = 0; i < 120; i++)
    {
        const Vei2 gridPos = { xDist(rng), yDist(rng) };
        if (!TileAt(gridPos).IsRevealed())
            TileAt(gridPos).Reveal();
    }

}

void MineField::Draw(Graphics& gfx) const
{   
    //the background of the tiles is currently black - draw a big rect for the whole grid underneath the tiles
    gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
    //instead of i,j - we can make one initializer {0,0}
    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++) //rows
    {
        for (gridPos.x = 0; gridPos.x < width; gridPos.x++) //cols
        {
            TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
        }
    }
}

RectI MineField::GetRect() const
{
    return RectI(0,width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

MineField::Tile& MineField::TileAt(const Vei2& gridPos)
{
    return field[gridPos.y * width + gridPos.x];
}

const MineField::Tile& MineField::TileAt(const Vei2& gridPos) const
{
    return field[gridPos.y * width + gridPos.x];
}

const Vei2 MineField::ScreenToGrid(const Vei2& screenPos)
{
    return (screenPos / SpriteCodex::tileSize);
}
