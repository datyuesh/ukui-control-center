/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef QL_SWICHBUTTON_H
#define QL_SWICHBUTTON_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QGSettings>
#include <QVariant>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit        SwitchButton(QWidget *parent = nullptr);
    ~SwitchButton();
    void            set_swichbutton_val(const int &bIsOn);
    int             get_swichbutton_val() const;
    void            set_id(const int &buttonID);
    int             get_id() const;
    int             get_active() const;
    void            set_active(const bool &ok);
private:
    QColor          m_bgColorOff;
    QColor          m_bgColorOn;
    QGSettings      *m_qtThemeSetting;
    QGSettings      *m_gtkThemeSetting;
    int             m_bIsActive = 1;
    int             m_bIsOn = 1;
    QColor          *m_buttonColor;
    QTimer          *m_cTimer;
    float           m_fWidth;
    float           m_fHeight;
    float           m_fCurrentValue;
    int             m_buttonID;
    void            paintEvent(QPaintEvent *event);
    void            mousePressEvent(QMouseEvent *event);
signals:
    void            status(int bIsOn,int buttonID);
private slots:
    void startAnimation();
};

#endif // QL_SWICHBUTTON_H
