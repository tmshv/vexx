#include "QPainter"
#include "QBitmap"
#include "nodepiemenu.h"
#include "math.h"

#define TWOPI 3.14159265 * 2

nodePieMenu::nodePieMenu(const QString &text, QWidget *parent) : QtPieMenu(text, parent)
{

}

void nodePieMenu::paintEvent(QPaintEvent *)
{
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRect adjustedRect = pix.rect().adjusted(0, 0, -1, -1);

    // Special case: 0 or 1 items
    if (items.size() < 2) {
        if (hItem == 0 && items.at(0)->isEnabled())
            p.setBrush(palette().highlight());
        else
            p.setBrush(palette().background());

        if (shaded)
            p.setBrush(QBrush(p.brush().color().dark(125)));

        p.fillRect(rect(), p.brush());
    }

    // Draw the pie with borders and selected slice.
    if (items.size() > 1) {
        for (int i = 0; i < items.size(); ++i) {
            if (i == hItem && items.at(i)->isEnabled())
                p.setBrush(QBrush(palette().highlight()));
            else
                p.setBrush(QBrush(palette().background()));

            p.setPen(palette().mid().color());
            if (shaded) {
                p.setPen(QPen(p.pen().color().dark(125)));
                p.setBrush(QBrush(p.brush().color().dark(125)));
            }

            double a = indexToAngle(i, items.size(), true);
            double b = indexToAngle(i + 1, items.size(), true);

            if (b < a)
                b += TWOPI;
            double size = b - a;
            if (size < 0)
                size += TWOPI;
            int startAngle = (int) ((a * 360.0 * 16.0) / TWOPI);

            p.drawPie(adjustedRect, startAngle, (int) ((size * 360.0 * 16.0) / TWOPI));
        }
    }

    QColor background;
    QColor dark = palette().dark().color();
    QColor light = palette().light().color();
    if (hItem == -1)
        background = palette().highlight().color();
    else
        background = palette().background().color();

    if (shaded) {
        background = background.dark(125);
        dark = dark.dark(125);
        light = light.dark(125);
    }

    // Shading
    QRect innerRect(adjustedRect.center().x() - innerRad, adjustedRect.center().y() - innerRad,
                    innerRad * 2 + 1, innerRad * 2 + 1);

    p.setPen(Qt::NoPen);
    p.setBrush(background);

    p.drawPie(innerRect, 0, 360 * 16);

    light.setAlpha(128);
    p.setPen(QPen(light, 1));
    p.setBrush(Qt::NoBrush);
    p.drawArc(innerRect, 225 * 16, 180 * 16);

    p.setPen(QPen(dark, 1));
    p.drawArc(innerRect, 45 * 16, 180 * 16);

    p.setPen(QPen(light, 1));
    p.setBrush(Qt::NoBrush);
    p.drawArc(adjustedRect, 45 * 16, 180 * 16);
    p.setPen(QPen(dark, 1));
    p.drawArc(adjustedRect, 225 * 16, 180 * 16);

    // Draw cancel zone
    if (innerRad > 0) {
        innerRect.setLeft(innerRect.left() + 1);
        innerRect.setRight(innerRect.right() - 1);
        innerRect.setTop(innerRect.top() + 1);
        innerRect.setBottom(innerRect.bottom() - 1);

        p.setPen(QPen(dark, 1));
        p.drawArc(innerRect, 225 * 16, 180 * 16);
        p.setPen(QPen(light, 1));
        p.drawArc(innerRect, 45 * 16, 180 * 16);
        p.setPen(QPen(dark, 1));

        p.setBrush(background);

        p.drawArc(innerRect, 225 * 16, 180 * 16);
        p.setPen(QPen(light, 1));
        p.drawArc(innerRect, 45 * 16, 180 * 16);
    }

    QRect r = innerRect;
    innerRect.setLeft(r.center().x() + ((r.left() - r.center().x()) / 3) * 1);
    innerRect.setRight(r.center().x() + ((r.right() - r.center().x()) / 3) * 1);
    innerRect.setTop(r.center().y() + ((r.top() - r.center().y()) / 3) * 1);
    innerRect.setBottom(r.center().y() + ((r.bottom() - r.center().y()) / 3) * 1);

    QColor text = hItem == -1 ? palette().highlightedText().color() : palette().text().color();
    if (shaded)
        text = text.dark(125);

    p.setPen(QPen(text, 2));
    p.drawLine(innerRect.topLeft(), innerRect.bottomRight());
    p.drawLine(innerRect.topRight(), innerRect.bottomLeft());

    // Distance from edge of pie to tip of arrow.
    int h1 = 2;

    // Height of arrow.
    int h2 = 6;

    // Width of arrow in radians.
    double w = 0.1;

    // Draw little arrows on the edge of the items that hide a
    // submenu.
    int i;
    for (i = 0; i < items.size(); ++i) {
        if (!items.at(i) || items.at(i)->type() != SubMenu)
            continue;

        if (i == hItem) {
            p.setBrush(palette().highlightedText());

            // Smooth edges of the arrow by drawing with a pen that
            // has the average color of the foreground and the
            // background.
            QColor c = palette().highlightedText().color();
            QColor d = palette().highlight().color();
            p.setPen(QColor((c.red() + d.red()) / 2,
                            (c.green() + d.green()) / 2,
                            (c.blue() + d.blue()) / 2));
        } else {
            p.setBrush(palette().text());

            QColor c = palette().text().color();
            QColor d = palette().background().color();
            p.setPen(QColor((c.red() + d.red()) / 2,
                            (c.green() + d.green()) / 2,
                            (c.blue() + d.blue()) / 2));
        }

        if (shaded) {
            p.setPen(QPen(p.pen().color().dark(125)));
            p.setBrush(QBrush(p.brush().color().dark(125)));
        }

        double angle;
        if (count() == 1) {
            // If there is only one pie menu item, and the parent does
            // not inherit QtPieMenu (qualifies as the rool level pie
            // menu), then the arrow should be drawn straight
            // north. If this is not the root menu however, the arrow
            // should be drawn on the same axis that intersects both
            // the parent's center and this menu's center, at the
            // point where it intersects with this menu's outer
            // radius.
            if (!parent()->inherits("QtPieMenu")) {
                angle = 0;
            } else {
                QtPieMenu *parentPie = (QtPieMenu *)parent();
                QPoint ppos = mapToGlobal(parentPie->pos());
                ppos.setX(ppos.x() + parentPie->width()/2);
                ppos.setY(ppos.y() + parentPie->height()/2);

                QPoint opos = mapToGlobal(pos());
                opos.setX(opos.x() + width()/2);
                opos.setY(opos.y() + height()/2);

                double xxdist = (double)(opos.x() - ppos.x());
                double yydist = (double)(opos.y() - ppos.y());
                double rrad = sqrt(xxdist * xxdist + yydist * yydist);
                angle = acos(xxdist / rrad);

                if (yydist > 0)
                    angle = TWOPI - angle;
            }
        } else {
            double a = indexToAngle(i, count(), true);
            double b = indexToAngle(i + 1, count(), true);
            if (b > a)
                angle = (a + b) / 2;
            else
                angle = (a + b + TWOPI) / 2;
        }

        double trad = (double)(outerRad - h1);
        double tb = trad * cos(angle);
        double tc = - trad * sin(angle);
        double lrad = (double)(outerRad - h2);
        double lb = lrad * cos(angle + w);
        double lc = - lrad * sin(angle + w);
        double rrad = (double)(outerRad - h2);
        double rb = rrad * cos(angle - w);
        double rc = - rrad * sin(angle - w);

        QPolygon ar(3);
        ar.setPoint(0, pix.rect().center().x() + (int) tb, pix.rect().center().y() + (int) tc);
        ar.setPoint(1, pix.rect().center().x() + (int) rb, pix.rect().center().y() + (int) rc);
        ar.setPoint(2, pix.rect().center().x() + (int) lb, pix.rect().center().y() + (int) lc);

        p.drawPolygon(ar);
    }

    // Draw the text and/or icon on each slice.
    for (i = 0; i < items.size(); ++i) {
        QString text = items.at(i)->text();

        double angle1 = indexToAngle(i, items.size(), true);
        double angle2 = indexToAngle(i + 1, items.size(), true);
        if (angle2 < angle1)
            angle2 += TWOPI;
        double angle = (angle1 + angle2) / 2.0;
        double rad = (double)(innerRad + outerRad) / 2.0;
        double b = rad * cos(angle);
        double c = - rad * sin(angle);

        QFontMetrics fontMetrics = p.fontMetrics();
        QRect boundingRect;
        if (!text.isEmpty())
            boundingRect = fontMetrics.boundingRect(text);
        int textWidth = boundingRect.width();

        QPixmap icon = items.at(i)->icon().pixmap(QSize(32, 32),
                                                  QIcon::Normal,
                                                  QIcon::On);
        int x = pix.rect().center().x();
        int y = pix.rect().center().y();

        // Draw icon
        if (!items.at(i)->icon().isNull()) {
            QRect r(x + int(b) - (icon.width() + textWidth) / 2,
                    y + int(c) - icon.height() / 2,
                    icon.width(), icon.height());

            if (shaded) {
                // Only paint every second pixel.
                QBitmap bitmap(icon.width(), icon.height());
                QPainter mapper(&bitmap);
                int xorer = 0;
                for (int b = 0; b < icon.height(); ++b) {
                    xorer = b & 1;
                    for (int a = 0; a < icon.width(); ++a) {
                        mapper.setPen(xorer++ & 1 ? Qt::color1 : Qt::color0);
                        mapper.drawPoint(a, b);
                    }
                }

                icon.setMask(bitmap);
            }

            p.drawPixmap(r, icon);
        }

        // Draw text
        if (!text.isEmpty()) {
            QBrush pen;
            QBrush brush;

            if (i == hItem) {
                if (highlightedItemPtr()->isEnabled()) {
                    pen = palette().highlightedText();
                    brush = palette().highlight();
                } else {
                    pen = palette().mid();
                    brush = palette().background();
                }
            } else {
                if (items.at(i)->isEnabled())
                    pen = palette().text();
                else
                    pen = palette().mid();

                brush = palette().background();
            }

            if (shaded) {
                pen = QBrush(pen.color().dark(125));
                brush = QBrush(brush.color().dark(125));
            }

            // Draw text horizontally in center of slice.
            double angle1 = indexToAngle(i, items.size(), true);
            double angle2 = indexToAngle(i + 1, items.size(), true);
            if (angle2 < angle1)
                angle2 += TWOPI;
            double angle = (angle1 + angle2) / 2.0;

            double rad = (double)(innerRad + outerRad) / 2.0;
            double b = rad * cos(angle);
            double c = - rad * sin(angle);

            // Draw rich text, always centered.
            QRect r(x + (int) b - (icon.width() + textWidth) / 2 + icon.width(),
                    y + (int) c - boundingRect.height() / 2, textWidth, boundingRect.height());

            p.setPen(pen.color());
            p.setBrush(brush);
            p.drawText(r.left(), r.top() + boundingRect.height(), text);
        }
    }

    QPainter p2(this);
    p2.drawPixmap(QPoint(0, 0), pix);
}


