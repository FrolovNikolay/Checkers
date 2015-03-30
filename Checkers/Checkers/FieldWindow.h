// Автор: Фролов Николай.

// Описание: класс дочернего окна, отвечающего за работу одного поля доски.

#pragma once

#include <Windows.h>
#include <Checker.h>
#include <FieldDrawer.h>

class CFieldWindow {
public:
	CFieldWindow( const CChecker* const & field );

	// регистрация класса окна
    static bool RegisterClass();

    // создание экземпляра окна
    bool Create( HWND parent, int x, int y, int cx, int cy );

    // показать окно
    void Show( int cmdShow ) const;

protected:
    void OnDestroy() const;

	void OnPaint() const;

private:
    HWND handle;

	static const CFieldDrawer drawer;

	const CChecker* const & windowField;

    static LRESULT __stdcall fieldWindowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};