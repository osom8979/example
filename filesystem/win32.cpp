
#include <iostream>
#include <string>

#include <Windows.h>

using namespace std;

// FileAPI.h (include Windows.h); Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364992(v=vs.85).aspx
std::string getTempDir()
{
    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        cerr << "GetTempPathA() ERROR: " << GetLastError() << endl;
        return std::string();
    }
    return std::string(buffer);
}

// WinBase.h (include Windows.h); Kernel32.lib
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364934(v=vs.85).aspx
std::string getWorkDir()
{
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    // Multithreaded applications and shared library code should not use the GetCurrentDirectory function
    // and should avoid using relative path names.
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

// Shlobj.h (Shell32.lib)
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181.aspx
// http://stackoverflow.com/questions/9542611/how-to-get-the-current-users-home-directory-in-windows
#include <Shlobj.h>
std::string getHomeDir()
{
    // See more: Access Control. @ref https://msdn.microsoft.com/en-us/library/windows/desktop/aa374860.aspx
    HANDLE const USUALLY_TOKEN = NULL;
    HANDLE const DEFAULT_USER_TOKEN = (HANDLE)-1;

    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    HRESULT const RESULT = SHGetFolderPathA(NULL, CSIDL_PROFILE, USUALLY_TOKEN, 0, buffer);
    if (RESULT != S_OK) {
        cerr << "SHGetFolderPathA() SECOND ERROR: " << RESULT << endl;
        return std::string();
    }
    return std::string(buffer);
}

// http://stackoverflow.com/questions/2647429/c-windows-path-to-the-folder-where-the-executable-is-located
std::string getExePath(std::size_t extend_buffer_size = MAX_PATH + 1)
{
    std::string buffer(extend_buffer_size, '\0');

    // hModule: If this parameter is NULL, GetModuleFileName retrieves the path of the executable file of the current process.
    DWORD const COPIED_LENGTH = GetModuleFileNameA(NULL, &buffer[0], buffer.size());

    if (COPIED_LENGTH == 0) {
        cerr << "GetModuleFileNameA() SECOND ERROR: " << GetLastError() << endl;
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

bool isDirectory(std::string const & path)
{
    DWORD const ATTRIBUTES = GetFileAttributesA(&path[0]);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

bool isRegularFile(std::string const & path)
{
    DWORD const ATTRIBUTES = GetFileAttributesA(&path[0]);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

int main(int argc, char ** argv)
{
    cout << "win32 api test program!\n";
    cout << "getTempDir(): " << getTempDir() << endl;
    cout << "getWorkDir(): " << getWorkDir() << endl;
    cout << "getHomeDir(): " << getHomeDir() << endl;
    cout << "getExePath(): " << getExePath() << endl;

    cout << isDirectory(getTempDir()) << isRegularFile(getTempDir()) << endl;
    cout << isDirectory(getWorkDir()) << isRegularFile(getWorkDir()) << endl;
    cout << isDirectory(getHomeDir()) << isRegularFile(getHomeDir()) << endl;
    cout << isDirectory(getExePath()) << isRegularFile(getExePath()) << endl;

    return 0;
}

