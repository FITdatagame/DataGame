/** 
 * \file addstudents.h
 */

#ifndef ADDSTUDENTS_H
#define ADDSTUDENTS_H

#include <QDialog>
#include <QFileDialog>
#include "student.h"
#include <vector>
#include <fstream>
#include "overwritedialog.h"

namespace Ui {
class AddStudents;
}

/** \brief Allows the user to add students.
 * 
 *  Students are added to program's memory. The user can also save them to a file.
 *  Allows various ways of adding the students.
 */
class AddStudents : public QDialog
{
    Q_OBJECT

public:
    /** 
     \param students A pointer to the shared vector containing students. Stored in m_Students.
    */
    explicit AddStudents(vector<Student *> * students, QWidget *parent = 0);
    ~AddStudents();

    /**\brief Sets the user interface to default state.
    */
    void run();
private slots:

    void on_radioButton_Zvlast_toggled(bool checked);

    void on_checkBox_First_toggled(bool checked);

    void on_checkBox_Second_toggled(bool checked);

    void on_radioButton_Dohromady_toggled(bool checked);

    void on_radioButton_Soubor_toggled(bool checked);

    /**\brief Decides which private method to call.
    */
    void on_pushButton_Add_clicked();

    /**\brief Handles saving students to a file.
    *   Creates and shows "OverwriteDialog" if needed.   
    */
    void on_pushButton_Write_clicked();

    void on_toolButton_clicked();



private:

    /**\brief Handles adding students with the option "dohromady".
     */
    void dohromady();
    /**\brief Handles adding students with the option "zvlast".
     */
    void zvlast();
    /**\brief Handles adding students with the option "soubor".
     */
    void soubor();

    Ui::AddStudents *ui;

    /**\brief A pointer to a shared vector with students.
    */
    vector<Student*> * m_Students;
};

#endif // ADDSTUDENTS_H
