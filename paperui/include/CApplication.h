#ifndef PAPERUI_CAPPLICATION_H
#define PAPERUI_CAPPLICATION_H

#include "platformdef.h"
#include "Render/CFont.h"

class PAPERUI_API CApplication
{
public:
    CApplication(int argc, char** argv);
    virtual ~CApplication();

    void SetDefaultFont(const CFont& font);

    inline CFont* DefaultFont() {
        return m_defaultFont;
    }

    int Run();
    //退出程序，会等待消息循环退出
    static void Quit(int code = 0);
    //强行退出程序
    static void Exit(int code = 0);
private:
    bool Initialize();
    void Destroy();
    bool m_bInitOK;
    CFont* m_defaultFont;
};

#endif  // PAPERUI_CAPPLICATION_H