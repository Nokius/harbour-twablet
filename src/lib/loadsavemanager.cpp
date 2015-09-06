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

#include "loadsavemanager.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStandardPaths>
#include <QtCore/QLoggingCategory>

LoadSaveManager::LoadSaveManager()
    : m_configFilePath{configFilePath()}
{
}

QString LoadSaveManager::configFilePath()
{
    QDir dir {QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)};
    if (!dir.exists()) {
        if (!QDir::root().mkpath(dir.absolutePath())) {
            qCWarning(QLoggingCategory("load-save-manager")) << "Failed to create root config dir"
                                                             << dir.absolutePath();
            return QString();
        }
    }
    const QString &appName {QCoreApplication::instance()->applicationName()};
    if (!dir.exists(appName)) {
        if (!dir.mkdir(appName)) {
            qCWarning(QLoggingCategory("load-save-manager")) << "Failed to create config dir"
                                                             << dir.absoluteFilePath(appName);
            return QString();
        }
    }
    if(!dir.cd(appName)) {
        qCWarning(QLoggingCategory("load-save-manager")) << "Failed to enter in config dir"
                                                         << dir.absoluteFilePath(appName);
        return QString();
    }
    return dir.absoluteFilePath(QLatin1String("config.json"));
}

bool LoadSaveManager::load(ILoadSave &loadSave)
{
    QFile file {m_configFilePath};
    if (!file.exists()) {
        qCDebug(QLoggingCategory("load-save-manager")) << "Failed to find config file"
                                                       << m_configFilePath;
        return true; // No file, so no config to load
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(QLoggingCategory("load-save-manager")) << "Failed to open config file"
                                                         << m_configFilePath;
        return false;
    }
    QJsonParseError error {-1, QJsonParseError::NoError};
    QJsonDocument document {QJsonDocument::fromJson(file.readAll(), &error)};
    file.close();

    if (error.error != QJsonParseError::NoError) {
        qCWarning(QLoggingCategory("load-save-manager")) << "Failed to parse configuration file";
        qCWarning(QLoggingCategory("load-save-manager")) << "Error" << error.errorString();
        return false;
    }

    loadSave.load(document.object());
    return true;
}

bool LoadSaveManager::save(const ILoadSave &loadSave)
{
    QFile file {m_configFilePath};
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCWarning(QLoggingCategory("load-save-manager")) << "Failed to open config file"
                                                         << m_configFilePath;
        return false;
    }
    QJsonParseError error {-1, QJsonParseError::NoError};
    QJsonDocument document {QJsonDocument::fromJson(file.readAll(), &error)};

    QJsonObject config {document.object()};
    loadSave.save(config);
    document.setObject(config);
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool LoadSaveManager::clear()
{
    QFile configFile {m_configFilePath};
    if (!configFile.exists()) {
        return true;
    }
    if (!configFile.remove()) {
        qCWarning(QLoggingCategory("load-save-manager")) << "Failed to remove config file"
                                                         << m_configFilePath;
        return false;
    }
    return true;
}
