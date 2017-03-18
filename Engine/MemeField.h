#pragma once

#include "Graphics.h"
#include "Sound.h"
#include <future>
#include <mutex>

class MemeField
{
public:
	enum class State
	{
		Fucked,
		Winrar,
		Memeing
	};
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
		void SpawnMeme();
		bool HasMeme() const;
		void Draw( const Vei2& screenPos,MemeField::State fucked,Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		bool HasNoNeighborMemes() const;
		void SetNeighborMemeCount( int memeCount );
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
	struct RecursionPoint
	{
		Vei2 gridPos;
		Color color;
	};
public:
	MemeField( const Vei2& center,int nMemes );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	State GetState() const;
	void Sync();
private:
	bool IsBusy();
	std::unique_lock<std::mutex> RevealTile( const Vei2& gridPos,Color parentColor,std::unique_lock<std::mutex> lock );
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos ) const;
	bool GameIsWon() const;
	void DrawFocus( Graphics& gfx, const RecursionPoint& rp) const;
private:
	static constexpr int width = 8;
	static constexpr int height = 6;
	static constexpr int borderThickness = 10;
	static constexpr Color rColorBase = {0,128,0};
	static constexpr Color borderColor = Colors::Blue;
	bool recursing = false;
	mutable std::mutex mutex;
	std::future<void> future;
	std::vector<RecursionPoint> rpts;
	Sound sndLose = Sound( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	Tile field[width * height];
};