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
			calculatePossibleTurnsForField( i, std::deque<int>() );
		}
	}
	for( std::map< int, std::list< std::deque<int> > >::iterator iter = possibleTurns.begin(); iter != possibleTurns.end(); ++iter ) {
		if( iter->first >= 0 ) {
			playBoard[iter->first].HasBorder = true;
		}
		::InvalidateRect( playBoard[iter->first].Window, 0, true );
	}
}

// Расчитываем возможный ход из поля fieldNumber.
// В calculatedTurn находится последовательность уже сделанных в данном ходу переходов.
void CCheckersEngine::calculatePossibleTurnsForField( int fieldNumber, std::deque<int>& calculatedTurn )
{
	if( !shortcutPlayBoard[fieldNumber].second ) {
		std::vector< std::pair<int, int> > neighbours = calculateNonKingNeighbourFields( fieldNumber );
		if( shortcutPlayBoard[fieldNumber].first == FC_WhiteChecker ) {
			bool IsTriedToAddTurn = false;
			for( size_t i = 0; i < neighbours.size(); ++i ) {
				if( shortcutPlayBoard[neighbours[i].first].first == FC_Empty && calculatedTurn.empty() && neighbours[i].first < fieldNumber ) {
					calculatedTurn.push_back( neighbours[i].first );
					tryAddTurn( fieldNumber, calculatedTurn );
					IsTriedToAddTurn = true;
					calculatedTurn.pop_back();
				} else if( shortcutPlayBoard[neighbours[i].first].first == FC_BlackChecker && neighbours[i].second != -1 && shortcutPlayBoard[neighbours[i].second].first == FC_Empty ) {
					IsTriedToAddTurn = true;
					if( calculatedTurn.empty() ) {
						calculatedTurn.push_back( fieldNumber );
					}
					calculatedTurn.push_back( neighbours[i].first );
					calculatedTurn.push_back( neighbours[i].second );
					shortcutPlayBoard[neighbours[i].first].first = FC_Empty;
					std::swap( shortcutPlayBoard[fieldNumber], shortcutPlayBoard[neighbours[i].second] );
					calculatePossibleTurnsForField( neighbours[i].second, calculatedTurn );
					std::swap( shortcutPlayBoard[fieldNumber], shortcutPlayBoard[neighbours[i].second] );
					shortcutPlayBoard[neighbours[i].first].first = FC_BlackChecker;
					calculatedTurn.pop_back();
					calculatedTurn.pop_back();
				}
			}
			if( !IsTriedToAddTurn && !calculatedTurn.empty() ) {
				tryAddTurn( calculatedTurn[0], calculatedTurn );
			}
		} else if( shortcutPlayBoard[fieldNumber].first == FC_BlackChecker ) {
			bool IsTriedToAddTurn = false;
			for( size_t i = 0; i < neighbours.size(); ++i ) {
				if( shortcutPlayBoard[neighbours[i].first].first == FC_Empty && calculatedTurn.empty() && neighbours[i].first > fieldNumber ) {
					calculatedTurn.push_back( neighbours[i].first );
					tryAddTurn( fieldNumber, calculatedTurn );
					IsTriedToAddTurn = true;
					calculatedTurn.pop_back();
				} else if( shortcutPlayBoard[neighbours[i].first].first == FC_WhiteChecker && neighbours[i].second != -1 && shortcutPlayBoard[neighbours[i].second].first == FC_Empty ) {
					IsTriedToAddTurn = true;
					if( calculatedTurn.empty() ) {
						calculatedTurn.push_back( fieldNumber );
					}
					calculatedTurn.push_back( neighbours[i].first );
					calculatedTurn.push_back( neighbours[i].second );
					shortcutPlayBoard[neighbours[i].first].first = FC_Empty;
					std::swap( shortcutPlayBoard[fieldNumber], shortcutPlayBoard[neighbours[i].second] );
					calculatePossibleTurnsForField( neighbours[i].second, calculatedTurn );
					std::swap( shortcutPlayBoard[fieldNumber], shortcutPlayBoard[neighbours[i].second] );
					shortcutPlayBoard[neighbours[i].first].first = FC_WhiteChecker;
					calculatedTurn.pop_back();
				}
			}
			if( !IsTriedToAddTurn && !calculatedTurn.empty() ) {
				tryAddTurn( calculatedTurn[0], calculatedTurn );
			}
		}
	} else {
	}
}

