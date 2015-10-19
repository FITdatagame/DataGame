/**
 * \file choosefiledialog.h
 */


#ifndef CHOOSEFILEDIALOG_H
#define CHOOSEFILEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <string>
#include <fstream>

using namespace std;

namespace Ui {
class chooseFileDialog;
}

/** \brief Lets the user choose filename for a file.
*/
class chooseFileDialog : public QDialog
{
    Q_OBJECT

public:
	/** \brief Constructor

         \param result A pointer to a string, where entered filename is expected
     */
    explicit chooseFileDialog(string * result, QWidget *parent = 0);
    ~chooseFileDialog();

private slots:
    /** \brief Checks whether the file exists and sets the m_Result accordingly.
     */
    void on_pushButton_clicked();

    /** \brief Triggers file-browser.
     */
    void on_toolButton_clicked();

private:
    Ui::chooseFileDialog *ui;

	/** \brief A pointer to a string, where entered filename is expected
     */
    string * m_Result;
};

#endif // CHOOSEFILEDIALOG_H
