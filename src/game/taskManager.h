/**
* @file taskManager.h
*/

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
using namespace std;

#include "group.h"
#include "folder.h"
#include "text.h"
#include "disk.h"
#include "../gameResult.h"
#include "firewallManager.h"

#define NO_TIME_LIMIT -99.0

/**
 * @class Task
 * @brief Úkol, který hráč dostane od hry.
 */
class Task {
public:
	Task() { id = taskIdGenerator++; }

	bool isDone() const { return done; }
	string getName() const { return name; }

protected:
	static int taskIdGenerator;

	friend class TaskManager;

	/** @brief Jedinečný identifikátor úkolu.
	 */
	int id;

	/** @brief Popis úkolu.
	 */
	string description;

	/** @brief Jméno úkolu.
	*/
	string name;

	/** @brief Počet bodů, které může hráč získat.
	*/
	int pointsGain;

	/** @brief Počet bodů, které může hráč ztratit.
	*/
	int pointsLose;

	/** @brief Zbývající počet sekund na vyřešení úkolu.
	*/
	double timeLeft;

	bool done = false;
};

/**
 * @class UndeleteTask
 * @brief Úkol na obnovení dříve smazaného souboru.
 */
class UndeleteTask : public Task {
public:
	UndeleteTask(Folder * rootFolder, int missingData) : rootFolder(rootFolder), missingData(missingData){}

	/** @brief Zkontroluje, zda byl úkol splněn (zjistí s DiskChange).
	*/
	void check(const DiskChange & c);

private:
	Folder * rootFolder;
	int missingData;
};

/**
 * @class FirewallTask
 * @brief Úkol na otevření portu ve firewallu.
 */
class FirewallTask : public Task {
public:
	FirewallTask(Folder * rootFolder, int port) : rootFolder(rootFolder), port(port) {}

	/** @brief Zkotroluje, zda byl úkol splněn (zjistí z FirewallManager).
	*/
	void check(FirewallManager * f);

private:
	Folder * rootFolder;
	int port;
};

/**
 * @class AntivirusTask
 * @brief Úkol na smazání všech virů.
 */
class AntivirusTask : public Task {
public:
	AntivirusTask() : count(1) {}

	/** @brief Zkotroluje, zda byl úkol splněn (zjistí ze seznamu virů).
	*/
	void check(vector<Virus *> & viruses);

	/** @brief Zaregistruje nový virus, který je potřeba zlikvidovat.
	*/
	void inc() { count++; }

	/** @brief Vrátí počet zaregistrovaných virů.
	*/
	int cnt() const { return count; }

private:
	int count;

};

/**
 * @class TaskManager
 * @brief Třída zobrazuje přehled aktuálních úkolů.
 */
class TaskManager : public Group {
public:
	TaskManager(Folder * dwnFolder, Folder * imgFolder, Folder * docFolder, Folder * exeFolder, Folder * vidFolder, FirewallManager * firewallManager);
	~TaskManager();

	/** @brief Zobrazí okno.
	*/
	void show();

	/** @brief Skryje okno.
	*/
	void hide();


	/** @brief Pravidelně volaná funkce, která kontroluje splňování jednotlivých úloh a případně generuje nové úlohy.
	*/
	void taskUpdate();

	/** @brief Volá se po každé změně na disku.
	*/
	void diskChange(const DiskChange & c);

	vector<Virus*> & getViruses() { return viruses; }

	void setOnNewTask(function<void (Task * t)> fn) { onNewTask = fn; }
	void setOnNewFile(function<void (File * t)> fn) { onNewFile = fn; }
	void setOnTaskFinished(function<void (Task * t)> fn) { onTaskFinished = fn; }

private:

	/** @brief Obnoví okno.
	*/
	void refresh();

	/** @brief Zkontroluje, zda task skončil nebo ne.
	*/
	bool checkTask(Task * t);

	/** @brief Zkontroluje, zda má soubor správně nastavená práva.
	*/
	bool checkFilePermissions(File * f);

	/** @brief Zkontroluje, zda jsou ve firewallu zavřené porty. Jinak škodí.
	*/
	void firewallCheck();


	/** @brief Pole úkolů, které byly zadané. Splněné úkoly již neevidujeme.
	*/
	map<int, Task*> tasks;

	/** @brief Měří čas do příštího stáhnutí souboru.
	*/
	double nextFileDownload;

	/** @brief Měří čas do příštího vytvoření úkolu.
	*/
	double nextTaskTimeout;

	/** @brief Měří čas do příštího obnovení seznamu úkolů.
	*/
	double nextListRefresh;

	/** @brief Měří čas do příští kontroly firewallu.
	*/
	double nextFirewallCheck;

	/** @brief Složka stažené.
	*/
	Folder * dwnFolder;

	/** @brief Složka obrázky.
	*/
	Folder * imgFolder;

	/** @brief Složka dokumenty.
	*/
	Folder * docFolder;

	/** @brief Složka programy.
	*/
	Folder * exeFolder;

	/** @brief Složka videa.
	*/
	Folder * vidFolder;

	/** @brief Ukazatel na správce firewallu.
	*/
	FirewallManager * firewallManager;


	/** @brief Grafické prvky ohledně seznamu úkolů
	*/
	Group * tasksGroup;

	/** @brief Ostatní grafické prvky.
	*/
	Group * elements;

	/** @brief Text popisující třídění souborů.
	*/
	Text * filesTask;

	/** @brief Text ukazující skóre.
	*/
	Text * scoreText;

	/** @brief Podrobný výsledek hry.
	*/
	GameResult gameResult;

	/** @brief Seznam smazaných souborů a jejich dat.
	*/
	vector<pair<string, int>> deletedData;

	/** @brief Seznam úkolů na obnovu souboru.
	*/
	vector<UndeleteTask*> undeleteTasks;

	/** @brief Úkol na otevření portu.
	*/
	FirewallTask * firewallTask = NULL;

	/** @brief Pole virů v systému.
	*/
	vector<Virus*> viruses;

	/** @brief Úkol na smazání virů.
	*/
	AntivirusTask * antivirusTask = NULL;


	/** @brief Callback při vytvoření úkolu.
	*/
	function<void (Task * t)> onNewTask;

	/** @brief Callback při vytvoření souboru.
	*/
	function<void (File * f)> onNewFile;

	/** @brief Callback při dokončení úkolu.
	*/
	function<void (Task * t)> onTaskFinished;
};

#endif
