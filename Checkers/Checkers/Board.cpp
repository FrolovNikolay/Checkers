// Автор: Николай Фролов.

#include <Board.h>
#include <cassert>

CBoard::CBoard( size_t _BoardSize, size_t _startNumberOfCheckers )
	: BoardSize( _BoardSize )
	, startNumberOfCheckers( _startNumberOfCheckers )
{
	for( int i = 0; i < BoardSize * BoardSize / 2; ++i ) {
		playBoard.push_back( CField( i ) );
	}
	generateNewBoard();
}

void CBoard::Reset()
{
	generateNewBoard();
}

// Создание доски для новой игры.
void CBoard::generateNewBoard()
{
	for( size_t i = 0; i < startNumberOfCheckers; ++i ) {
		playBoard[i].Condition = FC_BlackChecker;
		playBoard[playBoard.size() - 1 - i].Condition = FC_WhiteChecker;
	}
	
	for( size_t i = startNumberOfCheckers; i < playBoard.size() - startNumberOfCheckers; ++i ) {
		playBoard[i].Condition = FC_Empty;
	}
}