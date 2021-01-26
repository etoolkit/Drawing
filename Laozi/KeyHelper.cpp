#include "StdAfx.h"
#include "KeyHelper.h"
#include "MyString.h"


CKeyHelper::CKeyHelper(void)
{
}


CKeyHelper::~CKeyHelper(void)
{
}

CString CKeyHelper::GetKeyStr(UINT key)
{
	CString s;
	 if(key  >= VK_F1 && key <= VK_F12)
	{		
		s.Format(L"%sF%d",s,key - VK_F1 + 1);
	} 
	else if(key>=VK_NUMPAD0 && key <= VK_NUMPAD9)
	{
		s.Format(L"%sNum %d",s,key - VK_NUMPAD0);
	}
	else if((key>=0x41 && key <=0x5A)/*||(n>=0x61 && n < 0x7A)*/ || (key>=0x30 && key <= 0x39))
	{
		s+= (TCHAR)key;
	}

	else
	{
		switch(key)
		{
		case  VK_UP :
			s += "Up";break;
		case  VK_LEFT: 
			s += "Left";break;
		case  VK_DOWN :
			s += "Down";break;
		case  VK_RIGHT :
			s += "Right";break;

		case VK_OEM_COMMA:
			s +=",";break;
		case  VK_OEM_PERIOD:
			s +=".";break;
		case  VK_OEM_MINUS:
			s +="-";break;
		case VK_OEM_1:
			s += ";";break;
		case VK_OEM_2:
			s += "/";break;
		case  VK_OEM_3:
			s += "`";break;
		case VK_OEM_4:
			s += "[";break;
		case VK_OEM_5:
			s += "\\";break;
		case  VK_OEM_6:
			s += "]";break;
		case  VK_OEM_7:
			s += "'";break;
		case  VK_OEM_NEC_EQUAL:
		case VK_OEM_PLUS:
			s += '=';break;;
		case  VK_HOME:
			s += "Home";break;
		case  VK_END:
			s += "End";break;
		case  VK_INSERT:
			s += "Insert";break;
		case  VK_DELETE:
			s += "Delete";break;
		case  VK_PRIOR:
			s += "PageUp";break;
		case   VK_NEXT:
			s += "PageDown";break;
		case  VK_NUMLOCK:
			s +="NumLock";break;
		case   VK_DECIMAL:
			s += "NumDel";break;
		case   VK_DIVIDE:
			s += "Num/";break;
		case   VK_CLEAR:
			s += "Num5";break;
		case   VK_MULTIPLY:
			s += "Num*";break;
		case   VK_SUBTRACT:
			s += "Num-";break;
		case   VK_ADD:
			s += "Num+";break;
		case   VK_PAUSE:
			s += "Pause";break;
		case   VK_SNAPSHOT:
			s += "Snapshot";break;
		case   VK_SCROLL:
			s += "Scroll";break;
		default:
			s = "";break;
		}
	}
	return s;
}
UINT CKeyHelper::GetKey(CString key)
{
	UINT k = -1;
	key = key.Trim();
	if( key.CompareNoCase(L"Ctrl")==0)
		k = VK_CONTROL;
	else if(key.CompareNoCase(L"Alt")==0)
		k = VK_MENU;
	else if(key.CompareNoCase(L"Shift")==0)
		k = VK_SHIFT;
	else if(key.CompareNoCase(L"Win")==0)
		k = VK_LWIN ;
	else if(key.CompareNoCase(L"F1")==0)
		k = VK_F1;
	else if(key.CompareNoCase(L"F2")==0)
		k = VK_F2;
	else if(key.CompareNoCase(L"F3")==0)
		k = VK_F3;
	else if(key.CompareNoCase(L"F4")==0)
		k = VK_F4;
	else if(key.CompareNoCase(L"F5")==0)
		k = VK_F5;
	else if(key.CompareNoCase(L"F6")==0)
		k = VK_F6;
	else if(key.CompareNoCase(L"F7")==0)
		k = VK_F7;
	else if(key.CompareNoCase(L"F8")==0)
		k = VK_F8;
	else if(key.CompareNoCase(L"F9")==0)
		k = VK_F9;
	else if(key.CompareNoCase(L"F10")==0)
		k = VK_F10;
	else if(key.CompareNoCase(L"F11")==0)
		k = VK_F11;
	else if(key.CompareNoCase(L"F12")==0)
		k = VK_F12;
	else if(key.CompareNoCase(L"Up")==0)
		k = VK_UP;
	else if(key.CompareNoCase(L"Left")==0)
		k = VK_LEFT;
	else if(key.CompareNoCase(L"Down")==0)
		k = VK_DOWN;
	else if(key.CompareNoCase(L"Right")==0)
		k = VK_RIGHT;
	else if(key.CompareNoCase(L",")==0)
		k = VK_OEM_COMMA;
	else if(key.CompareNoCase(L".")==0)
		k = VK_OEM_PERIOD;
	else if(key.CompareNoCase(L"-")==0)
		k = VK_OEM_MINUS;
	else if(key.CompareNoCase(L"Home")==0)
		k = VK_HOME;
	else if(key.CompareNoCase(L"End")==0)
		k = VK_END;
	else if(key.CompareNoCase(L"Insert")==0)
		k = VK_INSERT;
	else if(key.CompareNoCase(L"Delete")==0)
		k = VK_DELETE;
	else if(key.CompareNoCase(L"PageUp")==0)
		k = VK_PRIOR;
	else if(key.CompareNoCase(L"PageDown")==0)
		k = VK_NEXT;
	else if(key.CompareNoCase(L"NumLock")==0)
		k = VK_NUMLOCK;
	else if(key.CompareNoCase(L"NumDel")==0)
		k = VK_DECIMAL;
	else if(key.CompareNoCase(L"Num/")==0)
		k = VK_DIVIDE;
	else if(key.CompareNoCase(L"Num5")==0)
		k = VK_CLEAR;
	else if(key.CompareNoCase(L"Num*")==0)
		k = VK_MULTIPLY;
	else if(key.CompareNoCase(L"Num-")==0)
		k = VK_SUBTRACT;
	else if(key.CompareNoCase(L"Num+")==0)
		k = VK_ADD;
	else if(key.CompareNoCase(L"Pause")==0)
		k = VK_PAUSE;
	else if(key.CompareNoCase(L"Snapshot")==0)
		k = VK_SNAPSHOT;
	else if(key.CompareNoCase(L"Scroll")==0)
		k = VK_SCROLL;
	else if(key.Find(L"Num ")==0)
	{
		CString s = key.Mid(4);
		if(s.GetLength()==1 && s.GetAt(0)>'0' && s.GetAt(0)<'9')
			k = VK_NUMPAD0 + s.GetAt(0) - '0';

	}
	else if(key.GetLength()==1)
		k = key[0];	 
	return k;
}

BOOL CKeyHelper::RegisetHotkey(HWND hwnd, int id, CString strkey)
{
	CStringArray skeyArr;
	Split(strkey,skeyArr, '+');
	if(strkey.GetLength()>=4 &&strkey.Find(L"Num+")==strkey.GetLength() - 4)
	{
		skeyArr.RemoveAt(skeyArr.GetCount()-1) ;
		skeyArr.Add(L"Num+");
	}
	UINT vk; UINT MOD = 0;
	for(int i = 0; i<skeyArr.GetCount(); i++)
	{
		UINT k = CKeyHelper::GetKey(skeyArr.GetAt(i));

		if(k == VK_CONTROL)
			MOD |= MOD_CONTROL;
		else if(k == VK_SHIFT)
			MOD |= MOD_SHIFT;
		else if(k == VK_MENU)
			MOD |= MOD_ALT;
		else if(k == VK_LWIN || k == VK_RWIN)
			MOD |= MOD_WIN;
		else
			vk = k;
	}
	// BOOL b = RegisterHotKey(this->m_hWnd, theApp.m_hotKeyIdHideWnd, /*MOD_CONTROL|MOD_ALT */0, 'W');
	return  RegisterHotKey(hwnd, id, MOD, vk);
	 
}