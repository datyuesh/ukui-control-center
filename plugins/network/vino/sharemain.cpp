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
#include "sharemain.h"

#include <QProcess>
#include <QHBoxLayout>
#include <QAbstractButton>

ShareMain::ShareMain(QWidget *parent)
    : QWidget(parent) {

    mVlayout = new QVBoxLayout(this);
    mVlayout->setContentsMargins(0, 0, 32, 0);
    initUI();
    initConnection();
}

ShareMain::~ShareMain() {

}

void ShareMain::initUI() {
    mShareTitleLabel = new QLabel(tr("Share"), this);
    mShareTitleLabel->setStyleSheet("QLabel{font-size: 18px; color: palette(windowText);}");

    mEnableFrame = new QFrame(this);
    mEnableFrame->setFrameShape(QFrame::Shape::Box);
    mEnableFrame->setMinimumSize(550, 50);
    mEnableFrame->setMaximumSize(960, 50);

    QHBoxLayout * enableHLayout = new QHBoxLayout();

    mEnableBox = new QCheckBox(this);
    mEnableLabel = new QLabel(tr("Allow others to view your desktop"), this);
    enableHLayout->addWidget(mEnableBox);
    enableHLayout->addWidget(mEnableLabel);
    enableHLayout->addStretch();

    mEnableFrame->setLayout(enableHLayout);

    mViewFrame = new QFrame(this);
    mViewFrame->setFrameShape(QFrame::Shape::Box);
    mViewFrame->setMinimumSize(550, 50);
    mViewFrame->setMaximumSize(960, 50);

    QHBoxLayout * viewHLayout = new QHBoxLayout();

    mViewBox = new QCheckBox(this);
    mViewLabel = new QLabel(tr("Allow connection to control screen"), this);
    viewHLayout->addWidget(mViewBox);
    viewHLayout->addWidget(mViewLabel);
    viewHLayout->addStretch();

    mViewFrame->setLayout(viewHLayout);

    mSecurityTitleLabel = new QLabel(tr("Security"), this);

    mSecurityFrame = new QFrame(this);
    mSecurityFrame->setFrameShape(QFrame::Shape::Box);
    mSecurityFrame->setMinimumSize(550, 50);
    mSecurityFrame->setMaximumSize(960, 50);

    QHBoxLayout * secHLayout = new QHBoxLayout();

    mAccessBox = new QCheckBox(this);
    mAccessLabel = new QLabel(tr("You must confirm every visit for this machine"), this);
    secHLayout->addWidget(mAccessBox);
    secHLayout->addWidget(mAccessLabel);
    secHLayout->addStretch();

    mSecurityFrame->setLayout(secHLayout);

    mSecurityPwdFrame = new QFrame(this);
    mSecurityPwdFrame->setFrameShape(QFrame::Shape::Box);
    mSecurityPwdFrame->setMinimumSize(550, 50);
    mSecurityPwdFrame->setMaximumSize(960, 50);

    QHBoxLayout * pwdHLayout = new QHBoxLayout();

    mPwdBox = new QCheckBox(this);
    mPwdsLabel = new QLabel(tr("Require user to enter this password: "), this);
    mPwdLineEdit = new QLineEdit(this);
    pwdHLayout->addWidget(mPwdBox);
    pwdHLayout->addWidget(mPwdsLabel);
    pwdHLayout->addStretch();
    pwdHLayout->addWidget(mPwdLineEdit);

    mSecurityPwdFrame->setLayout(pwdHLayout);

    mVlayout->addWidget(mShareTitleLabel);
    mVlayout->addWidget(mEnableFrame);
    mVlayout->addWidget(mViewFrame);

    mVlayout->addWidget(mSecurityTitleLabel);
    mVlayout->addWidget(mSecurityFrame);
    mVlayout->addWidget(mSecurityPwdFrame);

    mVlayout->addStretch();
}

void ShareMain::initConnection() {
    QByteArray id(kVinoSchemas);
    if (QGSettings::isSchemaInstalled(id)) {
        mVinoGsetting = new QGSettings(kVinoSchemas, QByteArray(), this);

        initEnableStatus();

        connect(mEnableBox, &QCheckBox::clicked, this, &ShareMain::enableSlot);
        connect(mViewBox, &QCheckBox::clicked, this, &ShareMain::viewBoxSlot);
        connect(mAccessBox, &QCheckBox::clicked, this, &ShareMain::accessSlot);
        connect(mPwdBox, &QCheckBox::clicked, this, &ShareMain::pwdEnableSlot);
        connect(mPwdLineEdit, &QLineEdit::textChanged, this, &ShareMain::pwdInputSlot);
    }
}

void ShareMain::initEnableStatus() {
    bool isShared = mVinoGsetting->get(kVinoViewOnlyKey).toBool();
    bool secPwd = mVinoGsetting->get(kVinoPromptKey).toBool();
    QString pwd = mVinoGsetting->get(kAuthenticationKey).toString();

    mAccessBox->setChecked(secPwd);
    mViewBox->setChecked(!isShared);
    if (pwd == "vnc") {
        mPwdBox->setChecked(true);
        mPwdsLabel->setEnabled(true);
    } else {
        mPwdBox->setChecked(false);
        mPwdLineEdit->setVisible(false);
        mPwdsLabel->setEnabled(false);
    }

    QProcess *process = new QProcess;

    process->start("systemctl", QStringList() << "--user" << "is-active" << "vino-server.service");
    process->waitForFinished();
    setFrameVisible((process->readAllStandardOutput().replace("\n","") == "active"));

    process->close();
}

void ShareMain::setFrameVisible(bool visible) {
    mEnableBox->setChecked(visible);

    mViewFrame->setVisible(visible);
    mSecurityFrame->setVisible(visible);
    mSecurityPwdFrame->setVisible(visible);
    mSecurityTitleLabel->setVisible(visible);
}

void ShareMain::enableSlot(bool status) {
    QProcess process;
    QString cmd;

    if(status) {
        cmd = "start";
    } else {
        cmd = "stop";
    }
    process.startDetached("systemctl", QStringList() << "--user" << cmd << "vino-server.service");

    setFrameVisible(status);
}

void ShareMain::viewBoxSlot(bool status) {
    mVinoGsetting->set(kVinoViewOnlyKey, !status);
}

void ShareMain::accessSlot(bool status) {
    if (status) {
        mVinoGsetting->set(kVinoPromptKey, true);
    } else {
        mVinoGsetting->set(kVinoPromptKey, false);
    }
}

void ShareMain::pwdEnableSlot(bool status) {
    if (status) {
        mVinoGsetting->set(kAuthenticationKey, "vnc");
        mPwdsLabel->setEnabled(true);
        mPwdLineEdit->setVisible(true);
    } else {
        mPwdsLabel->setEnabled(false);
        mPwdLineEdit->setVisible(false);
        mVinoGsetting->set(kAuthenticationKey, "none");
    }
}

void ShareMain::pwdInputSlot(QString pwd) {
    Q_UNUSED(pwd);
    QByteArray text = pwd.toLocal8Bit();
    QByteArray secPwd = text.toBase64();
    mVinoGsetting->set(kVncPwdKey, secPwd);
}
