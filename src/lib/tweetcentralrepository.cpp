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

#include "tweetcentralrepository.h"
#include "hometimelinequeryhandler.h"
#include "mentionstimelinequeryhandler.h"
#include "private/twitterqueryutil.h"
#include <QtCore/QLoggingCategory>
#include <QtNetwork/QNetworkReply>

TweetCentralRepository::TweetCentralRepository()
    : m_network{new QNetworkAccessManager()}
{
}

void TweetCentralRepository::query(const Account &account, const Query &query,
                                          TweetRepository &repository)
{
    // Create the request
    IQueryHandler * handler = getQueryHandler(query);
    if (handler == nullptr) {
        return;
    }

    QString path {};
    std::map<QString, QString> parameters {};
    handler->createRequest(path, parameters);
    QNetworkReply *reply = m_network->get(TwitterQueryUtil::createGetRequest(path, parameters, account));
    repository.start();

    connect(reply, &QNetworkReply::finished, this, [this, account, query, handler, reply, &repository]() {
        QObjectPtr<QNetworkReply> replyPtr {reply};

        if (replyPtr->error() != QNetworkReply::NoError) {
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Error happened during request for layout" << account.userId() << query.type();
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Error code:" << reply->error();
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Error message (Qt):" << reply->errorString();
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Error message (Twitter):" << reply->readAll();
            repository.error(tr("Network error. Please try again later."));
            return;
        }

        std::vector<Tweet> items {};
        QString errorMessage {};
        IQueryHandler::Placement placement {IQueryHandler::Discard};
        if (!handler->treatReply(reply->readAll(), items, errorMessage, placement)) {
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Error happened during request for layout" << account.userId() << query.type();
            qCWarning(QLoggingCategory("twitter-tweet-central-repository")) << "Parsing error: " << errorMessage;
            repository.error(tr("Internal error"));
            return;
        } else {
            for (const Tweet &tweet : items) {
                m_data.emplace(tweet.id(), tweet);
            }
            qDebug(QLoggingCategory("twitter-tweet-central-repository")) << "New data available for layout" << account.userId() << query.type() << ". Count:" << items.size();
            switch (placement) {
            case IQueryHandler::Append:
                repository.append(items);
                break;
            case IQueryHandler::Prepend:
                repository.prepend(items);
                break;
            case IQueryHandler::Discard:
                break;
            }
            repository.finish();
        }
    });
}

IQueryHandler * TweetCentralRepository::getQueryHandler(const Query &query)
{
    auto it = m_queries.find(query);
    if (it != std::end(m_queries)) {
        return it->second.get();
    }

    switch (query.type()) {
    case Query::Home:
    {
        std::unique_ptr<IQueryHandler> handler {new HomeTimelineQueryHandler()};
        return m_queries.emplace(query, std::move(handler)).first->second.get();
        break;
    }
    case Query::Mentions:
    {
        std::unique_ptr<IQueryHandler> handler {new MentionsTimelineQueryHandler()};
        return m_queries.emplace(query, std::move(handler)).first->second.get();
        break;
    }
    default:
        return nullptr;
        break;
    }
}


bool TweetCentralRepository::QueryComparator::operator()(const Query &first,
                                                                       const Query &second) const
{
    if (first.type() < second.type()) {
        return true;
    }
    if (first.type() > second.type()) {
        return false;
    }
    return first.arguments() < second.arguments();
}