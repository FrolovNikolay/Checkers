// јвтор: ‘ролов Ќиколай.

// ќписание: класс реализующий концепцию игровой доски.

#pragma once

#include <Checker.h>
#include <vector>

class CBoard {
public:
	CBoard();
	~CBoard();

	const std::vector<CChecker*>& GetBoard() const { return board; };

	// —овершить ход или его часть(в случае, если происходит множественное вз€тие) передвинув шашку с позиции from на позицию to.
	void MakeTurn( int from, int to );
	// ¬ернуть доску в стартовое состо€ние дл€ начала новой игры.
	void Reset();

private:
	// –азмер доски.
	static const size_t boardSize = 10;
	//  оличество шашек у каждого игрока в начальный момент.
	static const size_t startNumberOfCheckers = 20;

	// ќписывает поле дл€ игры - в игре участвует всего 50 клеток, которые нумеруютс€ в соответствии с определенными правилами.
	std::vector<CChecker*> board;

	// ƒобавить на поле новые шашки.
	void createNewCheckers();

	// ќчистить поле от шашек.
	void deleteAllCheckers();
};