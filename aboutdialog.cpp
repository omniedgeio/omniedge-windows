#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    adjustSize();
    setFixedSize(size());
    this->setModal(false);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

