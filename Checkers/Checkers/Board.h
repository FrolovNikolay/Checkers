// Автор: Фролов Николай.

// Описание: класс реализующий концепцию игровой доски.

#pragma once

#include <Field.h>
#include <vector>

class CBoard {
public:
	// Размер доски.
	static const size_t BoardSize = 10;

	CBoard();

	std::vector<CField>& GetBoard() { return playBoard; };

	// Вернуть доску в стартовое состояние для начала новой игры.
	void Reset();

private:
	// Количество шашек у каждого игрока в начальный момент.
	static const size_t startNumberOfCheckers = 20;

	// Описывает поле для игры - в игре участвует всего 50 клеток, которые нумеруются в соответствии с определенными правилами.
	std::vector<CField> playBoard;

	void generateNewBoard();
};