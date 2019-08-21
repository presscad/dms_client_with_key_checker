; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CListenSocket
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ChatServer.h"

ClassCount=5
Class1=CChatServerApp
Class2=CChatServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CServerSocket
Class5=CListenSocket
Resource3=IDD_CHATSERVER_DIALOG

[CLS:CChatServerApp]
Type=0
HeaderFile=ChatServer.h
ImplementationFile=ChatServer.cpp
Filter=N

[CLS:CChatServerDlg]
Type=0
HeaderFile=ChatServerDlg.h
ImplementationFile=ChatServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON_SEND

[CLS:CAboutDlg]
Type=0
HeaderFile=ChatServerDlg.h
ImplementationFile=ChatServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHATSERVER_DIALOG]
Type=1
Class=CChatServerDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_EDIT_WORDS,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_LIST_WORDS,listbox,1352728833
Control6=IDC_BUTTON_SEND,button,1476460545
Control7=IDC_BUTTON_CLEAR,button,1342242816
Control8=IDC_BUTTON_SAVE,button,1342242816
Control9=IDC_BUTTON_ABOUT,button,1342242816

[CLS:CServerSocket]
Type=0
HeaderFile=ServerSocket.h
ImplementationFile=ServerSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q

[CLS:CListenSocket]
Type=0
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=CListenSocket

