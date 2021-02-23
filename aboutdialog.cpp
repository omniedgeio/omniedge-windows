#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //ui->label_contact->setText(QString::fromLocal8Bit("<a style='color: green;' href = www.doshow.com> 查看</a>"));
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    adjustSize();
    setFixedSize(size());
    this->setModal(false);
    ui->label_contact->setTextFormat(Qt::RichText);
    ui->label_contact->setText(QString("Contact Us: ")+"<style> a {text-decoration: none} </style> <a href='mailto:support@omniedge.io'>support@omniedge.io</a>");
    ui->label_contact->setOpenExternalLinks(true);
    ui->label_twitter->setOpenExternalLinks(true);
    ui->label_twitter->setText(QString("Twitter:")+QString::fromLocal8Bit("<style> a {text-decoration: none} </style> <a href = https://twitter.com/omniedgeio> @omniedgeio</a>"));

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

