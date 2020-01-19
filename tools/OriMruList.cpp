#include "OriMruList.h"
#include "OriSettings.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>

namespace Ori {

//------------------------------------------------------------------------------
//                                MruList
//------------------------------------------------------------------------------

MruList::MruList(QObject *parent): QObject(parent)
{
    _actionClearAll = new QAction(tr("Clear History"), this);
    connect(_actionClearAll, SIGNAL(triggered()), this, SLOT(clearAll()));

    _actionClearInvalids = new QAction(tr("Delete Invalid Items"), this);
    connect(_actionClearInvalids, SIGNAL(triggered()), this, SLOT(clearInvalids()));
}

void MruList::append(const QString& item)
{
    if (item.isEmpty()) return;

    auto a = action(item);
    if (a)
        _actions.removeOne(a);
    else
        a = makeAction(item);
    _actions.prepend(a);
    trimActions();
    update();
}

QAction* MruList::action(const QString& item) const
{
    int index = indexOf(item);
    return index >= 0? _actions.at(index): nullptr;
}

int MruList::indexOf(const QString& item) const
{
    for (int i = 0; i < _actions.size(); i++)
        if (sameItems(_actions.at(i)->text(), item))
            return i;
    return -1;
}

bool MruList::sameItems(const QString& item1, const QString& item2) const
{
    return item1 == item2;
}

void MruList::load(const QString &key)
{
    Ori::Settings s;
    load(s.settings(), key);
}

void MruList::load(QSettings* settings, const QString& key)
{
    _settingGroup = settings->group();
    _settingsKey = key.isEmpty()? "mru": key;
    auto items = settings->value(_settingsKey).toStringList();
    for (const QString& item : items)
        if (_maxCount < 0 || _actions.size() < _maxCount)
            _actions.append(makeAction(item));
    update();
}

void MruList::save()
{
    if (!_settingsKey.isEmpty())
    {
        Settings s;
        if (!_settingGroup.isEmpty())
            s.beginGroup(_settingGroup);
        s.setValue(_settingsKey, items());
    }
}

QStringList MruList::items() const
{
    QStringList list;
    foreach (const QAction* action, _actions)
        list.append(action->text());
    return list;
}

QAction* MruList::makeAction(const QString &item)
{
    QAction *action = new QAction(item, this);
    connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));
    return action;
}

void MruList::update()
{
    _actionClearAll->setEnabled(!_actions.empty());
    _actionClearInvalids->setEnabled(!_actions.empty());

    save();

    emit changed();
}

void MruList::actionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action)
    {
        QString s = action->text();

        if (_actions.indexOf(action) > 0)
            append(s); // move to top

        if (canClick(s))
            emit clicked(s);
    }
}

void MruList::clearAll()
{
    foreach (QAction *a, _actions) delete a;
    _actions.clear();
    update();
}

void MruList::clearInvalids()
{
    auto invalids = invalidItems();
    for (auto action : invalids)
    {
        _actions.removeOne(action);
        delete action;
    }
    QString msg;
    if (invalids.size() > 0)
    {
        update();
        msg = tr("Items deleted: %1").arg(invalids.size());
    }
    else
        msg = tr("All items are ok");
    QMessageBox::information(qApp->activeWindow(), qApp->applicationName(), msg);
}

QList<QAction*> MruList::invalidItems() const
{
    QList<QAction*> list;
    for (auto action : _actions)
        if (!validateItem(action->text()))
            list.append(action);
    return list;
}

void MruList::setMaxCount(int value)
{
    _maxCount = value;
    trimActions();
}

void MruList::trimActions()
{
    if (_maxCount < 0) return;
    int prevCount = _actions.size();
    while (_actions.size() > _maxCount)
    {
        auto a = _actions.last();
        _actions.removeLast();
        delete a;
    }
    if (prevCount != _actions.size())
        emit changed();
}

//------------------------------------------------------------------------------
//                               MruFileList
//------------------------------------------------------------------------------

bool MruFileList::sameItems(const QString& item1, const QString& item2) const
{
    return QFileInfo(item1) == QFileInfo(item2);
}

bool MruFileList::validateItem(const QString& item) const
{
    return QFile::exists(item);
}

bool MruFileList::canClick(const QString& item) const
{
    if (!validateItem(item))
    {
        QMessageBox::critical(qApp->activeWindow(), qApp->applicationName(), tr("File not found"));
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Ori

