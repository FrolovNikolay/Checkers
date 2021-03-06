﻿// Автор: Фролов Николай.

// Описание: класс реализующий концепцию игровой доски.

#pragma once

#include <Field.h>

#include <vector>

class CBoard {
public:
	// Размер доски.
	const int BoardSize;

	CBoard( int _BoardSize = 10, int _startNumberOfCheckers = 20 );

	std::vector<CField>& GetBoard() { return playBoard; };

	// Вернуть доску в стартовое состояние для начала новой игры.
	void Reset();

private:
	
	// Количество шашек у каждого игрока в начальный момент игры.
	const int startNumberOfCheckers;
	// Описывает поле для игры, содержит описания только черных, которые нумеруются в соответствии с определенными правилами.
	std::vector<CField> playBoard;

	// Создание доски для новой игры.
	void generateNewBoard();
};