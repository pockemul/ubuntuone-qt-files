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
#include "MainWindow.h"
#include <QtCore>
#include <QtQuick>
#include "Utils.h"

MainWindow *MainWindow::instance = NULL;

MainWindow *MainWindow::getInstance()
{
    if (MainWindow::instance == NULL)
        MainWindow::instance = new MainWindow();
    return MainWindow::instance;
}

MainWindow::MainWindow()
    : engine(new QQmlEngine(this)),
      window(NULL),
      toastTimer(new QTimer(this))
{
    Utils::setGlobalProperties(this->engine->rootContext());
    QQmlComponent component(this->engine, QUrl("qrc:/qml/MainWindow.qml"));

    if (!component.isReady())
        qWarning("%s", qPrintable(component.errorString()));

    this->window = qobject_cast<QQuickWindow *>(component.create());

    if (!this->window)
        qFatal("Error: Your root item has to be a Window.");

    this->engine->setIncubationController(this->window->incubationController());

    connect(this->window, SIGNAL(popStackView()), this, SLOT(pop()));
    connect(this->toastTimer, SIGNAL(timeout()), this, SLOT(hideToast()));
}

void MainWindow::show() const
{
    this->window->show();
}

void MainWindow::showLoadingSpinner(bool visible, const QString &text)
{
    QQuickItem *loadingSpinner = this->window->findChild<QQuickItem *>("loadingSpinner");
    loadingSpinner->setProperty("visible", visible);
    loadingSpinner->setProperty("loadingSpinnerText", text);
}

void MainWindow::showToast(bool visible, const QString &text, int seconds)
{
    QQuickItem *toast = this->window->findChild<QQuickItem *>("toast");
    if (visible) {
        QMetaObject::invokeMethod(toast, "show");
        toast->setProperty("toastText", text);
        this->toastTimer->setSingleShot(true);
        this->toastTimer->start(seconds*1000);
    } else {
        QMetaObject::invokeMethod(toast, "hide");
    }
}

void MainWindow::showAboutDialog()
{
    QQuickItem *aboutDialog = this->window->findChild<QQuickItem *>("aboutDialog");
    QMetaObject::invokeMethod(aboutDialog, "show");
}

void MainWindow::push(QQuickItem *view)
{
    QVariant viewItem = QVariant::fromValue(qobject_cast<QQuickItem *>(view));
    QQuickItem *stackView = this->window->findChild<QQuickItem *>("stackView");

    if (!stackView->property("busy").toBool())
        QMetaObject::invokeMethod(stackView, "push", Q_ARG(QVariant, viewItem));
}

void MainWindow::pop()
{
    QQuickItem *stackView = this->window->findChild<QQuickItem *>("stackView");
    int numberOfItems = stackView->childItems().count();

    if (numberOfItems >= 2) {
        QQuickItem *previousItem = stackView->childItems().at(numberOfItems - 2);
        QMetaObject::invokeMethod(stackView, "pop", Q_ARG(QVariant, QVariant::fromValue(previousItem)));
    }
}

void MainWindow::clear()
{
    QQuickItem *stackView = this->window->findChild<QQuickItem *>("stackView");
    QMetaObject::invokeMethod(stackView, "clear");
}

void MainWindow::hideToast()
{
    this->showToast(false);
}
