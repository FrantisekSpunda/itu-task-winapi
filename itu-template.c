#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Global variable
HINSTANCE hInst;
UINT MessageCount = 0;
UINT Count = 0;
int posX = 0;
int posY = 0;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

COLORREF squareColor = RGB(200, 200, 255); // Výchozí barva čtverce

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  MSG msg;
  BOOL bRet;
  WNDCLASS wcx; // register class
  HWND hWnd;

  hInst = hInstance; // Save the application-instance handle.
                     // Fill in the window class structure with parameters that describe the main window.

  wcx.style = CS_HREDRAW | CS_VREDRAW;             // redraw if size changes
  wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
  wcx.cbClsExtra = 0;                              // no extra class memory
  wcx.cbWndExtra = 0;                              // no extra window memory
  wcx.hInstance = hInstance;                       // handle to instance
  wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);     // predefined app. icon
  wcx.hCursor = LoadCursor(NULL, IDC_ARROW);       // predefined arrow
  wcx.hbrBackground = GetStockObject(WHITE_BRUSH); // white background brush
  wcx.lpszMenuName = (LPCSTR) "MainMenu";          // name of menu resource
  wcx.lpszClassName = (LPCSTR) "MainWClass";       // name of window class

  // Register the window class.

  if (!RegisterClass(&wcx))
    return FALSE;

  // create window of registered class

  hWnd = CreateWindow(
      "MainWClass",        // name of window class
      "ITU",               // title-bar string
      WS_OVERLAPPEDWINDOW, // top-level window
      200,                 // default horizontal position
      25,                  // default vertical position
      1000,                // default width
      700,                 // default height
      (HWND)NULL,          // no owner window
      (HMENU)NULL,         // use class menu
      hInstance,           // handle to application instance
      (LPVOID)NULL);       // no window-creation data
  if (!hWnd)
    return FALSE;

  // Show the window and send a WM_PAINT message to the window procedure.
  // Record the current cursor position.

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  // loop of message processing
  while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
  {
    if (bRet == -1)
    {
      // handle the error and possibly exit
    }
    else
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(
    HWND hWnd,     // handle to window
    UINT uMsg,     // message identifier
    WPARAM wParam, // first message parameter
    LPARAM lParam) // second message parameter
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT cursorPosition;

  // init cursor position
  GetCursorPos(&cursorPosition);
  ScreenToClient(hWnd, &cursorPosition);

  HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 255)); // Modrý štětec
  HPEN hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));  // Modré pero, šířka 3
  HGDIOBJ oldBrush = SelectObject(hDC, hBrush);
  HGDIOBJ oldPen = SelectObject(hDC, hPen);

  switch (uMsg)
  {
  case WM_CREATE:
    break;

  // character input
  case WM_CHAR:
    switch (wParam)
    {
    case 0x08: // backspace
    case 0x0A: // linefeed
    case 0x1B: // escape
      break;

    case 0x09: // tab
      break;

    default:
      break;
    }
    break;

  // key input
  case WM_KEYDOWN:
    switch (wParam)
    {
      // update posX and posY in order to move object
    case VK_LEFT: // left arrow
      break;
    case VK_RIGHT: // right arrow
      break;
    case VK_UP: // up arrow
      break;
    case VK_DOWN: // down arrow
      break;

    // react on the other pressed keys
    case VK_SPACE: // space
      break;
    case VK_BACK: // backspace
      break;
    case VK_TAB: // tab
      break;
      // more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    }
    break;

  // get cursor position
  case WM_MOUSEMOVE:
    // Nastavení posX a posY z proměnných cursorPosition
    posX = cursorPosition.x;
    posY = cursorPosition.y;

    // Vyvolá překreslení okna
    InvalidateRect(hWnd, NULL, TRUE);
    break;

  // react on mouse clicks
  case WM_LBUTTONDOWN:
    squareColor = RGB(0, 0, 0);
    InvalidateRect(hWnd, NULL, TRUE);
    break;
  case WM_LBUTTONUP:
    squareColor = RGB(200, 200, 255);
    InvalidateRect(hWnd, NULL, TRUE);
    break;

  // paint objects
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
    paintPosition(hWnd, hDC, ps);
    // paint other objects
    // paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
    // paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
    EndPaint(hWnd, &ps);
    // DeleteDC(hDC);
    break;

    //
    // Process other messages.
    //

  case WM_CLOSE:
    DestroyWindow(hWnd);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
  char text[40]; // buffer to store an output text
  HFONT font;    // new large font
  HFONT oldFont; // saves the previous font

  font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
  oldFont = (HFONT)SelectObject(hDC, font);
  sprintf(text, "Position -- x:%d, y:%d", posX, posY);
  TextOut(hDC, 50, 600, text, (int)strlen(text));
  SelectObject(hDC, oldFont);
  DeleteObject(font);
}

void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
  // Nastavení barvy pro štětec a pero
  HBRUSH hBrush = CreateSolidBrush(squareColor);      // Použijeme squareColor
  HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // Modré pero, šířka 3
  HGDIOBJ oldBrush = SelectObject(hDC, hBrush);
  HGDIOBJ oldPen = SelectObject(hDC, hPen);

  // Vykreslení obdélníku
  int shift = 0;
  int size = 50;
  Ellipse(hDC, size + posX, size + posY + shift, -size + posX, -size + posY + shift);
  shift += size * 2;
  size = 60;
  Ellipse(hDC, size + posX, size + posY + shift, -size + posX, -size + posY + shift);
  shift += size * 2;
  size = 70;
  Ellipse(hDC, size + posX, size + posY + shift, -size + posX, -size + posY + shift);

  shift = 20;
  size = 5;
  Ellipse(hDC, size + posX + shift, size + posY - 20, -size + posX + shift, -size + posY - 20);
  shift = -20;
  size = 5;
  Ellipse(hDC, size + posX + shift, size + posY - 20, -size + posX + shift, -size + posY - 20);

  // Vykreslení mnohoúhelníku (trojúhelníku)
  POINT points[] = {
      {posX, posY - 10},
      {posX - 100, posY},
      {posX, posY + 10}};
  Polygon(hDC, points, sizeof(points) / sizeof(points[0]));

  // Obnovení původního štětce a pera
  SelectObject(hDC, oldBrush);
  SelectObject(hDC, oldPen);
  DeleteObject(hBrush);
  DeleteObject(hPen);
}