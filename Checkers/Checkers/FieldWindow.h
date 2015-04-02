// Автор: Фролов Николай.

// Описание: класс дочернего окна, отвечающего за работу одного поля доски.

#pragma once

#include <Field.h>
#include <FieldDrawer.h>
#include <CheckersEngine.h>

#include <Windows.h>

class CFieldWindow {
public:
	CFieldWindow( CField& field, int& _focusedWindowIdx, CCheckersEngine& engine );
	// Регистрация класса окна.
    static bool RegisterClass();

    // Создание экземпляра окна.
    bool Create( HWND parent, int x, int y, int cx, int cy );

    // Показать окно.
    void Show( int cmdShow ) const;

protected:
    void OnDestroy() const;

	void OnPaint() const;

	void OnLButtonDown() const;

	void OnKillFocus() const;

private:
	// Описатель данного окна.
    HWND handle;

	// Класс-механизм отрисовки для данного типа окон
	static const CFieldDrawer drawer;
	// Соответствующее данному окну игровое поле.
	CField& windowField;

	mutable int& focusedWindowIdx;

	CCheckersEngine& engine;

    static LRESULT __stdcall fieldWindowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};