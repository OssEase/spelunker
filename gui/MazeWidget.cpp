/**
 * MazeWidget.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QColor>

#include <types/Direction.h>
#include <maze/Maze.h>
#include <maze/DFSMazeGenerator.h>

#include "MazeWidget.h"

namespace spelunker::gui {
    MazeWidget::MazeWidget(const maze::Maze &m, QWidget *parent)
    : QWidget{parent}, maze{m} {}

    void MazeWidget::paintEvent(QPaintEvent *e) {

        Q_UNUSED(e)

        doPainting();
    }

    void MazeWidget::resizeWithBorder(int w, int h) {
        resize(w + 2 * BORDER_WIDTH, h + 2 * BORDER_WIDTH);
    }

    void MazeWidget::doPainting() {
        QPainter painter { this };

        const auto w = width();
        const auto h = height();
        const auto mw = maze.getWidth();
        const auto mh = maze.getHeight();

        // Determine the size per cell at the current resolution.
        const auto cellW = (1.0 * w - 2 * WALL_WIDTH) / mw;
        const auto cellH = (1.0 * h - 2 * WALL_WIDTH) / mh;

        // Draw the floor.
        for (auto y = 0; y < mh; ++y)
            for (auto x = 0; x < mw; ++x)
                painter.fillRect(QRectF{x * cellW, y * cellH, cellW, cellH}, (x + y) % 2 ? FLOOR_COLOUR1 : FLOOR_COLOUR2);
        const QRect fullRect{0, 0, w, h};
        //painter.fillRect(fullRect, FLOOR_COLOUR);

        // Draw the border.
        // Increase the border width by 1 because otherwise sometimes it leaves a small
        // gap on the right / lower sides.
        QPen pen{WALL_COLOUR};
        pen.setWidth(BORDER_WIDTH+1);
        painter.setPen(pen);
        painter.drawRect(fullRect);

        // Draw the walls by iterating over the cells.
        pen.setWidth(WALL_WIDTH);
        //auto ypos = 0.5 * BORDER_WIDTH;
        auto ypos = -0.5 * WALL_WIDTH;
        for (auto y=0; y < mh; ++y) {
            auto xpos = -0.5 * WALL_WIDTH;
            for (auto x=0; x < mw; ++x) {
                // Determine the walls.
                const auto UPPER_LEFT  = QPointF{xpos, ypos};
                const auto UPPER_RIGHT = QPointF{xpos + cellW, ypos};
                const auto LOWER_LEFT  = QPointF{xpos, ypos + cellH};
                const auto LOWER_RIGHT = QPointF{xpos + cellW, ypos + cellH};

                if (maze.wall(x, y, types::Direction::NORTH))
                    painter.drawLine(UPPER_LEFT, UPPER_RIGHT);
                if (maze.wall(x, y, types::Direction::EAST))
                    painter.drawLine(UPPER_RIGHT, LOWER_RIGHT);
                if (maze.wall(x, y, types::Direction::SOUTH))
                    painter.drawLine(LOWER_LEFT, LOWER_RIGHT);
                if (maze.wall(x, y, types::Direction::WEST))
                    painter.drawLine(UPPER_LEFT, LOWER_LEFT);

                xpos += cellW;
            }

            ypos += cellH;
        }
    }
}
