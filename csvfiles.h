    #ifndef CSVFILES_H
    #define CSVFILES_H

    #include <qdir>
    #include <QJsonDocument>
    #include <QJsonObject>
    #include <QJsonArray>
    #include <iostream>
    #include <QSharedDataPointer>
    #include <QDebug>
    #include <QtSql/QSqlError>
    #include <QtSql/QSqlQuery>
    #include <QFileInfo>
    #include <QTableView>
    #include <QStandardItemModel>
    #include <QHeaderView>
    #include <QApplication>
    #include <QtSql/QSql>
    #include <QtSql/qtsqlversion.h>
    #include<QtSql/QSqlDatabase>
    #include<QtSql/QSqlDriver>


    class CSVfiles
    {
    public:
        CSVfiles(){}
        CSVfiles(QStringList columnNames);
        ~CSVfiles();
        bool read(QString filepath);
        void addToData(QString DataPart, QString columnName);
        void sort();
        void convertToJson();
        bool connectToDb();
        bool createTable(QSqlDatabase &db);
        void showInTableView(QStandardItemModel* model);


    private:
        std::unordered_map<QString, std::vector<QString>> data;
        std::vector<std::pair<QString, int>> timList;
        std::vector<QString> columnNames;

    };

    #endif // CSVFILES_H
