#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analyzer.hpp"
#include <QFileDialog>
#include <QElapsedTimer>
#include <QDir>
#include <QtCore>
#include <fstream>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include <QSqlError>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->radioButtonCheck, SIGNAL(clicked()), this, SLOT(optionsradiobutt_clicked()));
    connect(ui->radioButtonTwofiles, SIGNAL(clicked()), this, SLOT(optionsradiobutt_clicked()));

    connect(ui->chooseButton, SIGNAL(clicked()), this, SLOT(choosebuttonsclicked()));
    connect(ui->chooseButton_2, SIGNAL(clicked()), this, SLOT(choosebuttonsclicked()));

    ui->linePath_2->hide();
    ui->chooseButton_2->hide();

    ui->lineName2->hide();
    ui->labelName2->hide();

    //database reading
    QDir dir(QApplication::applicationDirPath() + "/database");
    qDebug() << QApplication::applicationDirPath();
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);   //sort
    QFileInfoList list = dir.entryInfoList();
    for(auto elem : list)
    {
        ui->listWidget->addItem(elem.fileName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::choosebuttonsclicked()
{
    QString path = QFileDialog::getOpenFileName(0, "Choose file", "");
    QPushButton* pushbutt = (QPushButton*)sender();
    if(pushbutt->objectName() == "chooseButton")
    {
        ui->linePath->setText(path);
    }
    else
    {
        ui->linePath_2->setText(path);
    }
}


void MainWindow::optionsradiobutt_clicked()
{
    QRadioButton* rbutt = (QRadioButton*)sender();

    if(rbutt->text() == "Standard check")
    {
        ui->linePath_2->hide();
        ui->chooseButton_2->hide();
        ui->groupBoxSettings->show();
        ui->listWidget->show();

        ui->lineName2->hide();
        ui->labelName2->hide();
    }
    else
    {
        ui->linePath_2->show();
        ui->chooseButton_2->show();
        ui->groupBoxSettings->hide();
        ui->listWidget->hide();

        ui->lineName2->show();
        ui->labelName2->show();
    }
}
void error_back_fill(QWidget* widget)
{
    QElapsedTimer time;
    time.start();
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::Key_Green);
    widget->setPalette(palette);
    widget->repaint();
    for(;time.elapsed() < 400;){}
    palette.setColor(QPalette::Base,QColor(186, 189, 182));
    widget->setPalette(palette);
}

void bring_to_standard_view(std::string& str)
{
    Analyzer::delete_unnecessary(str);

    Analyzer::distinguish_operators(str);

    std::string before = "";
    std::string current;
    size_t index = 0;
    size_t curr_pos = 1;
    std::string new_str = "";
    //replacement of names with standard
    unsigned int id = 0;

    str += ' ';
    while((index = str.find(' ', curr_pos)) != std::string::npos)
    {
        current = str.substr(curr_pos, index - curr_pos);
        curr_pos = index + 1;
        if(before == "") before = current;
        else
        {
            auto double_iterator = Analyzer::find_words_id_and_special(before);
            if(double_iterator.first != Analyzer::words_id_end())
            {
                new_str += (double_iterator.first)->second + ' ';
                before = current;
            }
            else if(double_iterator.second != Analyzer::special_end())
            {
                new_str += (double_iterator.second)->second + ' ';
                before = current;
            }
            else if(Analyzer::belong_to_stl(before, current))
            {
                new_str += before + ' ' + current + ' ';
                before = "";
            }
            else if((before.length() == 1 && (!std::isalpha(before[0]) || before[0] == '_')) || std::all_of(before.begin(), before.end(), ::isdigit))
            {
                new_str += before + ' ';
                before = current;
            }
            else if((before[0] == '"' && before[before.length() - 1] == '"')
                 || (before[0] == '\'' && before[before.length() - 1] == '\''))
            {
                new_str += before + ' ';
                before = current;
            }
            else
            {
                std::string tmperid = std::to_string(id++);
                std::string tmper = "";
                for    (size_t i = 0; i < (6 - tmperid.length()); ++i)
                {
                    tmper += '0';
                }
                tmper = "ID" + tmper + tmperid;
                Analyzer::insert_to_words_id(tmper, before);
                new_str += tmper + ' ';
                before = current;
            }
        }
    }
    str = new_str;
    Analyzer::clear();
}

bool read(std::string& str, const QString& path)
{
    std::ifstream in(path.toUtf8().constData());
    if (in.is_open())
    {
        std::string tmp;
        while (std::getline(in, tmp))
        {
            str += (tmp + '\n');
        }
        return true;
    }
    else
        return false;
}

void MainWindow::start_that_shit0()
{
    std::string fstr = "", sstr = "";
    if(!read(fstr, (ui->linePath->text())))
    {
        QMessageBox::warning(this, "Warning", "Can not open " + (ui->linePath->text()));
    }
    else if(fstr.length() > 8000)
    {
        QMessageBox::warning(this, "Warning", "There is no reason to check so big file ");
        error_back_fill(ui->linePath);
    }
    else if(!read(sstr, (ui->linePath_2->text())))
    {
        QMessageBox::warning(this, "Warning", "Can not open " + (ui->linePath_2->text()));
    }
    else if(sstr.length() > 8000)
    {
        QMessageBox::warning(this, "Warning", "There is no reason to check so big file ");
        error_back_fill(ui->linePath_2);
    }
    else
    {
        bring_to_standard_view(fstr);
        bring_to_standard_view(sstr);
        unsigned delta = Analyzer::wagner_fisher(fstr, sstr);
        QString percent = QString::number((1.0 - ((double)delta / (sstr.length() > fstr.length() ? sstr.length() : fstr.length()))) * 100);

        //summary
        ui->summaryText->setText(percent + "% similar");
        ui->additionalAnnotation->setText("Made by W-F algorithm. Below 70 it is not entirely accurate");
        //
    }
}

void MainWindow::start_that_shit1()
{
    //all work with our candidate
    std::string fstr = "";
    if(!read(fstr, (ui->linePath->text()).toUtf8().constData()))
    {
        QMessageBox::warning(this, "Warning", "Can not open " + (ui->linePath->text()));
    }
    else if(fstr.length() > 8000)
    {
        QMessageBox::warning(this, "Warning", "There is no reason to check so big file ");
        error_back_fill(ui->linePath);
    }
    else
    {
        bring_to_standard_view(fstr);
        //HERE

        //Connection to database
        QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QApplication::applicationDirPath() + "/database/" + ui->listWidget->currentItem()->text());


        //reading
        if(db.open())
        {
           QSqlQuery query;
           unsigned min_actions = 10000, length_sus_str = 1;
           QString suspect;
           query.exec("SELECT _id, Name, String From " + ui->listWidget->currentItem()->text());
           while (query.next())
           {
               std::string check_str = (query.value(2).toString()).toUtf8().constData();
               unsigned actions = Analyzer::wagner_fisher(fstr, check_str);
               if(actions < min_actions)
               {
                   min_actions = actions;
                   suspect = query.value(1).toString();
                   length_sus_str = check_str.length() > fstr.length() ? check_str.length() : fstr.length();
               }
           }
           //summary
           QString percent = QString::number(((1.0 - ((double)min_actions / length_sus_str)) * 100));
           ui->summaryText->setText("Most likely, this code was borrowed from " + suspect + '(' + percent + "%)");
           ui->additionalAnnotation->setText("Made by W-F algorithm. Below 70 it is not entirely accurate");
           //
           query.prepare("INSERT INTO " + ui->listWidget->currentItem()->text() + "(Name, String) values(:Name,:String)");
           query.bindValue(":Name", ui->lineName->text());
           query.bindValue(":String", QString::fromStdString(fstr));
           query.exec();
           db.close();
        }
        else
        {
            QMessageBox::warning(this, "Warning", ui->listWidget->currentItem()->text() + " is damaged");
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    //wrong order of check(not srs)
    if(ui->lineName->text().isEmpty())
    {
        ui->statusbar->showMessage("candidate name is required");
        error_back_fill(ui->lineName);
    }
    else if(ui->linePath->text() == "")
    {
        ui->statusbar->showMessage("path is empty");
        error_back_fill(ui->linePath);
    }
    else if((ui->linePath->text()).right(3) != "cpp")
    {
        ui->statusbar->showMessage("wrong format of file");
        error_back_fill(ui->linePath);
    }
    else if(ui->linePath_2->isVisible())
    {
        if(ui->lineName2->text().isEmpty())
        {
            ui->statusbar->showMessage("candidate name is required");
            error_back_fill(ui->lineName2);
        }
        else if(ui->linePath_2->text() == "")
        {
            ui->statusbar->showMessage("path is empty");
            error_back_fill(ui->linePath_2);
        }
        else if((ui->linePath_2->text()).right(3) != "cpp")
        {
            ui->statusbar->showMessage("wrong format of file");
            error_back_fill(ui->linePath_2);
        }
        //Two Files compare
        else
            start_that_shit0();
    }
    //Standard check
    else
    {
        if(!ui->listWidget->currentItem())
        {
            ui->statusbar->showMessage("contest is not choosen");
            error_back_fill(ui->listWidget);
        }
        else
            start_that_shit1();
    }
}
