

#if !defined(AFX_DIRECTORYCHANGES_H__02E53FDE_CB22_4176_B6D7_DA3675D9F1A6__INCLUDED_)
#define AFX_DIRECTORYCHANGES_H__02E53FDE_CB22_4176_B6D7_DA3675D9F1A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <afxmt.h>
#include <afxtempl.h>


#define READ_DIR_CHANGE_BUFFER_SIZE 4096
class CBaseDirMonitor;
typedef struct _thread_param{
	char cType;
	ULONGLONG  id;
	PVOID   mpParam;
	CTime mtime;
	CString mStrRootPath,mFileName,mNewFileName;
	ULONGLONG batchID;
	CBaseDirMonitor* mpBaseDirMonitor;
public:
	_thread_param(char type,ULONGLONG nid,ULONGLONG bID,PVOID pParam,CTime time,
		CString strRootPath,CString fileName,CString newFileName,CBaseDirMonitor* pBaseDirMonitor){
		id = nid;
		batchID = bID;
		cType = type;
		mpParam = pParam;
		mtime = time;
		mStrRootPath = strRootPath;
		mFileName = fileName;
		mNewFileName = newFileName;
		mpBaseDirMonitor = pBaseDirMonitor;

	};
}THREAD_PARAM;

class CFileNotifyInformation;//helper class 
class CDirectoryChangeWatcher;
class CDelayedDirectoryChangeHandler;//helper class...used in implementation
class CFileMonitor;
class CDirectoryChangeHandler

{
public:

	CDirectoryChangeHandler();
	virtual ~CDirectoryChangeHandler();

	//this class is reference counted
	long AddRef();
	long Release();
	long CurRefCnt()const;


	BOOL UnwatchDirectory();//causes CDirectoryChangeWatcher::UnwatchDirectory() to be called.
	
	const CString & GetChangedDirectoryName() const { return m_strChangedDirectoryName;}//WARNING: don't use this, this function will be removed in a future release.
								//returns the directory name where the change occured.  This contains
							   //the last directory to have changed if the same CDirectoryChangeHandler is
							   //being used to watch multiple directories. It will return an empty string
							   //if no changes have been made to a directory yet.   It will always be the 
							   //name of the currently changed directory(as specified in CDirectoryChangeWatcher::WatchDirectory())
							   //if called in the context of one of the
							   //On_Filexxx() functions.
protected:
	//
	//	Override these functions:
	//	These functions are called when the directory to watch has had a change made to it
	virtual void On_FileAdded(const ULONGLONG id,const CTime time,const CString & strFileName); //=0;
			//
			//	On_FileAdded()
			//
			//	This function is called when a file in one of the watched folders(or subfolders)
			//	has been created.
			//
			//	For this function to execute you'll need to specify FILE_NOTIFY_CHANGE_FILE_NAME or FILE_NOTIFY_CHANGE_DIR_NAME(for directories)
			//  when you call CDirectoryChangeWatcher::WatchDirectory()
			//
	virtual void On_FileRemoved(const ULONGLONG id,const CTime time,const CString & strFileName);// = 0;
			//
			//	On_FileRemoved()
			//
			//	This function is called when a file in one of the watched folders(or subfolders)
			//	has been deleted(or moved to another directory)
			//
			//	For this function to execute you'll need to specify FILE_NOTIFY_CHANGE_FILE_NAME or FILE_NOTIFY_CHANGE_DIR_NAME(for directories)
			//  when you call CDirectoryChangeWatcher::WatchDirecotry()
			//

	virtual void On_FileNameChanged(const ULONGLONG id,const CTime time,const CString & strOldFileName, const CString & strNewFileName);// = 0;
			//
			//	On_FileNameChanged()
			//
			//	This function is called when a file in one of the watched folders(or subfolders)
			//	has been renamed.
			//
			//
			//	You'll need to specify FILE_NOTIFY_CHANGE_FILE_NAME (or FILE_NOTIFY_CHANGE_DIR_NAME(for directories))
			//	when you call CDirectoryChangeWatcher::WatchDirectory()
			//
			//	

