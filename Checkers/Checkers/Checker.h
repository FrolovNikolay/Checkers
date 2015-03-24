// Автор: Фролов Николай.

// Описание: класс, реализующий концепцию одной шашки.

#pragma once

class CChecker {
public:
	CChecker( bool _isWhite = true, bool _isKing = false )
		: isWhite( _isWhite )
		, isKing( _isKing )
	{ }

	bool IsWhite() const { return isWhite; }

	bool IsKing() const { return isKing; }

private:
	// Флаг отвечает за цвет(true - белая, false - чёрная).
	bool isWhite;

	// Флаг отвечает за то, является ли данная шашка дамкой.
	bool isKing;
};