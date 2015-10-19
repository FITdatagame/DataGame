/**
 * \file sendsettings.h
 */

#ifndef SENDSETTINGS_H
#define SENDSETTINGS_H

#include <QDialog>
#include <vector>
#include "student.h"
#include "choosefiledialog.h"



namespace Ui {
class SendSettings;
}

/** \brief Allows the user to send setting to students.
 * 
 *  Tries to read students from memory. 
 *  If none are found, prompts the user to choose a file to load them from.
 */
class SendSettings : public QDialog
{
    Q_OBJECT

public:
    /** 
     \param students A pointer to the shared vector containing students. Stored in m_Students.
    */
    explicit SendSettings( vector<Student *> * students, QWidget *parent = 0);
    ~SendSettings();

    /**\brief Sets the user interface to default state.
    */
    void run();
private slots:

    /**\brief Tries to create new files containing chosen settings at loaded student's game folders.
    */
    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    /**\brief Handles loading students from memory or files.
    *   Checks if m_Students contains any students.
    *   If not, prompts user for a file with previously saved students.
    */
    void setStudents();
    /**\brief Tries to load students from given file.
    */
    bool loadFile(string fpath);

    Ui::SendSettings *ui;

    /**\brief A pointer to the shared vector containing students.
    */
    vector<Student *> * m_Students;
};

#endif // SENDSETTINGS_H
