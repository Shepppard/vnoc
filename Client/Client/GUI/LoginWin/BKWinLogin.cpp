#include "stdafx.h"
#include "BKWinLogin.h"
#include "..\..\Logic\UserSession\StrMatch.hpp"

#define TimerID_LoginTimeout	0

BEGIN_MSG_MAP_EX_IMP(CLoginWnd)
	MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
	CHAIN_MSG_MAP(CBkDialogImpl<CLoginWnd>)
	MSG_WM_TIMER(OnTimer)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP_IMP();

CLoginWnd::CLoginWnd() : CBkDialogImpl(BKDlg_LoginWin),CFrameBase(module_LoginWin)
{
	m_loginState = 0;
}

VOID CLoginWnd::OnFrameStart( XMessage_FrameStart* pmsg )
{
	OnShowWnd(NULL);
}

void CLoginWnd::OnBkBtnClose()
{
	EndDialog(0);
	XMessage_ShowRoomListWnd msg;
	SendXMessage(&msg);
}

void CLoginWnd::OnLoginClick()
{
	//Disable();
	//m_loginState = 1;
	//SetTimer(TimerID_LoginTimeout,5000);
	CString userName;
	CString pwd;
	CBkWindow* pUsernameEdit = NULL;
	CBkWindow* pPwdEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pUsernameEdit && pPwdEdit)
	{
		userName = pUsernameEdit->GetInnerText();
		pwd = pPwdEdit->GetInnerText();
		if (IsStringMatch(userName.GetString()) &&
			IsStringMatch(pwd.GetString(), MATCH_TYPE_PWD))
		{
			Disable();
			m_loginState = 1;
			SetTimer(TimerID_LoginTimeout,5000);

			XMessage_Login msg;
			msg.username = userName;
			msg.pwd = pwd;
			SendXMessage(&msg);
		}
		else
		{
			MessageBox(TEXT("请输入有效的用户名或密码"));
		}
	}
}

VOID CLoginWnd::OnLoginResult( XMessage_Login_Result* pMsg )
{
	if (pMsg && m_loginState==1)
	{
		if (pMsg->success == TRUE)
		{
			MessageBox(_T("成功登陆!"));
			OnBkBtnClose();
		}
		else
		{
			MessageBox(_T("登陆失败!"));
		}
		m_loginState = 2;
		Enable();
	}
}

VOID CLoginWnd::Disable()
{
	CBkWindow* pUsernameEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	if (pUsernameEdit)
	{
		pUsernameEdit->ModifyState(BkWndState_Disable,0);
	}
	CBkWindow* pPwdEdit = NULL;
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pPwdEdit)
	{
		pPwdEdit->ModifyState(BkWndState_Disable,0);
	}
	CBkWindow* pLoginBtn = NULL;
	pLoginBtn = m_richView.FindChildByCmdID(DlgControl_LoginWin_Button_Login);
	if (pLoginBtn)
	{
		pLoginBtn->ModifyState(BkWndState_Disable,0);
	}
}

VOID CLoginWnd::Enable()
{
	CBkWindow* pUsernameEdit = NULL;
	pUsernameEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_UserName);
	if (pUsernameEdit)
	{
		pUsernameEdit->ModifyState(0,BkWndState_Disable);
	}
	CBkWindow* pPwdEdit = NULL;
	pPwdEdit = m_richView.FindChildByCmdID(DlgControl_LoginWin_Edit_PassWord);
	if (pPwdEdit)
	{
		pPwdEdit->ModifyState(0,BkWndState_Disable);
	}
	CBkWindow* pLoginBtn = NULL;
	pLoginBtn = m_richView.FindChildByCmdID(DlgControl_LoginWin_Button_Login);
	if (pLoginBtn)
	{
		pLoginBtn->ModifyState(0,BkWndState_Disable);
		pLoginBtn->NotifyInvalidate();
	}
}

VOID CLoginWnd::OnTimer( UINT_PTR id )
{
	if (m_loginState == 1)
	{
		KillTimer(id);
		MessageBox(_T("登陆超时"));
	}
	m_loginState = 0;
	Enable();
}

VOID CLoginWnd::OnShowWnd( XMessage_ShowLogin* pmsg )
{
	DoModal();
}
