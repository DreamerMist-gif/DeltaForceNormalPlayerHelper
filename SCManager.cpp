#include "SCManager.h"

bool StopWindowsService(const wchar_t* serviceName) {
    // 使用明确的宽字符版本 OpenSCManagerW
    SC_HANDLE hSCManager = OpenSCManagerW(
        NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCManager == NULL) {
        MessageBoxA(NULL, "打开服务管理器失败", "错误", MB_OK | MB_SYSTEMMODAL);
        return false;
    }

    // 使用明确的宽字符版本 OpenServiceW
    SC_HANDLE hService = OpenServiceW(
        hSCManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);

    if (hService == NULL) {
        MessageBoxA(NULL, "打开服务失败", "错误", MB_OK | MB_SYSTEMMODAL);
        CloseServiceHandle(hSCManager);
        return false;
    }

    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    if (!ControlService(
        hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssStatus)) {

        MessageBoxA(NULL, "服务控制失败", "错误", MB_OK | MB_SYSTEMMODAL);
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return false;
    }

    while (ssStatus.dwCurrentState != SERVICE_STOPPED) {
        Sleep(ssStatus.dwWaitHint);
        if (!QueryServiceStatusEx(
            hService, SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {

            std::cerr << "\n更新服务状态失败，错误代码：" << GetLastError() << std::endl;
            break;
        }

        if (ssStatus.dwCurrentState == SERVICE_STOPPED)
            break;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return ssStatus.dwCurrentState == SERVICE_STOPPED;
}
