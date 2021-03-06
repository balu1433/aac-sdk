/*
 * Copyright 2017-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef AACE_AUDIO_GSTREAMER_GSTMEDIAPLAYER_H_
#define AACE_AUDIO_GSTREAMER_GSTMEDIAPLAYER_H_

#include <memory>
#include <thread>
#include <atomic>
#include <gst/gst.h>
#include <AACE/Alexa/MediaPlayer.h>
#include <AACE/Alexa/Speaker.h>

#include "GstPlayer.h"

namespace aace {
namespace audio {

static constexpr uint8_t DEFAULT_VOLUME = 100;

class GstMediaPlayer :
	public alexa::MediaPlayer,
	public alexa::Speaker,
	public Context::Listener,
	public std::enable_shared_from_this<GstMediaPlayer>
{
public:
	static std::shared_ptr<GstMediaPlayer> create(
		const std::string &name,
		const std::shared_ptr<GstPlayer> &player);

	GstMediaPlayer(
		const std::string &name,
		const std::shared_ptr<GstPlayer> &player);

	~GstMediaPlayer() = default;

	// Context::Listener interface
	void onStreamStart() override;
	void onStreamEnd() override;
	void onStreamError() override;
	void onStateChanged(GstState state) override;

	// MediaPlayer interface
	bool prepare() override;
	bool prepare(const std::string &url) override;
	bool play() override;
	bool stop() override;
	bool pause() override;
	bool resume() override;
	int64_t getPosition() override;
	bool setPosition(int64_t position) override;

	// Speaker interface
	bool setVolume(int8_t volume) override;
	bool adjustVolume(int8_t delta) override;
	bool setMute(bool mute) override;
	int8_t getVolume() override;
	bool isMuted() override;

private:
	bool init();
	void setURI(const std::string &uri);

	const std::string TAG;
	const std::string m_name;

	std::shared_ptr<GstPlayer> m_player;

	std::atomic<uint64_t> m_pendingPosition;
	std::string m_currentURI;
	uint8_t m_currentVolume = DEFAULT_VOLUME;
	bool m_currentMute = false;
	int64_t m_currentPosition = 0;
	std::atomic<bool> m_stopRequested;
};

}
}

#endif //AACE_AUDIO_GSTREAMER_GSTMEDIAPLAYER_H_