#ifndef POLYTABLE_H
#define POLYTABLE_H


#include <QTabWidget>
#include <stack>
#include "poly.h"

namespace Ui {
class PolyTable;
}

class PolyTable : public QWidget
{
    Q_OBJECT

public:
    explicit PolyTable(QWidget *parent = nullptr);
    explicit PolyTable(QWidget *parent, Poly &op);
    ~PolyTable();
    const Poly &getPoly();

private slots:
    void on_showPolyButton_clicked();

    void on_getCoefButton_clicked();

    void on_addElementButton_clicked();

    void on_delElementButton_clicked();

    void on_add_clicked();

    void on_minus_clicked();

    void on_multiply_clicked();

    void on_divide_clicked();

    void on_remainder_clicked();

    const Poly &getTabs();

    void on_renameButton_clicked();

    void on_dupliButton_clicked();

    void on_clearButton_clicked();

    void operatorHandler(const char oper);
private:
    Ui::PolyTable *ui;
    Poly p;
    QTabWidget *parentTab;
    std::stack<Poly> stk;
};

#endif // POLYTABLE_H
