/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	menu( { gfx.GetRect().GetCenter().x,200 } )
	//field( gfx.GetRect().GetCenter(),4 )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	while( !wnd.mouse.IsEmpty() )
	{
		const auto e = wnd.mouse.Read();
		if( state == State::Memesweeper )
		{
			if( field->GetState() == MemeField::State::Memeing )
			{
				if( e.GetType() == Mouse::Event::Type::LPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( field->GetRect().Contains( mousePos ) )
					{
						field->OnRevealClick( mousePos );
					}
				}
				else if( e.GetType() == Mouse::Event::Type::RPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( field->GetRect().Contains( mousePos ) )
					{
						field->OnFlagClick( mousePos );
					}
				}
			}
		}
		else
		{
			const SelectionMenu::Size s = menu.ProcessMouse( e );
			switch( s )
			{
			case SelectionMenu::Size::Small:
			{
				field = new MemeField(gfx.GetRect().GetCenter(), 8,4);
				break;
			}
			case SelectionMenu::Size::Medium:
			{
				field = new MemeField(gfx.GetRect().GetCenter(), 12, 6);
				break;
			}
			case SelectionMenu::Size::Large:
			{
				field = new MemeField(gfx.GetRect().GetCenter(), 16, 8);
				break;
			}
				state = State::Memesweeper;
			}
		}
	}
}

void Game::ComposeFrame()
{
	if( state == State::Memesweeper )
	{
		field->Draw( gfx );
		if( field->GetState() == MemeField::State::Winrar )
		{
			SpriteCodex::DrawWin( gfx.GetRect().GetCenter(),gfx );
		}
	}
	else
	{
		menu.Draw( gfx );
	}
}
