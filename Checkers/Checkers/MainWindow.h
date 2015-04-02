// Автор: Фролов Николай.

// Описание: класс главного окна игры.

#pragma once

#include <FieldWindow.h>
#include <Board.h>
#include <CheckersEngine.h>

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
	// Описатель данного окна.
    HWND handle;

	// Размер одного поля.
	static const int fieldSize = 65;
	// Определяет размеры игровой доски.
	static const int boardSize = 10;
	// Количество шашек у каждого игрока в начале игры.
	static const int startNumberOfCheckers = 20;
	// Высота и ширина окна.
	static const int height = boardSize * fieldSize + 25;
	static const int width = boardSize * fieldSize + 5;
	
	// Игровое поле.
	CBoard board;
	// Поля, которые учавствуют в игре.
	std::vector<CFieldWindow> fields;

	CCheckersEngine engine;

	int focusedWindowIdx;

	void createChildren( HWND hwnd );

    static LRESULT __stdcall mainWindowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};