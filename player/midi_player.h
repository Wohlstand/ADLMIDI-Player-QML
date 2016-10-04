#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QTimer>

#include "../ADLMIDI/adlmidi.h"

#define MAX_OPLGEN_BUFFER_SIZE  4096

class AdlMidiInterface : public QIODevice
{
    Q_OBJECT

    friend class MidiPlayer;
    int             m_sampleRate;
    ADL_MIDIPlayer *m_player;
    bool            m_isPlaying;
public:

    AdlMidiInterface(int sampleRate, QObject *parent);
    virtual ~AdlMidiInterface();

    void start();
    void stop();

    void openFile(QString file);
    void closeFile();

    void play();
    void pause();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

};

//AdlMidiInterface

class MidiPlayer : public QObject
{
    Q_OBJECT

    AdlMidiInterface*   m_generator;

    /* ********** Audio output stuff ********** */
    //! Buffer for audio data transfering
    QByteArray          m_buffer;
    //! Timer to push audio data
    QTimer              m_pushTimer;

    //! Audio device spec
    QAudioDeviceInfo    m_device;
    //! Audio output interface
    QAudioOutput*       m_audioOutput;
    //! Pointer to audio output interface
    QIODevice*          m_output;//not owned
    //! Audio format preferences
    QAudioFormat        m_format;

private slots:
    /**
     * @brief Waveout playing callback
     */
    void pushTimerExpired();

public:
    explicit MidiPlayer(QObject *parent = 0);
    virtual ~MidiPlayer();

    /*!
     * \brief Initializes audio subsystem and FM generator
     */
    Q_INVOKABLE void initAudio();
    Q_INVOKABLE void open(QString file);
    Q_INVOKABLE void close();
    Q_INVOKABLE void playpause();
    Q_INVOKABLE void stop();
};

#endif // MIDIPLAYER_H
