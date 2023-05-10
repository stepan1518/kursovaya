#include <windows.h>
#include <gl/gl.h>
#include "stb/stb_easy_font.h"
#include "stb/stb_image.h"
#define LENGTH 1000000
#define ZERO 0.0001f

static void drawOS(const float min_x, const float max_x, const float min_y, const float max_y, float* pos_x, float* pos_y);
static void draw(const POINTFLOAT* points);
static void drawDottedLine(const float start_x, const float start_y, const float end_x, const float end_y);
static void drawFormat(const POINTFLOAT* points, const POINTFLOAT* max_point, const POINTFLOAT* min_point, const float pos_x, const float pos_y);
static float absolute(const float x);
static void print_string(float x, float y, char *text, float r, float g, float b);

POINTFLOAT points[LENGTH];
int isCalc = 1;
float min_y, max_y, start_x, end_x, start_y, end_y;
POINTFLOAT* max_point = points, *min_point = points;

void graph(float (*f)(const float), const float min_x, const float max_x)
{
    start_x = min_x;
    end_x = max_x;
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
            {
                min_point = &points[i];
                if (x <= max_x)
                    min_y = points[i].y;
            }
            if (max_y < points[i].y)
            {
                max_point = &points[i];
                if (x <= max_x)
                    max_y = points[i].y;
            }
        }

        float data = f(max_x);
        max_y = max_y < data? data : max_y;

        const float scale_max = 0.8f, scale_min = -0.8f;
        x = min_x;
        for (int i = 0; i < LENGTH; i++, x += step)
        {
            const float proc_x = (x - min_x) / (max_x - min_x), proc_y = (points[i].y - min_y) / (max_y - min_y);
            points[i].x = proc_x * (scale_max - scale_min) + scale_min;
            points[i].y = proc_y * (scale_max - scale_min) + scale_min;
        }

        isCalc = 0;
    }

    const float pos_x, pos_y;

    glLineWidth(2);
    draw(points);
    drawOS(min_x, max_x, min_y, max_y, &pos_x, &pos_y);
    drawFormat(points, max_point, min_point, pos_x, pos_y);
}

static void drawOS(const float min_x, const float max_x, const float min_y, const float max_y, float* pos_x, float* pos_y)
{
    const float scale_min = -0.8f, scale_max = 0.8f;
    *pos_x = ((-min_x) / (max_x - min_x)) * (scale_max - scale_min) + scale_min;
    *pos_y = ((-min_y) / (max_y - min_y)) * (scale_max - scale_min) + scale_min;

    if (min_y > ZERO)
        *pos_y = -0.9f;
    else if (max_y < ZERO)
        *pos_y = 0.9f;

    if (min_x > ZERO)
        *pos_x = -0.9f;
    else if (max_x < ZERO)
        *pos_x = 0.9f;

    //По OY
    glBegin(GL_LINES);

    glVertex2f(*pos_x, -1.0f);
    glVertex2f(*pos_x, 1.0f);

    glVertex2f(*pos_x, 1.0f);
    glVertex2f(*pos_x - 0.01f, 1.0f - 0.02f);

    glVertex2f(*pos_x, 1.0f);
    glVertex2f(*pos_x + 0.01f, 1.0f - 0.02f);

    //По OX
    glVertex2f(-1.0f, *pos_y);
    glVertex2f(1.0f, *pos_y);

    glVertex2f(1.0f, *pos_y);
    glVertex2f(1.0f - 0.02f, *pos_y - 0.01f);

    glVertex2f(1.0f, *pos_y);
    glVertex2f(1.0f - 0.02f, *pos_y + 0.01f);

    glEnd();
}

static void draw(const POINTFLOAT* points)
{
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < LENGTH; i++)
        glVertex2f(points[i].x, points[i].y);
    glEnd();
}

static void drawDottedLine(const float start_x, const float start_y, const float end_x, const float end_y)
{
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(3,666);
      glBegin(GL_LINE_LOOP);
        glVertex2f(start_x, start_y);
        glVertex2f(end_x, end_y);
      glEnd();
    glDisable(GL_LINE_STIPPLE);
}

static void drawFormat(const POINTFLOAT* points, const POINTFLOAT* max_point, const POINTFLOAT* min_point, const float pos_x, const float pos_y)
{
    drawDottedLine(points[0].x, points[0].y, points[0].x, pos_y);
    drawDottedLine(points[LENGTH - 1].x, points[LENGTH - 1].y, points[LENGTH - 1].x, pos_y);
    drawDottedLine(min_point->x, min_point->y, pos_x, min_point->y);
    drawDottedLine(max_point->x, max_point->y, pos_x, max_point->y);

    const int k = 250;
    const float indent = 0.03f;
    glScalef(0.004, -0.004, 1);
    print_string((1 - indent) * k, (-pos_y + indent) * k, "X", 0, 0, 0);
    print_string((pos_x + indent) * k, (-1 + indent) * k, "Y", 0, 0, 0);

    char text[20];

    snprintf(text, 20, "%.1f", min_y);
    print_string((pos_x + indent) * k, (-min_point->y + indent) * k, text, 0, 0, 0);

    snprintf(text, 20, "%.1f", max_y);
    print_string((pos_x + indent) * k, (-max_point->y + indent) * k, text, 0, 0, 0);

    snprintf(text, 20, "%.1f", start_x);
    print_string((points[0].x + indent) * k, (-pos_y + indent) * k, text, 0, 0, 0);

    snprintf(text, 20, "%.1f", end_x);
    print_string((points[LENGTH - 1].x + indent) * k, (-pos_y + indent) * k, text, 0, 0, 0);
}

static void print_string(float x, float y, char *text, float r, float g, float b)
{
  static char buffer[99999]; // ~500 chars
  int num_quads;

  num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

  glColor3f(r,g,b);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 16, buffer);
  glDrawArrays(GL_QUADS, 0, num_quads*4);
  glDisableClientState(GL_VERTEX_ARRAY);
}

static float absolute(const float x)
{
    return x > 0? x : -x;
}
