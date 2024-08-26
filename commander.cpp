#include "commander.h"
#include <QDataStream>
#include <QSerialPort>
#include <QThread>

Q_LOGGING_CATEGORY(commander, "commander", QtDebugMsg)

#pragma pack(push, 1)

// 적외선 신호 정의
// 1. 매직넘버
// 2. LSB first 유무
// 3. nibble 사용 유무 (nibble일 경우 4비트, 아닐경우 8비트)
// 4. 신호 길이
//  4.1. 리드 길이
//  4.2. 신호 1 길이
//  4.3. 신호 0 길이
//  4.4. 종료 길이
// 5. 적외선 데이터
typedef struct
{
    int32_t mark;
    int32_t space;
} bit_us_t;

typedef struct
{
    bit_us_t lead;
    bit_us_t one;
    bit_us_t zero;
    bit_us_t end;
} duration_t;

typedef struct
{
    int16_t magic;
    int8_t lsb;
    int8_t nibble;
    duration_t duration;
} header_t;

#pragma pack(pop)

static const header_t header =
{
    .magic = int16_t(0xCAFE),
    .lsb = 0,
    .nibble = 1,
    .duration =
    {
        .lead = { .mark = 3100, .space = 9800 },
        .one = { .mark = 500, .space = 1500 },
        .zero = { .mark = 500, .space = 550 },
        .end = { .mark = 500, .space = 0 },
    },
};

static QByteArray generatePacket(const QList<char> &data)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << header.magic;
    stream << header.lsb;
    stream << header.nibble;
    stream << header.duration.lead.mark << header.duration.lead.space;
    stream << header.duration.one.mark << header.duration.one.space;
    stream << header.duration.zero.mark << header.duration.zero.space;
    stream << header.duration.end.mark << header.duration.end.space;

    int sum = 0;
    for (const char ch : data)
    {
        sum += ch;
        stream << ch;
    }
    stream << char(sum & 0x0f);

    return packet;
}

static void writeToSerial(const QString &device, const QByteArray &packet)
{
    QSerialPort serial;
    serial.setPortName(device);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);

    if (serial.open(QIODevice::WriteOnly))
    {
        qCDebug(commander).noquote().nospace() << "hex: " << packet.toHex();

        serial.write(packet.toHex());
        serial.write("\n");

        // Qt의 IO는 비동기 방식이므로 버퍼의 데이터를 전부 기록할 때 까지 체크 & 대기하여 객체를 종료해야 함
        do
        {
            serial.waitForBytesWritten();
        }
        while (serial.bytesToWrite());

#if 0
        // 시리얼 출력 메시지 확인용
        QThread::sleep(1);
        serial.waitForReadyRead();
        qCDebug(commander).noquote().nospace() << serial.readAll();
#endif
    }
    else
    {
        qCWarning(commander).noquote().nospace() << "open failed: " << serial.error();
    }
}

Commander::Commander(QObject *parent)
    : QObject{parent}
{
}

Commander::~Commander()
{
}

void Commander::setDevice(const QString &device)
{
    this->device = device;

    qCDebug(commander).noquote().nospace() << "device: " << device;
}

void Commander::fan(bool powerOn, FanSpeed fanSpeed)
{
    qCDebug(commander).noquote().nospace() << "power on: " << powerOn << ", fan speed: " << fanSpeed;

    QList<char> data({ 0x08, 0x08, 0x00 });
    data << char(0x02 + (powerOn ? 0x00 : 0x08));
    data << char(0x03);
    data << char(fanSpeed);

    writeToSerial(device, generatePacket(data));
}

void Commander::cool(bool powerOn, int temperature, FanSpeed fanSpeed)
{
    qCDebug(commander).noquote().nospace() << "power on: " << powerOn << ", temperature: " << temperature << ", fan speed: " << fanSpeed;

    QList<char> data({ 0x08, 0x08, 0x00 });
    data << char(0x00 + (powerOn ? 0x00 : 0x08));
    data << char(temperature - 15);
    data << char(fanSpeed);

    writeToSerial(device, generatePacket(data));
}

void Commander::dry(bool powerOn, FanSpeed fanSpeed)
{
    qCDebug(commander).noquote().nospace() << "power on: " << powerOn << ", fan speed: " << fanSpeed;

    QList<char> data({ 0x08, 0x08, 0x00 });
    data << char(0x01 + (powerOn ? 0x00 : 0x08));
    data << char(0x09);
    data << char(fanSpeed);

    writeToSerial(device, generatePacket(data));
}

void Commander::ai(bool powerOn, Ai ai)
{
    qCDebug(commander).noquote().nospace() << "power on: " << powerOn << ", ai: " << ai;

    QList<char> data({ 0x08, 0x08, 0x00 });
    data << char(0x03 + (powerOn ? 0x00 : 0x08));
    data << char(powerOn ? Commander::AiZero : ai); // 전원을 킬 때는 적당할 때의 값으로만 넘어감
    data << char(0x05);

    writeToSerial(device, generatePacket(data));
}

void Commander::turbo()
{
    qCDebug(commander).noquote().nospace() << "turbo";

    QList<char> data({ 0x08, 0x08, 0x01, 0x00, 0x00, 0x08 });

    writeToSerial(device, generatePacket(data));
}

void Commander::powerOff()
{
    qCDebug(commander).noquote().nospace() << "power off";

    QList<char> data({ 0x08, 0x08, 0x0c, 0x00, 0x00, 0x05 });

    writeToSerial(device, generatePacket(data));
}

void Commander::toggle(bool on, Toggle toggle)
{
    qCDebug(commander).noquote().nospace() << "on: " << on << ", toggle: " << toggle;

    QList<char> data({ 0x08, 0x08, 0x01 });

    switch (toggle)
    {
    case Commander::ToggleEnergySave:
        data.append(0x00);
        data.append(0x00);
        data.append(on ? 0x04 : 0x05);
        break;
    case Commander::ToggleSwingVertical:
        data.append(0x03);
        data.append(0x01);
        data.append(on ? 0x04 : 0x05);
        break;
    case Commander::ToggleComfortAir:
        data.append(0x03);
        data.append(0x00);
        data.append(on ? 0x09 : 0x04);
        break;
    default:
        return;
    }

    writeToSerial(device, generatePacket(data));
}
