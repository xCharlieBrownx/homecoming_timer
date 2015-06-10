/*---------------------------------------------
 *                  INCLUDES
 * --------------------------------------------*/

/*      project file headers    */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*      c++ library headers    */

#include <fstream>
#include <iostream>

/*         QT headers          */

#include <QDir>
#include <QMessageBox>
#include <QScrollBar>

/*----------------------------------------------
 *
 *      ===========mainwindow.cpp==========
 *
 *
 * Function definitions for the mainwindow class.
 * These handle all ui events and control what the
 * user sees on the screen
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/

/*----------------------------------------------
 *          DEFINITIONS AND ENUMS
 * --------------------------------------------*/


/* ----------------------------------------------
 *          GLOBAL INSTANCE VARIABLES
 * ----------------------------------------------*/


/*------------------------------------------------
 *          MAINWINDOW FUNCTION DEFS
 * -----------------------------------------------*/

void MainWindow::clear_student_lists()
{
    ui->list_name->clear();
    ui->list_status->clear();
    ui->list_hrs->clear();
    ui->list_req->clear();
}

void MainWindow::refresh_student_lists()
{
    fill_name_list();
    fill_status_list();
    fill_hours_complete_list();
    fill_hours_required_list();
}


void MainWindow::fill_name_list()
{
    //clear the name list widgets first just in case
    ui->list_name->clear();

    int i = 0;

    QString curr_name;
    while (i < controller->get_student_count())
    {
        curr_name = QString::fromStdString(controller->get_name_from_index(i));
        QListWidgetItem *itm = new QListWidgetItem;
        itm->setText(curr_name);
        ui->list_name->insertItem(i, itm);
        ++i;
    }
}

void MainWindow::fill_status_list()
{
    //clear the status list first for good measure
    ui->list_status->clear();

    int i = 0;
    bool status;


    while(i < controller->get_student_count())
    {
        status = controller->get_status_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        if (status == false)
        {
            itm->setText(QString("Clocked Out"));
            itm->setTextColor(QColor(255,0,0));
        }
        else
        {
            itm->setText(QString("Clocked In"));
            itm->setTextColor(QColor(0, 205, 0));
        }
        ui->list_status->insertItem(i, itm);
        ++i;
    }

}
void MainWindow::fill_hours_required_list()
{
    //clear the name list widgets first just in case
    ui->list_req->clear();
    int i = 0;
    QString final;

    while (i < controller->get_student_count())
    {
        float curr_time = controller->get_hours_req_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        final.sprintf("%05.2f", curr_time);
        itm->setText(final);
        ui->list_req->insertItem(i, itm);
        ++i;
    }
}
void MainWindow::fill_hours_complete_list()
{
    //clear the hours complete list widgets first just in case
    ui->list_hrs->clear();
    int i = 0;
    float curr_time;
    QString final;

    while (i < controller->get_student_count())
    {
        curr_time = controller->get_hours_comp_from_index(i);
        QListWidgetItem *itm = new QListWidgetItem;
        final.sprintf("%05.2f", curr_time);
        itm->setText(final);
        ui->list_hrs->insertItem(i, itm);
        ++i;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init = false;
    controller = new DB_Controller();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
}

void MainWindow::on_btn_tsting_clicked()
{
    controller->sort(DB_Sort::TIME_REQUIRED);
    refresh_student_lists();
}

//Syncing the QListViewWidgets
void MainWindow::on_list_name_currentRowChanged(int currentRow)
{
    QModelIndex index;
    int scroll_index;

    index = ui->list_name->currentIndex();
    QScrollBar *bar = ui->list_name->verticalScrollBar();
    scroll_index = bar->value();

    ui->list_status->setCurrentRow(currentRow);
    bar = ui->list_status->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_hrs->setCurrentRow(currentRow);
    bar = ui->list_hrs->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_req->setCurrentRow(currentRow);
    bar = ui->list_req->verticalScrollBar();
    bar->setValue(scroll_index);

    ui->list_name->setCurrentRow(currentRow);
    bar = ui->list_name->verticalScrollBar();
    bar->setValue(scroll_index);

}

void MainWindow::on_btn_find_clicked()
{
    int index = controller->search_name(ui->txt_cwid_name->text().toUtf8().constData());
    if(index == -1)
        index = controller->search_card_number(ui->txt_cwid_name->text().toUtf8().constData());
    if(index != -1)
        ui->list_name->setCurrentRow(index);
    else
    {
        QMessageBox box;
        box.setText("Name not found!");
        box.exec();
    }
}

void MainWindow::on_btn_clk_in_out_clicked()
{
    std::string name = ui->txt_cwid_name->text().toUtf8().constData();
    int index = ui->list_name->currentRow();
    if(name.compare("") == 0)
    {
        controller->toggle_status_from_index(index);
        fill_status_list();
        if(!controller->get_status_from_index(index))
            fill_hours_complete_list();
        ui->list_name->setCurrentRow(index);
    }
    else
    {
        index = controller->search_name(ui->txt_cwid_name->text().toUtf8().constData());
        if(index == -1)
            index = controller->search_card_number(ui->txt_cwid_name->text().toUtf8().constData());
        if(index == -1)
        {
            QMessageBox box;
            box.setText("Not found!");
            box.exec();
        }
        else
        {
            controller->toggle_status_from_index(index);
            fill_status_list();
            fill_hours_complete_list();
            ui->list_name->setCurrentRow(index);
        }
    }
}

void MainWindow::on_combo_db_selection_currentIndexChanged(int index)
{
    if(!init)
    {
       controller->begin();
       init = true;
    }
    switch(index)
    {
    case 0:
        clear_student_lists();
        return;
    case 1:
        controller->set_gender(DB_Controller::GUYS);
        break;
    case 2:
        controller->set_gender(DB_Controller::GIRLS);
        break;
    }

    refresh_student_lists();
}

void MainWindow::on_btn_add_clicked()
{

}

void MainWindow::on_btn_sort_clicked()
{
    controller->sort( (DB_Sort::sort_by)ui->combo_sort_by->currentIndex() );
}
