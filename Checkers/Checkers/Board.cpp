// �����: ������� ������.

#include <Board.h>
#include <cassert>

CBoard::CBoard()
	: board( std::vector<CChecker*> ( boardSize * boardSize / 2,  0 ) )
{
	createNewCheckers();
}

CBoard::~CBoard()
{
	deleteAllCheckers();
}

void CBoard::MakeTurn( int from, int to )
{
	// ���� � ���� from ��� �����, ������ ���-�� ����� �� ���.
	assert( board[to] != 0 );
	// ���� � ���� to ����� �����, ������ ���-�� ����� �� ���.
	assert( board[from] == 0 );

	std::swap( board[to], board[from] );
}

void CBoard::Reset()
{
	deleteAllCheckers();
	createNewCheckers();
}

// �������� �� ���� ����� �����.
void CBoard::createNewCheckers()
{
	// ��������� ����� ��� ������ �������.
	for( size_t i = 0; i < startNumberOfCheckers; ++i ) {
		board[i] = new CChecker( false, false );
	}

	// ��������� ����� ��� ����� �������.
	for( size_t i = board.size() - startNumberOfCheckers; i < board.size(); ++i ) {
		board[i] = new CChecker( true, false );
	}
}

// �������� ���� �� �����.
void CBoard::deleteAllCheckers()
{
	for( size_t i = 0; i < board.size(); ++i ) {
		if( board[i] != 0 ) {
			delete board[i];
		}
	}
}