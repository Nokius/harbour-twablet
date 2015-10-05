/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
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

#ifndef RETWEETQUERYITEM_H
#define RETWEETQUERYITEM_H

#include "abstractqueryitem.h"

namespace qml
{

class RetweetQueryItem : public AbstractQueryItem
{
    Q_OBJECT
    Q_PROPERTY(QString tweetId READ tweetId WRITE setTweetId NOTIFY tweetIdChanged)
public:
    explicit RetweetQueryItem(QObject *parent = 0);
    DISABLE_COPY_DISABLE_MOVE(RetweetQueryItem);
    QString tweetId() const;
    void setTweetId(QString tweetId);
signals:
    void tweetIdChanged();
private:
    bool isQueryValid() const override final;
    QNetworkReply * createQuery() const override final;
    void handleReply(const QByteArray &reply, QNetworkReply::NetworkError networkError,
                     const QString &errorMessage) override final;
    QString m_tweetId {};
};

}

#endif // RETWEETQUERYITEM_H