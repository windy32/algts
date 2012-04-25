#ifndef RATINGMETHODDIALOG_H
#define RATINGMETHODDIALOG_H

#include <QDialog>

namespace Ui {
class RatingMethodDialog;
}

class RatingMethodDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RatingMethodDialog(QWidget *parent = 0);
    ~RatingMethodDialog();
    
private:
    Ui::RatingMethodDialog *ui;
};

#endif // RATINGMETHODDIALOG_H
