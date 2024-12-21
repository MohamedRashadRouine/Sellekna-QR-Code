/* 
 * QR Code generator library (C++)
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 */

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>


namespace qrcodegen {

/* 
 * A QR Code symbol, which is a type of two-dimension barcode.
 * Invented by Denso Wave and described in the ISO/IEC 18004 standard.
 * Instances of this class represent an immutable square grid of dark and light cells.
 * The class provides static factory functions to create a QR Code from text or binary data.
 * The class covers the QR Code Model 2 specification, supporting all versions (sizes)
 * from 1 to 40, all 4 error correction levels, and 4 character encoding modes.
 */
class QrCode final {
	
	/*---- Public helper enumeration ----*/
	
	/* 
	 * The error correction level in a QR Code symbol.
	 */
	public: enum class Ecc {
		LOW = 0,  // The QR Code can tolerate about  7% erroneous codewords
		MEDIUM,   // The QR Code can tolerate about 15% erroneous codewords
		QUARTILE, // The QR Code can tolerate about 25% erroneous codewords
		HIGH,     // The QR Code can tolerate about 30% erroneous codewords
	};
	
	
	// Returns a QR Code representing the given Unicode text string at the given error correction level.
	public: static QrCode encodeText(const char *text, Ecc ecl) {
		std::vector<QrSegment> segs = QrSegment::makeSegments(text);
		return encodeSegments(segs, ecl);
	}
	
	
	// Returns this QR Code's version number, in the range [1, 40].
	public: int getVersion() const {
		return version;
	}
	
	
	// Returns this QR Code's size, in the range [21, 177].
	public: int getSize() const {
		return size;
	}
	
	
	// Returns the color of the module (pixel) at the given coordinates, which is false
	// for light or true for dark. The top left corner has the coordinates (x=0, y=0).
	// If the given coordinates are out of bounds, then false (light) is returned.
	public: bool getModule(int x, int y) const {
		return 0 <= x && x < size && 0 <= y && y < size && module(x, y);
	}
	
	
	/*---- Private fields ----*/
	
	private: int version;
	private: int size;
	private: int mask;
	private: std::vector<std::vector<bool>> modules;
	private: std::vector<std::vector<bool>> isFunction;
	
	
	/*---- Private helper methods ----*/
	
	private: bool module(int x, int y) const {
		return modules.at(y).at(x);
	}
	
};



/* 
 * A segment of character/binary/control data in a QR Code symbol.
 * Instances of this class are immutable.
 * The mid-level way to create a segment is to take the payload data
 * and call a static factory function such as QrSegment::makeNumeric().
 * The low-level way to create a segment is to custom-make the bit buffer
 * and call the QrSegment() constructor with appropriate values.
 * This segment class imposes no length restrictions, but QR Codes have restrictions.
 * Even in the most favorable conditions, a QR Code can only hold 7089 characters of data.
 * Any segment longer than this is meaningless for the purpose of generating QR Codes.
 */
class QrSegment final {
	
	/*---- Static factory functions (mid level) ----*/
	
	// Returns a segment representing the given text string encoded in alphanumeric mode.
	// The characters allowed are: 0 to 9, A to Z (uppercase only), space,
	// dollar, percent, asterisk, plus, hyphen, period, slash, colon.
	public: static std::vector<QrSegment> makeSegments(const char *text) {
		std::vector<QrSegment> result;
		if (*text != '\0')
			result.push_back(makeBytes(reinterpret_cast<const std::vector<std::uint8_t>&>(
				std::string(text))));
		return result;
	}
	
	
	// Returns a segment representing the given binary data encoded in byte mode.
	public: static QrSegment makeBytes(const std::vector<std::uint8_t> &data) {
		return QrSegment(Mode::BYTE, data.size(), data, data.size() * 8);
	}
	
	
	/*---- Instance fields ----*/
	
	// The mode indicator of this segment.
	private: Mode mode;
	
	// The length of this segment's unencoded data. Measured in characters for
	// numeric/alphanumeric/kanji mode, bytes for byte mode, and 0 for ECI mode.
	// Always zero or positive. Not the same as the data's bit length.
	private: int numChars;
	
	// The data bits of this segment. Accessed through getData().
	private: std::vector<bool> data;
	
	// The number of valid data bits used in the vector. Requires 0 <= bitLength <= data.size().
	// The character count (numChars) must agree with the mode and these bits.
	private: int bitLength;
	
	
	/*---- Constructor (low level) ----*/
	
	// Creates a new QR Code segment with the given attributes and data.
	// The character count (numChars) must agree with the mode and the bit buffer length,
	// but the constraint isn't checked. The given bit buffer is copied and stored.
	public: QrSegment(Mode md, int numCh, const std::vector<std::uint8_t> &data, int bitLen) :
			mode(md),
			numChars(numCh),
			data(data.size() * 8),
			bitLength(bitLen) {
		for (int i = 0; i < bitLen; i++)
			this->data.at(i) = ((data.at(i >> 3) >> (7 - (i & 7))) & 1) != 0;
	}
	
	
	/*---- Method ----*/
	
	// Returns the data bits of this segment.
	public: const std::vector<bool> &getData() const {
		return data;
	}
	
	
	/*---- Constants ----*/
	
	// Describes precisely all strings that are encodable in numeric mode.
	private: static const std::int8_t NUMERIC_REGEX[];
	
	// Describes precisely all strings that are encodable in alphanumeric mode.
	private: static const std::int8_t ALPHANUMERIC_REGEX[];
	
	
	/*---- Public helper enumeration ----*/
	
	/* 
	 * Describes how a segment's data bits are interpreted.
	 */
	public: enum class Mode {
		NUMERIC,
		ALPHANUMERIC,
		BYTE,
		KANJI,
		ECI,
	};
	
};

}