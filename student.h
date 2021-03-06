#ifndef STUDENT_H
#define STUDENT_H

/*      importing QString for use with name and card number     */
#include <QString>
#include <QTime>
#include <sstream>


/*----------------------------------------------
 *
 *      ===========student.h==========
 *
 *
 * Student class definition with member variables
 * and function definitions
 *
 * Author: Sam McAnelly
 * Last Revised: 5/21/2015
 *
 *
 * ---------------------------------------------*/


class Student
{
private:
    std::string     name;
    std::string     first_name;
    std::string     last_name;
    QString         card_number;
    float           hours_required;
    float           hours_deducted;
    float           hours_complete;
    float           hours_day_total;
    bool            status;
    bool            names_computed;
    QTime           timer;

    void compute_names();

public:
    Student();
    Student(std::string _name);
    Student(std::string _name, QString _card_num, float _hours_req);
    ~Student();

    Student& operator=(const Student &old_stud);

    float get_hours_required() const;
    void set_hours_required(float new_hours_complete);

    float get_base_hours() const;

    float get_deductions() const;
    void  set_deductions(float deductions);

    void set_hours_day_total(float total);

    bool get_status() const;
    void set_status(bool new_status);
    void toggle_status();

    float get_hours_deducted() const;
    float get_hours_day_total() const;
    float get_hours_complete();
    float get_base_hours_complete() const;
    float get_hours_incomplete() const;
    void set_hours_complete(float hrs);
    void increment_hours_complete(float addition);

    std::string get_name() const;
    const std::string get_last_name() const;
    const std::string get_first_name() const;
    void set_name(std::string new_name);

    std::string get_card_number() const;
    void set_card_number(QString new_card_number);

    QTime get_timer() const;
    void clock_in();
    void clock_out();
    float get_current_complete_time();

    void output_debug_info();

};

#endif // STUDENT_H
