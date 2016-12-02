// FilesystemExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

using namespace std;

// FileAPI.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364992(v=vs.85).aspx
std::string getTempDir()
{
    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    // The GetTempPath function checks for the existence of environment variables in the following order and uses the first path found:
    //  # The path specified by the TMP environment variable.
    //  # The path specified by the TEMP environment variable.
    //  # The path specified by the USERPROFILE environment variable.
    //  # The Windows directory.
    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        cerr << "GetTempPathA() ERROR: " << GetLastError() << endl;
        return std::string();
    }
    return std::string(buffer);
}

// WinBase.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364934(v=vs.85).aspx
std::string getWorkDir()
{
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    // [WARNING] Multithreaded applications and shared library code should not use the GetCurrentDirectory function
    // and should avoid using relative path names.
    // ---------------------------------------------------------------------------------------------------------------------
    // Each process has a single current directory that consists of two parts:
    // # A disk designator that is either a drive letter followed by a colon, or a server name followed by a share name(\\servername\sharename)
    // # A directory on the disk designator
    DWORD const WRITTEN_LENGTH = GetCurrentDirectoryA(BUFFER_LENGTH, buffer);
    
    if (WRITTEN_LENGTH == 0) {
        cerr << "GetCurrentDirectoryA() FIRST ERROR: " << GetLastError() << endl;
        return std::string();

    } else if (WRITTEN_LENGTH > BUFFER_LENGTH) {
        // If the buffer that is pointed to by lpBuffer is not large enough,
        // the return value specifies the required size of the buffer,
        // in characters, including the null-terminating character.
        std::string result;
        result.resize(WRITTEN_LENGTH);
        result[WRITTEN_LENGTH - 1] = '\0';

        if (GetCurrentDirectoryA(WRITTEN_LENGTH, &result[0]) == 0) {
            cerr << "GetCurrentDirectoryA() SECOND ERROR: " << GetLastError() << endl;
            return std::string();
        }
        return result;
    }

    return std::string(buffer);
}

// Shlobj.h
// Shell32.lib
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181.aspx
#include <Shlobj.h>
std::string getHomeDir()
{
    // See more: Access Control.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa374860.aspx
    //HANDLE const DEFAULT_USER_TOKEN = (HANDLE)-1;

    HANDLE const USUALLY_TOKEN = NULL;

    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    HRESULT const RESULT = SHGetFolderPathA(NULL, CSIDL_PROFILE, USUALLY_TOKEN, 0, buffer);
    if (RESULT != S_OK) {
        cerr << "SHGetFolderPathA() SECOND ERROR: " << RESULT << endl;
        return std::string();
    }
    return std::string(buffer);
}

// Winbase.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms683197(v=vs.85).aspx
std::string getExePath(std::size_t extend_buffer_size = MAX_PATH + 1)
{
    std::string buffer(extend_buffer_size, '\0');

    // hModule: If this parameter is NULL, GetModuleFileName retrieves the path of the executable file of the current process.
    DWORD const COPIED_LENGTH = GetModuleFileNameA(NULL, &buffer[0], buffer.size());

    if (COPIED_LENGTH == 0) {
        cerr << "GetModuleFileNameA() ERROR: " << GetLastError() << endl;
        return std::string();

    } else if (COPIED_LENGTH == buffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // If the buffer is too small to hold the module name,
        // the string is truncated to nSize characters including the terminating null character,
        // the function returns nSize, and the function sets the last error to ERROR_INSUFFICIENT_BUFFER.
        return getExePath(extend_buffer_size * 2);
    }

    // If the function succeeds,
    // the return value is the length of the string that is copied to the buffer,
    // in characters, not including the terminating null character.
    buffer.resize(COPIED_LENGTH);
    return buffer;
}

