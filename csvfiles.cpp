#include "csvfiles.h"
#include "MainWindow.h"

CSVfiles::CSVfiles(QStringList columnNames){
    std::vector<QString> vec;
    for(auto &x : columnNames){
        this->data[x] = {};
        vec.push_back(x);
    }
    this->columnNames = vec;
}

CSVfiles::~CSVfiles() {}

void CSVfiles::addToData(QString DataPart, QString columnName)
{
    this->data[columnName].push_back(DataPart);
    if(columnName == "Time"){
        this->timList.push_back({DataPart, this->data[columnName].size()});
    }
}

bool timeComparator(const std::pair<QString, int> &t1, const std::pair<QString, int> &t2) {
    QTime time1 = QTime::fromString(t1.first, "h:mm:ss AP");
    QTime time2 = QTime::fromString(t2.first, "h:mm:ss AP");
    return time1 < time2;
}

void CSVfiles::sort(){
    std::vector<QString> timeList;
    std::sort(this->timList.begin(), this->timList.end(), timeComparator);
}

bool CSVfiles::read(QString filePath){
    std::vector<QStringList> data;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream stream(&file);
    QString line = stream.readLine();
    if (line.isEmpty()) {
        return false;
    }

    QStringList columnNames = line.split(',');

    while (!stream.atEnd()) {
        line = stream.readLine();
        data.push_back(line.split(','));
    }

    file.close();

    int len = columnNames.length();
    for(int j=0; j<data.size(); j++){
        for(int i=0; i<len; i++){
            addToData(data[j][i], columnNames[i]);
        }
    }

    sort();

    return true;
}

void CSVfiles::convertToJson(){
    QJsonObject jsonObject;
    for (const auto& pair : data) {
        QJsonArray jsonArray;
        for (auto &value : timList) {
            auto element = data[pair.first][value.second];
            if(pair.first == "Time"){
                QDateTime dateTime = QDateTime::fromString(element, "yyyy-MM-dd hh:mm:ss");
                element = dateTime.toString(Qt::ISODate);
                data[pair.first][value.second] = element;
            }

            jsonArray.append(element);
        }
        jsonObject.insert(pair.first, jsonArray);
    }

    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);


    QFile file("output.json");
    if (!file.open(QIODevice::WriteOnly)) {
        std::cout << "Failed to open file for writing";
        return;
    }
    file.write(jsonString.toUtf8());
    file.close();
}

bool CSVfiles::connectToDb(){


    // return createTable(this->db);
    return false;
}

bool CSVfiles::createTable(QSqlDatabase &db){
    QSqlQuery query(db);
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS data_table (
            key_column TEXT,
            value_column TEXT
        );
    )";

    if (!query.exec(createTableSQL)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Table created successfully!";

    query.prepare("INSERT INTO data_table (key_column, value_column) VALUES (:key_column, :value_column)");

    for (const auto& pair : timList) {
        int index = pair.second;
        for (auto& key : columnNames) {
            auto element = data[key][index];
            query.bindValue(":key_column", key);
            query.bindValue(":value_column", element);

            if (!query.exec()) {
                qDebug() << "Failed to insert data:" << query.lastError().text();
                return false;
            }
        }
    }

    qDebug() << "Query executed created successfully!";

    return true;
}

void CSVfiles::showInTableView(QStandardItemModel* model){
    model->clear();

    QStringList headers;
    for (const auto& pair : data) {
        headers << pair.first;
    }
    model->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < timList.size(); ++row) {
        int col = 0;
        int index = timList[row].second;
        for (auto& pair : data) {
            QString value = pair.second[index];
            QStandardItem* item = new QStandardItem(value);
            model->setItem(row, col, item);  // Set value in the appropriate column
            ++col;
        }
    }

}



