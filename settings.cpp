#include "settings.h"
#include <QGuiApplication>
#include <QSettings>
#include <QDir>

constexpr char GROUP_COMMON[] = "common";
constexpr char GROUP_AC[] = "air-conditioner";
constexpr char KEY_COMMON_DEVICE[] = "device";
constexpr char KEY_AC_MODE[] = "mode";
constexpr char KEY_AC_FAN_SPEED[] = "fan_speed";
constexpr char KEY_AC_AI[] = "ai";
constexpr char KEY_AC_TEMPERATURE[] = "temperature";
constexpr char KEY_AC_TOGGLE[] = "toggle";

#ifdef Q_OS_WIN
constexpr char DEFAULT_DEVICE[] = "COM1";
#else
constexpr char DEFAULT_DEVICE[] = "/dev/ACM0";
#endif

constexpr Commander::Mode DEFAULT_MODE = Commander::ModeCool;
constexpr Commander::FanSpeed DEFAULT_FAN_SPEED = Commander::FanSpeedMedium;
constexpr Commander::Ai DEFAULT_AI = Commander::AiZero;
constexpr int DEFAULT_TEMPERATURE = 27;
constexpr Commander::Toggle DEFAULT_TOGGLE = Commander::ToggleEnergySave;

Q_LOGGING_CATEGORY(settings, "settings", QtDebugMsg)

static void initialize(const QString &filename)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_COMMON);
    if (!settings.contains(KEY_COMMON_DEVICE))
    {
        settings.setValue(KEY_COMMON_DEVICE, DEFAULT_DEVICE);
    }
    settings.endGroup();

    settings.beginGroup(GROUP_AC);
    if (!settings.contains(KEY_AC_MODE))
    {
        settings.setValue(KEY_AC_MODE, int(DEFAULT_MODE));
    }
    if (!settings.contains(KEY_AC_FAN_SPEED))
    {
        settings.setValue(KEY_AC_FAN_SPEED, int(DEFAULT_FAN_SPEED));
    }
    if (!settings.contains(KEY_AC_AI))
    {
        settings.setValue(KEY_AC_AI, int(DEFAULT_AI));
    }
    if (!settings.contains(KEY_AC_TEMPERATURE))
    {
        settings.setValue(KEY_AC_TEMPERATURE, DEFAULT_TEMPERATURE);
    }
    if (!settings.contains(KEY_AC_TOGGLE))
    {
        settings.setValue(KEY_AC_TOGGLE, int(DEFAULT_TOGGLE));
    }
    settings.endGroup();
}

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    QDir dir(QGuiApplication::applicationDirPath());

    filename = dir.filePath("settings.ini");

    initialize(filename);
}

Settings::~Settings()
{
}

QString Settings::device() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_COMMON);
    QString device = settings.value(KEY_COMMON_DEVICE, DEFAULT_DEVICE).toString();
    settings.endGroup();

    return device;
}

void Settings::setDevice(const QString &device)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_COMMON);
    settings.setValue(KEY_COMMON_DEVICE, device);
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "device: " << device;

    emit deviceChanged(device);
}

Commander::Mode Settings::mode() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    Commander::Mode mode = Commander::Mode(settings.value(KEY_AC_MODE, DEFAULT_MODE).toInt());
    settings.endGroup();

    return mode;
}

void Settings::setMode(Commander::Mode mode)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    settings.setValue(KEY_AC_MODE, int(mode));
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "mode: " << mode;

    emit modeChanged(mode);
}

Commander::FanSpeed Settings::fanSpeed() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    Commander::FanSpeed mode = Commander::FanSpeed(settings.value(KEY_AC_FAN_SPEED, DEFAULT_FAN_SPEED).toInt());
    settings.endGroup();

    return mode;
}

void Settings::setFanSpeed(Commander::FanSpeed fanSpeed)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    settings.setValue(KEY_AC_FAN_SPEED, int(fanSpeed));
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "fanSpeed: " << fanSpeed;

    emit fanSpeedChanged(fanSpeed);
}

Commander::Ai Settings::ai() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    Commander::Ai ai = Commander::Ai(settings.value(KEY_AC_AI, DEFAULT_AI).toInt());
    settings.endGroup();

    return ai;
}

void Settings::setAi(Commander::Ai ai)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    settings.setValue(KEY_AC_AI, int(ai));
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "ai: " << ai;

    emit aiChanged(ai);
}

int Settings::temperature() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    int temperature = settings.value(KEY_AC_TEMPERATURE, DEFAULT_TEMPERATURE).toInt();
    settings.endGroup();

    return temperature;
}

void Settings::setTemperature(int temperature)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    settings.setValue(KEY_AC_TEMPERATURE, temperature);
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "temperature: " << temperature;

    emit temperatureChanged(temperature);
}

Commander::Toggle Settings::toggle() const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    Commander::Toggle toggle = Commander::Toggle(settings.value(KEY_AC_TOGGLE, DEFAULT_TOGGLE).toInt());
    settings.endGroup();

    return toggle;
}

void Settings::setToggle(Commander::Toggle toggle)
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.beginGroup(GROUP_AC);
    settings.setValue(KEY_AC_TOGGLE, toggle);
    settings.endGroup();

    qCDebug(::settings).noquote().nospace() << "toggle: " << toggle;

    emit toggleChanged(toggle);
}
