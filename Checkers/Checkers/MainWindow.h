// Автор: Фролов Николай.

// Описание: класс главного окна игры.

#pragma once

#include <FieldWindow.h>
#include <Board.h>

#include <Windows.h>
#include <vector>

class CMainWindow {
public:
	CMainWindow();

	// регистрация класса окна
    static bool RegisterClass();

    // создание экземпляра окна
    bool Create();

    // показать окно
    void Show( int cmdShow ) const;

protected:
    void OnDestroy() const;

private:
    HWND handle;

	static const int fieldSize = 60;
	static const int height = CBoard::BoardSize * fieldSize + 30;
	static const int width = CBoard::BoardSize * fieldSize + 200;
	
	// Игровое поле.
	CBoard board;
	// Поля, которые учавствуют в игре.
	std::vector<CFieldWindow> fields;

	void createChildren( HWND hwnd );

    static LRESULT __stdcall windowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};