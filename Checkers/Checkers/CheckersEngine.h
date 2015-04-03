// Автор: Фролов Николай.

// Описание: класс, отвечающий за логику игры в партии.

#pragma once

#include <Board.h>

#include <map>
#include <vector>
#include <deque>
#include <list>

enum GameResult {
	GR_StillPlaying,
	GR_WhiteWon,
	GR_BlackWon,
	GR_Draw
};

class CCheckersEngine {
public:
	CCheckersEngine( CBoard& _board );

	// Запустить начало игры.
	void StartGame();
	// Проверить доступен ли в данный момент ход из клетки from, в клетку to. Если да, то выполнить его.
	void TryTurn( int from, int to );
	// Отметить клетку под номеров fieldIdx как выбранную, и отобразить для нее возможные ходы.
	void AddFocus( int fieldIdx );
	// Отменить выделение для клетки fieldIdx и прекратить отображение возможных из нее ходов.
	void DelFocus( int fieldIdx );
	
private:
	// Описание игровой доски.
	CBoard& board;
	// Определяет чей сейчас ход.
	bool isWhiteTurn;
	// В данную переменную попадает состояние игры.
	GameResult result;

	// Здесь кешируется информация, связанные с вычислением следующих возможных ходов.
	std::map< int, std::list< std::deque<int> > > possibleTurns;
	std::vector< std::pair<FieldCondition, bool> > shortcutPlayBoard;

	// Отображение клеток в пары соседних. 
	// Пары имеют вид - <соседняя клетка, клетка, на которую будет сделан ход, если через соседнюю клетку будет проходить взятие>.
	mutable std::map< int, std::vector< std::pair<int, int> > > calculatedNonKingNeighbourFields;
	// Определяет есть ли доступные взятия в следующем ходу.
	mutable bool isTurnHasTakings;

	// Проверка на то, закончена ли игра.
	bool isGameFinished();
	// Рассчитать возможные следующие ходы.
	void calculateNextTurn();
	// Расчитываем возможный ход из поля fieldNumber.
	// В calculatedTurn находится последовательность уже сделанных в данном ходу переходов.
	void calculatePossibleTurnsForField( int fieldNumber, std::deque<int>& calculatedTurn );

	// Получить элемент отображения calculatedNonKingNeighbourFields, связанный с клеткой fieldNumber.
	const std::vector< std::pair<int, int> >& calculateNonKingNeighbourFields( int fieldNumber ) const;

	// Попытка добавить к возможным ходам ход, описываемыый последовательностью calculatedTurn.
	// Ход невозможно добавить, если уже есть ходы, в которых происходит больше взятий, чем в описанном.
	void tryAddTurn( int fromField, std::deque<int>& calculatedTurn );
};