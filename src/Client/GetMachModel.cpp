#include "Client.h"

#ifdef _WIN32

namespace HeartBeat {
    void __GetMachInfoFromWMI(IWbemClassObject *& pclsObj, const wchar_t* property, std::string &value)
    {
        VARIANT vtProp;
        VariantInit(&vtProp);
        HRESULT hRes = pclsObj->Get(property, 0, &vtProp, nullptr, nullptr);

        if (SUCCEEDED(hRes))
        {
            if (vtProp.vt == VT_BSTR and vtProp.bstrVal)
            {
                value = _bstr_t(vtProp.bstrVal);
            }
            else if (vtProp.vt == VT_NULL)
            {
                value = "[NULL]";
            }
            else if (vtProp.vt == VT_EMPTY)
            {
                value = "[EMPTY]";
            }
            else
            {
                value = "[Unknown Type]";
            }
        }
        else
        {
            std::println("Failed to get property.");
            value = "[]";
        }

        VariantClear(&vtProp);
    }
}
#endif

using namespace HeartBeat;

void HeartBeat::GetMachModel()
{
    if (machModelInitialized) return;

#ifdef _WIN32

    // Use WMI to get additional system information
    IWbemLocator *pLoc = nullptr;
    IWbemServices *pSvc = nullptr;

    HRESULT hRes = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (SUCCEEDED(hRes))
    {
        hRes = CoCreateInstance(
            CLSID_WbemLocator,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            reinterpret_cast<LPVOID *>(&pLoc)
            );

        if (SUCCEEDED(hRes) && pLoc)
        {
            hRes = pLoc->ConnectServer(
                _bstr_t(L"ROOT\\CIMV2"),
                nullptr, nullptr, nullptr,
                0, nullptr, nullptr,
                &pSvc
            );

            if (SUCCEEDED(hRes) && pSvc)
            {
                hRes = CoSetProxyBlanket(
                    pSvc,
                    RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                    RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                    nullptr, EOAC_NONE
                );

                if (SUCCEEDED(hRes))
                {
                    // GTet Computer System Model and Serial Number
                    IEnumWbemClassObject *pEnumerator = nullptr;
                    hRes = pSvc->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t("SELECT * FROM Win32_ComputerSystem"),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        nullptr, &pEnumerator);

                    if (SUCCEEDED(hRes) && pEnumerator)
                    {
                        IWbemClassObject *pclsObj = nullptr;
                        ULONG uReturn = 0;

                        while (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK)
                        {
                            __GetMachInfoFromWMI(pclsObj, L"Model", computerModel);
                            __GetMachInfoFromWMI(pclsObj, L"SystemSKUNumber", computerSKU);

                            pclsObj->Release();
                            break;
                        }
                        pEnumerator->Release();
                    }

                    // Get Machine UUID
                    pEnumerator = nullptr;
                    hRes = pSvc->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t("SELECT * FROM Win32_ComputerSystemProduct"),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        nullptr, &pEnumerator);

                    if (SUCCEEDED(hRes) && pEnumerator)
                    {
                        IWbemClassObject *pclsObj = nullptr;
                        ULONG uReturn = 0;

                        while (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK)
                        {
                            __GetMachInfoFromWMI(pclsObj, L"UUID", computerUUID);
                            pclsObj->Release();
                            break;
                        }

                        pEnumerator->Release();
                    }

                    // Get Machine Board Model and Serial Number
                    pEnumerator = nullptr;
                    hRes = pSvc->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t("SELECT * FROM Win32_BaseBoard"),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        nullptr, &pEnumerator);

                    if (SUCCEEDED(hRes) && pEnumerator)
                    {
                        IWbemClassObject *pclsObj = nullptr;
                        ULONG uReturn = 0;

                        std::string boardManufacturer, boardName;

                        while (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK)
                        {
                            __GetMachInfoFromWMI(pclsObj, L"Manufacturer", boardManufacturer);
                            __GetMachInfoFromWMI(pclsObj, L"Product", boardName);
                            __GetMachInfoFromWMI(pclsObj, L"SerialNumber", boardSerial);
                            pclsObj->Release();
                            break;
                        }

                        boardModel = boardManufacturer + boardName;
                        pEnumerator->Release();
                    }

                    // Get Machine SN, BIOS version and manufacturer
                    pEnumerator = nullptr;
                    hRes = pSvc->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t("SELECT * FROM Win32_BIOS"),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        nullptr, &pEnumerator);

                    if (SUCCEEDED(hRes) && pEnumerator)
                    {
                        IWbemClassObject *pclsObj = nullptr;
                        ULONG uReturn = 0;

                        while (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK)
                        {
                            __GetMachInfoFromWMI(pclsObj, L"SerialNumber", computerSerial);

                            __GetMachInfoFromWMI(pclsObj, L"SMBIOSBIOSVersion", computerFirmwareVersion);

                            __GetMachInfoFromWMI(pclsObj, L"Manufacturer", computerFirmwareManufacturer);

                            pclsObj->Release();
                            break;
                        }
                        pEnumerator->Release();
                    }
                }
                pSvc->Release();
            }
            pLoc->Release();
        }
        CoUninitialize();
    }

#elifdef __linux__

#endif

}