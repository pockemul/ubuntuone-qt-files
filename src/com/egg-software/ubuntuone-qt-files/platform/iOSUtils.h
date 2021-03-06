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
#ifndef IOSUTILS_H
#define IOSUTILS_H

#include <QtCore/QString>
#ifdef Q_OS_IOS
#include "PlatformUtils.h"

/**
 * The iOSUtils class brings easy access to the iOS specified functionality.
 */
class iOSUtils : public PlatformUtils
{

public:

    iOSUtils(QObject *parent = 0);

    virtual void openFile(const QString &filePath);
    virtual void shareLink(const QString &link);

};

#endif // Q_OS_IOS
#endif // IOSUTILS_H
