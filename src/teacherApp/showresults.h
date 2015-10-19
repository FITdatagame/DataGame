/**
 * \file showresults.h
 */

#ifndef SHOWRESULTS_H
#define SHOWRESULTS_H

#include <QDialog>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "student.h"
#include "choosefiledialog.h"

using namespace std;

namespace Ui {
class ShowResults;
}

/** \brief Allows the user to show scores of students.
 * 
 *  Tries to read students from memory. 
 *  If none are found, prompts the user to choose a file to load them from.
 */
class ShowResults : public QDialog
{
    Q_OBJECT

public:
    /** 
     \param students A pointer to the shared vector containing students. Stored in m_Students.
    */
    explicit ShowResults(vector <Student*> * students, QWidget *parent = 0);
    ~ShowResults();

    /**\brief Sets the user interface to default state.
    */
    void run();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_clicked();

private:

    /**\brief Shows results for students in program's memory.
    */
    void showRes(int index);

    /**\brief Tries to load students from given file.
    */
    bool loadFile(string fpath);

    /**\brief Handles loading students from memory or files.
    *   Checks if m_Students contains any students.
    *   If not, prompts user for a file with previously saved students.
    */
    void setStudents();

    Ui::ShowResults *ui;

    /**\brief A pointer to the shared vector containing students.
    */
    vector<Student *> * m_Students;

};

#endif // SHOWRESULTS_H
