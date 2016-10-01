#ifndef ORI_BASIC_CONFIG_DLG_H
#define ORI_BASIC_CONFIG_DLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QListWidget;
class QStackedWidget;
class QLabel;
QT_END_NAMESPACE

namespace Ori {
namespace Dlg {

class BasicConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BasicConfigDialog(QWidget* parent = 0);

    virtual void populate() {}
    virtual void collect() {}

    bool run() { return exec() == QDialog::Accepted; }

    int currentPageIndex() const;
    void setCurrentPageIndex(int index);

protected:
    void createPages(QList<QWidget*>);

protected slots:
    void pageListItemSelected(int index);
    void showHelp();
    void accept();

private:
    QListWidget* pageList;
    QStackedWidget* pageView;
    QLabel* pageHeader;
    QWidget* helpButton;

    void adjustPageList();
    void adjustHelpButton();
    QString currentHelpTopic() const;
};


class BasicConfigPage : public QWidget
{
public:
    explicit BasicConfigPage(const QString& title,
                             const QString& iconPath = QString(),
                             const QString& helpTopic = QString());

    const QString& helpTopic() const { return _helpTopic; }

    void add(std::initializer_list<QObject*> items);

private:
    QString _helpTopic;
    QVBoxLayout* _mainLayout;
};

} // namespace Dlg
} // namespace Ori

#endif // ORI_BASIC_CONFIG_DLG_H
