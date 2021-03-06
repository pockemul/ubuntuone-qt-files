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
#include "AbstractMessage.h"
#include "LoginInfoDTO.h"
#include "LogoutController.h"
#include <QtCore>
#include <QtNetwork>

AbstractMessage::AbstractMessage(LoginInfoDTO *loginInfo, QObject *parent)
    : QObject(parent),
      loginInfo(loginInfo),
      networkAccessManager(new QNetworkAccessManager(this))
{
    connect(this->networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

AbstractMessage::~AbstractMessage()
{

}

QNetworkReply *AbstractMessage::oauthGetRequest(const QString &url)
{
    QString oauthUrl = this->generateOAuthUrl(OAuth::Http::Get, url);
    return this->networkAccessManager->get(QNetworkRequest(oauthUrl));
}

QNetworkReply *AbstractMessage::oauthPutRequest(const QString &url, const QByteArray &data)
{
    QString oauthUrl = this->generateOAuthUrl(OAuth::Http::Put, url);
    return this->networkAccessManager->put(QNetworkRequest(oauthUrl), data);
}

QNetworkReply *AbstractMessage::oauthDeleteRequest(const QString &url)
{
    QString oauthUrl = this->generateOAuthUrl(OAuth::Http::Delete, url);
    return this->networkAccessManager->deleteResource(QNetworkRequest(oauthUrl));
}

QString AbstractMessage::generateOAuthUrl(const OAuth::Http::RequestType requestType, const QString &url)
{
    OAuth::Consumer consumer(this->loginInfo->consumerKey.toStdString(), this->loginInfo->consumerSecret.toStdString());
    OAuth::Token token(this->loginInfo->token.toStdString(), this->loginInfo->tokenSecret.toStdString());
    OAuth::Client oauth(&consumer, &token);
    QString oauthParameters = QString::fromStdString(oauth.getURLQueryString(requestType, url.toStdString()));
    QString baseUrl = QUrl(url).toString(QUrl::RemoveQuery);
    return baseUrl + "?" + oauthParameters;
}

QString AbstractMessage::toPercentEncoding(const QString &url)
{
    // FIXME Having problems with "?" and ";"
    return QUrl::toPercentEncoding(url, "/()[]'=,:!@$*+;");
}

void AbstractMessage::printJson(const QString &json)
{
    if (json.isEmpty() || json.isNull())
        return;

    int tabLevel = 1;
    QString output = "";
    QString jsonWithLines = json;
    jsonWithLines = jsonWithLines.replace("\n", "")
            .replace("[", "[\n")
            .replace("{", "{\n")
            .replace("]", "]\n")
            .replace("}", "\n}")
            .replace(",", ",\n");
    QStringList lines = jsonWithLines.split("\n");

    for (int n=0; n<lines.count(); n++) {
        QString line = lines.at(n);

        if (line.contains("}") || line.contains("]"))
            tabLevel--;

        for (int t=0; t<tabLevel; t++)
            output += "\t";
        output += " " + line.trimmed();
        if (n != lines.count()-1)
            output += "\n";

        if (line.contains("{") || line.contains("["))
            tabLevel++;
    }

    qDebug() << qPrintable(output);
}

void AbstractMessage::replyFinished(QNetworkReply *reply)
{
    // If the OAuth token is deleted in the web page an AuthenticationRequiredError is received. Show the first screen
    // and delete the database
    if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
        LogoutController::logout();
    }
}
