/* Copyright 2013 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "logic/net/NetJob.h"

#include <QUrl>

class UpdateChecker : public QObject
{
	Q_OBJECT

public:
	UpdateChecker();
	void checkForUpdate();

	void setCurrentChannel(const QString &channel) { m_currentChannel = channel; }
	void setChannelListUrl(const QString &url) { m_channelListUrl = url; }

	/*!
	 * Causes the update checker to download the channel list from the URL specified in config.h (generated by CMake).
	 * If this isn't called before checkForUpdate(), it will automatically be called.
	 */
	void updateChanList();

	/*!
	 * An entry in the channel list.
	 */
	struct ChannelListEntry
	{
		QString id;
		QString name;
		QString description;
		QString url;
	};

	/*!
	 * Returns a the current channel list.
	 * If the channel list hasn't been loaded, this list will be empty.
	 */
	QList<ChannelListEntry> getChannelList() const;

	/*!
	 * Returns true if the channel list is empty.
	 */
	bool hasChannels() const;

signals:
	//! Signal emitted when an update is available. Passes the URL for the repo and the ID and name for the version.
	void updateAvailable(QString repoUrl, QString versionName, int versionId);

	//! Signal emitted when the channel list finishes loading or fails to load.
	void channelListLoaded();

private slots:
	void updateCheckFinished();
	void updateCheckFailed();

	void chanListDownloadFinished();
	void chanListDownloadFailed();

private:
	friend class UpdateCheckerTest;

	NetJobPtr indexJob;
	NetJobPtr chanListJob;

	QString m_repoUrl;

	QString m_channelListUrl;
	QString m_currentChannel;

	QList<ChannelListEntry> m_channels;

	/*!
	 * True while the system is checking for updates.
	 * If checkForUpdate is called while this is true, it will be ignored.
	 */
	bool m_updateChecking;

	/*!
	 * True if the channel list has loaded.
	 * If this is false, trying to check for updates will call updateChanList first.
	 */
	bool m_chanListLoaded;

	/*!
	 * Set to true while the channel list is currently loading.
	 */
	bool m_chanListLoading;

	/*!
	 * Set to true when checkForUpdate is called while the channel list isn't loaded.
	 * When the channel list finishes loading, if this is true, the update checker will check for updates.
	 */
	bool m_checkUpdateWaiting;
};
