﻿// Автор: Фролов Николай.

#include <FieldWindow.h>

bool CFieldWindow::RegisterClass()
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
	windowWND.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_GRAYTEXT + 1);
	windowWND.lpszMenuName = 0;
	windowWND.lpszClassName = L"CFieldWindow";
	windowWND.hIconSm = 0;

	return ::RegisterClassEx( &windowWND ) != 0;
}

bool CFieldWindow::Create( HWND parent, int x, int y, int cx, int cy )
{
	handle = ::CreateWindowEx( 0, L"CFieldWindow", L"CFieldWindow", WS_CHILD, x, y, cx, cy, parent, 0,
		static_cast<HINSTANCE>( ::GetModuleHandle( 0 ) ), this );

	return handle != 0;
}

void CFieldWindow::Show( int cmdShow ) const
{
	::ShowWindow( handle, cmdShow );
}

void CFieldWindow::OnDestroy() const 
{
	::PostQuitMessage( 0 );
}

LRESULT __stdcall CFieldWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	CFieldWindow* window = reinterpret_cast<CFieldWindow*>( ::GetWindowLong( handle, GWL_USERDATA ) );
	switch( message ) {
		case WM_NCCREATE:
			::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>( reinterpret_cast<CREATESTRUCT*>( lParam )->lpCreateParams ) );
			return 1;
			break;
		case WM_DESTROY:
			window->OnDestroy();
			break;
		default:
			return ::DefWindowProc( handle, message, wParam, lParam );
			break;
	}
	return 0;
}