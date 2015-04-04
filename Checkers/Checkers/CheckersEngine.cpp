// Автор: Фролов Николай.

#include <CheckersEngine.h>
#include <Board.h>

#include <algorithm>

CCheckersEngine::CCheckersEngine( CBoard& _board )
	: board( _board )
	, isWhiteTurn( true )
	, result( GR_StillPlaying )
{ }

void CCheckersEngine::StartGame()
{
	// Выставляем все параметры в стартовое состояние.
	board.Reset();
	isWhiteTurn = true;
	result = GR_StillPlaying;
	isTurnHasTakings = false;

	// Начинаем игру, запуская расчет доступных первых ходов.
	calculateNextTurn();
}

void CCheckersEngine::AddFocus( int fieldIdx )
{
	std::list< std::deque<int> >& possibleTurnForField = possibleTurns[fieldIdx];
	for( auto iter = possibleTurnForField.begin(); iter != possibleTurnForField.end(); ++iter ) {
		board.GetBoard()[( *iter )[isTurnHasTakings ? 1 : 0]].HasBorder = true;
		::InvalidateRect( board.GetBoard()[( *iter )[isTurnHasTakings ? 1 : 0]].Window, 0, true );
	}
	::InvalidateRect( board.GetBoard()[fieldIdx].Window, 0, true );
}

void CCheckersEngine::DelFocus( int fieldIdx )
{
	std::list< std::deque<int> >& possibleTurnForField = possibleTurns[fieldIdx];
	for( auto iter = possibleTurnForField.begin(); iter != possibleTurnForField.end(); ++iter ) {
		board.GetBoard()[( *iter )[isTurnHasTakings ? 1 : 0]].HasBorder = false;
		::InvalidateRect( board.GetBoard()[( *iter )[isTurnHasTakings ? 1 : 0]].Window, 0, true );
	}
	::InvalidateRect( board.GetBoard()[fieldIdx].Window, 0, true );
}

void CCheckersEngine::TryTurn( int from, int to )
{
	std::list< std::deque<int> >& possibleTurnForField = possibleTurns[from];
	std::vector<CField>& playBoard = board.GetBoard();
	bool isPossibleTurn = false;
	std::list< std::deque<int> > tmp;
	for( auto iter = possibleTurnForField.begin(); iter != possibleTurnForField.end(); ++iter ) {
		if( ( *iter )[isTurnHasTakings ? 1 : 0] == to ) {
			if( isTurnHasTakings ) {
				playBoard[*iter->begin()].Condition = FC_Empty;
				playBoard[*iter->begin()].IsKing = false;
				::InvalidateRect( playBoard[*iter->begin()].Window, 0, true );
				iter->pop_front();
			}
			iter->pop_front();
			if( !iter->empty() ) {
				tmp.push_back( *iter );
			}
			isPossibleTurn = true;
		}
	}

	if( isPossibleTurn ) {
		playBoard[to].Condition = playBoard[from].Condition;
		playBoard[from].Condition = FC_Empty;
		playBoard[to].IsKing = playBoard[from].IsKing;
		playBoard[from].IsKing = false;
		possibleTurns.clear();
		for( size_t i = 0; i < playBoard.size(); ++i ) {
			if( playBoard[i].HasBorder ) {
				playBoard[i].HasBorder = false;
				::InvalidateRect( playBoard[i].Window, 0, true );
			}
		}
		if( !tmp.empty() ) {
			for( auto iter = tmp.begin(); iter != tmp.end(); ++iter ) {
				playBoard[*++iter->begin()].HasBorder = true;
				::InvalidateRect( playBoard[*++iter->begin()].Window, 0, true );
			}
			playBoard[to].HasBorder = true;
			possibleTurns[to] = tmp;
			::InvalidateRect( playBoard[to].Window, 0, true );
		} else {
			if( !playBoard[to].IsKing ) {
				if( ( to < board.BoardSize / 2 && isWhiteTurn ) 
					|| ( to > ( static_cast<int>( playBoard.size() ) - board.BoardSize / 2 ) && !isWhiteTurn ) ) {
					playBoard[to].IsKing = true;
					::InvalidateRect( playBoard[to].Window, 0, true );
				}
			}
			isWhiteTurn = !isWhiteTurn;
			isTurnHasTakings = false;
			calculateNextTurn();
		}
	}
}

