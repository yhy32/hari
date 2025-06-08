#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "fstream"
#include "cstdio"
#include "QString"
#include "string"
#include "vector"
#include "QMessageBox"

using namespace std;

/**
 * @brief MainWindow 构造函数
 * @param parent 父窗口指针，默认 nullptr
 * 负责初始化 UI 界面
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * @brief MainWindow 析构函数
 * 负责释放 UI 资源
 */
MainWindow::~MainWindow()
{
    delete ui;
}

// 存储查找结果的全局变量
vector<SearchInfo> searchInfo;

// 需要搜索的文件名数组
const char *books[8] = {
    "J.K. Rowling - Quidditch Through the Ages.txt",
    "J.K. Rowling - HP 6 - Harry Potter and the Half-Blood Prince.txt",
    "J.K. Rowling - HP 4 - Harry Potter and the Goblet of Fire.txt",
    "J.K. Rowling - HP 3 - Harry Potter and the Prisoner of Azkaban.txt",
    "J.K. Rowling - HP 0 - Harry Potter Prequel.txt",
    "HP7--Harry_Potter_and_the_Deathly_Hallows_Book_7_.txt",
    "HP2--Harry_Potter_and_the_Chamber_of_Secrets_Book_2_.txt",
    "J.K. Rowling - The Tales of Beedle the Bard.txt"
};