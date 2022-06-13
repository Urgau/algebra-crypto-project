#ifndef CLICKABLELABEL_HPP_
#define CLICKABLELABEL_HPP_

#include <Qt>
#include <QLabel>
#include <QWidget>

/// \brief ClickLabel class
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ///\brief Constructor
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR);
    ///\brief Destructor
    ~ClickableLabel();

signals:
    ///\brief Signal when someone click
    void clicked();

protected:
    ///\brief Mouse Press Event
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_HPP_
