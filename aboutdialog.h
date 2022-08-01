#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    #define APPVERSION  "0.2.4"
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();


private:
    Ui::Dialog *ui;
};

#endif // ABOUTDIALOG_H
