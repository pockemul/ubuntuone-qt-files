/*
 * This file is part of UbuntuOne Qt Files.
 *
 * UbuntuOne Qt Files is free software:  you can redistribute it  and/or  modify it under the terms of the  GNU  General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * UbuntuOne Qt Files  is distributed in the hope  that it will be useful,  but  WITHOUT ANY WARRANTY;  without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with Foobar.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "LoginWindow.h"
#include "MainWindow.h"
#include "Utils.h"
#include "LoginController.h"
#include "NodeListController.h"
#include "NodeListView.h"
#include <QtCore>
#include <QtQuick>

LoginWindow::LoginWindow()
    : BaseWindow("qrc:/qml/LoginWindow.qml")
{
    connect(this->view, SIGNAL(login(QString,QString)), this, SLOT(loginButtonPressed(QString,QString)));
    connect(this->view, SIGNAL(menuAbout()), this, SLOT(menuAbout()));
}

void LoginWindow::loginButtonPressed(const QString &username, const QString &password)
{
    this->loginController = new LoginController(this);
    connect(this->loginController, SIGNAL(loginError(QString)), this, SLOT(loginError(QString)));
    connect(this->loginController, SIGNAL(loginFinished()), this, SLOT(loginFinished()));
    this->loginController->login(username, password);
}

void LoginWindow::loginFinished()
{
    delete this->loginController;

    MainWindow::getInstance()->clear();
    MainWindow::getInstance()->push(NodeListController::getInstance()->createView(
            NodeListController::ROOT_PATH)->getView());
}

void LoginWindow::loginError(const QString &errorDescription)
{
    delete this->loginController;
    MainWindow::getInstance()->showToast(true, errorDescription);
}

void LoginWindow::menuAbout()
{
    MainWindow::getInstance()->showAboutDialog();
}
