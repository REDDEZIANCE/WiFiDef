#include <windows.h>
#include <iostream>

// Функция для активации режима защиты (установка ограничений)
void activateProtection() {
    HKEY hKey;
    LONG result;

    // Установим политику блокировки в реестре
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          TEXT("SOFTWARE\\Policies\\Microsoft\\Windows\\WcmSvc"),
                          0,
                          KEY_WRITE | KEY_WOW64_64KEY,
                          &hKey);

    if (result != ERROR_SUCCESS) {
        std::cerr << "error_code_1.\n";
        return;
    }

    DWORD value = 1; // Блокировка добавления сетей

    result = RegSetValueEx(hKey,
                           TEXT("wifiAllowManualConfig"),
                           0,
                           REG_DWORD,
                           reinterpret_cast<const BYTE*>(&value),
                           sizeof(DWORD));

    if (result != ERROR_SUCCESS) {
        std::cerr << "error_code_2.\n";
    }

    RegCloseKey(hKey);

    // Отключим службу беспроводных сетей
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager) {
        std::cerr << "error_code_3.\n";
        return;
    }

    SC_HANDLE schService = OpenService(schSCManager, TEXT("WlanSvc"), SERVICE_CHANGE_CONFIG);
    if (!schService) {
        CloseServiceHandle(schSCManager);
        std::cerr << "error_code_4.\n";
        return;
    }

    BOOL success = ChangeServiceConfig(
        schService,
        SERVICE_NO_CHANGE,
        SERVICE_DISABLED,
        SERVICE_NO_CHANGE,
        NULL,
        NULL,
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL
    );

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);

    if (!success) {
        std::cerr << "error_code_5.\n";
    }

    std::cout << "def activeted!\n";
}

// Функция для снятия режима защиты (возвращение возможностей)
void deactivateProtection() {
    HKEY hKey;
    LONG result;

    // Убираем политику блокировки в реестре
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                          TEXT("SOFTWARE\\Policies\\Microsoft\\Windows\\WcmSvc"),
                          0,
                          KEY_WRITE | KEY_WOW64_64KEY,
                          &hKey);

    if (result != ERROR_SUCCESS) {
        std::cerr << "error_code_6.\n";
        return;
    }

    DWORD value = 0; // Разрешение ручной настройки Wi-Fi

    result = RegSetValueEx(hKey,
                           TEXT("wifiAllowManualConfig"),
                           0,
                           REG_DWORD,
                           reinterpret_cast<const BYTE*>(&value),
                           sizeof(DWORD));

    if (result != ERROR_SUCCESS) {
        std::cerr << "error_code_7.\n";
    }

    RegCloseKey(hKey);

    // Включаем службу беспроводных сетей
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!schSCManager) {
        std::cerr << "error_code_8.\n";
        return;
    }

    SC_HANDLE schService = OpenService(schSCManager, TEXT("WlanSvc"), SERVICE_CHANGE_CONFIG);
    if (!schService) {
        CloseServiceHandle(schSCManager);
        std::cerr << "error_code_9.\n";
        return;
    }

    BOOL success = ChangeServiceConfig(
        schService,
        SERVICE_NO_CHANGE,
        SERVICE_AUTO_START,
        SERVICE_NO_CHANGE,
        NULL,
        NULL,
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL
    );

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);

    if (!success) {
        std::cerr << "error_code_10.\n";
    }

    std::cout << "def deacteveted!\n";
}

int main() {
    int choice;

    while (true) {
        std::cout << "\nMenu by red:\n";
        std::cout << "1. def (on)\n";
        std::cout << "2. undef (off)\n";
        std::cout << "3. exit\n";
        std::cout << "Please, select command: ";
        std::cin >> choice;

        switch(choice) {
            case 1:
                activateProtection(); break;
            case 2:
                deactivateProtection(); break;
            case 3:
                exit(EXIT_SUCCESS); break;
            default:
                std::cout << "Do not enter, please try again:\n";
        }
    }

    return 0;
}