// Автор: Фролов Николай.

// Описание: класс реализующий концепцию игровой доски.

#pragma once

#include <Field.h>
#include <vector>

class CBoard {
public:
	// Размер доски.
	const int BoardSize;

	CBoard( size_t _BoardSize = 10, size_t _startNumberOfCheckers = 20 );

	std::vector<CField>& GetBoard() { return playBoard; };

	// Вернуть доску в стартовое состояние для начала новой игры.
	void Reset();

private:
	
	// Количество шашек у каждого игрока в начальный момент игры.
	const size_t startNumberOfCheckers;

	// Описывает поле для игры, содержит описания только черных, которые нумеруются в соответствии с определенными правилами.
	std::vector<CField> playBoard;

	void generateNewBoard();
};