#ifndef PARAMTER_H
#define PARAMTER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QSpinBox>

namespace Ui {
class paramter;
}

class paramter : public QWidget
{
    Q_OBJECT

public:
    explicit paramter(QWidget *parent = nullptr);
    ~paramter();

private slots:
    void on_pushButton_clicked();

private:
    Ui::paramter *ui;
    QStandardItemModel *m_model;
    QStringList strHeader;
    QHeaderView *_headView;

    QSpinBox *sp1;
    QSpinBox *sp2;
    QSpinBox *sp3;
    QSpinBox *sp4;
    QSpinBox *sp5;
    QSpinBox *sp6;
    QSpinBox *sp7;

    QLineEdit *line1;
    QLineEdit *line2;
    QLineEdit *line3;
    QLineEdit *line4;
    QLineEdit *line5;
    QLineEdit *line6;
    QLineEdit *line7;
};

#endif // PARAMTER_H
