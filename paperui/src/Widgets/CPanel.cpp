#include "Widgets/CPanel.h"

CPanel::CPanel(CWidget* parent /*= nullptr*/)
	: CWidget(parent)
{

}

CPanel::~CPanel()
{

}

void CPanel::OnPaint(CPainter& painter)
{

}

void CPanel::NcPaint(CPainter& painter)
{
	CPainter newPainter = painter.CreateCompatiblePainter(m_rect.Width(), m_rect.Height());
	newPainter.BeginPaint();
	newPainter.Clear(m_backgroundColor);
	OnPaint(newPainter);
	newPainter.EndPaint();
	CBitmap bitmap = newPainter.GetBitmap();
	painter.DrawBitmap(bitmap, m_rect);
	bitmap.Destroy();
	newPainter.Destroy();
}
