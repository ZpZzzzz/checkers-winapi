#include <Windows.h>
#include <cstdio>
#include <cstdlib>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



// Глобальные переменные
const int BOARD_SIZE = 8;  // Размер доски
const int SQUARE_SIZE = 60;  // Размер квадрата на доске
const int BORDER_SIZE = 20;  // Размер границы вокруг доски
const int WINDOW_WIDTH = BORDER_SIZE * 2 + BOARD_SIZE * SQUARE_SIZE;  // Ширина окна
const int WINDOW_HEIGHT = BORDER_SIZE * 2 + BOARD_SIZE * SQUARE_SIZE;  // Высота окна

int board[BOARD_SIZE][BOARD_SIZE];  // Доска

// Объявление переменных для хранения координат двух клеток
POINT firstCell = { -1, -1 };
static int clickCount = 1;



#define ID_START_BUTTON 1001
HINSTANCE hInstance;
#define GET_X_LPARAM(lParam) ((int)(short)LOWORD(lParam))
#define GET_Y_LPARAM(lParam) ((int)(short)HIWORD(lParam))

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна
    const wchar_t CLASS_NAME[] = L"CheckersApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    ::hInstance = hInstance;
    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Шашки",
        WS_OVERLAPPEDWINDOW | WS_EX_CLIENTEDGE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)          //обработка ошибки
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



