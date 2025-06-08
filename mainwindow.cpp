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