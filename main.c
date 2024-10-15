#include <windows.h>

// Deklarace funkcí
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Globální proměnné
HWND hEdit;  // Odkaz na textové pole
HFONT hFont; // Odkaz na font

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // Nastavení kódování na UTF-8
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  const char CLASS_NAME[] = "SimpleWindowClass";

  // Registrace okna
  WNDCLASS wc = {0};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  // Vytvoření okna
  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "WinAPI Example (UTF-8)",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
      NULL, NULL, hInstance, NULL);

  if (hwnd == NULL)
  {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // Vytvoření fontu
  hFont = CreateFont(
      20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
      EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

  // Hlavní smyčka zpráv
  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  DeleteObject(hFont); // Uvolnění paměti při ukončení aplikace
  return 0;
}

// Zpracování zpráv
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
  {
    // Vytvoření textu
    HWND hStatic = CreateWindow("STATIC", "Zadej text:",
                                WS_VISIBLE | WS_CHILD,
                                20, 20, 100, 20,
                                hwnd, NULL, NULL, NULL);
    SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Vytvoření textového pole
    hEdit = CreateWindow("EDIT", "",
                         WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                         20, 50, 200, 20,
                         hwnd, NULL, NULL, NULL);
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Vytvoření tlačítka
    HWND hButton = CreateWindow("BUTTON", "Klikni mě",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                20, 80, 100, 30,
                                hwnd, (HMENU)1, NULL, NULL);
    SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
  }
  break;

  case WM_COMMAND:
  {
    if (LOWORD(wParam) == 1)
    { // Pokud bylo kliknuto na tlačítko
      char text[256];
      GetWindowText(hEdit, text, sizeof(text));
      MessageBox(hwnd, text, "Zadaný text", MB_OK);
    }
  }
  break;

  case WM_CTLCOLORSTATIC:
  {
    // Změna barvy statického textu na modrou
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, RGB(0, 0, 255));
    SetBkMode(hdcStatic, TRANSPARENT);
    return (INT_PTR)GetStockObject(NULL_BRUSH);
  }

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
