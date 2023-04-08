#include <windows.h>
#include <gl/gl.h>
#define LENGTH 1000000

static void drawOS(const float min_x, const float max_x, const float min_y, const float max_y);
static void draw(POINTFLOAT* points);

POINTFLOAT points[LENGTH];
int isCalc = 1;
float min_y, max_y;

void graph(float (*f)(const float), const float min_x, const float max_x)
{
    const float step = (max_x - min_x) / LENGTH;

    if (isCalc)
    {
        points[0].y = f(min_x);
        min_y = points[0].y, max_y = points[0].y;
        float x = min_x + step;

        for (int i = 1; i < LENGTH; i++, x += step)
        {
            points[i].y = f(x);
            if (min_y > points[i].y)
                min_y = points[i].y;
            if (max_y < points[i].y)
                max_y = points[i].y;
        }

        const float scale_max = 1.0f, scale_min = -1.0f;
        x = min_x;
        for (int i = 0; i < LENGTH; i++, x += step)
        {
            const float proc_x = (x - min_x) / (max_x - min_x), proc_y = (points[i].y - min_y) / (max_y - min_y);
            points[i].x = proc_x * (scale_max - scale_min) + scale_min;
            points[i].y = proc_y * (scale_max - scale_min) + scale_min;
        }

        isCalc = 0;
    }

    glLineWidth(2);
    draw(points);
    drawOS(min_x, max_x, min_y, max_y);
}

static void drawOS(const float min_x, const float max_x, const float min_y, const float max_y)
{
    const float scale_min = -1.0f, scale_max = 1.0f;

    //По игорю
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    if (min_x <= 0 && max_x >= 0)
    {
        const float pos_x = ((-min_x) / (max_x - min_x)) * (scale_max - scale_min) + scale_min;

        glVertex2f(pos_x, -1.0f);
        glVertex2f(pos_x, 1.0f);

        glVertex2f(pos_x, 1.0f);
        glVertex2f(pos_x - 0.01f, 1.0f - 0.02f);

        glVertex2f(pos_x, 1.0f);
        glVertex2f(pos_x + 0.01f, 1.0f - 0.02f);
    }

    //По иксу
    if (min_y <= 0 && max_y >= 0)
    {
        const float pos_y = ((-min_y) / (max_y - min_y)) * (scale_max - scale_min) + scale_min;

        glVertex2f(-1.0f, pos_y);
        glVertex2f(1.0f, pos_y);

        glVertex2f(1.0f, pos_y);
        glVertex2f(1.0f - 0.02f, pos_y - 0.01f);

        glVertex2f(1.0f, pos_y);
        glVertex2f(1.0f - 0.02f, pos_y + 0.01f);
    }
    glEnd();
}

static void draw(POINTFLOAT* points)
{
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < LENGTH; i++)
        glVertex2f(points[i].x, points[i].y);
    glEnd();
}
