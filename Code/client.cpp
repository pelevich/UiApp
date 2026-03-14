#include "client.h"

#pragma comment(lib, "Ws2_32.lib")

/// <summary>
/// </summary>
Client::Client() {
}

/// <summary>
/// Отключаемся от пайпов
/// </summary>
Client::~Client() {
    CloseHandle(hPipe);
    CloseHandle(hPipeCallFunction);
}

/// <summary>
/// Подключаемся к пайпу
/// Если не удалось подключится запускаем сервис и пробуем опять подключится
/// </summary>
/// <param name="name">Имя пайпа для подключения</param>
/// <returns>Возвращаем дескриптор пайпа</returns>
HANDLE Client::ConnectedServer(string name) {
    string path = "\\\\.\\pipe\\" + name;
    HANDLE hPipe = CreateFileA(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        STARTUPINFOA si = { 0 };
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
            hPipe = ConnectedServer(name);
        }
    }

    return hPipe;
}

/// <summary>
/// Очищаем буфер для записи сообщеиня от мусора и читаем сообщение от сервера
/// </summary>
/// <param name="hPipe">Дескриптор пайпа для чтения сообщения</param>
/// <param name="buffer">Ссылка на буффер для записи</param>
/// <param name="size">Размер буффера</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Client::Read(HANDLE hPipe, char* buffer, DWORD size) {
    memset(buffer, 0, size);
    bool result = ReadFile(hPipe, buffer, size - 1, &dwRead, NULL);
    return result;
}

/// <summary>
/// отправляем сообщение серверу
/// </summary>
/// <param name="hPipe">Дескриптор пайпа для отправки сообщения</param>
/// <param name="send_message">Отправляемое сообщение</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Client::Write(HANDLE hPipe, string& send_message) {
    bool result = WriteFile(hPipe, send_message.c_str(), send_message.size(), &dwWritten, NULL);
    return result;
}
