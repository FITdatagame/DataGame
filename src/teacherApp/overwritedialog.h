/**
 * \file overwritedialog.h
 */

#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QDialog>
#include <fstream>
#include <string>

using namespace std;

namespace Ui {
class OverwriteDialog;
}

/** \brief Is created and shown when a file with deafult filename exists.
*
*
*	Lets the user choose a new filename \
*	and asks wether to overwrite or append to existing files.
*/
class OverwriteDialog : public QDialog
{
    Q_OBJECT

public:

    /** \brief Constructor

     \param result A pointer to a bool. Stores the decision to truncate or append file contents.
     \param fpath A pointer to a string, where entered filename is expected.
    */
    explicit OverwriteDialog(bool * result,string * fpath, QWidget *parent = 0);
    ~OverwriteDialog();

    /**\brief Sets the user interface to default state.
    */
    void run();
private slots:
	/**\brief User chose to overwrite existing file.
    */
    void on_pushButton_clicked();
    
    /**\brief User chose to append to an existing file.
    */
    void on_pushButton_2_clicked();

    /**\brief Checks the filename user entered.
    *	If the file exists, prompts the user to decide to truncate or append file contents.
    */
    void on_pushButton_3_clicked();

private:
	/**\brief Stores the decision to truncate or append file contents.
    */
    bool * m_Result;
    /**\brief Stores the filename.
    */
    string * m_Fpath;
    Ui::OverwriteDialog *ui;
};

#endif // OVERWRITEDIALOG_H