	virtual void On_FileModified(const ULONGLONG id,const CTime time,const CString & strFileName);// = 0;
			//
			//	On_FileModified()
			//
			//	This function is called whenever an attribute specified by the watch
			//	filter has changed on a file in the watched directory or 
			//
			//	Specify any of the following flags when you call CDirectoryChangeWatcher::WatchDirectory()
			//  
			//
			//	FILE_NOTIFY_CHANGE_ATTRIBUTES
			//	FILE_NOTIFY_CHANGE_SIZE 
			//	FILE_NOTIFY_CHANGE_LAST_WRITE 
			//	FILE_NOTIFY_CHANGE_LAST_ACCESS
			//	FILE_NOTIFY_CHANGE_CREATION (* See Note # 1* )
			//	FILE_NOTIFY_CHANGE_SECURITY
			//
			//	
			//	General Note)  Windows tries to optimize some of these notifications.  You may not get 
			//				   a notification every single time a file is accessed for example.  
			//				   There's a MS KB article or something on this(sorry forgot which one).
			//
			//	Note #1	)   This notification isn't what you might think(FILE_NOTIFY_CHANGE_CREATION). 
			//				See the help files for ReadDirectoryChangesW...
			//				This notifies you of a change to the file's 
			//				creation time, not when the file is created.  
			//				Use FILE_NOTIFY_CHANGE_FILE_NAME to know about newly created files.
			//

	virtual void On_ReadDirectoryChangesError(DWORD dwError, const CString & strDirectoryName);
			//
			//	On_ReadDirectoryChangesError()
			//
			//	This function is called when ReadDirectoryChangesW() fails during normal
			//	operation (ie: some time after you've called CDirectoryChangeWatcher::WatchDirectory())
			//
			//
			//	NOTE:  *** READ THIS *** READ THIS *** READ THIS *** READ THIS ***
			//
			//	NOTE: If this function has been called, the watched directory has been automatically unwatched.
			//			You will not receive any further notifications for that directory until 
			//			you call CDirectoryChangeWatcher::WatchDirectory() again.
			//
			//	On_WatchStopped() will not be called.


	virtual void On_WatchStarted(DWORD dwError, const CString & strDirectoryName);
			//
			//	void On_WatchStarted()
			//
			//	This function is called when a directory watch has begun.  
			//	It will be called whether CDirectoryChangeWatcher::WatchDirectory() is successful or not. Check the dwError parameter.
			//
			//	PARAMETERS:
			//	DWORD dwError					 -- 0 if successful, else it's the return value of GetLastError() 
			//										indicating why the watch failed.
			//	const CString & strDirectoryName -- The full path and name of the directory being watched.
	
	virtual void On_WatchStopped(const CString & strDirectoryName);
			//
			//	void On_WatchStopped()
			//
			//	This function is called when a directory is unwatched (except on the case of the direcotry being unwatched due to an error)
			//
			//	WARNING:  *** READ THIS *** READ THIS *** READ THIS *** READ THIS ***
			//
			//	This function MAY be called before the destructor of CDirectoryChangeWatcher 
			//	finishes.  
			//
			//	Be careful if your implementation of this fuction
			//	interacts with some sort of a window handle or other object(a class, a file, etc.).  
			//	It's possible that that object/window handle will NOT be valid anymore the very last time
			//	that On_WatchStopped() is called.  
			//	This scenario is likely if the CDirectoryChangeWatcher instance is currently watching a
			//	directory, and it's destructor is called some time AFTER these objects/windows
			//	your change handler interacts with have been destroyed.
			//	
			//	If your CDirectoryChangeHandler derived class interacts w/ a window or other
			//	object, it's a good idea to unwatch any directories before the object/window is destroyed.
			//	Otherwise, place tests for valid objects/windows in the implementation of this function.
			//
			//  Failure to follow either tip can result in a mysterious RTFM error, or a 'Run time errors'
			//

