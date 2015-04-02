// Автор: Фролов Николай.

#include <MainWindow.h>

CMainWindow::CMainWindow()
	: board( boardSize, startNumberOfCheckers )
	, fields()
	, focusedWindowIdx( -1 )
	, engine( board )
{
	for( size_t  i = 0; i < board.BoardSize * board.BoardSize / 2; ++i ) {
		fields.push_back( CFieldWindow( board.GetBoard()[i], focusedWindowIdx, engine ) );
	}
}

bool CMainWindow::RegisterClass()
{
    WNDCLASSEX windowWND;

    windowWND.cbSize = sizeof( WNDCLASSEX );
    windowWND.style = CS_HREDRAW | CS_VREDRAW;
    windowWND.lpfnWndProc = mainWindowProc;
    windowWND.cbClsExtra = 0;
    windowWND.cbWndExtra = 0;
    windowWND.hInstance = static_cast<HINSTANCE>( GetModuleHandle( 0 ) );
    windowWND.hIcon = 0;
    windowWND.hCursor = ::LoadCursor( 0, IDC_ARROW );
    windowWND.hbrBackground = ::CreateSolidBrush( RGB( 207, 236, 255 ) );
    windowWND.lpszMenuName = 0;
    windowWND.lpszClassName = L"CMainWindow";
    windowWND.hIconSm = 0;

    return ::RegisterClassEx( &windowWND ) != 0;
}

bool CMainWindow::Create()
{
    handle = ::CreateWindowEx( 0, L"CMainWindow", L"Checkers", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, width, height, 0, 0, static_cast<HINSTANCE>( ::GetModuleHandle( 0 ) ), this );

    return handle != 0;
}

void CMainWindow::Show( int cmdShow ) const
{
    ::ShowWindow( handle, cmdShow );
	for( size_t i = 0; i < fields.size(); ++i ) {
		fields[i].Show( cmdShow );
	}
}

void CMainWindow::OnDestroy() const
{
    ::PostQuitMessage( 0 );
}

// Генерация дочерних окон на основе массива игровых полей.
void CMainWindow::createChildren( HWND hwnd )
{
	size_t numberOfCheckersInOneLine = boardSize / 2;
	for( size_t i = 0; i < fields.size(); ++i ) {
		int xStart = ( ( i % numberOfCheckersInOneLine ) * 2 + ( ( i / numberOfCheckersInOneLine + 1 ) % 2 ) ) * fieldSize;
		int yStart = i / numberOfCheckersInOneLine * fieldSize;
		fields[i].Create( hwnd, xStart, yStart, fieldSize, fieldSize );
	}
}

LRESULT __stdcall CMainWindow::mainWindowProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	CMainWindow* window = reinterpret_cast<CMainWindow*>( ::GetWindowLong( hwnd, GWL_USERDATA ) );

    switch( message ) {
		case WM_NCCREATE:
			::SetWindowLong( hwnd, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
			return ::DefWindowProc( hwnd, message, wParam, lParam );
			break;
		case WM_CREATE:
            window->createChildren( hwnd );
			return 1;
		case WM_DESTROY:
			window->OnDestroy();
			break;
		default:
			return ::DefWindowProc( hwnd, message, wParam, lParam );
			break;
    }
    return 0;
}