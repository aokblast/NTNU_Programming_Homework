#include "polytable.h"
#include "ui_polytable.h"
#include <string.h>
#include <QString>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

PolyTable::PolyTable(QWidget *parent) :
    QWidget(parent), parentTab((QTabWidget *)parent),
    ui(new Ui::PolyTable)
{
    ui->setupUi(this);
    ui->displayer->setFrameStyle(QFrame::NoFrame);
    QString s = QInputDialog::getText(parent, "Polynomial Input", "Please input the polynomial", QLineEdit::Normal, "2x^2-2x+1", NULL);

    while(true){
        try{
            p = Poly(s.toStdString());
            break;
        }catch(const char *msg){
            s = QInputDialog::getText(parent, "Polynomial Input", msg, QLineEdit::Normal, "0", NULL);
        }
    }

    ui->displayer->setText(QString::fromStdString(p.str()));
}

PolyTable::PolyTable(QWidget *parent, Poly &op):
    QWidget(parent), parentTab((QTabWidget *)parent),
    ui(new Ui::PolyTable)
{
    ui->setupUi(this);
    ui->displayer->setFrameStyle(QFrame::NoFrame);
    p = op;
    ui->displayer->setText(QString::fromStdString(p.str()));
}

PolyTable::~PolyTable()
{
    delete ui;
}

void PolyTable::on_showPolyButton_clicked()
{
    ui->displayer->setText(QString::fromStdString(p.str()));
}


void PolyTable::on_getCoefButton_clicked()
{
    int index;
    bool ok;
    index = QInputDialog::getInt(NULL, "Get Coefficient", "Please enter the exponent you want to check", 0, 0, INT_MAX, 1, &ok);
    if(!ok){
        return;
    }
    if(p.find(index) == p.end()){
        QMessageBox::information(NULL, "Warning", "Exponent not exist");
        return;
    }
    if(p.find(index) != p.end()){
        ui->displayer->setText(QString::fromStdString(std::to_string(p[index])));
    }else{
        ui->displayer->setText("Non exist term");
    }
}


void PolyTable::on_addElementButton_clicked()
{
    int index;
    double num;
    bool ok;
    index = QInputDialog::getInt(NULL, "Exponent input", "Enter the exponent you want to add/change", 0, 0, INT_MAX, 1, &ok);
    if(!ok)return;
    num = QInputDialog::getDouble(NULL, "Coef input", "Enter the coefficient you want to add/change", 0, INT_MIN, INT_MAX, 1, &ok);
    if(!ok)return;
    if(num == 0){
         QMessageBox::information(NULL, "Warning", "Zero is not permitted");
         return;
     }

    p[index] = num;
    ui->displayer->setText(QString::fromStdString(p.str()));
}


void PolyTable::on_delElementButton_clicked()
{
    int index;
    bool ok;
    index =  QInputDialog::getInt(NULL, "Exponent delete", "Enter the exponent you want to delete", 0, 0, INT_MAX, 1, &ok);
    if(!ok)return;
    if(p.find(index) == p.end()){
        QMessageBox::information(NULL, "Warning", "Exponent not exist");
        return;
    }
    p.erase(index);
    ui->displayer->setText(QString::fromStdString(p.str()));
}


void PolyTable::on_add_clicked()
{
    operatorHandler('+');
}


void PolyTable::on_minus_clicked()
{

    operatorHandler('-');
}


void PolyTable::on_multiply_clicked()
{
    operatorHandler('*');
}


void PolyTable::on_divide_clicked()
{
    operatorHandler('/');
}


void PolyTable::on_remainder_clicked()
{
    operatorHandler('%');
}

void PolyTable::operatorHandler(const char oper){
    Poly pu;

    auto choose = QMessageBox::information(NULL, "Hint", "Use exist polynomial?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if(choose == QMessageBox::Yes){
        try{
            pu = getTabs();
        }catch(const char *msg){
            QMessageBox::information(NULL, "Warning", msg, QMessageBox::Ok);
            return;
        }
    }else{
        bool ok;
        char text[500];
        sprintf(text, "Please input the polynomial you want to %s",  oper == '+' ? "add" : (oper == '-' ? "minus" : (oper == '*' ? "multiply" : "divide")));
        QString s = QInputDialog::getText(NULL, "Polynomial input", text, QLineEdit::Normal, "0", &ok);
        if(!ok)return;
        while(true){
            try{
                pu = Poly(s.toStdString());
                break;
            }catch(const char *msg){
                s = QInputDialog::getText(NULL, "Polynomial input", msg, QLineEdit::Normal, "0", &ok);
                if(!ok)return;
            }
        }
    }

    try{
        switch(oper){
            case '+':
                p += pu;
                break;
            case '-':
                p -= pu;
                break;
            case '*':
                p *= pu;
                break;
            case '/':
                p /= pu;
                break;
            case '%':
                p %= pu;
                break;
        }
    }catch(const char *msg){
        QMessageBox::information(NULL, "Warning", msg, QMessageBox::Ok);
        return;
    }

    ui->displayer->setText(QString::fromStdString(p.str()));
}

const Poly& PolyTable::getPoly(){
    return p;
}

const Poly &PolyTable::getTabs(){
    QStringList list;
    for(int i = 0; i < parentTab->count(); ++i){
        list << (QString("%0. ").arg(i + 1) + parentTab->tabText(i));
    }
    bool ok = false;
    QString choose = QInputDialog::getItem(NULL, "Chooser", "Choose a polynomial", list, 0, true, &ok);
    if(!ok){
        throw "You don't choose anything";
    }
    int idx = stoi(choose.toStdString());
    return ((PolyTable*)(parentTab->widget(idx - 1)))->getPoly();
}

void PolyTable::on_renameButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(NULL, "Rename", "Please enter a name", QLineEdit::Normal, parentTab->tabText(parentTab->currentIndex()), &ok);
    if(!ok)return;
    parentTab->setTabText(parentTab->currentIndex(), name);
}


void PolyTable::on_dupliButton_clicked()
{
    parentTab->addTab(new PolyTable((QWidget*) parentTab, p), parentTab->tabText(parentTab->currentIndex()) + "_dup");
}


void PolyTable::on_clearButton_clicked()
{
    if (!(QMessageBox::information(this,tr("Cleaner"),tr("Do you really want to clear the polynomial"),tr("Yes"),tr("No"))))
    {
        p = Poly();
        ui->displayer->setText(QString::fromStdString(p.str()));
    }
    QString s = QInputDialog::getText(NULL, "Polynomial input", "Please input the polynomial", QLineEdit::Normal, "0", NULL);

    while(true){
        try{
            p = Poly(s.toStdString());
            break;
        }catch(const char *msg){
            s = QInputDialog::getText(NULL, "Polynomial input", msg, QLineEdit::Normal, "0", NULL);
        }
    }

    ui->displayer->setText(QString::fromStdString(p.str()));
}

