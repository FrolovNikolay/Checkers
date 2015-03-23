// Автор: Фролов Николай.

#include <MainWindow.h>

bool CMainWindow::RegisterClass()
{
    WNDCLASSEX windowWND;

    windowWND.cbSize = sizeof( WNDCLASSEX );
    windowWND.style = CS_HREDRAW | CS_VREDRAW;
    windowWND.lpfnWndProc = windowProc;
    windowWND.cbClsExtra = 0;
    windowWND.cbWndExtra = 0;
    windowWND.hInstance = static_cast<HINSTANCE>( GetModuleHandle( 0 ) );
    windowWND.hIcon = 0;
    windowWND.hCursor = ::LoadCursor( 0, IDC_ARROW );
    windowWND.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_GRAYTEXT + 1 );
    windowWND.lpszMenuName = 0;
    windowWND.lpszClassName = L"CMainWindow";
    windowWND.hIconSm = 0;

    return ::RegisterClassEx( &windowWND ) != 0;

}

bool CMainWindow::Create()
{
    handle = ::CreateWindowEx( 0, L"CMainWindow", L"Checkers", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, static_cast<HINSTANCE>( ::GetModuleHandle( 0 ) ), this );

    return handle != 0;
}

void CMainWindow::Show( int cmdShow ) const
{
    ::ShowWindow( handle, cmdShow );
}

void CMainWindow::OnDestroy() const
{
    ::PostQuitMessage( 0 );
}

LRESULT __stdcall CMainWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message ) {
		case WM_NCCREATE:
			::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
			return ::DefWindowProc( handle, message, wParam, lParam );
			break;
		case WM_DESTROY:
			reinterpret_cast<CMainWindow*>( ::GetWindowLong( handle, GWL_USERDATA ) )->OnDestroy();
			break;
		default:
			return ::DefWindowProc( handle, message, wParam, lParam );
			break;
    }
    return 0;
}