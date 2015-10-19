/**
 * \file student.h
 */

#ifndef ADDSTUDENTS
#define ADDSTUDENTS
#include <QString>
#include <vector>
#include <../gameResult.h>
using namespace std;

/**\brief Basically a container for student's info.
*/
class Student
{
    public:
        Student(QString name, QString path){
            m_Name = name;
            m_Path = path;
        }

        ~Student(){}
        QString getName(){ return m_Name; }
        QString getPath(){ return m_Path; }


        /**\brief Loads student's results, using GameResult class.
        */
        vector<GameResult> getResults(){
            string file = m_Path.toLocal8Bit().constData();
            file += "results.bin";
            return GameResult::loadAllResults(file);
        }

    private:


        /**\brief Stores student's name.
        */
        QString m_Name;

        /**\brief Stores student's path to his game directory.
        */
        QString m_Path;
};

#endif //ADDSTUDENTS