// Рассчитать возможные следующие ходы.
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
	// У текущего игрока нет ни одного допустимого хода(все его шашки взяты/заблокированы).
	if( possibleTurns.empty() ) {
		if( isWhiteTurn ) {
			result = GR_BlackWon;
		} else {
			result = GR_WhiteWon;
		}
		// EndGame();
		return;
	}

	for( auto iter = possibleTurns.begin(); iter != possibleTurns.end(); ++iter ) {
		if( iter->first >= 0 ) {
			playBoard[iter->first].HasBorder = true;
		}
		if( isTurnHasTakings ) {
			for( auto turnIter = iter->second.begin(); turnIter != iter->second.end(); ++turnIter ) {
				turnIter->pop_front();
			}
		}
		::InvalidateRect( playBoard[iter->first].Window, 0, true );
	}
}

// Расчитываем возможный ход из поля fieldIdx.
// В calculatedTurn находится последовательность уже сделанных в данном ходу переходов.
void CCheckersEngine::calculatePossibleTurnsForField( int fieldIdx )
{
	if( isWhiteTurn ) {
		ally = FC_WhiteChecker;
		enemy = FC_BlackChecker;
	} else {
		ally = FC_BlackChecker;
		enemy = FC_WhiteChecker;
	}
	if( !shortcutPlayBoard[fieldIdx].second ) {
		handleNonKingTurn( fieldIdx, std::deque<int>() );
	} else {
		handleKingTurn( fieldIdx, std::deque<int>() );
	}
		
}

// Получить элемент отображения calculatedNeighbourFields, связанный с клеткой fieldIdx.
const std::vector< std::vector<int> >& CCheckersEngine::calculateNeighbourFields( int fieldIdx ) const
{
	if( calculatedNeighbourFields.find( fieldIdx ) == calculatedNeighbourFields.end() ) {
		int numberOfCheckersInRow = board.BoardSize / 2;
		// Для удобства вычислений переводим номер клетки, пару номеров, которая бы соответствовала
		// клетке в квадратной матрице размером BoardSize * BoardSize.
		int i = fieldIdx / numberOfCheckersInRow;
		int j = ( fieldIdx % numberOfCheckersInRow ) * 2 + ( ( fieldIdx / numberOfCheckersInRow + 1 ) % 2 );
		std::vector<int> currentDiag;

		for( int k = 1; k < std::min<int>( i + 1, j + 1 ); ++k ) {
			currentDiag.push_back( ( i - k ) * numberOfCheckersInRow + ( j - k ) / 2 );
		}
		if( !currentDiag.empty() ) {
			calculatedNeighbourFields[fieldIdx].push_back( currentDiag );
			currentDiag.clear();
		}

		for( int k = 1; k < std::min<int>( i + 1, board.BoardSize - j ); ++k ) {
			currentDiag.push_back( ( i - k ) * numberOfCheckersInRow + ( j + k ) / 2 );
		}
		if( !currentDiag.empty() ) {
			calculatedNeighbourFields[fieldIdx].push_back( currentDiag );
			currentDiag.clear();
		}

		for( int k = 1; k < std::min<int>( board.BoardSize - i, j + 1 ); ++k ) {
			currentDiag.push_back( ( i + k ) * numberOfCheckersInRow + ( j - k ) / 2 );
		}
		if( !currentDiag.empty() ) {
			calculatedNeighbourFields[fieldIdx].push_back( currentDiag );
			currentDiag.clear();
		}

		for( int k = 1; k < std::min<int>( board.BoardSize - i, board.BoardSize - j ); ++k ) {
			currentDiag.push_back( ( i + k ) * numberOfCheckersInRow + ( j + k ) / 2 );
		}
		if( !currentDiag.empty() ) {
			calculatedNeighbourFields[fieldIdx].push_back( currentDiag );
			currentDiag.clear();
		}
	}
	return calculatedNeighbourFields[fieldIdx];
}

// Попытка добавить к возможным ходам ход, описываемыый последовательностью calculatedTurn.
// Ход невозможно добавить, если уже есть ходы, в которых происходит больше взятий, чем в описанном.
void CCheckersEngine::tryAddTurn( int fromField, std::deque<int>& calculatedTurn )
{
	// Если размер последовательности больше 1, то есть взятия 
	// и значит первый элемент в последовательности - поле, из которого начинается ход. Оно нам больше не нужно.
	if( calculatedTurn.size() > 1 ) {
		isTurnHasTakings = true;
	}
	// Проверяем, что в возможные ходы в соответствие с правилами попадут лишь ходы с наибольшим числом взятий.
	if( possibleTurns.empty() || possibleTurns.begin()->second.begin()->size() == calculatedTurn.size() ) {
		possibleTurns[fromField].push_back( calculatedTurn );
	} else if( possibleTurns.begin()->second.begin()->size() < calculatedTurn.size() ) {
		possibleTurns.clear();
		possibleTurns[fromField].push_back( calculatedTurn );
	}
}