struct ColorRGB
{
    int red;
    int green;
    int blue;
    int sign;
};



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (msg)
    {
    case WM_CREATE:
    {
        // Создание кнопки "Начать"
        HWND hStartButton = CreateWindow(
            L"BUTTON",         // Класс окна кнопки
            L"Начать",         // Текст на кнопке
            WS_VISIBLE | WS_CHILD,  // Стиль окна кнопки
            (1 + BOARD_SIZE) * SQUARE_SIZE + 15, BOARD_SIZE * SQUARE_SIZE - 100, 200, 100,   // Координаты и размеры кнопки
            hwnd,              // Родительское окно
            (HMENU)ID_START_BUTTON, // Идентификатор кнопки
            hInstance,         // Дескриптор экземпляра приложения
            NULL               // Дополнительные параметры
        );
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case ID_START_BUTTON:
            // Код, выполняющийся при нажатии кнопки "Начать"
            MessageBox(hwnd, L"Кнопка \"Начать\" нажата!", L"Сообщение", MB_OK);
            break;
            // Другие обработчики команд
        }
    }
    break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        // Получаем размеры клиентской области окна
        GetClientRect(hwnd, &rect);

        // Отрисовка шашек
        void DrawBoard(HWND hwnd, HDC hdc);
        {
            RECT rect;
            rect.left = BORDER_SIZE;
            rect.top = BORDER_SIZE;
            rect.right = rect.left + BOARD_SIZE * SQUARE_SIZE;
            rect.bottom = rect.top + BOARD_SIZE * SQUARE_SIZE;

            // Заливка фона
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            // Отрисовка доски
            bool isWhite = true;
            for (int row = 0; row < BOARD_SIZE; row++)
            {
                for (int col = 0; col < BOARD_SIZE; col++)
                {
                    rect.left = BORDER_SIZE + col * SQUARE_SIZE;
                    rect.top = BORDER_SIZE + row * SQUARE_SIZE;
                    rect.right = rect.left + SQUARE_SIZE;
                    rect.bottom = rect.top + SQUARE_SIZE;

                    hBrush = CreateSolidBrush(isWhite ? RGB(240, 217, 181) : RGB(181, 136, 99));
                    FillRect(hdc, &rect, hBrush);
                    DeleteObject(hBrush);

                    isWhite = !isWhite;
                }
                isWhite = !isWhite;
            }

            // Отрисовка эллипсов
            // Цикл для черных клеток
            for (int row = 0; row < 3; row++)
            {
                // Установка цвета заливки для черных клеток
                COLORREF color = RGB(0, 0, 0);
                // Установка цвета заливки
                hBrush = CreateSolidBrush(color);
                SelectObject(hdc, hBrush);
                if (row == 1) {
                    for (int col = 0; col < BOARD_SIZE; col += 2)
                    {
                        int x = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4;
                        int y = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                        // Рисование эллипса
                        Ellipse(hdc, x, y, x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2);
                    }
                }
                else {
                    for (int col = 0; col < BOARD_SIZE; col += 2)
                    {
                        int x = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4 + SQUARE_SIZE;
                        int y = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                        // Рисование эллипса
                        Ellipse(hdc, x, y, x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2);
                    }
                }
                // Освобождение ресурсов кисти
                DeleteObject(hBrush);
            }

            // Цикл для белых клеток
            for (int row = 5; row < BOARD_SIZE; row++)
            {
                // Установка цвета заливки для белых клеток
                COLORREF color = RGB(255, 255, 255);
                // Установка цвета заливки
                hBrush = CreateSolidBrush(color);
                SelectObject(hdc, hBrush);
                if (row == 6) {
                    for (int col = 0; col < BOARD_SIZE; col += 2)
                    {
                        int x = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4 + SQUARE_SIZE;
                        int y = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                        // Рисование эллипса
                        Ellipse(hdc, x, y, x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2);
                    }
                }
                else {
                    for (int col = 1; col < BOARD_SIZE; col += 2)
                    {
                        int x = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4 - SQUARE_SIZE;
                        int y = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;


                        // Рисование эллипса
                        Ellipse(hdc, x, y, x + SQUARE_SIZE / 2, y + SQUARE_SIZE / 2);

                    }
                }
                // Освобождение ресурсов кисти
                DeleteObject(hBrush);
            }

        }
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
    {
        if (clickCount == 1)
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            // Преобразование координат клика в номер клетки
            int row = (yPos - BORDER_SIZE) / SQUARE_SIZE;
            int col = (xPos - BORDER_SIZE) / SQUARE_SIZE;

            if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                HDC hdc = GetDC(hwnd);
                COLORREF color = GetPixel(hdc, xPos, yPos);
                // Извлекаем значения красного, зеленого и синего цветов
                if (GetRValue(color) == 255) {
                    // Выбор первой клетки
                    firstCell.x = col;
                    firstCell.y = row;
                    HDC hdc = GetDC(hwnd);
                    // Рассчитываем координаты прямоугольника для эллипса
                    int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 8;
                    int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 8;
                    int right = left + SQUARE_SIZE - SQUARE_SIZE / 4;
                    int bottom = top + SQUARE_SIZE - SQUARE_SIZE / 4;
                    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
                    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
                    SelectObject(hdc, hPen);
                    SelectObject(hdc, hBrush);
                    // Рисуем эллипс
                    Ellipse(hdc, left, top, right, bottom);
                    // Освобождаем контекст устройства
                    ReleaseDC(hwnd, hdc);
                    DeleteObject(hBrush);
                    DeleteObject(hPen);
                }
                else { break; }
            }
            else { break; }
            clickCount = 2;
        }
        else if (clickCount == 2) {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            // Преобразование координат клика в номер клетки
            int row = (yPos - BORDER_SIZE) / SQUARE_SIZE;
            int col = (xPos - BORDER_SIZE) / SQUARE_SIZE;

            // Выбор второй клетки
            // Проверка, является ли выбранная клетка второй клеткой

            // Получаем цвет пикселя
            HDC hdc = GetDC(hwnd);
            COLORREF color = GetPixel(hdc, BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 2, BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 2);
            // Извлекаем значения красного, зеленого и синего цветов
            if (GetRValue(color) == 181) {
                if (col != firstCell.x && row < firstCell.y && abs(firstCell.x - col) < 3)
                {
                    //определение возможности побить шашку противника
                    int sum = (firstCell.y - row) / 2;
                    int col3 = col + (firstCell.x - col) / 2;
                    int row3 = row + sum;
                    HBRUSH hBrush;
                    if (sum == 0) {
                        // Рисование эллипса во второй клетке
                        if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                            HDC hdc = GetDC(hwnd);
                            // Рассчитываем координаты прямоугольника для эллипса
                            int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4;
                            int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                            int right = left + SQUARE_SIZE / 2;
                            int bottom = top + SQUARE_SIZE / 2;
                            // Рисуем эллипс
                            Ellipse(hdc, left, top, right, bottom);
                            // Освобождаем контекст устройства
                            ReleaseDC(hwnd, hdc);
                        }
                        // Перерисовка первой клетки
                        RECT rect;
                        rect.left = firstCell.x * SQUARE_SIZE + 20;
                        rect.top = firstCell.y * SQUARE_SIZE + 20;
                        rect.right = rect.left + SQUARE_SIZE;
                        rect.bottom = rect.top + SQUARE_SIZE;
                        hBrush = CreateSolidBrush(RGB(181, 136, 99));
                        FillRect(hdc, &rect, hBrush);
                        DeleteObject(hBrush);
                        // Сбрасываем выбор первой клетки
                        firstCell.x = -1;
                        firstCell.y = -1;
                        clickCount = 3;
                    }
                    else if (sum == 1) {            //чтоб бить шашку
                        HDC hdc = GetDC(hwnd);
                        COLORREF color = GetPixel(hdc, BORDER_SIZE + col3 * SQUARE_SIZE + SQUARE_SIZE / 2, BORDER_SIZE + row3 * SQUARE_SIZE + SQUARE_SIZE / 2);
                        // Извлекаем значения красного, зеленого и синего цветов
                        //char message[100];
                        //sprintf_s(message, sizeof(message), "Координаті цвет (%d, %d): R=%d, G=%d, B=%d", col3, row3, GetRValue(color), GetGValue(color), GetBValue(color));
                        //MessageBoxA(NULL, message, "Информация о цвете", MB_OK);
                        if (GetRValue(color) == 0) {
                            // Рисование эллипса во второй клетке
                            if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                                HDC hdc = GetDC(hwnd);
                                // Рассчитываем координаты прямоугольника для эллипса
                                int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4;
                                int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                                int right = left + SQUARE_SIZE / 2;
                                int bottom = top + SQUARE_SIZE / 2;
                                // Рисуем эллипс
                                Ellipse(hdc, left, top, right, bottom);
                                // Освобождаем контекст устройства
                                ReleaseDC(hwnd, hdc);
                            }
                            // Перерисовка первой клетки и битой
                            HBRUSH hBrush = CreateSolidBrush(RGB(181, 136, 99));
                            RECT rect;
                            rect.left = firstCell.x * SQUARE_SIZE + 20;
                            rect.top = firstCell.y * SQUARE_SIZE + 20;
                            rect.right = rect.left + SQUARE_SIZE;
                            rect.bottom = rect.top + SQUARE_SIZE;
                            FillRect(hdc, &rect, hBrush);

                            rect.left = col3 * SQUARE_SIZE + 20;
                            rect.top = row3 * SQUARE_SIZE + 20;
                            rect.right = rect.left + SQUARE_SIZE;
                            rect.bottom = rect.top + SQUARE_SIZE;
                            //sprintf_s(message, sizeof(message), "Координаті цвет (%d, %d,%d,%d): R=%d, G=%d, B=%d", rect.left, rect.top, rect.left, rect.bottom, GetRValue(color), GetGValue(color), GetBValue(color));
                            //MessageBoxA(NULL, message, "Информация о цвете", MB_OK);
                            FillRect(hdc, &rect, hBrush);
                            DeleteObject(hBrush);
                            // Сбрасываем выбор первой клетки
                            firstCell.x = -1;
                            firstCell.y = -1;
                            clickCount = 3;
                        }
                    }
                }
            }
        }
        else if (clickCount == 3) {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            // Преобразование координат клика в номер клетки
            int row = (yPos - BORDER_SIZE) / SQUARE_SIZE;
            int col = (xPos - BORDER_SIZE) / SQUARE_SIZE;

            // Проверка, является ли выбранная клетка первой или второй
            if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                HDC hdc = GetDC(hwnd);
                COLORREF color = GetPixel(hdc, xPos, yPos);
                // Извлекаем значения красного, зеленого и синего цветов
                if (GetRValue(color) == 0) {
                    // Выбор первой клетки
                    firstCell.x = col;
                    firstCell.y = row;
                    HDC hdc = GetDC(hwnd);
                    // Рассчитываем координаты прямоугольника для эллипса
                    int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 8;
                    int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 8;
                    int right = left + SQUARE_SIZE - SQUARE_SIZE / 4;
                    int bottom = top + SQUARE_SIZE - SQUARE_SIZE / 4;
                    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(hdc, hPen);
                    SelectObject(hdc, hBrush);
                    // Рисуем эллипс
                    Ellipse(hdc, left, top, right, bottom);
                    // Освобождаем контекст устройства
                    ReleaseDC(hwnd, hdc);
                    DeleteObject(hBrush);
                    DeleteObject(hPen);
                }
                else { break; }
            }
            else { break; }
            clickCount = 4;
        }
        else if (clickCount == 4) {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            // Преобразование координат клика в номер клетки
            int row = (yPos - BORDER_SIZE) / SQUARE_SIZE;
            int col = (xPos - BORDER_SIZE) / SQUARE_SIZE;
            // Выбор второй клетки
           // Проверка, является ли выбранная клетка второй клеткой

           // Получаем цвет пикселя
            HDC hdc = GetDC(hwnd);
            COLORREF color = GetPixel(hdc, BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 2, BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 2);
            // Извлекаем значения красного, зеленого и синего цветов
            if (GetRValue(color) == 181) {
                if (col != firstCell.x && row > firstCell.y && abs(firstCell.x - col) < 3) {
                    //определение возможности побить шашку противника
                    int sum = (row - firstCell.y) / 2;
                    int col3 = (col + firstCell.x) / 2;
                    int row3 = (row + firstCell.y) / 2;
                    if (sum == 0) {
                        // Рисование эллипса во второй клетке
                        if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                            HDC hdc = GetDC(hwnd);
                            // Рассчитываем координаты прямоугольника для эллипса
                            int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4;
                            int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                            int right = left + SQUARE_SIZE / 2;
                            int bottom = top + SQUARE_SIZE / 2;
                            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                            SelectObject(hdc, hPen);
                            SelectObject(hdc, hBrush);
                            // Рисуем эллипс
                            Ellipse(hdc, left, top, right, bottom);
                            // Освобождаем контекст устройства
                            ReleaseDC(hwnd, hdc);
                            DeleteObject(hBrush);
                            DeleteObject(hPen);
                        }
                        // Перерисовка первой клетки
                        RECT rect;
                        rect.left = firstCell.x * SQUARE_SIZE + 20;
                        rect.top = firstCell.y * SQUARE_SIZE + 20;
                        rect.right = rect.left + SQUARE_SIZE;
                        rect.bottom = rect.top + SQUARE_SIZE;
                        HBRUSH hBrush = CreateSolidBrush(RGB(181, 136, 99));
                        FillRect(hdc, &rect, hBrush);
                        DeleteObject(hBrush);
                        // Сбрасываем выбор первой клетки
                        firstCell.x = -1;
                        firstCell.y = -1;
                        clickCount = 1;
                    }
                    else if (sum == 1) {
                        HDC hdc = GetDC(hwnd);
                        COLORREF color = GetPixel(hdc, BORDER_SIZE + col3 * SQUARE_SIZE + SQUARE_SIZE / 2, BORDER_SIZE + row3 * SQUARE_SIZE + SQUARE_SIZE / 2);
                        // Извлекаем значения красного, зеленого и синего цветов
                        //char message[100];
                        //sprintf_s(message, sizeof(message), "Координаті цвет (%d, %d): R=%d, G=%d, B=%d", col3, row3, GetRValue(color), GetGValue(color), GetBValue(color));
                        //MessageBoxA(NULL, message, "Информация о цвете", MB_OK);
                        if (GetRValue(color) == 255) {
                            // Рисование эллипса во второй клетке
                            if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE && (row + col) % 2 == 1) {
                                HDC hdc = GetDC(hwnd);
                                // Рассчитываем координаты прямоугольника для эллипса
                                int left = BORDER_SIZE + col * SQUARE_SIZE + SQUARE_SIZE / 4;
                                int top = BORDER_SIZE + row * SQUARE_SIZE + SQUARE_SIZE / 4;
                                int right = left + SQUARE_SIZE / 2;
                                int bottom = top + SQUARE_SIZE / 2;
                                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                                SelectObject(hdc, hPen);
                                SelectObject(hdc, hBrush);
                                // Рисуем эллипс
                                Ellipse(hdc, left, top, right, bottom);
                                // Освобождаем контекст устройства
                                ReleaseDC(hwnd, hdc);
                                DeleteObject(hBrush);
                                DeleteObject(hPen);
                            }
                            // Перерисовка первой клетки и битой
                            HBRUSH hBrush = CreateSolidBrush(RGB(181, 136, 99));
                            RECT rect;
                            rect.left = firstCell.x * SQUARE_SIZE + 20;
                            rect.top = firstCell.y * SQUARE_SIZE + 20;
                            rect.right = rect.left + SQUARE_SIZE;
                            rect.bottom = rect.top + SQUARE_SIZE;
                            FillRect(hdc, &rect, hBrush);

                            rect.left = col3 * SQUARE_SIZE + 20;
                            rect.top = row3 * SQUARE_SIZE + 20;
                            rect.right = rect.left + SQUARE_SIZE;
                            rect.bottom = rect.top + SQUARE_SIZE;
                            //sprintf_s(message, sizeof(message), "Координаті цвет (%d, %d,%d,%d): R=%d, G=%d, B=%d", rect.left, rect.top, rect.left, rect.bottom, GetRValue(color), GetGValue(color), GetBValue(color));
                            //MessageBoxA(NULL, message, "Информация о цвете", MB_OK);
                            FillRect(hdc, &rect, hBrush);
                            DeleteObject(hBrush);
                            // Сбрасываем выбор первой клетки
                            firstCell.x = -1;
                            firstCell.y = -1;
                            clickCount = 1;
                        }
                    }
                }
            }
        }
        else { break; }
    }


    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);

    }
}







