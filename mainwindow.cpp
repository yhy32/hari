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

/**
 * @brief 处理“查找”按钮点击事件
 * 
 * 该函数遍历所有书籍文件，逐行读取，寻找包含用户输入关键字的文本行。
 * 匹配时将当前章节号、页码和文本行保存，并在界面显示编号及相关信息。
 */
void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();       // 清空之前显示的搜索结果
    searchInfo.clear();             // 清空上一次搜索缓存
    QString text = ui->lineEdit->text();   // 获取输入的关键字
    int num = 0;                   // 结果计数器

    for(int i = 0; i < 8; i++) {
        fstream file;
        file.open(books[i], ios::in);
        if (!file.is_open()) {
           cerr << "文件打开失败" << endl;
        }

        char name[100];
        char buf[1024] = {0};
        string chapter = "CHAPTER";
        string chapter1 = "Chapter";
        int cur_chapter = 0;  // 当前章节号
        int cur_page = 0;     // 当前页码
        strcpy(name, text.toStdString().c_str()); // 将关键字转为 char* 方便逐字比较

        while (file.getline(buf, sizeof(buf))) {
            // 跳过空行（防止乱码）
            if(buf[0] == 0 && (buf[1] == '\r' || buf[1] == '\n')) continue;

            try {
                // 判断当前行是否为章节标题（"CHAPTER"/"Chapter"）
                if (buf[0] == chapter[0] && buf[1] == chapter[1] && buf[2] == chapter[2] &&
                        buf[3] == chapter[3] && buf[4] == chapter[4] && buf[5] == chapter[5] && buf[6] == chapter[6]) {
                    cur_chapter++;
                    continue;
                } else if (buf[3] == chapter[0] && buf[4] == chapter[1] && buf[5] == chapter[2] &&
                           buf[6] == chapter[3] && buf[7] == chapter[4] && buf[8] == chapter[5] && buf[9] == chapter[6]){
                    // 跳过含 UTF-8 BOM 的章节行
                    cur_chapter++;
                    continue;
                } else if(buf[0] == chapter1[0] && buf[1] == chapter1[1] && buf[2] == chapter1[2] &&
                        buf[3] == chapter1[3] && buf[4] == chapter1[4] && buf[5] == chapter1[5] && buf[6] == chapter1[6]) {
                    cur_chapter++;
                    continue;
                } else if (buf[3] == chapter1[0] && buf[4] == chapter1[1] && buf[5] == chapter1[2] &&
                           buf[6] == chapter1[3] && buf[7] == chapter1[4] && buf[8] == chapter1[5] && buf[9] == chapter1[6]){
                    cur_chapter++;
                    continue;
                }
                // 判断是否纯数字行（页码）
                else if (stoi(string(buf)) && stoi(string(buf)) == cur_page + 1) {
                    cur_page++;
                    continue;
                }
            }  catch (...) {
                // 非数字行不会抛异常，忽略异常
            }

            // 逐字符比对是否包含关键字
            char *buf_point = buf;
            while (*buf_point != 0) {
                if (*buf_point == *name) {
                    int p = 0;
                    int sign = 1;
                    while(*(name+p) != 0){
                        if(*(name+p) != *(buf_point+p)){
                            sign = 0;
                            break;
                        }
                        p++;
                    }
                    if (sign == 0) {
                        buf_point++;
                        continue;
                    } else {
                        // 找到匹配，将结果存入searchInfo，并在textBrowser显示
                        searchInfo.push_back(SearchInfo(i, cur_page, cur_chapter, string(buf)));
                        ui->textBrowser->append(QString::fromStdString("<a href='" + to_string(++num) + "'>" + to_string(num) + "</a>"+ "&nbsp;&nbsp;&nbsp;&nbsp;" + name + "&nbsp;&nbsp;&nbsp;&nbsp;" + to_string(cur_page) + "&nbsp;&nbsp;&nbsp;&nbsp;" + to_string(cur_chapter) + "&nbsp;&nbsp;&nbsp;&nbsp;"+ string(books[i])));
                    }
                }
                buf_point++;
            }
        }
        file.close();
    }
}

/**
 * @brief SearchInfo 构造函数
 * @param bookNum 所属书籍编号
 * @param page 所在页码
 * @param chapter 所在章节
 * @param line 包含关键字的文本行
 */
SearchInfo::SearchInfo(int bookNum, int page, int chapter, std::string line)
    : bookNum(bookNum), page(page), chapter(chapter), line(line)
{

}

/**
 * @brief SearchInfo 析构函数
 */
SearchInfo::~SearchInfo(){}

/**
 * @brief 获取搜索结果文本行
 * @return string 返回存储的文本行
 */
string SearchInfo::getLine()
{
    return this->line;
}

/**
 * @brief 处理编号查询按钮点击事件
 * 
 * 根据用户输入的编号，展示对应编号的搜索结果文本行。
 * 若编号非法，弹出提示框警告。
 */
void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->lineEdit_2->text();//获取编号文本
    int num = 0;
    try {
        num = stoi(text.toStdString());
    }  catch (...) {
        QMessageBox::warning(this,tr("警告信息"),tr("请输入正确的编号"));
        return;
    }
    num--;
    if(num >= int(searchInfo.size()) || num < 0){
        QMessageBox::warning(this,tr("警告信息"),tr("输入的编号过大或无效"));
        return;
    }
    ui->textBrowser_2->clear();
    ui->textBrowser_2->append(QString::fromStdString(searchInfo[num].getLine()));
}

/**
 * @brief 处理textBrowser中的超链接点击事件
 * 
 * 用户点击显示结果中的超链接编号时触发。
 * 展示对应编号的搜索结果文本行。
 * @param arg1 被点击的超链接 URL
 */
void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    ui->textBrowser->setOpenExternalLinks(false);
    ui->textBrowser->setOpenLinks(false);
    QString clickedText = arg1.toString();
    int num = stoi(clickedText.toStdString()) - 1;
    if(num >= 0 && num < int(searchInfo.size())){
        ui->textBrowser_2->clear();
        ui->textBrowser_2->append(QString::fromStdString(searchInfo[num].getLine()));
    }
}