#include "showresults.h"
#include "ui_showresults.h"

ShowResults::ShowResults(vector <Student*> * students, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowResults)
{
    ui->setupUi(this);

    m_Students = students;

}

void ShowResults::run(){
    setStudents();

}

ShowResults::~ShowResults()
{
    delete ui;
}

void ShowResults::setStudents(){
    if(m_Students->empty()){ //no students added this run
        string fpath;
        chooseFileDialog cfd(&fpath);
        cfd.setModal(true);
        cfd.exec();

        if(!loadFile(fpath)){
            ui->textBrowser->append("Studenti nenačteni!");
            return;
        }

        QString text = "Studenti načteni z " + QString::fromStdString(fpath);
         ui->textBrowser->append(text);
    } else {
         ui->textBrowser->append("Studenti načteni z paměti programu.");
    }

    for(unsigned int i = 0; i < m_Students->size() ; i++){
        ui->comboBox->addItem((*m_Students)[i]->getName());
    }
}

bool ShowResults::loadFile(string fpath){
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
                ui->textBrowser->append("Studenti se opakují!");
                flag = true;
                break;
            }
        }
        if(flag) {
            flag = false;
        } else {
            m_Students->push_back(new Student(QString::fromStdString(name), QString::fromStdString(path)));
        }


    }

    return true;
}


void ShowResults::showRes(int index){
    if(index == -1)     // no students loaded
        return;

    ui->textBrowser->clear();
    Student * s = (*m_Students)[index];
    vector<GameResult> res = s->getResults();

    if(ui->checkBox->isChecked()){
        unsigned int j = res.size() -1;
        QString tmp = "Hra číslo " + QString::number(j+1) + (QString)":";
        ui->textBrowser->append(tmp);
        tmp = "\tskóre: \t\t\t\t";
        tmp += QString::number(res[j].calcScore());
        ui->textBrowser->append(tmp);
        tmp = "\tčas: \t\t\t\t";
        tmp += QString::number(res[j].time);
        ui->textBrowser->append(tmp);
        tmp = "\tvirů odstraněno: \t\t\t";
        tmp += QString::number(res[j].virusesDefeated);
        ui->textBrowser->append(tmp);
        tmp = "\tsprávně zařazených souborů: \t\t";
        tmp += QString::number(res[j].filesManagedCorrectly);
        ui->textBrowser->append(tmp);
        tmp = "\tnesprávně zařazených souborů: \t\t";
        tmp += QString::number(res[j].filesManagedIncorrectly);
        ui->textBrowser->append(tmp);
        tmp = "\tpožadavků na firewall vyřízeno: \t\t";
        tmp += QString::number(res[j].firewallTasksDone);
        ui->textBrowser->append(tmp);
        tmp = "\tpožadavků na firewall nevyřízeno: \t\t";
        tmp += QString::number(res[j].firewallTasksIgnored);
        ui->textBrowser->append(tmp);
        tmp = "\tpočet špatných nast. práv: \t\t";
        tmp += QString::number(res[j].numWrongPermissions);
        ui->textBrowser->append(tmp);
        tmp = "\tpočet průlomů firewallu: \t\t";
        tmp += QString::number(res[j].numFirewallBreaches);
        ui->textBrowser->append(tmp);
        tmp = "\tsprávných obnovení: \t\t\t";
        tmp += QString::number(res[j].numUndeleteCorrect);
        ui->textBrowser->append(tmp);
        tmp = "\tnesprávných obnovení: \t\t\t";
        tmp += QString::number(res[j].numUndeleteIncorrect);
        ui->textBrowser->append(tmp);

        tmp = "";
        ui->textBrowser->append(tmp);
    } else {
        for(unsigned int j = 0 ;  j < res.size(); j++){
            QString tmp = "Hra číslo " + QString::number(j+1) + (QString)":";
            ui->textBrowser->append(tmp);
            tmp = "\tskóre: \t\t\t\t";
            tmp += QString::number(res[j].calcScore());
            ui->textBrowser->append(tmp);
            tmp = "\tčas: \t\t\t\t";
            tmp += QString::number(res[j].time);
            ui->textBrowser->append(tmp);
            tmp = "\tvirů odstraněno: \t\t\t";
            tmp += QString::number(res[j].virusesDefeated);
            ui->textBrowser->append(tmp);
            tmp = "\tsprávně zařazených souborů: \t\t";
            tmp += QString::number(res[j].filesManagedCorrectly);
            ui->textBrowser->append(tmp);
            tmp = "\tnesprávně zařazených souborů: \t\t";
            tmp += QString::number(res[j].filesManagedIncorrectly);
            ui->textBrowser->append(tmp);
            tmp = "\tpožadavků na firewall vyřízeno: \t\t";
            tmp += QString::number(res[j].firewallTasksDone);
            ui->textBrowser->append(tmp);
            tmp = "\tpožadavků na firewall nevyřízeno: \t\t";
            tmp += QString::number(res[j].firewallTasksIgnored);
            ui->textBrowser->append(tmp);
            tmp = "\tpočet špatných nast. práv: \t\t";
            tmp += QString::number(res[j].numWrongPermissions);
            ui->textBrowser->append(tmp);
            tmp = "\tpočet průlomů firewallu: \t\t";
            tmp += QString::number(res[j].numFirewallBreaches);
            ui->textBrowser->append(tmp);
            tmp = "\tsprávných obnovení: \t\t\t";
            tmp += QString::number(res[j].numUndeleteCorrect);
            ui->textBrowser->append(tmp);
            tmp = "\tnesprávných obnovení: \t\t\t";
            tmp += QString::number(res[j].numUndeleteIncorrect);
            ui->textBrowser->append(tmp);

            tmp = "";
            ui->textBrowser->append(tmp);
        }
    }
}

void ShowResults::on_comboBox_currentIndexChanged(int index)
{
    showRes(index);
}

void ShowResults::on_checkBox_clicked()
{
    showRes(ui->comboBox->currentIndex());
}
