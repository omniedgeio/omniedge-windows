#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDate>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    adjustSize();

    //setFixedSize(size());
    this->setModal(false);
    ui->label_contact->setTextFormat(Qt::RichText);
    ui->label_contact->setText(QString(tr("Contact Us: "))+"<style> a {text-decoration: none} </style> <a href='mailto:support@omniedge.io'>support@omniedge.io</a>");
    ui->label_contact->setOpenExternalLinks(true);
    ui->label_twitter->setOpenExternalLinks(true);
    ui->label_twitter->setText(QString(tr("Twitter:"))+QString::fromLocal8Bit("<style> a {text-decoration: none} </style> <a href = https://twitter.com/omniedgeio> @omniedgeio</a>"));
    static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    ui->label_release->setText(tr("Release: ")+buildDate.toString("yyyy-MM-dd"));
    ui->label_version->setText(tr("Version: ")+ QString(APPVERSION));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

