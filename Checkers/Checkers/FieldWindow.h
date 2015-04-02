// Автор: Фролов Николай.

// Описание: класс дочернего окна, отвечающего за работу одного поля доски.

#pragma once

#include <Windows.h>
#include <Field.h>
#include <FieldDrawer.h>

class CFieldWindow {
public:
	CFieldWindow( const CField& field );
	// Регистрация класса окна.
    static bool RegisterClass();

    // Создание экземпляра окна.
    bool Create( HWND parent, int x, int y, int cx, int cy );

    // Показать окно.
    void Show( int cmdShow ) const;

protected:
    void OnDestroy() const;

	void OnPaint() const;

private:
	// Описатель данного окна.
    HWND handle;

	// Класс-механизм отрисовки для данного типа окон
	static const CFieldDrawer drawer;
	// Соответствующее данному окну игровое поле.
	const CField& windowField;

    static LRESULT __stdcall fieldWindowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};