// Stringapiset.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd319072(v=vs.85).aspx
std::wstring mbsToWcs(std::string const & path)
{
    if (path.empty()) {
        cerr << "Illegal argument: path is 0 length.\n";
        return std::wstring();
    }

    int const WRITTEN_LENGTH = MultiByteToWideChar(CP_ACP, 0, &path[0], path.size(), NULL, 0);
    std::wstring result;

    if (WRITTEN_LENGTH == 0) {
        result.resize(path.size());
    } else {
        result.resize(WRITTEN_LENGTH + 1);
    }

    int const WRITTEN_LENGTH2 = MultiByteToWideChar(CP_ACP, 0, &path[0], path.size(), &result[0], result.size());
    if (WRITTEN_LENGTH2 == 0) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_INSUFFICIENT_BUFFER:     cerr << "MultiByteToWideChar() A supplied buffer size was not large enough, or it was incorrectly set to NULL.\n";
        case ERROR_INVALID_FLAGS:           cerr << "MultiByteToWideChar() The values supplied for flags were not valid.\n";
        case ERROR_INVALID_PARAMETER:       cerr << "MultiByteToWideChar() Any of the parameter values was invalid.\n";
        case ERROR_NO_UNICODE_TRANSLATION:  cerr << "MultiByteToWideChar() Invalid Unicode was found in a string.\n";
        default:                            cerr << "MultiByteToWideChar() ERROR: " << ERROR_CODE << endl;
        }
        return std::wstring();
    }

    result.resize(WRITTEN_LENGTH2);
    return result;
}

std::string wcsToMbs(std::wstring const & path)
{
    if (path.empty()) {
        cerr << "Illegal argument: path is 0 length.\n";
        return std::string();
    }

    int const WRITTEN_LENGTH = WideCharToMultiByte(CP_ACP, 0, &path[0], path.size(), NULL, 0, NULL, NULL);
    std::string result;

    if (WRITTEN_LENGTH == 0) {
        result.resize(path.size());
    } else {
        result.resize(WRITTEN_LENGTH + 1);
    }

    int const WRITTEN_LENGTH2 = WideCharToMultiByte(CP_ACP, 0, &path[0], path.size(), &result[0], result.size(), NULL, NULL);
    if (WRITTEN_LENGTH2 == 0) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_INSUFFICIENT_BUFFER:     cerr << "WideCharToMultiByte() A supplied buffer size was not large enough, or it was incorrectly set to NULL.\n";
        case ERROR_INVALID_FLAGS:           cerr << "WideCharToMultiByte() The values supplied for flags were not valid.\n";
        case ERROR_INVALID_PARAMETER:       cerr << "WideCharToMultiByte() Any of the parameter values was invalid.\n";
        case ERROR_NO_UNICODE_TRANSLATION:  cerr << "WideCharToMultiByte() Invalid Unicode was found in a string.\n";
        default:                            cerr << "WideCharToMultiByte() ERROR: " << ERROR_CODE << endl;
        }
        return std::string();
    }

    result.resize(WRITTEN_LENGTH2);
    return result;
}

// FileAPI.h (include Windows.h);
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363855(v=vs.85).aspx
bool createDirectory(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = CreateDirectoryA(&path[0], NULL);
    } else {
        result = CreateDirectoryW(&WCS_PATH[0], NULL);
    }

    if (result == FALSE) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_ALREADY_EXISTS:  cerr << "CreateDirectoryA() The specified directory already exists.\n";
        case ERROR_PATH_NOT_FOUND:  cerr << "CreateDirectoryA() One or more intermediate directories do not exist.\n";
        default:                    cerr << "CreateDirectoryA() ERROR: " << ERROR_CODE << endl;
        }
        return false;
    }
    return true;
}

// FileAPI.h (include Windows.h);
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365488(v=vs.85).aspx
bool removeDirectory(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = RemoveDirectoryA(&path[0]);
    } else {
        result = RemoveDirectoryW(&WCS_PATH[0]);
    }

    if (result == FALSE) {
        cerr << "RemoveDirectoryA() ERROR: " << GetLastError() << endl;
        return false;
    }
    return true;
}

// WinBase.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365239(v=vs.85).aspx
bool rename(std::string const & from, std::string const & to)
{
    std::wstring const WCS_FROM = mbsToWcs(from);
    std::wstring const WCS_TO   = mbsToWcs(to);
    BOOL result = FALSE;

    if (WCS_FROM.size() == 0 || WCS_TO.size() == 0) {
        result = MoveFileA(&from[0], &to[0]);
    } else {
        result = MoveFileW(&WCS_FROM[0], &WCS_TO[0]);
    }

    if (result == FALSE) {
        cerr << "MoveFileA() ERROR: " << GetLastError() << endl;
        return false;
    }
    return true;
}