	virtual bool On_FilterNotification(DWORD dwNotifyAction, LPCTSTR szFileName, LPCTSTR szNewFileName);
			//
			//	bool On_FilterNotification(DWORD dwNotifyAction, LPCTSTR szFileName, LPCTSTR szNewFileName);
			//
			//	This function gives your class a chance to filter unwanted notifications.
			//
			//	PARAMETERS: 
			//			DWORD	dwNotifyAction	-- specifies the event to filter
			//			LPCTSTR szFileName		-- specifies the name of the file for the event.
			//			LPCTSTR szNewFileName	-- specifies the new file name of a file that has been renamed.
			//
			//			**	szFileName and szNewFileName will always be the full path and file name with extention.
			//
			//	RETURN VALUE:
			//			return true , and you will receive the notification.
			//			return false, and your class will NOT receive the notification.
			//
			//	Valid values of dwNotifyAction:
			//		FILE_ACTION_ADDED			-- On_FileAdded() is about to be called.
			//		FILE_ACTION_REMOVED			-- On_FileRemoved() is about to be called.
			//		FILE_ACTION_MODIFIED		-- On_FileModified() is about to be called.
			//		FILE_ACTION_RENAMED_OLD_NAME-- On_FileNameChanged() is about to be call.
			//
			//	  
			//	NOTE:  When the value of dwNotifyAction is FILE_ACTION_RENAMED_OLD_NAME,
			//			szFileName will be the old name of the file, and szNewFileName will
			//			be the new name of the renamed file.
			//
			//  The default implementation always returns true, indicating that all notifications will 
			//	be sent.
			//
			//	NOTE:	This function may or may not be called depending upon the flags you specify to control
			//			filter behavior.
			//			If you are specifying filters when watching the directory, you will not get this notification
			//			if the file name does not pass the filter test, even if this function returns true.
			//
			
	//
	//
	//	End Override these functions (ie: don't worry about the rest of this class)
	//
	
	void SetChangedDirectoryName(const CString & strChangedDirName);//please don't use this function, it will be removed in future releases.
	
private:
	long m_nRefCnt;
	
	CString m_strChangedDirectoryName;//will be removed in a future release.

	friend class CDirectoryChangeWatcher;
	friend class CDelayedDirectoryChangeHandler;
	//
	//	This class keeps a reference to the CDirectoryChangeHandler 
	//	that was used when an object of this type is passed 
	//	to CDirectoryChangeWatcher::WatchDirectory().
	//
	//	This way, when the CDirectoryChangeWatcher object is destroyed(or if CDirectoryChangeHandler::UnwatchDirectory() is called)
	//	AFTER CDirectoryChangeWatcher::UnwatchDirecotry() or CDirectoryChangeWatcher::UnwatchAllDirectories() is called 
	//	the directory(or direcotries) that this 
	//	CDirectoryChangeWatcher object is handling will be automatically unwatched
	//	If the CDirectoryChangeWatcher object is destroyed before the CDirectoryChangeHandler objects 
	//	that are being used with that watcher are destroyed, the reference counting prevents
	//	this class from referencing a destructed object.
	//	Basically, neither class needs to worry about the lifetime of the other(CDirectoryChangeWatcher && CDirectoryChangeHandler)
	//

	long  ReferencesWatcher(CDirectoryChangeWatcher * pDirChangeWatcher);
	long  ReleaseReferenceToWatcher(CDirectoryChangeWatcher * pDirChangeWatcher);
	CDirectoryChangeWatcher * m_pDirChangeWatcher;
	long   m_nWatcherRefCnt; //<-- when this reaches 0, m_pDirChangeWatcher is set to NULL
	CCriticalSection m_csWatcher;
};

///////////////////////////////////////////////////////////
class CBaseDirMonitor;
class CDirectoryChangeWatcher  
{
public:

