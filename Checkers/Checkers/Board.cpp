// Автор: Николай Фролов.

#include <Board.h>
#include <cassert>

CBoard::CBoard()
{
	generateNewBoard();
}

void CBoard::Reset()
{
	generateNewBoard();
}

// Создание доски для новой игры.
void CBoard::generateNewBoard()
{
	playBoard.clear();

	for( size_t i = 0; i < CBoard::BoardSize * CBoard::BoardSize / 2; ++i ) {
		playBoard.push_back( CField( i ) );
	}

	for( size_t i = 0; i < startNumberOfCheckers; ++i ) {
		playBoard[i].Condition = FC_BlackChecker;
		playBoard[playBoard.size() - 1 - i].Condition = FC_WhiteChecker;
	}
}