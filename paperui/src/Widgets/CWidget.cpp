#include "Widgets/CWidget.h"
#include "Render/CPainter.h"
CWidget::CWidget(CWidget* parent)
	: m_rect(10, 10, 74, 74)
	, m_parent(parent)
{
}
CWidget::~CWidget()
{

}

void CWidget::Resize(int width, int height)
{
	m_rect.Resize(width, height);
}

void CWidget::OnPaint(CPainter& painter)
{
	painter.Clear(m_backgroundColor);
}

void CWidget::NcPaint(CPainter& painter)
{
	OnPaint(painter);
}