	enum	{  //options for determining the behavior of the filter tests.
			   //
			   FILTERS_DONT_USE_FILTERS		= 1, //never test the include/exclude filters. CDirectoryChangeHandler::On_FilterNotification() is still called.
			   FILTERS_CHECK_FULL_PATH		= 2, //For the file path: "C:\FolderName\SubFolder\FileName.xyz", the entire path is checked for the filter pattern.
			   FILTERS_CHECK_PARTIAL_PATH	= 4, //For the file path: "C:\FolderName\SubFolder\FileName.xyz", only "SubFolder\FileName.xyz" is checked against the filter pattern, provided that you are watching the folder "C:\FolderName", and you are also watching subfolders.
			   FILTERS_CHECK_FILE_NAME_ONLY	= 8, //For the file path: "C:\FolderName\SubFolder\FileName.xyz", only "FileName.xyz" is checked against the filter pattern.
			   FILTERS_TEST_HANDLER_FIRST	= 16, //test CDirectoryChangeHandler::On_FilterNotification() before checking the include/exclude filters. the default is to check the include/exclude filters first.
			   FILTERS_DONT_USE_HANDLER_FILTER = 32, //CDirectoryChangeHander::On_FilterNotification() won't be called.
			   FILTERS_NO_WATCHSTART_NOTIFICATION = 64,//CDirectoryChangeHander::On_WatchStarted() won't be called.
			   FILTERS_NO_WATCHSTOP_NOTIFICATION  = 128,//CDirectoryChangeHander::On_WatchStopped() won't be called.
			   FILTERS_DEFAULT_BEHAVIOR	= (FILTERS_CHECK_FILE_NAME_ONLY ),
			   FILTERS_DONT_USE_ANY_FILTER_TESTS = (FILTERS_DONT_USE_FILTERS | FILTERS_DONT_USE_HANDLER_FILTER),
			   FILTERS_NO_WATCH_STARTSTOP_NOTIFICATION = (FILTERS_NO_WATCHSTART_NOTIFICATION | FILTERS_NO_WATCHSTOP_NOTIFICATION)
			};

	//ctor/dtor
	CDirectoryChangeWatcher(bool bAppHasGUI = true, DWORD dwFilterFlags = FILTERS_DEFAULT_BEHAVIOR);//see comments in ctor .cpp file.
	virtual ~CDirectoryChangeWatcher();

	CTime m_startTime;

	//ULONGLONG m_startID;

	//
	//	Starts a watch on a directory:
	//
	DWORD	WatchDirectory(const CString & strDirToWatch, 
						   DWORD dwChangesToWatchFor,
						   CBaseDirMonitor* pBaseDirMonitor, 
						   BOOL bWatchSubDirs = FALSE,
						   LPCTSTR szIncludeFilter = NULL,
						   LPCTSTR szExcludeFilter = NULL);

	BOOL	IsWatchingDirectory (const CString & strDirName)const;
	int		NumWatchedDirectories()const; //counts # of directories being watched.

	
	BOOL	UnwatchDirectory(const CString & strDirToStopWatching);//stops watching specified directory.
	BOOL	UnwatchAllDirectories();//stops watching ALL watched directories.

	DWORD	SetFilterFlags(DWORD dwFilterFlags);//sets filter behavior for directories watched AFTER this function has been called.
	DWORD	GetFilterFlags()const{return m_dwFilterFlags;}

protected:
	
	virtual void On_ThreadInitialize(){}//All file change notifications has taken place in the context of a worker thread...do any thread initialization here..
	virtual void On_ThreadExit(){}//do thread cleanup here
	
private:
	friend class CDirectoryChangeHandler;
	BOOL UnwatchDirectory(CDirectoryChangeHandler * pChangeHandler);//called in CDirectoryChangeHandler::~CDirectoryChangeHandler()


