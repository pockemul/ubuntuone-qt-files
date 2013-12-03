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
#include "AndroidUtils.h"
#include <QtCore>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>

void AndroidUtils::openFile(const QString &filePath)
{
    QAndroidJniObject path = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/ubuntuqtfiles/AndroidUtils",
            "openFile", "(Ljava/lang/String;)V", path.object<jstring>());
}

QString AndroidUtils::getSDCardPath()
{
    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment",
            "getExternalStorageDirectory", "()Ljava/io/File;");
    QAndroidJniObject mediaPath = mediaDir.callObjectMethod("getAbsolutePath", "()Ljava/lang/String;");
    return mediaPath.toString();
}

#endif // Q_OS_ANDROID
