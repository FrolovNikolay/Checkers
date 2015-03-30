// Автор: Фролов Николай.

// Описание: класс, отвечащий за отрисовку полей доски.

#pragma once

#include <Checker.h>
#include <Windows.h>

class CFieldDrawer {
public:
	CFieldDrawer();
	~CFieldDrawer();

	void DrawField( HWND window, const CChecker* const field ) const;

private:
	// Для отрисовки фигур.
	HBRUSH whiteBrush;
	HBRUSH kingWhiteBrush;
	HBRUSH blackBrush;
	HBRUSH kingBlackBrush;
	// Фон черных клеток.
	HBRUSH backgroundBrush;
	// Отступ, при отрисовке дамок.
	static const int indent = 5;
};