#ifdef FCN_EXPORTS
#define FCN_API __declspec(dllexport)
#else
#define FCN_API __declspec(dllimport)
#endif
#include <windows.h>

//FcnStartConnect err code
#define	FCN_CONN_ERR_HOST			0x1
#define	FCN_CONN_ERR_DEV_OPEN		0x2
#define	FCN_CONN_ERR_DRV_VER		0x3

//FcnGetStatus status
#define FCN_STATUS_CLOSED			0x0
#define FCN_STATUS_TCP_OK			0x1
#define FCN_STATUS_TLS_OK			0x2
#define FCN_STATUS_SVR_OK			0x3
#define FCN_STATUS_SESSION_OK		0x4
#define FCN_STATUS_CONN_OK			0x5

//FCN_API void FcnTest(const char* Uid);
FCN_API VOID WINAPI FcnSetUid(const char* Uid);
FCN_API VOID WINAPI FcnSetUser(const char* UserName);
FCN_API VOID WINAPI FcnSetPass(const char* UserPass);
FCN_API VOID WINAPI FcnSetUdpMode(int Mode);
FCN_API VOID WINAPI FcnSetRouteMode(int Mode);
FCN_API VOID WINAPI FcnSetHost(const char* FcnHost, unsigned short Port);
FCN_API UINT WINAPI FcnStartConnect(const char* FcnName);
FCN_API VOID WINAPI FcnStopConnect();
FCN_API	UINT WINAPI FcnGetStatus();
FCN_API	VOID WINAPI FcnGetLinkKbps(int* pSndKbps, int* pRcvKbps);