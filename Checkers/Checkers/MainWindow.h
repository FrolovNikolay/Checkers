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
	// Описатель данного окна.
    HWND handle;

	// Размер одного поля.
	static const int fieldSize = 65;
	// Высота и ширина окна.
	static const int height = CBoard::BoardSize * fieldSize + 30;
	static const int width = CBoard::BoardSize * fieldSize + 200;
	
	// Игровое поле.
	CBoard board;
	// Поля, которые учавствуют в игре.
	std::vector<CFieldWindow> fields;

	void createChildren( HWND hwnd );

    static LRESULT __stdcall mainWindowProc( HWND hanlde, UINT message, WPARAM wParam, LPARAM lParam );
};