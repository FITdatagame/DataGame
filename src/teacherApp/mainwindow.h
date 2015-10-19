/** 
 * \file mainwindow.h
 */

 

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addstudents.h"
#include "showresults.h"
#include "sendsettings.h"

namespace Ui {
class MainWindow;
}

/** \mainpage
 *  An application designed for teachers. Can be used for sending and defining game settings for students and showing their game results.
 */

/** \brief Simple menu with 4 options.
 * Handles the main vector with students and creates and shows respective windows as needed.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	/**\brief Handles deallocation of students.
     */
	~MainWindow();

private slots:
	/**\brief Creates sand shows ShowResults widget.
	 */
	void on_pushButton_clicked();

	/**\brief Creates and shows AddStudents widget.
	*/
	void on_pushButton_2_clicked();

	/**\brief Closes the application.
	*/
	void on_pushButton_3_clicked();

	/**\brief Creates and shows SendSettings widget.
	*/
	void on_pushButton_4_clicked();

private:
	Ui::MainWindow *ui;
	/**\brief A pointer to a shared vector with students.
	*
	*	It is shared with other widgets, but created and destroyed by this class.
    */
	vector<Student *> * m_Students;
};

#endif // MAINWINDOW_H
