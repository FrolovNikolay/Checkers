// Автор: Фролов Николай.

// Описание: класс реализующий концепцию игровой доски.

#pragma once

#include <Checker.h>
#include <vector>

class CBoard {
public:
	// Размер доски.
	static const size_t BoardSize = 10;

	CBoard();
	~CBoard();

	const std::vector<CChecker*>& GetBoard() const { return board; };

	// Совершить ход или его часть(в случае, если происходит множественное взятие) передвинув шашку с позиции from на позицию to.
	void MakeTurn( int from, int to );
	// Вернуть доску в стартовое состояние для начала новой игры.
	void Reset();

private:
	// Количество шашек у каждого игрока в начальный момент.
	static const size_t startNumberOfCheckers = 20;

	// Описывает поле для игры - в игре участвует всего 50 клеток, которые нумеруются в соответствии с определенными правилами.
	std::vector<CChecker*> board;

	// Добавить на поле новые шашки.
	void createNewCheckers();

	// Очистить поле от шашек.
	void deleteAllCheckers();
};