; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CTreeCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "cmyexplorer.h"
LastPage=0

ClassCount=7
Class1=CChildView
Class2=CCMyExplorerApp
Class3=CAboutDlg
Class4=CExplorerListView
Class5=CExplorerTreeView
Class6=CMainFrame

ResourceCount=2
Resource1=IDR_MAINFRAME
Class7=CExplorerTreeCtrl
Resource2=IDD_ABOUTBOX

[CLS:CChildView]
Type=0
BaseClass=CWnd
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp

[CLS:CCMyExplorerApp]
Type=0
BaseClass=CWinApp
HeaderFile=CMyExplorer.h
ImplementationFile=CMyExplorer.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=CMyExplorer.cpp
ImplementationFile=CMyExplorer.cpp
LastObject=CAboutDlg

[CLS:CExplorerListView]
Type=0
BaseClass=CListView
HeaderFile=ExplorerListView.h
ImplementationFile=ExplorerListView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CExplorerListView

[CLS:CExplorerTreeView]
Type=0
BaseClass=CTreeView
HeaderFile=ExplorerTreeView.h
ImplementationFile=ExplorerTreeView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=ID_EDIT_PASTE

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_VIEW_LARGEICON
Command9=ID_VIEW_SMALLICON
Command10=ID_VIEW_LIST
Command11=ID_APP_ABOUT
CommandCount=11

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[CLS:CExplorerTreeCtrl]
Type=0
HeaderFile=ExplorerTreeCtrl.h
ImplementationFile=ExplorerTreeCtrl.cpp
BaseClass=CTreeCtrl
Filter=W
VirtualFilter=GWC
LastObject=CExplorerTreeCtrl

