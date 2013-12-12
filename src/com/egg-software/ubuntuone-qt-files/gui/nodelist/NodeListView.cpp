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
#include "NodeListView.h"
#include "MainWindow.h"
#include "Utils.h"
#include "NodeListModel.h"
#include "NodeListController.h"
#include "FileActionsController.h"
#include "DownloadsController.h"
#include <QtCore>
#include <QtQuick>

NodeListView::NodeListView(const QString &path)
    : QQuickView(MainWindow::getInstance()->getWindow()),
      viewPath(path),
      fileAction(new FileActionsController(this)),
      model(new NodeListModel(this))
{
    Utils::setGlobalProperties(this->rootContext());
    this->rootContext()->setContextProperty("nodeListModel", this->model);
    this->setSource(QUrl("qrc:/qml/NodeListView.qml"));

    connect(this->rootObject(), SIGNAL(openFolder(int)), this, SLOT(openFolder(int)));
    connect(this->rootObject(), SIGNAL(openFile(int)), this, SLOT(openFile(int)));

    connect(this->rootObject(), SIGNAL(renameNode(int, QString)), this, SLOT(renameNode(int, QString)));
    connect(this->rootObject(), SIGNAL(deleteNode(int)), this, SLOT(deleteNode(int)));

    connect(this->rootObject(), SIGNAL(publishFile(int, bool)), this, SLOT(publishFile(int, bool)));
    connect(this->rootObject(), SIGNAL(copyPublicLink(int)), this, SLOT(copyPublicLink(int)));

    connect(this->fileAction, SIGNAL(actionFinished()), this, SLOT(refreshView()));
    connect(this->fileAction, SIGNAL(actionFinishedWithError(QString)), this, SLOT(showError(QString)));
}

void NodeListView::setToolBarTitle(const QString &toolBarTitle)
{
    this->rootObject()->setProperty("toolBarTitle", toolBarTitle);
}

void NodeListView::openFolder(int nodeIndex)
{
    QString path = this->model->getNode(nodeIndex)->path;
    MainWindow::getInstance()->push(NodeListController::getInstance()->createView(path));
}

void NodeListView::openFile(int nodeIndex)
{    
    DownloadsController *downloader = new DownloadsController(this);
    downloader->downloadAndOpenNode(this->model, nodeIndex);
}

void NodeListView::renameNode(int nodeIndex, const QString &newName)
{
    // TODO QInputDialog is not working on Android and is it not possible to build your own dialogs in QML, check the
    //      Qt Quick Dialogs documentation, so for the moment... to do
    QString path = this->model->getNode(nodeIndex)->path;
    qDebug() << "Rename file: " << path << " to " << newName;
}

void NodeListView::deleteNode(int nodeIndex)
{
    NodeInfoDTO *node = this->model->getNode(nodeIndex);
    this->fileAction->deleteNode(node);
}

void NodeListView::publishFile(int nodeIndex, bool publish)
{
    NodeInfoDTO *node = this->model->getNode(nodeIndex);
    this->fileAction->publishNode(node, publish);
}

void NodeListView::copyPublicLink(int nodeIndex)
{
    NodeInfoDTO *node = this->model->getNode(nodeIndex);
    this->fileAction->shareLink(node);
}

void NodeListView::refreshView()
{
    NodeListController::getInstance()->refreshView(this->viewPath);
}

void NodeListView::showError(const QString &errorMessage)
{
    // TODO Add an alert or something similar
    qDebug() << "Show error: " << errorMessage;
}
