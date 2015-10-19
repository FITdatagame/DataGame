/**
* @file utils.h
*/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;

class File;
class Disk;
class Folder;

/**
* @struct FilePath
* @brief Představuje cestu k souboru.
*/
struct FilePath {
	Disk * disk;
	vector<string> path;
	bool valid = false;
};

/* Zkopírováno z http://www.linuxquestions.org/questions/programming-9/wstring-utf8-conversion-in-pure-c-701084/ */
/** @brief Převede multibyte UTF8 string na wstring.
 */
void utf8toWStr(wstring & dest, const string & src);

/** @brief Převede wstring na multibyte UTF8 string.
 */
void wstrToUtf8(string & dest, const wstring & src);

/** @brief Převede velikost v bytech do textové podoby.
 */
string fileSizeToStr(long long int sz);

/** @brief Vytvoří cestu k danému souboru.
 */
FilePath getFilePath(File * f);

/** @brief Z cesty se pokusí získat cílový soubor.
 */
File * getFileFromPath(FilePath path);

/** @brief Vybere náhodný soubor se složky.
 */
File * findRandomFile(Folder * folder);

/** @brief Vybere náhodnou složku ze složky.
 */
Folder * findRandomFolder(Folder * folder);

#endif // UTILS_H
