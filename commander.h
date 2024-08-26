#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>

class Commander : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int temperatureMin READ temperatureMin CONSTANT)
    Q_PROPERTY(int temperatureMax READ temperatureMax CONSTANT)

    Q_PROPERTY(QString device WRITE setDevice FINAL)

public:
    int temperatureMin() { return 18; }
    int temperatureMax() { return 30; }

    enum Mode
    {
        ModeFan = 0,
        ModeCool,
        ModeDry,
        ModeAi,
        ModeToggle,
    };
    Q_ENUM(Mode)

    enum FanSpeed
    {
        FanSpeedLow = 0,
        FanSpeedMedium = 2,
        FanSpeedHigh = 4,
        FanSpeedNature = 5,
    };
    Q_ENUM(FanSpeed)

    enum Ai
    {
        AiN2 = 0,
        AiN1,
        AiZero,
        AiP1,
        AiP2
    };
    Q_ENUM(Ai)

    enum Toggle
    {
        ToggleEnergySave,
        ToggleSwingVertical,
        ToggleComfortAir,
    };
    Q_ENUM(Toggle)

    explicit Commander(QObject *parent = nullptr);
    virtual ~Commander();

    Q_INVOKABLE void setDevice(const QString &device);

    Q_INVOKABLE void fan(bool powerOn, FanSpeed fanSpeed);
    Q_INVOKABLE void cool(bool powerOn, int temperature, FanSpeed fanSpeed);
    Q_INVOKABLE void dry(bool powerOn, FanSpeed fanSpeed);
    Q_INVOKABLE void ai(bool powerOn, Ai ai);
    Q_INVOKABLE void turbo();
    Q_INVOKABLE void powerOff();
    Q_INVOKABLE void toggle(bool on, Toggle toggle);

private:
    QString device;
};

Q_DECLARE_LOGGING_CATEGORY(commander)

#endif // COMMANDER_H
