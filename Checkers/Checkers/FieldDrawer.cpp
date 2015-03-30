// Автор: Николай Фролов.

#include <FieldDrawer.h>
#include <cassert>

CFieldDrawer::CFieldDrawer()
{
	whiteBrush = ::CreateSolidBrush( RGB( 204, 204, 204 ) );
	kingWhiteBrush = ::CreateSolidBrush( RGB( 255, 255, 255 ) );
	blackBrush = ::CreateSolidBrush( RGB( 102, 102, 102 ) );
	kingBlackBrush = ::CreateSolidBrush( RGB( 51, 51, 51 ) );
	backgroundBrush = ::CreateSolidBrush( RGB( 85, 102, 114 ) ) ;
}

CFieldDrawer::~CFieldDrawer()
{
	
	::DeleteObject( whiteBrush );
	::DeleteObject( kingWhiteBrush );
	::DeleteObject( blackBrush );
	::DeleteObject( kingBlackBrush );
	::DeleteObject( backgroundBrush );
}

void CFieldDrawer::DrawField( HWND window, const CChecker* const field ) const
{
	PAINTSTRUCT paintInfo;
	HDC displayHandle = ::BeginPaint( window, &paintInfo );
	assert( displayHandle != 0 );

	RECT rectInfo = paintInfo.rcPaint;
	int width = rectInfo.right;
	int height = rectInfo.bottom;

	HDC tempHDC = ::CreateCompatibleDC( displayHandle );
	HBITMAP tempBitmap = ::CreateCompatibleBitmap( displayHandle, width, height );
	HBITMAP oldBitmap = static_cast<HBITMAP>( ::SelectObject( tempHDC, tempBitmap ) );

	HBRUSH oldBgBrush = static_cast<HBRUSH>( ::SelectObject( tempHDC, backgroundBrush ) );
	Rectangle( tempHDC, 0, 0, width, height );
	if( field != 0 ) {
		if( field->IsWhite() ) {
			::SelectObject( tempHDC, whiteBrush );
		} else {
			::SelectObject( tempHDC, blackBrush );
		}
		::Ellipse( tempHDC, 0, 0, width, height );
		if( field->IsKing() ) {
			if( field->IsWhite() ) {
			::SelectObject( tempHDC, kingWhiteBrush );
		} else {
			::SelectObject( tempHDC, kingBlackBrush );
		}
			::Ellipse( tempHDC, indent, indent, width - indent, height - indent );
		}
	}

	::SelectObject( tempHDC, oldBgBrush );
	::BitBlt( displayHandle, 0, 0, width, height, tempHDC, 0, 0, SRCCOPY );

	::DeleteObject( oldBitmap );
	::DeleteObject( tempHDC );
	::DeleteObject( tempBitmap );
	::EndPaint( window, &paintInfo );
}