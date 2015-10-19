#include "sendsettings.h"
#include "ui_sendsettings.h"
#include <fstream>
#include <cstdlib>

SendSettings::SendSettings(vector<Student *> * students, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendSettings)
{
    ui->setupUi(this);
    m_Students = students;

}

void SendSettings::run(){
    setStudents();
}

SendSettings::~SendSettings()
{
    delete ui;
}

void SendSettings::setStudents(){
    ui->label_2->setAlignment(Qt::AlignCenter);

    if(m_Students->empty()){ //no students added this run
        string fpath;
        chooseFileDialog cfd(&fpath);
        cfd.setModal(true);
        cfd.exec();
        if(!loadFile(fpath)){
            ui->label_2->setText("Studenti nenačteni!");
            return;
        }
        QString text = "Studenti načteni z " + QString::fromStdString(fpath);
        ui->label_2->setText(text);
    } else {
        ui->label_2->setText("Studenti načteni z paměti programu.");
    }
}


bool SendSettings::loadFile(string fpath){
    string name;
    string path;

    if(fpath.empty()){
        return false;
    }

    ifstream file(fpath.c_str());

    file.peek();
    if(!file.good()){
        return false;
    }

    bool flag = false;
    while(file.good()){
        file >> name;
        file >> path;
        if(!file.good())
            return true;
        for(unsigned int i = 0 ; i < m_Students->size(); i++){
            if((*m_Students)[i]->getName().toLocal8Bit().constData() == name
                    || (*m_Students)[i]->getPath().toLocal8Bit().constData() == path)
            {
                ui->label_2->setText("Studenti se opakují!");
                flag = true;
            }
        }
        if(flag) {
            flag = false;
            continue;
        }

        m_Students->push_back(new Student(QString::fromStdString(name), QString::fromStdString(path)));
    }

    return true;
}




void SendSettings::on_pushButton_clicked()
{
    if(m_Students->empty()){
        ui->label_2->setText("Nemohu rozeslat nastavení!");
        return;
    }
    ofstream file;

    int difficulty = ui->horizontalSlider->sliderPosition();

    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        string fpath = (*m_Students)[i]->getPath().toLocal8Bit().constData();
        fpath += "settings.txt";
        file.open(fpath.c_str() , fstream::out | fstream::trunc);

        if(!file.good()){
            ui->label_2->setText(QString::fromStdString("Nepodařilo se vytvořit soubor: " + fpath));
            file.close();
            return;
        }

        file << difficulty << endl << (*m_Students)[i]->getName().toLocal8Bit().constData() << endl;

        file.close();
    }
    ui->label_2->setText(QString::fromStdString("Nastavení rozeslána!"));
}

void SendSettings::on_horizontalSlider_sliderMoved(int position)
{
    char convert [4];
    snprintf(convert, sizeof(convert), "%d", position);
    ui->label_3->setText(QString::fromStdString(string(convert)));
}
