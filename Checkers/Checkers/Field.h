// Автор: Фролов Николай.

// Описание: структура, реализующая концепцию одного игрового поля.

#pragma once

enum FieldCondition {
	FC_Empty,
	FC_WhiteChecker,
	FC_BlackChecker
};

struct CField {
public:
	CField( int name, FieldCondition condition = FC_Empty, bool isKing = false, bool hasBorder = false )
		: Name( name )
		, Condition( condition )
		, IsKing( isKing )
		, HasBorder( hasBorder )
	{ }

	// Номер поля на доске.
	int Name;
	// Отвечает за наличие/отсутствие фигур в данном поле.
	FieldCondition Condition;
	// Флаг отвечает за то, является ли данная шашка дамкой.
	bool IsKing;
	// Выделено ли данное поле, как одно из возможных для хода.
	bool HasBorder;
};