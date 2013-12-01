#include "stdafx.h"
#include "resource.h"

#include "DlgSettingsBehavior.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

DlgSettingsBehavior::DlgSettingsBehavior(CComPtr<IXMLDOMElement>& pOptionsRoot)
: DlgSettingsBase(pOptionsRoot)
, m_nCopyNewlineChar(0)
, m_nScrollPageType(0)
, m_bFlashInactiveTab(false)
{
	IDD = IDD_SETTINGS_BEHAVIOR;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_behaviorSettings.Load(m_pOptionsRoot);

	m_nCopyNewlineChar	= static_cast<int>(m_behaviorSettings.copyPasteSettings.copyNewlineChar);
	m_nScrollPageType	= m_behaviorSettings.scrollSettings.dwPageScrollRows ? 1 : 0;
	m_bFlashInactiveTab	= (m_behaviorSettings.tabHighlightSettings.dwFlashes > 0);
	m_strLeftDelimiters	= m_behaviorSettings.copyPasteSettings.strLeftDelimiters.c_str();
	m_strRightDelimiters	= m_behaviorSettings.copyPasteSettings.strRightDelimiters.c_str();

	CUpDownCtrl	spin;
	UDACCEL		udAccel;

	spin.Attach(GetDlgItem(IDC_SPIN_SCROLL_PAGE_ROWS));
	spin.SetRange(1, 500);
	udAccel.nSec = 2;
	udAccel.nInc = 1;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_TAB_FLASHES));
	spin.SetRange(1, 500);
	udAccel.nSec = 2;
	udAccel.nInc = 1;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_EOL_SPACES));
	spin.SetRange(1, 15);
	udAccel.nSec = 2;
	udAccel.nInc = 1;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	DoDataExchange(DDX_LOAD);

	EnableScrollControls();
	EnableFlashTabControls();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDOK)
	{
		DoDataExchange(DDX_SAVE);

		m_behaviorSettings.copyPasteSettings.copyNewlineChar= static_cast<CopyNewlineChar>(m_nCopyNewlineChar);

		if (m_nScrollPageType == 0) m_behaviorSettings.scrollSettings.dwPageScrollRows = 0;

		if (!m_bFlashInactiveTab) m_behaviorSettings.tabHighlightSettings.dwFlashes = 0;

		m_behaviorSettings.copyPasteSettings.strLeftDelimiters = m_strLeftDelimiters;
		m_behaviorSettings.copyPasteSettings.strRightDelimiters = m_strRightDelimiters;

		BehaviorSettings& behaviorSettings = g_settingsHandler->GetBehaviorSettings();

		behaviorSettings = m_behaviorSettings;
		m_behaviorSettings.Save(m_pOptionsRoot);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior::OnClickedScrollType(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);
	EnableScrollControls();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior::OnClickedFlashTab(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);
	EnableFlashTabControls();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior::OnClickedNoWrap(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);
	EnableNoWrapControls();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void DlgSettingsBehavior::EnableScrollControls()
{
	GetDlgItem(IDC_SCROLL_PAGE_ROWS).EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_SCROLL_PAGE_ROWS).EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ROWS).EnableWindow(FALSE);

	if (m_nScrollPageType > 0)
	{
		if (m_behaviorSettings.scrollSettings.dwPageScrollRows == 0)
		{
			m_behaviorSettings.scrollSettings.dwPageScrollRows = 1;
			DoDataExchange(DDX_LOAD);
		}

		GetDlgItem(IDC_SCROLL_PAGE_ROWS).EnableWindow();
		GetDlgItem(IDC_SPIN_SCROLL_PAGE_ROWS).EnableWindow();
		GetDlgItem(IDC_STATIC_ROWS).EnableWindow();
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void DlgSettingsBehavior::EnableFlashTabControls()
{
	GetDlgItem(IDC_TAB_FLASHES).EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_TAB_FLASHES).EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_LEAVE_HIGHLIGHTED).EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_FLASHES).EnableWindow(FALSE);

	if (m_bFlashInactiveTab)
	{
		if (m_behaviorSettings.tabHighlightSettings.dwFlashes == 0)
		{
			m_behaviorSettings.tabHighlightSettings.dwFlashes = 1;
			DoDataExchange(DDX_LOAD);
		}

		GetDlgItem(IDC_TAB_FLASHES).EnableWindow();
		GetDlgItem(IDC_SPIN_TAB_FLASHES).EnableWindow();
		GetDlgItem(IDC_CHECK_LEAVE_HIGHLIGHTED).EnableWindow();
		GetDlgItem(IDC_STATIC_FLASHES).EnableWindow();
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void DlgSettingsBehavior::EnableNoWrapControls()
{
	GetDlgItem(IDC_EOL_SPACES).EnableWindow(m_behaviorSettings.copyPasteSettings.bNoWrap);
	GetDlgItem(IDC_SPIN_EOL_SPACES).EnableWindow(m_behaviorSettings.copyPasteSettings.bNoWrap);
	GetDlgItem(IDC_STATIC_EOL_SPACES_1).EnableWindow(m_behaviorSettings.copyPasteSettings.bNoWrap);
	GetDlgItem(IDC_STATIC_EOL_SPACES_2).EnableWindow(m_behaviorSettings.copyPasteSettings.bNoWrap);
}

//////////////////////////////////////////////////////////////////////////////




