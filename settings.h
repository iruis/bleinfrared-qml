#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QQmlEngine>
#include <QLoggingCategory>

#include "commander.h"

class Settings : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged FINAL)
    Q_PROPERTY(Commander::Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(Commander::FanSpeed fanSpeed READ fanSpeed WRITE setFanSpeed NOTIFY fanSpeedChanged FINAL)
    Q_PROPERTY(Commander::Ai ai READ ai WRITE setAi NOTIFY aiChanged FINAL)
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged FINAL)
    Q_PROPERTY(Commander::Toggle toggle READ toggle WRITE setToggle NOTIFY toggleChanged FINAL)

public:
    explicit Settings(QObject *parent = nullptr);
    virtual ~Settings();

    Q_INVOKABLE QString device() const;
    Q_INVOKABLE void setDevice(const QString &device);

    Q_INVOKABLE Commander::Mode mode() const;
    Q_INVOKABLE void setMode(Commander::Mode mode);

    Q_INVOKABLE Commander::FanSpeed fanSpeed() const;
    Q_INVOKABLE void setFanSpeed(Commander::FanSpeed fanSpeed);

    Q_INVOKABLE Commander::Ai ai() const;
    Q_INVOKABLE void setAi(Commander::Ai ai);

    Q_INVOKABLE int temperature() const;
    Q_INVOKABLE void setTemperature(int temperature);

    Q_INVOKABLE Commander::Toggle toggle() const;
    Q_INVOKABLE void setToggle(Commander::Toggle toggle);

signals:
    void deviceChanged(const QString &device);
    void modeChanged(Commander::Mode mode);
    void fanSpeedChanged(Commander::FanSpeed fanSpeed);
    void aiChanged(Commander::Ai ai);
    void temperatureChanged(int temperature);
    void toggleChanged(Commander::Toggle toggle);

private:
    QString filename;
};

Q_DECLARE_LOGGING_CATEGORY(settings)

#endif // SETTINGS_H
