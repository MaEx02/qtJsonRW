#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QFile"
#include "QJsonArray"
#include "QStandardItemModel"
#include "QSortFilterProxyModel"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readJsonFile();
}
QString name;
QString age;
QString jobDescription;
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readJsonFile(){

    QFile file("C:/Users/shaco/Desktop/data.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    QVariantMap jsonmap = jsonDoc.toVariant().toMap();
    file.close();

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << "Görev Açıklaması" << "İsim" << "Yaş");

    foreach (auto item, jsonmap["calisanlar"].toList()) {
        QVariantMap map = item.toMap();
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(map["GorevAciklaması"].toString()));
        rowItems.append(new QStandardItem(map["İsim"].toString()));
        rowItems.append(new QStandardItem(map["yas"].toString()));
        model->appendRow(rowItems);
    }
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    connect(ui->lineEdit, &QLineEdit::textChanged, proxyModel, &QSortFilterProxyModel::setFilterFixedString);
}
void MainWindow::writeJsonFile(){
    QFile file("C:/Users/shaco/Desktop/data.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray calisanlarArray = rootObj.value("calisanlar").toArray();
    QJsonObject yeniCalisanObj;
    yeniCalisanObj.insert("GorevAciklaması", jobDescription);
    yeniCalisanObj.insert("yas",age);
    yeniCalisanObj.insert("İsim", name);
    calisanlarArray.append(yeniCalisanObj);
    rootObj.insert("calisanlar", calisanlarArray);
    file.resize(0);
    file.write(QJsonDocument(rootObj).toJson());
    file.close();
}

void MainWindow::deleteSelectedRow(){
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.empty()) {
            int row = selectedRows[0].row();
            QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
            QFile file("C:/Users/shaco/Desktop/data.json");
            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                return;
            }
            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            QJsonObject rootObj = jsonDoc.object();
            QJsonArray calisanlarArray = rootObj.value("calisanlar").toArray();
            calisanlarArray.removeAt(row);
            rootObj.insert("calisanlar", calisanlarArray);
            file.resize(0);
            file.write(QJsonDocument(rootObj).toJson());
            file.close();
        }
        readJsonFile();
}

void MainWindow::on_pushButton_clicked()
{
    jobDescription = ui->textEdit_3->toPlainText();
    age = ui->textEdit->toPlainText();
    name = ui->textEdit_2->toPlainText();
    writeJsonFile();
    readJsonFile();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();

}

void MainWindow::on_pushButton_clicked(bool checked)
{

}


void MainWindow::on_pushButton_2_clicked()
{
    deleteSelectedRow();
}