	UINT static MonitorDirectoryChanges(LPVOID lpvThis );//the worker thread that monitors directories.
public:	
	class CDirWatchInfo 
		//this class is used internally by CDirectoryChangeWatcher
		//to help manage the watched directories
	{
	private:
		CDirWatchInfo();		//private & not implemented
		CDirWatchInfo & operator=(const CDirWatchInfo & rhs);//so that they're aren't accidentally used. -- you'll get a linker error
	public:
		CDirWatchInfo(HANDLE hDir, const CString & strDirectoryName, 
					  CBaseDirMonitor* pBaseDirMonitor, 
					  DWORD dwChangeFilter, BOOL bWatchSubDir, 
					  bool bAppHasGUI,
					  LPCTSTR szIncludeFilter,
					  LPCTSTR szExcludeFilter,
					  DWORD dwFilterFlags);
	private:
		~CDirWatchInfo( );//only I can delete myself....use DeleteSelf()
	public:
		void DeleteSelf(CDirectoryChangeWatcher * pWatcher);
		
		DWORD StartMonitor(HANDLE hCompPort);
		BOOL UnwatchDirectory( HANDLE hCompPort );
	protected:
		BOOL SignalShutdown( HANDLE hCompPort );
		BOOL WaitForShutdown();			   
	public:
		BOOL LockProperties() { return m_cs.Lock(); }
		BOOL UnlockProperties(){ return m_cs.Unlock();	}

		CDelayedDirectoryChangeHandler* GetChangeHandler() const;
		CDirectoryChangeHandler * GetRealChangeHandler() const;//the 'real' change handler is your CDirectoryChangeHandler derived class.
		CDirectoryChangeHandler * SetRealDirectoryChangeHandler(CDirectoryChangeHandler * pChangeHandler);
		
		BOOL CloseDirectoryHandle();

		CBaseDirMonitor* m_pBaseDirMonitor;
		
		//CDirectoryChangeHandler * m_pChangeHandler;
		//CDelayedDirectoryChangeHandler * m_pChangeHandler;
		HANDLE      m_hDir;//handle to directory that we're watching
		DWORD		m_dwChangeFilter;
		BOOL		m_bWatchSubDir;
		CString     m_strDirName;//name of the directory that we're watching
		CHAR        m_Buffer[ READ_DIR_CHANGE_BUFFER_SIZE ];//buffer for ReadDirectoryChangesW
		DWORD       m_dwBufLength;//length or returned data from ReadDirectoryChangesW -- ignored?...
		OVERLAPPED  m_Overlapped;
		DWORD		m_dwReadDirError;//indicates the success of the call to ReadDirectoryChanges()
		CCriticalSection m_cs;
		CEvent		m_StartStopEvent;
		ULONGLONG   m_StartID;
		enum eRunningState{
			 RUNNING_STATE_NOT_SET, RUNNING_STATE_START_MONITORING, RUNNING_STATE_STOP, RUNNING_STATE_STOP_STEP2,
			  RUNNING_STATE_STOPPED, RUNNING_STATE_NORMAL
			 };
		eRunningState m_RunningState;
		};//end nested class CDirWatchInfo 

	void ProcessChangeNotifications(IN CFileNotifyInformation & notify_info, 
									IN CDirWatchInfo * pdi,
									OUT DWORD & ref_dwReadBuffer_Offset,CTime time ,ULONGLONG id);
	friend class CDirWatchInfo;//so that CDirWatchInfo can call the following function.
	long ReleaseReferenceToWatcher(CDirectoryChangeHandler * pChangeHandler);

	BOOL	UnwatchDirectoryBecauseOfError(CDirWatchInfo * pWatchInfo);//called in case of error.
	int		AddToWatchInfo(CDirWatchInfo * pWatchInfo);
	//
	//	functions for retrieving the directory watch info based on different parameters
	//
	CDirWatchInfo *	GetDirWatchInfo(IN const CString & strDirName, OUT int & ref_nIdx)const;
	CDirWatchInfo *	GetDirWatchInfo(IN CDirWatchInfo * pWatchInfo, OUT int & ref_nIdx)const;
	CDirWatchInfo * GetDirWatchInfo(IN CDirectoryChangeHandler * pChangeHandler, OUT int & ref_nIdx)const;
	

	HANDLE m_hCompPort;	//i/o completion port
	HANDLE m_hThread;	//MonitorDirectoryChanges() thread handle
	DWORD  m_dwThreadID;
	CTypedPtrArray<CPtrArray, CDirWatchInfo*> m_DirectoriesToWatch; //holds info about the directories that we're watching.
	CCriticalSection m_csDirWatchInfo;

	bool	m_bAppHasGUI; //dispatch to main thread, or a worker thread?
	DWORD	m_dwFilterFlags;//options for determining the behavior of the filter tests.
	
};


#endif // !defined(AFX_DIRECTORYCHANGES_H__02E53FDE_CB22_4176_B6D7_DA3675D9F1A6__INCLUDED_)
