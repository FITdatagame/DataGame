#include "overwritedialog.h"
#include "ui_overwritedialog.h"

OverwriteDialog::OverwriteDialog(bool * result, string *fpath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverwriteDialog)
{
    ui->setupUi(this);
    m_Result = result;
    m_Fpath = fpath;
}

void OverwriteDialog::run(){
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
}

OverwriteDialog::~OverwriteDialog()
{
    delete ui;
}

void OverwriteDialog::on_pushButton_clicked()
{
    *m_Fpath = (string)ui->plainTextEdit->toPlainText().toLocal8Bit().constData();
    *m_Result = true;
    this->close();
}

void OverwriteDialog::on_pushButton_2_clicked()
{
    *m_Fpath = (string)ui->plainTextEdit->toPlainText().toLocal8Bit().constData();
    *m_Result = false;
    this->close();
}

void OverwriteDialog::on_pushButton_3_clicked()
{
    QString name = ui->plainTextEdit->toPlainText();
    if(name.size() == 0) {
        ui->label_4->setText("Zadejte validní název!");
    }
    std::ifstream file(name.toLocal8Bit().constData());

    if(file.good()){
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->pushButton->show();
        ui->pushButton_2->show();
    } else{
        *m_Fpath = (string) name.toLocal8Bit().constData();
        *m_Result = true;
        this->close();
    }


}
