#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#define APPVERSION "0.3.0"

#include <QDialog>

namespace Ui {
class Dialog;
}

class AboutDialog : public QDialog
{
  Q_OBJECT
public:
  explicit AboutDialog(QWidget *parent = nullptr);
  ~AboutDialog();

private:
  Ui::Dialog *ui;
};

#endif // ABOUTDIALOG_H
