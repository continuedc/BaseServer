//
//// stdafx.h : 标准系统包含文件的包含文件，
//// 或是经常使用但不常更改的
//// 特定于项目的包含文件
//
//#pragma once
//
//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
//#endif
//
//#include "targetver.h"
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
//#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // 移除对话框中的 MFC 控件支持
//
//// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
//#define _AFX_ALL_WARNINGS
//
//#include <afxwin.h>         // MFC 核心组件和标准组件
//#include <afxext.h>         // MFC 扩展
//
//
//#include <afxdisp.h>        // MFC 自动化类
//
//
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
//#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
//
//
//#include <afxsock.h>            // MFC 套接字扩展
//
//
//
//
//
//
//
//#ifdef _UNICODE
//#if defined _M_IX86
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
//#endif

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__8BCA41CE_7325_427C_8443_E982F204F462__INCLUDED_)
#define AFX_STDAFX_H__8BCA41CE_7325_427C_8443_E982F204F462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#define NO_WARN_MBCS_MFC_DEPRECATION
#endif // _MSC_VER > 1000

#pragma  once

#ifdef WIN32
#ifndef WINVER 
#define WINVER 0x0501
#endif  
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8BCA41CE_7325_427C_8443_E982F204F462__INCLUDED_)


