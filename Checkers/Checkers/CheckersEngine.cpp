// Автор: Фролов Николай.

#include <CheckersEngine.h>
#include <Board.h>

CCheckersEngine::CCheckersEngine( CBoard& _board )
	: board( _board )
	, isWhiteTurn( true )
	, result( GR_StillPlaying )
{ }

void CCheckersEngine::StartGame()
{
	board.Reset();
	isWhiteTurn = true;
	result = GR_StillPlaying;

	calculateNextTurn();
}

void CCheckersEngine::AddFocus( int )
{
}

void CCheckersEngine::DelFocus( int )
{
}

void CCheckersEngine::TryTurn( int, int )
{
}

void CCheckersEngine::calculateNextTurn()
{
	std::vector<CField>& playBoard = board.GetBoard();

	shortcutPlayBoard.resize( playBoard.size() );
	for( size_t i = 0; i < playBoard.size(); ++i ) {
		shortcutPlayBoard[i] = std::pair<FieldCondition, bool>( playBoard[i].Condition, playBoard[i].IsKing );
	}

	for( size_t i = 0; i < playBoard.size(); ++i ) {
		if( ( playBoard[i].Condition == FC_WhiteChecker && isWhiteTurn ) 
			|| ( playBoard[i].Condition == FC_BlackChecker && !isWhiteTurn ) ) {
			calculatePossibleTurnsForField( i );
		}
	}

	for( std::map< int, std::list< std::vector<int> > >::iterator iter = possibleTurns.begin(); iter != possibleTurns.end(); ++iter ) {
		playBoard[iter->first].HasBorder = true;
	}
}

void CCheckersEngine::calculatePossibleTurnsForField( int fieldNumber )
{

}