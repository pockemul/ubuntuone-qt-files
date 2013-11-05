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

MainWindow::MainWindow(QObject *parent)
    : QObject(parent),
      engine(new QQmlEngine(parent)),
      window(NULL)
{
    // Set global properties
    this->setIndependentResolutionScale();

    // Load the QML
    QQmlComponent component(this->engine, QUrl("qrc:/qml/MainWindow.qml"));

    if (!component.isReady())
        qWarning("%s", qPrintable(component.errorString()));

    this->window = qobject_cast<QQuickWindow *>(component.create());

    if (!this->window)
        qFatal("Error: Your root item has to be a Window.");

    this->engine->setIncubationController(this->window->incubationController());
}

void MainWindow::show() const
{
    this->window->showFullScreen();
}

void MainWindow::push(QQuickView *view)
{
    QVariant viewItem = QVariant::fromValue(qobject_cast<QQuickItem *>(view->rootObject()));
    QQuickItem *stackView = this->window->findChild<QQuickItem *>("stackView");
    QMetaObject::invokeMethod(stackView, "push", Q_ARG(QVariant, viewItem));
}

void MainWindow::pop()
{
    QQuickItem *stackView = this->window->findChild<QQuickItem *>("stackView");
    int previousItemNumber = stackView->childItems().count();

    if (previousItemNumber > 1) {
        QQuickItem *previousItem = stackView->childItems().at(previousItemNumber-1);
        QMetaObject::invokeMethod(stackView, "pop", Q_ARG(QVariant, QVariant::fromValue(previousItem)));
    }
}

void MainWindow::setIndependentResolutionScale()
{
    // In a standard resolution laptop screen->logicalDotsPerInch() is 72
    QScreen *screen = qApp->screens().at(0);
    qreal u = screen->logicalDotsPerInch()/72.0;
    this->engine->rootContext()->setContextProperty("u", u);
}
