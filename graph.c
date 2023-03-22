#include <gl/gl.h>
#include <stdlib.h>
#define LENGTH 1000000

static void drawOS(const float min_x, const float max_x, const float min_y, const float max_y);
static void draw(float* val_func, const float min_y, const float max_y, const float min_x, const float max_x, const float step);

float val_func[LENGTH];

void graph(float (*f)(const float), const float min_x, const float max_x)
{
    const float step = (max_x - min_x) / LENGTH;

    val_func[0] = f(min_x);
    float min_y = val_func[0], max_y = val_func[0];
    float x = min_x + step;

    for (int i = 1; i < LENGTH; i++, x += step)
    {
        val_func[i] = f(x);
        if (min_y > val_func[i])
            min_y = val_func[i];
        if (max_y < val_func[i])
            max_y = val_func[i];
    }

    glLineWidth(2);
    draw(val_func, min_y, max_y, min_x, max_x, step);
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

static void draw(float* val_func, const float min_y, const float max_y, const float min_x, const float max_x, const float step)
{
    const float scale_max = 1.0f, scale_min = -1.0f;
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    float x = min_x;
    for (int i = 0; i < LENGTH; i++, x += step)
    {
        const float proc_x = (x - min_x) / (max_x - min_x), proc_y = (val_func[i] - min_y) / (max_y - min_y);
        glVertex2f(proc_x * (scale_max - scale_min) + scale_min, proc_y * (scale_max - scale_min) + scale_min);
    }
    glEnd();
}
