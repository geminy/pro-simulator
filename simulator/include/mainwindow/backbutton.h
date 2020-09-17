#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include <QPushButton>

class BackButton : public QPushButton
{
    Q_OBJECT

public:
    explicit BackButton(QWidget *parent);
    ~BackButton();
};

#endif // BACKBUTTON_H
