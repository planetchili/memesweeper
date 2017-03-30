#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>

void MemeField::Tile::SpawnMeme()
{
	assert( !hasMeme );
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw( const Vei2& screenPos,MemeField::State fieldState,Graphics& gfx ) const
{
	if( fieldState != MemeField::State::Fucked )
	{
		switch( state )
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton( screenPos,gfx );
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton( screenPos,gfx );
			SpriteCodex::DrawTileFlag( screenPos,gfx );
			break;
		case State::Revealed:
			if( !HasMeme() )
			{
				SpriteCodex::DrawTileNumber( screenPos,nNeighborMemes,gfx );
			}
			else
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
			}
			break;
		}
	}
	else // we are fucked
	{
		switch( state )
		{
		case State::Hidden:
			if( HasMeme() )
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileButton( screenPos,gfx );
			}
			break;
		case State::Flagged:
			if( HasMeme() )
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
				SpriteCodex::DrawTileFlag( screenPos,gfx );
			}
			else
			{
				SpriteCodex::DrawTileBomb( screenPos,gfx );
				SpriteCodex::DrawTileCross( screenPos,gfx );
			}
			break;
		case State::Revealed:
			if( !HasMeme() )
			{
				SpriteCodex::DrawTileNumber( screenPos,nNeighborMemes,gfx );
			}
			else
			{
				SpriteCodex::DrawTileBombRed( screenPos,gfx );
			}
			break;
		}
	}
}

void MemeField::Tile::Reveal()
{
	assert( state == State::Hidden );
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert( !IsRevealed() );
	if( state == State::Hidden )
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

bool MemeField::Tile::HasNoNeighborMemes() const
{
	return nNeighborMemes == 0;
}

void MemeField::Tile::SetNeighborMemeCount( int memeCount )
{
	assert( nNeighborMemes == -1 );
	nNeighborMemes = memeCount;
}

MemeField::MemeField( const Vei2& center,int nMemes )
	:
	topLeft( center - Vei2( width * SpriteCodex::tileSize,height * SpriteCodex::tileSize ) / 2 )
{
	assert( nMemes > 0 && nMemes < width * height );
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> xDist( 0,width - 1 );
	std::uniform_int_distribution<int> yDist( 0,height - 1 );

	for( int nSpawned = 0; nSpawned < nMemes; ++nSpawned )
	{
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist( rng ),yDist( rng ) };
		}
		while( TileAt( spawnPos ).HasMeme() );

		TileAt( spawnPos ).SpawnMeme();
	}

	for( Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++ )
	{
		for( gridPos.x = 0; gridPos.x < width; gridPos.x++ )
		{
			TileAt( gridPos ).SetNeighborMemeCount( CountNeighborMemes( gridPos ) );
		}
	}
}

