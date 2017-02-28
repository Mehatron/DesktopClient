#ifndef _ANALOG_CLOCK_H_
#define _ANALOG_CLOCK_H_

#include <QWidget>

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogClock(QWidget *parent = 0);
    virtual ~AnalogClock(void);

    virtual QSize sizeHint(void) const override;

    /*
     * Getters and setters
     */

    inline bool isShowSeconds(void) const { return m_showSeconds; };
    inline void setShowSeconds(bool show) { m_showSeconds = show; };

    inline bool isUseAntialiasing(void) const { return m_useAntialiasing; };
    inline void setUseAntialiasing(bool use) { m_useAntialiasing = use; };

    inline const QColor &colorHours(void) const { return m_colorHours; };
    inline void colorHours(const QColor &color) { m_colorHours = color; };
    
    inline const QColor &colorMinutes(void) const { return m_colorMinutes; };
    inline void colorMinutes(const QColor &color) { m_colorMinutes = color; };

    inline const QColor &colorHoursPointer(void) const { return m_colorHoursPointer; };
    inline void colorHoursPointer(const QColor &color) { m_colorHoursPointer = color; };
    
    inline const QColor &colorMinutesPointer(void) const { return m_colorMinutesPointer; };
    inline void colorMinutesPointer(const QColor &color) { m_colorMinutesPointer = color; };
    
    inline const QColor &colorSecondsPointer(void) const { return m_colorSecondsPointer; };
    inline void colorSecondsPointer(const QColor &color) { m_colorSecondsPointer = color; };

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void anglesFromTime(const QTime &time, double *hours, double *minutes, double *seconds = nullptr);
    void drawPointers(QPainter &painter);

    bool m_showSeconds;
    bool m_useAntialiasing;

    int m_r;
    QPoint m_center;

    QColor m_colorHours;
    QColor m_colorMinutes;

    QColor m_colorHoursPointer;
    QColor m_colorMinutesPointer;
    QColor m_colorSecondsPointer;

private slots:
    void redraw(void);
};

#endif // _ANALOG_CLOCK_H_

