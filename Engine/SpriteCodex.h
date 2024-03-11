#pragma once

#include "Graphics.h"
#include "Vector2D.h"

class SpriteCodex
{
public:
	// width and height of all tiles
	static constexpr int tileSize = 16;
	// base color for all tiles
	static constexpr Color baseColor = { 192,192,192 };
	// 16x16 tile sprites assume (192,192,192) background and top left origin
	static void DrawTile0(const Vector2D& pos, Graphics& gfx);
	static void DrawTile1(const Vector2D& pos, Graphics& gfx);
	static void DrawTile2(const Vector2D& pos, Graphics& gfx);
	static void DrawTile3(const Vector2D& pos, Graphics& gfx);
	static void DrawTile4(const Vector2D& pos, Graphics& gfx);
	static void DrawTile5(const Vector2D& pos, Graphics& gfx);
	static void DrawTile6(const Vector2D& pos, Graphics& gfx);
	static void DrawTile7(const Vector2D& pos, Graphics& gfx);
	static void DrawTile8(const Vector2D& pos, Graphics& gfx);
	static void DrawTileButton(const Vector2D& pos, Graphics& gfx);
	static void DrawTileCross(const Vector2D& pos, Graphics& gfx);
	static void DrawTileFlag(const Vector2D& pos, Graphics& gfx);
	static void DrawTileBomb(const Vector2D& pos, Graphics& gfx);
	static void DrawTileBombRed(const Vector2D& pos, Graphics& gfx);
};