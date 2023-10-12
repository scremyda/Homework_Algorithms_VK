#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Huffman.h"

typedef unsigned char byte;

class BitReader {
 public:
    explicit BitReader(IInputStream& in_stream) : in(in_stream), pos(8) {}

    byte readBit(byte& value) {
        if (pos == 8) {
            in.Read(value);
            pos = 0;
        }
        ++pos;

        return (value >> (8 - pos)) & 1;
    }

 private:
    IInputStream& in;
    byte pos;
};


class BitWriter {
 public:
    explicit BitWriter(IOutputStream& out_stream) : out(out_stream),
    buf(0), pos(0) {}

    void writeBit(byte value) {
        value &= 1;
        buf = buf | (value << (7 - pos++));
        if (pos == 8) {
            flush();
        }
    }

    void flush() {
        if (pos > 0) {
            out.Write(buf);
            buf = 0;
            pos = 0;
        }
    }

 private:
    IOutputStream& out;
    byte buf;
    byte pos;
};


void CollectData(IInputStream& input, IOutputStream& output,
std::vector<byte>& inputData, std::vector<std::pair<int, byte>>& variablesFrequency) {
    variablesFrequency.resize(256);

    for (int i = 0; i < 256; ++i) {
        variablesFrequency[i].second = byte(i);
    }

    byte x;
    int n = 0;
    while (input.Read(x)) {
        ++n;
        ++variablesFrequency[x].first;
        inputData.push_back(x);
    }

    std::sort(variablesFrequency.begin(), variablesFrequency.end());

    auto it = std::remove_if(variablesFrequency.begin(), variablesFrequency.end(),
    [](const auto& elem) {
        return elem.first == 0;
    });

    variablesFrequency.erase(it, variablesFrequency.end());

    for (int i = 0; i < 4; ++i) {
        output.Write(byte(n >> (24 - (i << 3))));
    }
}

void ConstructEncodingTable(const std::vector<std::pair<int, byte>>& variablesFrequency,
std::unordered_map<byte, byte>& encodingTable) {
    byte newCode = 0;
    for (int i = 0; i < variablesFrequency.size(); ++i) {
        byte symbol = variablesFrequency[i].second;
        encodingTable[symbol] = newCode;
        newCode++;
    }
}

void EncodeData(const std::vector<byte>& inputData,
const std::unordered_map<byte, byte>& encodingTable, IOutputStream& output) {
    std::vector<byte> encodedMessage;
    for (int i = 0; i < inputData.size(); ++i) {
        byte symbol = inputData[i];
        byte newCode = encodingTable.at(symbol);
        encodedMessage.push_back(newCode);
    }

    BitWriter out(output);

    output.Write(encodingTable.size());

    int lastBitPos;
    byte num = encodingTable.size();
    for (int i = 0; i < 8; i++) {
        if ((num >> i) & 1) {
            lastBitPos = i;
        }
    }

    output.Write(byte(lastBitPos));

    for (auto it = encodingTable.begin(); it != encodingTable.end(); ++it) {
        for (int k = 0; k < 8; ++k) {
            out.writeBit(byte(it->first >> (7 - k)) & byte(1));
        }
        for (int j = lastBitPos; j >= 0; --j) {
            out.writeBit(byte(it->second >> j) & byte(1));
        }
    }

    for (size_t i = 0; i < encodedMessage.size(); ++i) {
        for (int j = lastBitPos; j >= 0; j--) {
            out.writeBit(byte(encodedMessage[i] >> j) & byte(1));
        }
    }

    out.flush();
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<std::pair<int, byte>> variablesFrequency;
    std::vector<byte> inputData;
    CollectData(original, compressed, inputData, variablesFrequency);

    std::unordered_map<byte, byte> encodingTable;
    ConstructEncodingTable(variablesFrequency, encodingTable);

    EncodeData(inputData, encodingTable, compressed);
}

void ReadSizeBeforeCompression(IInputStream& compressed, int& sizeBeforeCompression) {
    byte n;
    for (int i = 0; i < 4; i++) {
        compressed.Read(n);
        sizeBeforeCompression += int(n) << (24 - (i << 3));
    }
}

void ReverseEncodingTable(const std::unordered_map<byte, byte>& encodingTable,
std::unordered_map<byte, byte>& reversedEncodingTable) {
    for (const auto& item : encodingTable) {
        reversedEncodingTable[item.second] = item.first;
    }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    int sizeBeforeCompression = 0;
    ReadSizeBeforeCompression(compressed, sizeBeforeCompression);

    byte encodingTableSize;
    compressed.Read(encodingTableSize);

    byte lastBitPos;
    compressed.Read(lastBitPos);

    byte x = 0;
    BitReader in(compressed);
    std::unordered_map<byte, byte> encodingTable;
    for (byte i = 0; i < encodingTableSize; ++i) {
        byte k = 0; for (byte j = 0; j < 8; ++j) {
            k = (k << 1) + in.readBit(x);
        }
        for (byte j = 0; j <= lastBitPos; ++j) {
            encodingTable[k] = (encodingTable[k] << 1) + in.readBit(x);
        }
    }

    std::unordered_map<byte, byte> reversedEncodingTable;
    ReverseEncodingTable(encodingTable, reversedEncodingTable);

    for (int i = 0; i < sizeBeforeCompression; ++i) {
        byte w = 0;
        for (byte j = 0; j <= lastBitPos; ++j) {
            w = in.readBit(x) + (w << 1);
            if (j == lastBitPos) {
                original.Write(reversedEncodingTable.at(w));
            }
        }
    }
}
