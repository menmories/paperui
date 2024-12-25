#include <stdio.h>

#include <Window/CMainWindow.h>
#include <CApplication.h>
#include <string>

class MainWindow : public CMainWindow
{
public:
    MainWindow()
        : m_font("Microsoft YaHei", 16)
        , m_brush(CColor(255, 10, 0))
    {
        std::string image1 = "F:/i/keai/113969089_p1.jpg";
        if (!m_bitmap.Load(image1.c_str()))
        {
            fprintf(stderr, "Load image file %s failed.\n", image1.c_str());
        }
        Resize(m_bitmap.Width() / 3, m_bitmap.Height() / 3);
    }
    ~MainWindow()
    {

    }

    virtual void OnPaint(CPainter& painter)
    {
		/*CRect clientRect = this->GetRect();
		painter.DrawBitmap(m_bitmap, clientRect);
		painter.DrawText(clientRect, "Hello World!", m_font, m_brush);*/
    }

private:
    CFont m_font;
    CBrush m_brush;
    CBitmap m_bitmap;
    CRect rcImage;
};


int main(int argc, char** argv)
{
    CApplication app(argc, argv);
    MainWindow window;
    window.CenterWindow();
    window.Show();
    return app.Run();
}
