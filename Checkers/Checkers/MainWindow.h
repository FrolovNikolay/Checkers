// Автор: Фролов Николай.

// Описание: класс главного окна игры.

#pragma once

#include <Windows.h>

class CMainWindow {
public:
	// регистрация класса окна
    static bool RegisterClass();

    // создание экземпляра окна
    bool Create();

    // показать окно
    void Show(int cmdShow) const;

protected:
    void OnDestroy() const;

private:
    HWND handle;

    static LRESULT __stdcall windowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};