#ifndef ZOOMPANVIEW_H
#define ZOOMPANVIEW_H

#include <QGraphicsView>
#include <QtGui>

class QGestureEvent;
class QPinchGesture;

class ZoomPanView : public QGraphicsView
{
    public:
        ZoomPanView(QWidget* parent = nullptr);

        /**
         * @brief ZoomPanView::wheelEvent Zoom in or out depending on direction of mouse scroll
         * @param event Wheel scroll event
         */
        void wheelEvent(QWheelEvent* event);

        /**
         * @brief ZOOM_SCALE_FACTOR
         * Factor by which to rescale view
         */
        const double ZOOM_SCALE_FACTOR = 1.15;

        void grabGestures();

    private:

        /**
         * @brief zoom Perform a zoom on the view by the specified scaling factor.
         * @param scaleFactor
         */
        void zoom(double scaleFactor);

        bool gestureEvent(QGestureEvent *event);
        void pinchTriggered(QPinchGesture*);

        double pinchScaleFactor = 1;

    protected:

        bool event(QEvent *event) override;
};

#endif // ZOOMPANVIEW_H
