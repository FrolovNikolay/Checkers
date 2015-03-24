// �����: ������ �������.

// ��������: ����� ����������� ��������� ������� �����.

#pragma once

#include <Checker.h>
#include <vector>

class CBoard {
public:
	CBoard();
	~CBoard();

	const std::vector<CChecker*>& GetBoard() const { return board; };

	// ��������� ��� ��� ��� �����(� ������, ���� ���������� ������������� ������) ���������� ����� � ������� from �� ������� to.
	void MakeTurn( int from, int to );
	// ������� ����� � ��������� ��������� ��� ������ ����� ����.
	void Reset();

private:
	// ������ �����.
	static const size_t boardSize = 10;
	// ���������� ����� � ������� ������ � ��������� ������.
	static const size_t startNumberOfCheckers = 20;

	// ��������� ���� ��� ���� - � ���� ��������� ����� 50 ������, ������� ���������� � ������������ � ������������� ���������.
	std::vector<CChecker*> board;

	// �������� �� ���� ����� �����.
	void createNewCheckers();

	// �������� ���� �� �����.
	void deleteAllCheckers();
};