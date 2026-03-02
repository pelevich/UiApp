#include "client.h"

#pragma comment(lib, "Ws2_32.lib")

/// <summary>
/// Подключаемся к серверу, через метод ConnectedServer(), если не удалось (сервер выключин) запускаем сервер и опять подключаемся
/// </summary>
Client::Client() {
    ConnectedServer();

    if (hPipe == INVALID_HANDLE_VALUE) {
        STARTUPINFOA si = {0};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi;
        string serverPath = "..\\ServiceApp.exe";
        BOOL success = CreateProcessA(
            serverPath.c_str(),
            NULL,
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi
        );
        Sleep(1000);
        if (!success) {
            cout << GetLastError();
        }
        else {
            ConnectedServer();
        }
    }

}

/// <summary>
/// Отключаемся от пайпа
/// </summary>
Client::~Client() {
    CloseHandle(hPipe);
}

/// <summary>
/// Подключаемся к пайпу
/// </summary>
/// <returns>Возвращаем дескриптор пайпа</returns>
HANDLE Client::ConnectedServer() {
    hPipe = CreateFile(
        TEXT("\\\\.\\pipe\\MyPipe"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    return hPipe;
}

/// <summary>
/// Читаем отправленое сервером сообщение
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого читаем</param>
/// <param name="buffer">Куда будем записывать данные</param>
/// <param name="size">Размер буфера</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Client::Read(HANDLE hPipe, char* buffer, DWORD size) {
    bool result = ReadFile(hPipe, buffer, size - 1, &dwRead, NULL);
    return result;
}

/// <summary>
/// Отправляем сообщение серверу
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого отправляем сообщение</param>
/// <param name="send_message">Отправляемое сообщение</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Client::Write(HANDLE hPipe, string send_message) {
    bool result = WriteFile(hPipe, send_message.c_str(), send_message.size(), &dwWritten, NULL);
    return result;
}
