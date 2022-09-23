Simple game on c programming language to Full Stack course...started 20.09.2022

 WNDCLASSA wcl;
    memset(&wcl, 0, sizeOf(WNDCLASSA)); //fill all bytes in a memory by zero
    wcl.lpszClassName = "Game`s window";
    wcl.lpfnWndProc = DefWindowProcA; // default function to proccesing messages
    RegisterClassA(&wcl);

    HWND hwnd;
    hwnd = CreateWindow("Game`s window", "Catch me if you can", WS_OVERLAPPEDWINDOW, 
        10, 10, 640, 480, NULL, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOWNORMAL);
    while (1) {

    }