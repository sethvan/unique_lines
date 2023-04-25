#include "widget.h"
#include "Helpers.h"
#include "./ui_widget.h"
#include <QMimeData>
#include <string>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Widget::dropEvent(QDropEvent *event)
{

    if(ui->pathInput->text().isEmpty()) {
        qDebug() << "Line edit is empty!";
        ui->drop_window->clear();
        myUrls.clear();
        QMessageBox::warning(this, "Missing output directory path!", "Click on the Select button to set the path for "
                                                                     "the directory where output files will be placed.");
        return;
    }
    if(myUrls.size() >= 2) {
        ui->drop_window->clear();
        myUrls.clear();
    }
    QList<QUrl> urls = event->mimeData()->urls();
    QString file;
    for (const QUrl &url : urls) {        
        if(ui->drop_window->toPlainText().isEmpty()) {
            file = "File_One: " + url.toLocalFile();
        }
        else {
            file = "File_Two: " + url.toLocalFile();
        }
        ui->drop_window->append(file);
        myUrls.append(url);
        if(myUrls.size() >= 2) {

            auto file1 = myUrls[0].toLocalFile();
            auto file2 = myUrls[1].toLocalFile();

            QString outFilePath = getOutFilePath(ui->pathInput->text(), file1, file2);
            writeMissingLinesToFile(file1.toStdString(), file2.toStdString(), outFilePath.toStdString(), uniques);
            ui->drop_window->append("\nUnique lines from below files Were written to \'" + outFilePath + "\'");
            ui->drop_window->append(myUrls[0].toLocalFile());
            ui->drop_window->append(myUrls[1].toLocalFile());
            ui->drop_window->append("\n\nOutput file contents:");
            ui->drop_window->append(uniques);
        }
    }
}



void Widget::on_pushButton_clicked()
{
    ui->drop_window->clear();
    myUrls.clear();
}


void Widget::on_selectPathButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->pathInput->setText(dir);

}

