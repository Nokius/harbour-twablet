/*
 * Copyright (C) 2015 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.twablet 1.0

Item {
    id: container
    property string source
    property alias image: image
    property alias progress: image.progress
    property alias status: image.status

    onSourceChanged: image.setSource()

    Connections {
        target: NetworkMonitor
        onOnlineChanged: image.setSource()
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: Theme.secondaryHighlightColor
        opacity: 0.5

        Behavior on opacity {
            FadeAnimation {}
        }
    }

    Image {
        id: image
        function setSource() {
            if (!NetworkMonitor.online) {
                if (image.status != Image.Ready) {
                    image.source = ""
                }
            } else {
                if (image.status == Image.Null) {
                    image.source = container.source
                }
            }
        }

        anchors.fill: parent
        smooth: true
        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        clip: true
        opacity: 0
        sourceSize.width: width
        sourceSize.height: height

        states: State {
            name: "visible"; when: image.status === Image.Ready
            PropertyChanges {
                target: image
                opacity: 1
            }
            PropertyChanges {
                target: background
                opacity: 0
            }
        }

        Behavior on opacity {
            FadeAnimation {}
        }
    }

    Image {
        anchors.centerIn: parent
        width: Theme.iconSizeSmall
        height: Theme.iconSizeSmall
        source: "image://theme/icon-s-high-importance?" + Theme.highlightColor
        visible: image.status === Image.Error
    }
}

