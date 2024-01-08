#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>

void createTray(HWND windowHandle)
{
    return;

    NOTIFYICONDATA nid = {};
    nid.uID = 5621;
    nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.hWnd = windowHandle;
    // icon doesn't work :/
    nid.hIcon = (HICON) LoadImage(NULL, "./resources/pics/fullbowody.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
    strcpy(nid.szInfo, "szInfo");
    strcpy(nid.szInfoTitle, "szInfoTitle");
    Shell_NotifyIcon(NIM_ADD, &nid);
}