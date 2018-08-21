/**
 * @file   Config.hpp
 * @brief  Config class prototype.
 * @author zer0
 * @date   2017-04-01
 */

#ifndef __INCLUDE_DSN__CONFIG_HPP__
#define __INCLUDE_DSN__CONFIG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <wx/wx.h>
#include <wx/fileconf.h>

#ifndef DSN_CONFIG_EXEC_FILE_DEFAULT_VALUE
# if defined(WIN32)
#  define DSN_CONFIG_EXEC_FILE_DEFAULT_VALUE "${EXE_DIR}/bin/run.exe";
# else
#  define DSN_CONFIG_EXEC_FILE_DEFAULT_VALUE "${EXE_DIR}/bin/run";
# endif
#endif

#ifndef DSN_CONFIG_PROPERTIES
#define DSN_CONFIG_PROPERTIES(_XX) \
    _XX(wxString,  app,      name, wxT("Application")) \
    _XX(wxString, view,      type, wxT("image")) \
    _XX(wxString, view,     image, wxT("${EXE_DIR}/preview.jpg")) \
    _XX(wxString, update,    base, wxT("osom8979.github.io")) \
    _XX(wxString, update, ver_xml, wxT("/tbag/version.xml")) \
    _XX(wxString, update,  imgage, wxT("/tbag/images/blacktocat.png")) \
    _XX(wxString, exec,      file, wxT(DSN_CONFIG_EXEC_FILE_DEFAULT_VALUE)) \
    /* -- END -- */
#endif

#ifndef DSN_CONFIG_POD_PROPERTIES
#define DSN_CONFIG_POD_PROPERTIES(_XX) \
    _XX( int, app,  width,   800) \
    _XX( int, app, height,   600) \
    _XX(bool, app,  debug, false) \
    /* -- END -- */
#endif

/**
 * Config class prototype.
 *
 * @author zer0
 * @date   2017-04-01
 */
class Config
{
#ifndef _DSN_CONFIG_XX
#define _DSN_CONFIG_XX(type, group, key, def) \
    private: \
        type _##group##_##key; \
    public: \
        inline type get_##group##_##key() const { return _##group##_##key; } \
        inline void set_##group##_##key(type const & v = type()) { _##group##_##key = v; } \
        inline void def_##group##_##key() { _##group##_##key = def; } \
    public: \
        inline wxString path_##group##_##key() { return Config::getPath(wxT(#group), wxT(#key)); } \
        inline bool read_##group##_##key(wxFileConfig & config) { return config.Read(path_##group##_##key(), &_##group##_##key); } \
        inline bool write_##group##_##key(wxFileConfig & config) { return config.Write(path_##group##_##key(), _##group##_##key); } \
    private:
#endif

#ifndef _DSN_CONFIG_XX2
#define _DSN_CONFIG_XX2(type, group, key, def) \
    public: \
        inline type env_##group##_##key() const { return Config::reg(_##group##_##key); } \
    private:
#endif

DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX);
DSN_CONFIG_PROPERTIES(_DSN_CONFIG_XX2);
DSN_CONFIG_POD_PROPERTIES(_DSN_CONFIG_XX);

#undef _DSN_CONFIG_XX2
#undef _DSN_CONFIG_XX

public:
    Config();
    Config(wxString const & path);
    virtual ~Config();

public:
    void clear();
    void setDefault();

public:
    bool read(wxString const & path);
    bool save(wxString const & path, bool with_debug = false);

public:
    static bool saveDefault(bool with_debug = false);
    static bool existsDefault();
    static wxString getDefaultPath();

public:
    static wxString getPath(wxString const & group, wxString const & key);

public:
    static int replaceValue(wxString & str, wxString const & name, wxString const & value);
    static int replaceHomeDir(wxString & str);
    static int replaceWorkDir(wxString & str);
    static int replaceExeDir(wxString & str);
    static int replaceAll(wxString & str);

public:
    static wxString reg(wxString const & str);

public:
    static wxString getExePath();
    static wxString getExeDir();

public:
    static bool exists(wxString const & path);

public:
    static void print();
};

#endif // __INCLUDE_DSN__CONFIG_HPP__

