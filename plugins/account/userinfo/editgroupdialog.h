/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2020 KYLINOS Information Technology Co., Ltd.
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

#ifndef EDITGROUPDIALOG_H
#define EDITGROUPDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#include <QCheckBox>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QListWidget>

#include "qtdbus/systemdbusdispatcher.h"

typedef struct _UserInfomations {
    QString objpath;
    QString username;
    QString iconfile;
    QString passwd;
    int accounttype;
    int passwdtype;
    bool current;
    bool logined;
    bool autologin;
    bool noPwdLogin;
    qint64 uid;
}UserInfomations;

class ChangeGroupDialog;
namespace Ui {
class EditGroupDialog;
}

class EditGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditGroupDialog(QString, QString, QString, bool, QWidget *parent = nullptr);
    ~EditGroupDialog();

public:
    void limitInput();
    void getUsersList(QString usergroup);
    void refreshCertainBtnStatus();
    QLineEdit * lineNameComponent();
    QLineEdit * lineIdComponent();
    QListWidget * listWidgetComponent();
    void idSetEnabled(bool );
    void nameSetEnabled();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::EditGroupDialog *ui;
    ChangeGroupDialog *cgDialog = nullptr;
    bool _nameHasModified;
    bool _idHasModified;
    bool _boxModified;
    QString userGroup;
    QString groupId;
    QString groupName;
    QMap<QString, UserInfomations> allUserInfoMap;
    SystemDbusDispatcher * sysdispatcher;

    void setupInit();
    void signalsBind();
    UserInfomations _acquireUserInfo(QString objpath);

signals:
    void needRefresh();
};

#endif // EDITGROUPDIALOG_H
