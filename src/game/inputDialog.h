/**
* @file inputDialog.h
*/

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <functional>
using namespace std;

#include "group.h"
#include "text.h"

/**
 * @class InputDialog
 * @brief Slouží k zobrazení dialogu pro zadání textového vstupu (např. názvu nové složky).
 */
class InputDialog : public Group
{
public:
	InputDialog();
	~InputDialog();


	/** @brief Nastaví funkci, která se zavolá po potvrzení zadaného textu.
	*/
	void setDoneFunc(function<void (const string & txt)> fn) { onDone = fn; }

private:

	/** @brief Zobrazuje zadaný text.
	*/
	Text * text;

	/** @brief Funkce, která se zavolá po potvrzení dialogu. V parametru dostane zadaný text.
	*/
	function<void (const string & txt)> onDone;
};

#endif // INPUTDIALOG_H
