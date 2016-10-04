#include "midi_player.h"
#include <QtDebug>
#include <QUrl>

AdlMidiInterface::AdlMidiInterface(int sampleRate, QObject *parent):
      QIODevice(parent)
{
    m_sampleRate = sampleRate;
    m_isPlaying = false;
    m_player = nullptr;
}

AdlMidiInterface::~AdlMidiInterface()
{
    m_isPlaying = false;
    stop();
}

void AdlMidiInterface::start()
{
    open(QIODevice::ReadOnly);
}

void AdlMidiInterface::stop()
{
    m_isPlaying = false;
    closeFile();
    close();
}

void AdlMidiInterface::openFile(QString file)
{
    closeFile();
    m_player = adl_init(m_sampleRate);
    m_isPlaying = false;
    adl_setBank(m_player, 62);
    adl_setHTremolo(m_player, 0);
    adl_setHVibrato(m_player, 0);
    adl_openFile(m_player, file.toUtf8().data());
    qDebug() << "Opened file" << file;
}

void AdlMidiInterface::closeFile()
{
    if(m_player)
        adl_close(m_player);
    m_player = nullptr;
}

void AdlMidiInterface::play()
{
    m_isPlaying = true;
}

qint64 AdlMidiInterface::readData(char *data, qint64 maxlen)
{
    short* _out = (short*)data;

    if(!m_isPlaying || !m_player)
        return 0;

    return adl_play(m_player, maxlen/2, _out)*2;
}

qint64 AdlMidiInterface::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

qint64 AdlMidiInterface::bytesAvailable() const
{
    if(!m_isPlaying || !m_player)
        return 0;
    return 409600;// + QIODevice::bytesAvailable();
}

void MidiPlayer::initAudio()
{
    /*INIT AUDIO!!!*/
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    connect(&m_pushTimer, SIGNAL(timeout()), SLOT(pushTimerExpired()));

    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(m_device);
    if (!info.isFormatSupported(m_format))
    {
        //qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_audioOutput->setVolume(1.0);
    m_generator = new AdlMidiInterface(44100, this);

    m_generator->start();
    m_audioOutput->setBufferSize(20480);
    //m_audioOutput->start(m_generator);
    //m_output = m_audioOutput->start();
    //m_pushTimer.start(4);
}

void MidiPlayer::open(QString file)
{
    close();
    m_generator->openFile(QUrl(file).toLocalFile());
}

void MidiPlayer::close()
{
    m_audioOutput->stop();
}

void MidiPlayer::playpause()
{
    qDebug() << "Attempt to play/pause";

    if(m_audioOutput->state() != QAudio::StoppedState)
        m_audioOutput->stop();

    m_generator->m_isPlaying = !m_generator->m_isPlaying;

    if(m_generator->m_isPlaying)
        m_audioOutput->start(m_generator);
}

void MidiPlayer::stop()
{
    m_generator->m_isPlaying = false;
    m_audioOutput->stop();
}

void MidiPlayer::pushTimerExpired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState)
    {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks) {
           const qint64 len = m_generator->read(m_buffer.data(), m_audioOutput->periodSize());
           if (len)
               m_output->write(m_buffer.data(), len);
           if (len != m_audioOutput->periodSize())
               break;
           --chunks;
        }
    }
}

MidiPlayer::MidiPlayer(QObject *parent) : QObject(parent)
{
    initAudio();
}

MidiPlayer::~MidiPlayer()
{
    m_pushTimer.stop();
    m_generator->m_isPlaying=false;
    m_generator->stop();
    delete m_generator;
}

