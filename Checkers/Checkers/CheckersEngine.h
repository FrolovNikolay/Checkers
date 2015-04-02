// Автор: Фролов Николай.

// Описание: класс, отвечающий за логику игры в партии.

#pragma once

#include <Board.h>

#include <map>
#include <vector>
#include <list>

enum GameResult {
	GR_StillPlaying,
	GR_WhiteWon,
	GR_BlackWon,
	GR_Draw
};

class CCheckersEngine {
public:
	CCheckersEngine( CBoard& _board );

	void StartGame();

	void TryTurn( int from, int to );

	void AddFocus( int fieldIdx );

	void DelFocus( int fieldIdx );
	
private:
	CBoard& board;
	bool isWhiteTurn;
	GameResult result;

	std::map< int, std::list< std::vector<int> > > possibleTurns;
	std::vector< std::pair<FieldCondition, bool> > shortcutPlayBoard;

	bool isGameFinished();

	void calculateNextTurn();

	void calculatePossibleTurnsForField( int fieldNumber );
};