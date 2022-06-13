/*
** EPITECH PROJECT, 2020
** client/src/ClickableLabel
** File description:
** segmentation fault (core dumped)
*/

#include "ClickableLabel.hpp"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{ /* ignore */ }

ClickableLabel::~ClickableLabel()
{ /* ignore */ }

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    (void) event;
    emit clicked();
}
