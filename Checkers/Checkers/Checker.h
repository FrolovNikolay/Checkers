// �����: ������ �������.

// ��������: �����, ����������� ��������� ����� �����.

#pragma once

class CChecker {
public:
	CChecker( bool _isWhite = true, bool _isKing = false )
		: isWhite( _isWhite )
		, isKing( _isKing )
	{ }

	bool IsWhite() const { return isWhite; }

	bool IsKing() const { return isKing; }

private:
	// ���� �������� �� ����(true - �����, false - ������).
	bool isWhite;

	// ���� �������� �� ��, �������� �� ������ ����� ������.
	bool isKing;
};