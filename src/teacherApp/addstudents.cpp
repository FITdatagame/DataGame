#include "addstudents.h"
#include "ui_addstudents.h"

AddStudents::AddStudents(vector<Student *> *students, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudents)
{
    ui->setupUi(this);

    m_Students = students;
}


void AddStudents::run(){
    ui->label_ZvlastPath->setDisabled(true);
    ui->label_ZvlastName->setDisabled(true);
    ui->label_SouborPath->setDisabled(true);
    ui->plainTextEdit_ZvlastPath->setDisabled(true);
    ui->plainTextEdit_ZvlastName->setDisabled(true);
    ui->plainTextEdit_SouborPath->setDisabled(true);
    ui->toolButton->setDisabled(true);
}


AddStudents::~AddStudents()
{
    delete ui;
}

void AddStudents::dohromady(){
    QString path = "";
    QString name = "";

    if(!(ui->checkBox_First->isChecked()) && !(ui->checkBox_Second->isChecked())){
        ui->label_ButtonAdd->setText("Špatná cesta!");
        return;
    }

    if(ui->plainTextEdit_DohromadyName->toPlainText().size() == 0){
        ui->label_ButtonAdd->setText("Jméno nevyplněno!");
        return;
    }
    name = ui->plainTextEdit_DohromadyName->toPlainText();

    if(ui->checkBox_First->isChecked()){
        if(ui->plainTextEdit_DohromadyPath1->toPlainText().size() == 0){
            ui->label_ButtonAdd->setText("První část cesty nevyplněna!");
            return;
        }

        path += ui->plainTextEdit_DohromadyPath1->toPlainText();
    }

    path += name;

    if(ui->checkBox_Second->isChecked()){
        if(ui->plainTextEdit_DohromadyPath2->toPlainText().size() == 0){
            ui->label_ButtonAdd->setText("Druhá část cesty nevyplněna!");
            return;
        }
        path += ui->plainTextEdit_DohromadyPath2->toPlainText();
    }

    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        if((*m_Students)[i]->getName() == name){
            ui->label_ButtonAdd->setText("Student s takovým jménem již přidán!");
            return;
        }
        if((*m_Students)[i]->getPath() == path){
            ui->label_ButtonAdd->setText("Student s takovou cestou již přidán!");
            return;
        }
    }

    m_Students->push_back(new Student(name, path));
    ui->label_ButtonAdd->setText("Student přidán!");
}

void AddStudents::zvlast(){
    QString path = ui->plainTextEdit_ZvlastPath->toPlainText();
    QString name = ui->plainTextEdit_ZvlastName->toPlainText();

    if(path.size() == 0){
        ui->label_ButtonAdd->setText("Špatná cesta!");
        return;
    }

    if(name.size() == 0 ){
        ui->label_ButtonAdd->setText("Špatné jméno!");
        return;
    }
    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        if((*m_Students)[i]->getName() == name){
            ui->label_ButtonAdd->setText("Student s takovým jménem již přidán!");
            return;
        }
        if((*m_Students)[i]->getPath() == path){
            ui->label_ButtonAdd->setText("Student s takovou cestou již přidán!");
            return;
        }
    }
    m_Students->push_back(new Student(name, path));
    ui->label_ButtonAdd->setText("Student přidán!");
}

void AddStudents::soubor(){
    QString fpath = ui->plainTextEdit_SouborPath->toPlainText();

    if(fpath.size() == 0){
        ui->label_ButtonAdd->setText("Špatná cesta!");
        return;
    }

    string path, name;
    ifstream file(fpath.toLocal8Bit().constData());
    if(!file.good()){
        ui->label_ButtonAdd->setText("Soubor se nepodařilo otevřít!");
        return;
    }
    while(file.good()){
        name = "";
        path = "";

        file >> name;
        file >> path;

        for(unsigned int i = 0 ; i < m_Students->size(); i++){
            if((*m_Students)[i]->getName().toLocal8Bit().constData() == name
                    || (*m_Students)[i]->getPath().toLocal8Bit().constData() == path){
                ui->label_ButtonAdd->setText("Studenti se opakují!");
                return;
            }
        }

        m_Students->push_back(new Student(QString::fromStdString(name), QString::fromStdString(path)));
    }

    ui->label_ButtonAdd->setText("Studenti přidáni!");
    file.close();

}

void AddStudents::on_pushButton_Add_clicked()
{
    if(ui->radioButton_Dohromady->isChecked()){
        dohromady();
    } else if(ui->radioButton_Zvlast->isChecked()) {
        zvlast();
    } else {
        soubor();
    }
}

