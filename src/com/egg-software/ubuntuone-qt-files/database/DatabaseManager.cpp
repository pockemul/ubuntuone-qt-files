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
#include "DatabaseManager.h"
#include "VersionDAO.h"
#include "LoginInfoDAO.h"
#include "LoginInfoDTO.h"
#include "NodeListDAO.h"
#include <QtCore>
#include <QtSql>

namespace
{
    static const QString DB_NAME = "u1.db";
}

DatabaseManager *DatabaseManager::instance = NULL;

DatabaseManager *DatabaseManager::getInstance()
{
    if (DatabaseManager::instance == NULL)
        DatabaseManager::instance = new DatabaseManager();
    return DatabaseManager::instance;
}

DatabaseManager::DatabaseManager()
    : db(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")))
{
    QString dbFolder = this->getDatabaseFolder();
    QString dbPath   = this->getDatabsePath();

    qDebug() << "Opening DB at " << dbPath;
    QDir().mkpath(dbFolder);
    this->db->setDatabaseName(dbPath);
    if (!this->db->open())
        qFatal("Can not open the DB");
    qDebug() << "DB correctly openned";

    // Initialize all the tables
    VersionDAO versionDAO(this->db);
    versionDAO.createTableIfNotExists();
    versionDAO.setDatabaseVersion(VERSION);

    LoginInfoDAO loginInfoDAO(this->db);
    loginInfoDAO.createTableIfNotExists();

    NodeListDAO nodeListDAO(this->db);
    nodeListDAO.createTableIfNotExists();
}

void DatabaseManager::closeDatabase()
{
    if (this->instance != NULL) {
        this->db->close();
        delete this->db;
        delete this->instance;
        this->instance = NULL;
    }
}

void DatabaseManager::deleteDatabase()
{
    this->db->close();
    delete this->db;
    QFile::remove(this->getDatabsePath());
    delete this->instance;
    this->instance = NULL;
}

void DatabaseManager::setLoginInfo(LoginInfoDTO *loginInfo)
{
    LoginInfoDAO loginInfoDAO(this->db);
    loginInfoDAO.setLoginInfo(loginInfo);
}

LoginInfoDTO *DatabaseManager::getLoginInfo()
{
    LoginInfoDAO loginInfoDAO(this->db);
    return loginInfoDAO.getLoginInfo();
}

void DatabaseManager::setNodeList(const QString &parentPath, QList<NodeInfoDTO *> *nodeList)
{
    NodeListDAO nodeListDAO(this->db);
    nodeListDAO.setNodeList(parentPath, nodeList);
}

QList<NodeInfoDTO *> *DatabaseManager::getNodeList(const QString &parentPath)
{
    NodeListDAO nodeListDAO(this->db);
    return nodeListDAO.getNodeList(parentPath);
}

QString DatabaseManager::getDatabaseFolder()
{
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
}

QString DatabaseManager::getDatabsePath()
{
    QString dbFolder = this->getDatabaseFolder();
    return dbFolder.endsWith(QDir::separator())
            ? dbFolder + DB_NAME
            : dbFolder + QDir::separator() + DB_NAME;
}
