#include "choosefiledialog.h"
#include "ui_choosefiledialog.h"

chooseFileDialog::chooseFileDialog(string *result, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseFileDialog)
{
    ui->setupUi(this);
    m_Result = result;
    ui->plainTextEdit->appendPlainText("default.txt");
}

chooseFileDialog::~chooseFileDialog()
{
    delete ui;
}

void chooseFileDialog::on_pushButton_clicked()
{
    QString fpath = ui->plainTextEdit->toPlainText();

    ifstream file(fpath.toLocal8Bit().constData());
    if(file.good()){
        *m_Result = fpath.toLocal8Bit().constData();
        this->close();
    }else {
        ui->label->setText("Soubor nelze otevřít.");
    }

}

void chooseFileDialog::on_toolButton_clicked()
{
    QString fpath;
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("Text files (*.txt)"));
    if(!fileNames.isEmpty()){
        fpath = fileNames.front();
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText(fpath);
    }
}