void CCheckersEngine::handleNonKingTurn( int fieldIdx, std::deque<int>& calculatedTurn )
{
	const std::vector< std::vector<int> >& neighbours = calculateNeighbourFields( fieldIdx );
	bool IsTriedToAddTurn = false;
	for( size_t i = 0; i < neighbours.size(); ++i ) {
		if( shortcutPlayBoard[neighbours[i][0]].first == FC_Empty && calculatedTurn.empty() 
			&& ( ( neighbours[i][0] < fieldIdx && isWhiteTurn ) || ( neighbours[i][0] > fieldIdx && !isWhiteTurn ) ) ) {
			calculatedTurn.push_back( neighbours[i][0] );
			tryAddTurn( fieldIdx, calculatedTurn );
			IsTriedToAddTurn = true;
			calculatedTurn.pop_back();
		} else if( shortcutPlayBoard[neighbours[i][0]].first == enemy && neighbours[i].size() > 1 && shortcutPlayBoard[neighbours[i][1]].first == FC_Empty ) {
			IsTriedToAddTurn = true;
			if( calculatedTurn.empty() ) {
				calculatedTurn.push_back( fieldIdx );
			}
			calculatedTurn.push_back( neighbours[i][0] );
			calculatedTurn.push_back( neighbours[i][1] );
			shortcutPlayBoard[neighbours[i][0]].first = FC_Empty;
			std::swap( shortcutPlayBoard[fieldIdx], shortcutPlayBoard[neighbours[i][1]] );
			handleNonKingTurn( neighbours[i][1], calculatedTurn );
			std::swap( shortcutPlayBoard[fieldIdx], shortcutPlayBoard[neighbours[i][1]] );
			shortcutPlayBoard[neighbours[i][0]].first = enemy;
			calculatedTurn.pop_back();
			calculatedTurn.pop_back();
		}
	}
	if( !IsTriedToAddTurn && !calculatedTurn.empty() ) {
		tryAddTurn( calculatedTurn[0], calculatedTurn );
	}
}

void CCheckersEngine::handleKingTurn( int fieldIdx, std::deque<int>& calculatedTurn )
{
	const std::vector< std::vector<int> >& neighbours = calculateNeighbourFields( fieldIdx );
	bool IsTriedToAddTurn = false;
	for( size_t i = 0; i < neighbours.size(); ++i ) {
		int metEnemyIdx = -1;
		for( size_t j = 0; j < neighbours[i].size(); ++j ) {
			if( shortcutPlayBoard[neighbours[i][j]].first == ally 
				|| ( shortcutPlayBoard[neighbours[i][j]].first == enemy && metEnemyIdx != -1 ) ) {
				break;
			} else if( shortcutPlayBoard[neighbours[i][j]].first == enemy ) {
				metEnemyIdx = neighbours[i][j];
			} else if( calculatedTurn.empty() && shortcutPlayBoard[neighbours[i][j]].first == FC_Empty && metEnemyIdx == -1 ) {
				calculatedTurn.push_back( neighbours[i][j] );
				tryAddTurn( fieldIdx, calculatedTurn );
				IsTriedToAddTurn = true;
				calculatedTurn.pop_back();
			} else if( shortcutPlayBoard[neighbours[i][j]].first == FC_Empty && metEnemyIdx != -1 ) {
				IsTriedToAddTurn = true;
				if( calculatedTurn.empty() ) {
					calculatedTurn.push_back( fieldIdx );
				}
				calculatedTurn.push_back( metEnemyIdx );
				calculatedTurn.push_back( neighbours[i][j] );
				shortcutPlayBoard[metEnemyIdx].first = FC_Empty;
				std::swap( shortcutPlayBoard[fieldIdx], shortcutPlayBoard[neighbours[i][j]] );
				handleKingTurn( neighbours[i][j], calculatedTurn );
				std::swap( shortcutPlayBoard[fieldIdx], shortcutPlayBoard[neighbours[i][j]] );
				shortcutPlayBoard[metEnemyIdx].first = enemy;
				calculatedTurn.pop_back();
				calculatedTurn.pop_back();
			}
		}
	}
	if( !IsTriedToAddTurn && !calculatedTurn.empty() ) {
		tryAddTurn( calculatedTurn[0], calculatedTurn );
	}
}