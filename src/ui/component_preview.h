#ifndef COMPONENTPREVIEW_H
#define COMPONENTPREVIEW_H

#include <QRadioButton>
#include <QWidget>

#include "src/model/component.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ComponentPreview; }
QT_END_NAMESPACE

namespace Ohmcha
{

class ComponentPreview : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentPreview(QWidget *parent = nullptr);
    ~ComponentPreview();

private:
    Ui::ComponentPreview *ui;
    Ohmcha::Component *component;
    QRadioButton anchors[3][3];
};

}
#endif // COMPONENTPREVIEW_H
