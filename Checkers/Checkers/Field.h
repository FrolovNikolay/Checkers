// Автор: Фролов Николай.

// Описание: структура, реализующая концепцию одного игрового поля.

#pragma once

#include <Windows.h>

enum FieldCondition {
	FC_Empty,
	FC_WhiteChecker,
	FC_BlackChecker
};

struct CField {
public:
	CField( int _Name, FieldCondition _Condition = FC_Empty, bool _IsKing = false, bool _HasBorder = false,
			HWND window = 0 )
		: Name( _Name )
		, Condition( _Condition )
		, IsKing( _IsKing )
		, HasBorder( _HasBorder )
	{ }

	// Номер поля на доске.
	int Name;
	// Отвечает за наличие/отсутствие фигур в данном поле.
	FieldCondition Condition;
	// Флаг отвечает за то, является ли данная шашка дамкой.
	bool IsKing;
	// Выделено ли данное поле, как одно из возможных для хода.
	bool HasBorder;
	// Окно, в котором отображается данное поле.
	HWND Window;
};