// FileAPI.h (include Windows.h);
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363915(v=vs.85).aspx
bool remove(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = DeleteFileA(&path[0]);
    } else {
        result = DeleteFileW(&WCS_PATH[0]);
    }

    if (result == FALSE) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_FILE_NOT_FOUND:  cerr << "DeleteFileA() The system cannot find the file specified.\n";
        case ERROR_ACCESS_DENIED:   cerr << "DeleteFileA() Access is denied.\n";
        default:                    cerr << "DeleteFileA() ERROR: " << ERROR_CODE << endl;
        }
        return false;
    }
    return true;
}

// Shlwapi.h
// Shlwapi.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb773584(v=vs.85).aspx
#include <Shlwapi.h>
bool exists(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = PathFileExistsA(&path[0]);
    } else {
        result = PathFileExistsW(&WCS_PATH[0]);
    }

    if (result == FALSE) {
        cerr << "PathFileExistsA() ERROR: " << GetLastError() << endl;
        return false;
    }
    return true;
}

static DWORD getAttribute(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    if (WCS_PATH.size() == 0) {
        return GetFileAttributesA(&path[0]);
    } else {
        return GetFileAttributesW(&WCS_PATH[0]);
    }
}

bool isDirectory(std::string const & path)
{
    DWORD const ATTRIBUTES = getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

bool isRegularFile(std::string const & path)
{
    DWORD const ATTRIBUTES = getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

// FileAPI.h (include Windows.h)
// Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364418(v=vs.85).aspx
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365200(v=vs.85).aspx
#include <Strsafe.h> // StringCchLength
std::vector<std::string> scanDir(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.
    std::size_t length = 0;
    if (FAILED(StringCchLengthW(&WCS_PATH[0], MAX_PATH, &length))) {
        // The value in psz is NULL, cchMax is larger than STRSAFE_MAX_CCH, or psz is longer than cchMax.
        cerr << "StringCchLengthW() ERROR.\n";
        return std::vector<std::string>();
    }

    if (length > (MAX_PATH - 3)) {
        cerr << "Directory path is too long." << endl;
        return std::vector<std::string>();
    }

    wchar_t scan_directory[MAX_PATH] = { 0, };
    if (FAILED(StringCchCopyW(scan_directory, MAX_PATH, &WCS_PATH[0]))) {
        cerr << "StringCchCopyW() ERROR.\n";
        return std::vector<std::string>();
    }
    if (FAILED(StringCchCatW(scan_directory, MAX_PATH, L"\\*"))) {
        cerr << "StringCchCatW() ERROR.\n";
        return std::vector<std::string>();
    }

    WIN32_FIND_DATAW find_data = { 0, };
    HANDLE find_handle = FindFirstFileW(scan_directory, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        cerr << "FindFirstFileW() ERROR: " << GetLastError() << endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> result;

    // List all the files in the directory with some info about them.
    do {
        // DIRECTORY:
        //  @code
        //    find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
        //  @endcode
        // SIZE:
        //  @code
        //    LARGE_INTEGER file_size
        //    file_size.LowPart = find_data.nFileSizeLow;
        //    file_size.HighPart = find_data.nFileSizeHigh;
        //    static_cast<int64_t>(file_size.QuadPart);
        //  @endcode
        result.push_back(wcsToMbs(find_data.cFileName));
    } while (FindNextFileW(find_handle, &find_data) == TRUE);

    FindClose(find_handle);
    return std::vector<std::string>();
}

int main(int argc, char ** argv)
{
    cout << "win32 api test program!\n";
    cout << "getTempDir(): " << getTempDir() << endl;
    cout << "getWorkDir(): " << getWorkDir() << endl;
    cout << "getHomeDir(): " << getHomeDir() << endl;

    cout << isDirectory(getTempDir()) << isRegularFile(getTempDir()) << endl;
    cout << isDirectory(getWorkDir()) << isRegularFile(getWorkDir()) << endl;
    cout << isDirectory(getHomeDir()) << isRegularFile(getHomeDir()) << endl;
    cout << isDirectory(getExePath()) << isRegularFile(getExePath()) << endl;

    return 0;
}

