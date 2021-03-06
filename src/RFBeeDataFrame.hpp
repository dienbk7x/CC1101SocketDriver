/*
 * rfcc1101 - SPI Protocol Driver for TI CC1101 RF communication module.
 *
 * Copyright (C) 2013 Wolfgang Klenk <wolfgang.klenk@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RFBEEDATAFRAME_HPP_
#define RFBEEDATAFRAME_HPP_

#include <stdint.h>
#include <stddef.h>

#include "IDataFrame.hpp"
#include "Protocol.hpp"

/**
 * Decodes/Encodes the data in frames identical to the RFBee firmware.
 * The firmware of the RFBee uses the following data frame format:
 *
 * Byte 0: Number of following bytes (n).
 * Byte 1: Destination address
 * Byte 2: Source address
 * Byte 3 to Byte n: Payload
 * Byte n+1: RSSI (Received Signal Strength Indicator)
 * Byte n+2: LQI (Link Quality Indicator)
 *
 * Payload is limited to 255 bytes.
 */
class RFBeeDataFrame : public IDataFrame {

public:
	static const int MAX_PAYLOAD_BYTES = 256;

	uint8_t buffer[MAX_PAYLOAD_BYTES];
	uint8_t len;

	uint8_t srcAddress;
	uint8_t destAddress;

	/** Received Signal Strength Indicator */
	uint8_t rssi;

	/** Format used to write data frame to the socket */
	int outputFormat;

	/**
	 * Link Quality Indicator. LQI is best used as a relative measurement
	 * of the link quality (a low value indicates a better link than what a
	 * high value does), since the value is dependent on the modulation format.
	 */
	uint8_t lqi;

	RFBeeDataFrame(Protocol* protocol);

	virtual ~RFBeeDataFrame() {};

	/**
	 * This method must only be called if we can be sure that actually a
	 * complete frame exists in the RX FIFO.
	 * There are some basic checks to make sure the frame in the RXFIFO is
	 * valid and not just random garbage. However, this may NOT be sufficient
	 * in some cases.
	 *
	 * Returns 0 if a data frame could be read from the RX FIFO.
	 * Returns -1 on error.
	 */
	virtual int receive();

	virtual int transmit();


	/**
	 * Writes the data frame to a file descriptor.
	 */
	virtual void writeToSocket(int fd);
};


#endif /* RFBEEDATAFRAME_HPP__ */
