#include "OriStatusBar.h"
#include "OriLabels.h"

namespace Ori {
namespace Widgets {

////////////////////////////////////////////////////////////////////////////////
//                                StatusBar
////////////////////////////////////////////////////////////////////////////////

StatusBar::StatusBar(int count, QWidget *parent) : QStatusBar(parent)
{
    for (int i = 0; i < count; i++)
    {
        auto label = new ClickableLabel;
        _sections.push_back(label);
        addWidget(label);
    }
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

void StatusBar::setText(int index, const QString &text)
{
    _sections[index]->setText(QString("  %1  ").arg(text));
}

void StatusBar::setText(int index, const QString& text, const QString& tooltip)
{
    setText(index, text);
    setHint(index, tooltip);
}

void StatusBar::setIcon(int index, const QString &path)
{
    _sections[index]->setPixmap(QPixmap(path));
}

void StatusBar::setToolTip(int index, const QString& tooltip)
{
    _sections[index]->setToolTip(tooltip);
}

void StatusBar::clear(int index)
{
    setText(index, QString());
    setHint(index, QString());
    setIcon(index, QString());
    highlightReset(index);
}

void StatusBar::clear()
{
    for (int i = 0; i < _sections.size(); i++) clear(i);
}

void StatusBar::connect(int index, const char *signal, const QObject *receiver, const char *method)
{
    QStatusBar::connect(_sections[index], signal, receiver, method);
}

void StatusBar::highlightError(int index)
{
    _sections[index]->setStyleSheet("background:red;color:white;font-weight:bold");
}

void StatusBar::highlightReset(int index)
{
    _sections[index]->setStyleSheet(QString());
}

} // namespace Widgets
} // namespace Ori