void MemeField::Draw( Graphics& gfx ) const
{
	std::lock_guard<std::mutex> lock( mutex );
	gfx.DrawRect( GetRect().GetExpanded( borderThickness ),borderColor );
	gfx.DrawRect( GetRect(),SpriteCodex::baseColor );
	for( Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++ )
	{
		for( gridPos.x = 0; gridPos.x < width; gridPos.x++ )
		{
			TileAt( gridPos ).Draw( topLeft + gridPos * SpriteCodex::tileSize,state,gfx );
		}
	}
	if (recursing)
	{
		for (const auto& p : rpts)
		{
			DrawFocus(gfx,p );
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI( topLeft,width * SpriteCodex::tileSize,height * SpriteCodex::tileSize );
}

void MemeField::OnRevealClick( const Vei2& screenPos )
{
	std::unique_lock<std::mutex> lock( mutex );
	if( !IsBusy() && state == State::Memeing )
	{
		const Vei2 gridPos = ScreenToGrid( screenPos );
		assert( gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height );
		future = std::async(std::launch::async, 
			[gridPos,this]() { RevealTile(gridPos,rColorBase,std::unique_lock<std::mutex>( mutex ) ); }
		);
	}
}

void MemeField::OnFlagClick( const Vei2& screenPos )
{
	std::lock_guard<std::mutex> lock( mutex );
	if( !IsBusy() && state == State::Memeing )
	{
		const Vei2 gridPos = ScreenToGrid( screenPos );
		assert( gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height );
		Tile& tile = TileAt( gridPos );
		if( !tile.IsRevealed() )
		{
			tile.ToggleFlag();
			if( tile.IsFlagged() && GameIsWon() )
			{
				state = State::Winrar;
			}
		}
	}
}

MemeField::State MemeField::GetState() const
{
	std::lock_guard<std::mutex> lock( mutex );
	return state;
}

void MemeField::Sync()
{
	using namespace std::chrono_literals;
	if( future.valid() )
	{
		switch( future.wait_for( 0ms ) )
		{
		case std::future_status::deferred:
		case std::future_status::ready:
			future.get();
			if( GameIsWon() )
			{
				state = State::Winrar;
			}
			break;
		case std::future_status::timeout:
			return;
		}
	}
	recursing = false;
}

bool MemeField::IsBusy()
{
	return recursing;
}

std::unique_lock<std::mutex> MemeField::RevealTile( const Vei2& gridPos,Color parentColor,std::unique_lock<std::mutex> lock )
{
	using namespace std::chrono_literals;
	const Color c( parentColor.GetR() + 30,parentColor.GetG() - 10,parentColor.GetB() + 10 );
	rpts.push_back( { gridPos,c } );
	recursing = true;
	lock.unlock();
	std::this_thread::sleep_for( 120ms );
	lock.lock();

	Tile& tile = TileAt( gridPos );
	if( !tile.IsRevealed() && !tile.IsFlagged() )
	{
		tile.Reveal();
		if( tile.HasMeme() )
		{
			state = State::Fucked;
			sndLose.Play();
		}
		else if( tile.HasNoNeighborMemes() )
		{
			const int xStart = std::max( 0,gridPos.x - 1 );
			const int yStart = std::max( 0,gridPos.y - 1 );
			const int xEnd = std::min( width - 1,gridPos.x + 1 );
			const int yEnd = std::min( height - 1,gridPos.y + 1 );

			for( Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++ )
			{
				for( gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++ )
				{
					lock = RevealTile( gridPos,c,std::move( lock ) );
				}
			}
		}
	}
	rpts.pop_back();
	return std::move( lock );
}


MemeField::Tile& MemeField::TileAt( const Vei2& gridPos )
{
	return field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt( const Vei2 & gridPos ) const
{
	return field[gridPos.y * width + gridPos.x];
}

Vei2 MemeField::ScreenToGrid( const Vei2& screenPos )
{
	return (screenPos - topLeft) / SpriteCodex::tileSize;
}

int MemeField::CountNeighborMemes( const Vei2& gridPos ) const
{
	const int xStart = std::max( 0,gridPos.x - 1 );
	const int yStart = std::max( 0,gridPos.y - 1 );
	const int xEnd = std::min( width - 1,gridPos.x + 1 );
	const int yEnd = std::min( height - 1,gridPos.y + 1 );

	int count = 0;
	for( Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++ )
	{
		for( gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++ )
		{
			if( TileAt( gridPos ).HasMeme() )
			{
				count++;
			}
		}
	}

	return count;
}

bool MemeField::GameIsWon() const
{
	for( const Tile& t : field )
	{
		if( (t.HasMeme() && !t.IsFlagged()) ||
			(!t.HasMeme() && !t.IsRevealed()) )
		{
			return false;
		}
	}
	return true;
}

void MemeField::DrawFocus( Graphics& gfx,const RecursionPoint& rp ) const
{
	const Vei2 offset( 2,2 );
	const auto topLeft = rp.gridPos * SpriteCodex::tileSize + this->topLeft + offset;
	const auto bottomRight = (rp.gridPos + Vei2( 1,1 )) * SpriteCodex::tileSize + this->topLeft - offset;

	for( int x = topLeft.x; x < bottomRight.x; x++ )
	{
		gfx.PutPixel( x,topLeft.y,rp.color );
		gfx.PutPixel( x,bottomRight.y - 1,rp.color );
	}
	for( int y = topLeft.y; y < bottomRight.y; y++ )
	{
		gfx.PutPixel( topLeft.x,y,rp.color );
		gfx.PutPixel( bottomRight.x - 1,y,rp.color );
	}
}
