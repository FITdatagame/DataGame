/**
* @file gameResult.h
*/

#ifndef GAMERESULT_H
#define GAMERESULT_H

#define ANTIVIRUS_TASK_BONUS		500
#define FILE_TASK_BONUS				10
#define FIREWALL_TASK_BONUS			100
#define UNDELETE_TASK_BONUS			100

#define FILE_TASK_PENALTY			(-100)
#define PERMISSIONS_PENALTY			(-50)
#define FIREWALL_TASK_PENALTY		(-100)
#define FIREWALL_BREACH_PENALTY		(-500)
#define UNDELETE_TASK_PENALTY		(-100)

#define RESULT_FORMAT "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

#include "binfiles.h"

/**
 * @class GameResult
 * @brief Představuje podrobný výsledek hry. Obsahuje funkce na uložení a načtení.
 */
class GameResult {
public:
	int time = 0;
	int virusesDefeated = 0;
	int filesManagedCorrectly = 0;
	int filesManagedIncorrectly = 0;
	int firewallTasksDone = 0;
	int firewallTasksIgnored = 0;
	int numWrongPermissions = 0;
	int numFirewallBreaches = 0;
	int numUndeleteCorrect = 0;
	int numUndeleteIncorrect = 0;

	/** @brief Vypočítá celkové skóre.
	*/
	int calcScore()
	{
		return virusesDefeated * ANTIVIRUS_TASK_BONUS
			 + filesManagedCorrectly * FILE_TASK_BONUS
			 + filesManagedIncorrectly * FILE_TASK_PENALTY
			 + firewallTasksDone * FIREWALL_TASK_BONUS
			 + firewallTasksIgnored * FIREWALL_TASK_PENALTY
			 + numWrongPermissions * PERMISSIONS_PENALTY
			 + numFirewallBreaches * FIREWALL_BREACH_PENALTY
			 + numUndeleteCorrect * UNDELETE_TASK_BONUS
			 + numUndeleteIncorrect * UNDELETE_TASK_PENALTY;
	}

	/** @brief Převede výsledek na string.
	*/
	string serialize() const
	{
		char buf[1024];

		sprintf(buf,
				RESULT_FORMAT,
				time,
				virusesDefeated,
				filesManagedCorrectly,
				filesManagedIncorrectly,
				firewallTasksDone,
				firewallTasksIgnored,
				numWrongPermissions,
				numFirewallBreaches,
				numUndeleteCorrect,
				numUndeleteIncorrect
				);

		return buf;
	}

	/** @brief Načte výsledek ze stringu.
	*/
	void deserialize(const string & str)
	{
		sscanf(str.c_str(),
			   RESULT_FORMAT,
			   &time,
			   &virusesDefeated,
			   &filesManagedCorrectly,
			   &filesManagedIncorrectly,
			   &firewallTasksDone,
			   &firewallTasksIgnored,
			   &numWrongPermissions,
			   &numFirewallBreaches,
			   &numUndeleteCorrect,
			   &numUndeleteIncorrect
			   );
	}

	/** @brief Načte všechny výsledky ze souboru do pole.
	*/
	static vector<GameResult> loadAllResults(const string & filename)
	{
		vector<GameResult> res;

		istringstream str(readScore(filename));
		string line;

		while(getline(str, line))
		{
			if(line.length() > 10) // pro jistotu
			{
				GameResult g;
				g.deserialize(line);
				res.push_back(g);
			}
		}

		return res;
	}

	/** @brief Načte poslední uložený výsledek ze souboru.
	*/
	static void loadLatestResult(GameResult & res, const string & filename)
	{
		const vector<GameResult> & results = loadAllResults(filename);

		if(results.size() > 0)
			res = results.back();
	}

	/** @brief Vloží výsledek do souboru.
	*/
	static void appendResult(const GameResult & res, const string & filename)
	{
		string str = "";
		str += res.serialize() + "\n";
		writeScore(str, filename);
	}
};

#endif // GAMERESULT_H

