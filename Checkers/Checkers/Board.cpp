// Автор: Николай Фролов.

#include <Board.h>
#include <cassert>

CBoard::CBoard()
	: board( std::vector<CChecker*> ( BoardSize * BoardSize / 2,  0 ) )
{
	createNewCheckers();
}

CBoard::~CBoard()
{
	deleteAllCheckers();
}

void CBoard::MakeTurn( int from, int to )
{
	// Если в поле from нет шашки, значит что-то пошло не так.
	assert( board[to] != 0 );
	// Если в поле to стоит шашка, значит что-то пошло не так.
	assert( board[from] == 0 );

	std::swap( board[to], board[from] );
}

void CBoard::Reset()
{
	deleteAllCheckers();
	createNewCheckers();
}

// Добавить на поле новые шашки.
void CBoard::createNewCheckers()
{
	// Добавляем шашки для чёрной стороны.
	for( size_t i = 0; i < startNumberOfCheckers; ++i ) {
		board[i] = new CChecker( false, false );
	}

	// Добавляем шашки для белой стороны.
	for( size_t i = board.size() - startNumberOfCheckers; i < board.size(); ++i ) {
		board[i] = new CChecker( true, false );
	}
}

// Очистить поле от шашек.
void CBoard::deleteAllCheckers()
{
	for( size_t i = 0; i < board.size(); ++i ) {
		if( board[i] != 0 ) {
			delete board[i];
		}
	}
}