// Автор: Фролов Николай.

// Описание: класс дочернего окна, отвечающего за работу одного поля доски.

#pragma once

#include <Windows.h>

class CFieldWindow {
public:
	// регистрация класса окна
    static bool RegisterClass();

    // создание экземпляра окна
    bool Create( HWND parent, int x, int y, int cx, int cy );

    // показать окно
    void Show( int cmdShow ) const;

protected:
    void OnDestroy() const;

private:
    HWND handle;

    static LRESULT __stdcall windowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};