// Получить элемент отображения calculatedNonKingNeighbourFields, связанный с клеткой fieldNumber.
const std::vector< std::pair<int, int> >& CCheckersEngine::calculateNonKingNeighbourFields( int fieldNumber ) const
{
	if( calculatedNonKingNeighbourFields.find( fieldNumber ) == calculatedNonKingNeighbourFields.end() ) {
		int numberOfCheckersInRow = board.BoardSize / 2;
		// Для удобства вычислений переводим номер клетки, пару номеров, которая бы соответствовала
		// клетке в квадратной матрице размером BoardSize * BoardSize.
		int i = fieldNumber / numberOfCheckersInRow;
		int j = ( fieldNumber % numberOfCheckersInRow ) * 2 + ( ( fieldNumber / numberOfCheckersInRow + 1 ) % 2 );
		if( i - 2 >= 0 && j - 2 >= 0 ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i - 1 ) * numberOfCheckersInRow + ( j - 1 ) / 2,
				( i - 2 ) * numberOfCheckersInRow + ( j - 2 ) / 2 ) );
		} else if( i - 1 >= 0 && j - 1 >= 0 ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i - 1 ) * numberOfCheckersInRow + ( j - 1 ) / 2, -1 ) );
		}
		if( i - 2 >= 0 && j + 2 < board.BoardSize ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i - 1 ) * numberOfCheckersInRow + ( j + 1 ) / 2,
				( i - 2 ) * numberOfCheckersInRow + ( j + 2 ) / 2 ) );
		} else if( i - 1 >= 0 && j + 1 < board.BoardSize ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i - 1 ) * numberOfCheckersInRow + ( j + 1 ) / 2, -1 ) );
		}
		if( i + 2 < board.BoardSize && j - 2 >= 0 ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i + 1 ) * numberOfCheckersInRow + ( j - 1 ) / 2,
				( i + 2 ) * numberOfCheckersInRow + ( j - 2 ) / 2 ) );
		} else if( i + 1 < board.BoardSize && j - 1 >= 0 ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i + 1 ) * numberOfCheckersInRow + ( j - 1 ) / 2, -1 ) );
		}
		if( i + 2 < board.BoardSize && j + 2 < board.BoardSize ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i + 1 ) * numberOfCheckersInRow + ( j + 1 ) / 2,
				( i + 2 ) * numberOfCheckersInRow + ( j + 2 ) / 2 ) );
		} else if( i + 1 < board.BoardSize && j + 1 < board.BoardSize ) {
			calculatedNonKingNeighbourFields[fieldNumber].push_back( std::pair<int, int>( ( i + 1 ) * numberOfCheckersInRow + ( j + 1 ) / 2, -1 ) );
		}
	}
	return calculatedNonKingNeighbourFields[fieldNumber];
}

// Попытка добавить к возможным ходам ход, описываемыый последовательностью calculatedTurn.
// Ход невозможно добавить, если уже есть ходы, в которых происходит больше взятий, чем в описанном.
void CCheckersEngine::tryAddTurn( int fromField, std::deque<int>& calculatedTurn )
{
	// Если размер последовательности больше 1, то есть взятия 
	// и значит первый элемент в последовательности - поле, из которого начинается ход. Оно нам больше не нужно.
	if( calculatedTurn.size() > 1 ) {
		isTurnHasTakings = true;
		calculatedTurn.pop_front();
	}
	// Проверяем, что в возможные ходы в соответствие с правилами попадут лишь ходы с наибольшим числом взятий.
	if( possibleTurns.empty() || possibleTurns.begin()->second.begin()->size() == calculatedTurn.size() ) {
		possibleTurns[fromField].push_back( calculatedTurn );
	} else if( possibleTurns.begin()->second.begin()->size() < calculatedTurn.size() ) {
		possibleTurns.clear();
		possibleTurns[fromField].push_back( calculatedTurn );
	}
}