void AddStudents::on_checkBox_First_toggled(bool checked)
{
    if(checked){
        ui->plainTextEdit_DohromadyPath1->setDisabled(false);
    } else {
        ui->plainTextEdit_DohromadyPath1->setDisabled(true);
    }
}

void AddStudents::on_checkBox_Second_toggled(bool checked)
{
    if(checked){
        ui->plainTextEdit_DohromadyPath2->setDisabled(false);
    } else {
        ui->plainTextEdit_DohromadyPath2->setDisabled(true);
    }
}

void AddStudents::on_radioButton_Dohromady_toggled(bool checked)
{
    if(checked){
        ui->label_DohromadyPath->setDisabled(false);
        ui->label_DohromadyName->setDisabled(false);
        ui->checkBox_First->setDisabled(false);
        ui->checkBox_Second->setDisabled(false);
        ui->plainTextEdit_DohromadyName->setDisabled(false);

        if(ui->checkBox_First->isChecked())
            ui->plainTextEdit_DohromadyPath1->setDisabled(false);
        if(ui->checkBox_Second->isChecked())
            ui->plainTextEdit_DohromadyPath2->setDisabled(false);
    } else {
        ui->label_DohromadyPath->setDisabled(true);
        ui->label_DohromadyName->setDisabled(true);
        ui->checkBox_First->setDisabled(true);
        ui->checkBox_Second->setDisabled(true);
        ui->plainTextEdit_DohromadyName->setDisabled(true);
        ui->plainTextEdit_DohromadyPath1->setDisabled(true);
        ui->plainTextEdit_DohromadyPath2->setDisabled(true);
    }
}

void AddStudents::on_radioButton_Zvlast_toggled(bool checked)
{
    if(checked){
        ui->label_ZvlastName->setDisabled(false);
        ui->label_ZvlastPath->setDisabled(false);
        ui->plainTextEdit_ZvlastName->setDisabled(false);
        ui->plainTextEdit_ZvlastPath->setDisabled(false);
    } else {
        ui->label_ZvlastName->setDisabled(true);
        ui->label_ZvlastPath->setDisabled(true);
        ui->plainTextEdit_ZvlastName->setDisabled(true);
        ui->plainTextEdit_ZvlastPath->setDisabled(true);
    }
}

void AddStudents::on_radioButton_Soubor_toggled(bool checked)
{
    if(checked){
        ui->label_SouborPath->setDisabled(false);
        ui->plainTextEdit_SouborPath->setDisabled(false);
        ui->toolButton->setDisabled(false);
    } else {
        ui->label_SouborPath->setDisabled(true);
        ui->plainTextEdit_SouborPath->setDisabled(true);
        ui->toolButton->setDisabled(true);
    }
}



void AddStudents::on_pushButton_Write_clicked()
{
    string fpath = "default.txt";
    string name = "";
    string path = "";
    bool result = true;
    ofstream ofile;
    ifstream ifile;
    if (ifstream(fpath.c_str()))
    {
        OverwriteDialog od(&result, &fpath);
        od.setModal(true);
        od.run();
        od.exec();
    }


    if(result){ //overwrite

    } else { //append
        ifile.open(fpath.c_str(), fstream::in);
        while(ifile.good()){
            name = "";
            path = "";

            ifile >> name;
            ifile >> path;

            for(unsigned int i = 0 ; i < m_Students->size(); i++){

                if((*m_Students)[i]->getName().toLocal8Bit().constData() == name
                        || (*m_Students)[i]->getPath().toLocal8Bit().constData() == path){
                    ui->label_ButtonFile->setText("Studenti se opakují!");
                    return;
                }
            }

            m_Students->push_back(new Student(QString::fromStdString(name), QString::fromStdString(path)));

        }
        ifile.close();
    }

    ofile.open(fpath.c_str(), fstream::out | fstream::trunc);
    if(!ofile.good()){
        ui->label_ButtonAdd->setText("Soubor se nepodařilo otevřít!");
        return;
    }

    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        ofile << (*m_Students)[i]->getName().toLocal8Bit().constData() << " "
             << (*m_Students)[i]->getPath().toLocal8Bit().constData() << endl;
    }
    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        delete (*m_Students)[i];
    }
    m_Students->clear();
    ofile.close();
    ui->label_ButtonFile->setText("Úspěšně zapsáno do souboru.");
}

void AddStudents::on_toolButton_clicked()
{
    QString path;
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("Text files (*.txt)"));
    if(!fileNames.isEmpty()){
        path = fileNames.front();
        ui->plainTextEdit_SouborPath->clear();
        ui->plainTextEdit_SouborPath->appendPlainText(path);
    }

}
