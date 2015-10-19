#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Students = new vector<Student *> ();
}

MainWindow::~MainWindow()
{
    for(unsigned int i = 0 ; i < m_Students->size(); i++){
        delete ((*m_Students)[i]);
    }
    delete m_Students;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ShowResults sr(m_Students);
    sr.setModal(true);
    sr.run();
    sr.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    AddStudents as(m_Students);
    as.setModal(true);
    as.run();
    as.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
//    this->~MainWindow();
    this->close();
//    exit(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    SendSettings ss(m_Students);
    ss.setModal(true);
    ss.run();
    ss.exec();

}
