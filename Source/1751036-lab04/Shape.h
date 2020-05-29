#ifndef SHAPE
#define SHAPE

#include <windows.h> // for MS Windows
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"
#include "Dependencies/soil/SOIL.h"

int WIDTH = 800;
int HEIGHT = 600;

float PI = 3.1415926f;
int NUM_TEXTURE = 18;
bool texLoaded = false, resetPaused = false, paused = false;
GLuint *texture = new GLuint[NUM_TEXTURE];

float _random(int lower, int upper, float factor) {
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(lower, upper);
    return dist(engine) * factor;
}

float getRandomFloat() {
    return _random(1, 20, 0.2f);
}

float getRandomDepth() {
    return _random(6, 15, -1);
}

int loadGLTextures() // Load Bitmaps And Convert To Textures
{
    std::ifstream myfile("Textures/total-number-of-textures.txt");
    if (myfile.is_open())
    {
        while (myfile >> NUM_TEXTURE);
        myfile.close();
    }

    std::cout << "Total number of textures: " << NUM_TEXTURE << std::endl;

    /* load an image file directly as a new OpenGL texture */
    for (int i = 0; i < NUM_TEXTURE; ++i) {
        std::string filename = "Textures/tex" + std::to_string(i) + ".jpg";
        texture[i] = SOIL_load_OGL_texture
        (
            filename.c_str(),
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_POWER_OF_TWO |
            SOIL_FLAG_INVERT_Y
        );
        if (texture[i]) std::cout << filename << " loaded." << std::endl;
    }

    for (int i = 0; i < NUM_TEXTURE; ++i) {

        if (texture[i] == 0) {
            std::cout << "Unable to load tex" << i << ".jpg, ";
            std::string filename = "Textures/tex" + std::to_string(16) + ".jpg";
            std::cout << "fallback to " << filename << std::endl;
            texture[i] = SOIL_load_OGL_texture
            (
                filename.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_POWER_OF_TWO |
                SOIL_FLAG_INVERT_Y
            );
        }
    }
    
    std::cout << "Program is running..." << std::endl << std::endl;
    
    // Typical Texture Generation Using Data From The Bitmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texLoaded = true;
    return true;
}

class Shape {
private:
public:
	Shape() {
		;
	}

	virtual void render(GLfloat * x, GLfloat * y) = 0;

    ~Shape() {
        std::cout << "Deleted shape" << std::endl;
    }
};

class Flat : public Shape {
private:
    GLfloat angle, ROT, depth, r;
    int texID, total;
public:
    Flat() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        r = getRandomFloat();
        if (!texLoaded) loadGLTextures();
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);
        total = 120;

        std::cout << "     Circular Plane:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Radius: " << r << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }
    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);

        GLfloat X, Y, Z, midTex = 0.5f;
        glBegin(GL_TRIANGLE_FAN);
        for (GLfloat i = 0; i <= 2 * PI; i += 2 * PI / total) {
            X = r * cos(i);
            Y = r * sin(i);
            Z = -0.05f;
            glTexCoord2f(midTex + midTex * cos(i), midTex + midTex * sin(i));
            glVertex3f(X, Y, Z);
        }
        glEnd();

        angle -= paused ? 0 : ROT;
    }
};


class Cube : public Shape {
private:
    GLfloat angle, ROT, depth;
    int texID;
public:
    Cube() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        if (!texLoaded) loadGLTextures();
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Cube:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }

    

	void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(*x, *y, depth); // Move x, y and into the screen
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate clockwise about (1,1,1)-axis [NEW]
        

        glPushMatrix();
        glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.6f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.3f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.25f, 0.3f); glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.25f, 0.6f); glVertex3f(-1.0f, 1.0f, 1.0f);
        // Top Face
        glTexCoord2f(0.6f, 0.25f); glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.3f, 0.25f); glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.3f, 0.5f); glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.6f, 0.5f); glVertex3f(-1.0f, 1.0f, -1.0f);
        // Back Face
        glTexCoord2f(0.5f, 0.6f); glVertex3f(-1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.5f, 0.3f); glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.75f, 0.3f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.75f, 0.6f); glVertex3f(-1.0f, -1.0f, -1.0f);
        // Bottom Face
        glTexCoord2f(0.75f, 0.6f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.75f, 0.3f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.3f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.6f); glVertex3f(-1.0f, -1.0f, 1.0f);
        // Right face
        glTexCoord2f(0.0f, 0.3f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.25f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
        glTexCoord2f(0.25f, 0.3f); glVertex3f(1.0f, 1.0f, 1.0f);
        // Left Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.6f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(0.25f, 0.6f); glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.25f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
	}
};


class Pyramid : public Shape {
private:
    GLfloat angle, ROT, depth;
    int texID;
public:
    Pyramid() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Pyramid:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }

    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);
        
        GLfloat middleTex = 0.5f;

        glPushMatrix();
        glBegin(GL_TRIANGLE_FAN);        
        glTexCoord2f(middleTex, middleTex); glVertex3f(0, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glEnd();

        glBegin(GL_QUADS);
        // Bottom Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
    }
};


class Sphere : public Shape {
private:
    GLfloat angle, ROT, depth, r;
    int texID;

    int total;
    std::vector<std::vector<std::vector<float>>> sphere;
public:
    Sphere() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        r = getRandomFloat();
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);
        total = 60;
        sphere.resize(total + 1, std::vector<std::vector<float>>(total + 1, std::vector<float>(5)));

        std::cout << "     Sphere:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Radius: " << r << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;

        float _x, _y, _z, _u, _v;

        for (int i = 0; i < total + 1; ++i) {
            float lon = i * 2 * PI / total;
            for (int j = 0; j < total + 1; ++j) {
                float lat = j * PI / total;
                _x = r * cos(lon) * sin(lat);
                _y = r * sin(lon) * sin(lat);
                _z = r * cos(lat);
                _u = lat / PI;
                _v = lon / PI / 2;
                std::vector<float> point{ _x, _y, _z, _u, _v};
                sphere[i][j] = point;
            }
        }
    }

    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);

        glPushMatrix();
        for (int i = 0; i < total; ++i) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < total + 1; ++j) {
                glTexCoord2f(sphere[i][j][3], sphere[i][j][4]);
                glVertex3f(sphere[i][j][0], sphere[i][j][1], sphere[i][j][2]);
                glTexCoord2f(sphere[i + 1][j][3], sphere[i + 1][j][4] + 1 / total);
                glVertex3f(sphere[i + 1][j][0], sphere[i + 1][j][1], sphere[i + 1][j][2]);
            }
            glEnd();
        }
        glPopMatrix();


        angle -= paused ? 0 : ROT;
    }
};


class Torus : public Shape {
private:
    GLfloat angle, ROT, depth, R, r;
    int texID;

    int total;

public:
    Torus() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        R = getRandomFloat();
        r = getRandomFloat();

        while (R == r) {
            r = getRandomFloat();
        }

        if (r > R) {
            GLfloat tmp = R;
            R = r;
            r = tmp;
        }

        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        total = 50;

        std::cout << "     Torus:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Big Radius: " << R << std::endl;
        std::cout << "          Small Radius: " << r << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }

    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);
        
        GLfloat X, Y, Z;
        GLfloat slice, ring, u, v;

        glPushMatrix();
        for (float slice = 0; slice < total; slice+=1) {
            glBegin(GL_QUAD_STRIP);

            for (float ring = 0; ring <= total; ring+=1) {

                X = (R + r * cos(slice * 2 * PI / total)) * cos(ring * 2 * PI / total);
                Y = (R + r * cos(slice * 2 * PI / total)) * sin(ring * 2 * PI / total);
                Z = r * sin(slice * 2 * PI / total);

                u = slice / total;
                v = ring / total;

                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);


                X = (R + r * cos((slice + 1) * 2 * PI / total)) * cos(ring * 2 * PI / total);
                Y = (R + r * cos((slice + 1) * 2 * PI / total)) * sin(ring * 2 * PI / total);
                Z = r * sin((slice + 1) * 2 * PI / total);

                u = (slice + 1) / total;
                v = ring / total;

                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);
            }
            glEnd();
        }
        glPopMatrix();

        angle -= paused ? 0 : ROT;
    }
};


class Cylinder : public Shape {
private:
    GLfloat angle, ROT, depth, r, h;
    int texID, total;
public:
	Cylinder() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        h = getRandomFloat();
        r = getRandomFloat();
        total = 50;
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Cylinder:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Radius: " << r << std::endl;
        std::cout << "          Height: " << h << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
	}

	void render(GLfloat * x, GLfloat * y) {
		glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);
        
        glPushMatrix();
        glBegin(GL_QUAD_STRIP);
        for (float i = 0; i <= 2 * PI + PI / total; i += PI/total) {
            float textCoord = i / (2 * PI);
            glTexCoord2f(textCoord, 1.0);
            glVertex3f(cos(i) * r, sin(i) * r, +h);
            glTexCoord2f(textCoord, 0.0);
            glVertex3f(cos(i) * r, sin(i) * r, -h);
        }
        glEnd();

        GLfloat middleTex = 0.5f;
        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(middleTex * cos(i) + middleTex, middleTex * sin(i) + middleTex);
            glVertex3f(r * cos(i), r * sin(i), h);
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(middleTex * cos(i) + middleTex, middleTex * sin(i) + middleTex);
            glVertex3f(r * cos(i), r * sin(i), -h);
        }
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
	}
};


class Cone : public Shape {
private:
	GLfloat angle, r, h, ROT, depth;
    int texID, total;

public:
	Cone() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
		r = getRandomFloat();
        h = getRandomFloat();
        total = 50;
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Cone:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Radius: " << r << std::endl;
        std::cout << "          Height: " << h << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
	}

	void render(GLfloat * x, GLfloat * y) {
		glLoadIdentity();
        glTranslatef(*x, *y, depth); 
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);

        glPushMatrix();
        GLfloat middleTex = 0.5f;
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(middleTex, middleTex);
        glVertex3f(0, 0, h);
        for (float i = -PI / total; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(middleTex * cos(i) + middleTex, middleTex * sin(i) + middleTex);
            glVertex3f(r * cos(i), r * sin(i), -h);
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(middleTex * cos(i) + middleTex, middleTex * sin(i) + middleTex);
            glVertex3f(r * cos(i), r * sin(i), -h);
        }
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
	}
};


class Paraboloid : public Shape {
private:
    GLfloat angle, R, h, ROT, depth;
    int texID, total;

public:
    Paraboloid() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        R = getRandomFloat();
        h = getRandomFloat();
        total = 50;
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Paraboloid:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Foot Radius: " << R << std::endl;
        std::cout << "          Height: " << h << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }

    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth); 
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f); 

        GLfloat r, X, Y, Z, u, v, midTex = 0.5f;

        glPushMatrix();
        for (GLfloat lat = 0; lat <= PI / 2; lat += PI / 2 / total) {
            glBegin(GL_QUAD_STRIP);
            for (GLfloat lon = 0; lon <= 2 * PI + 2 * PI / total; lon += 2 * PI / total) {
                r = lat / (PI / 2) * R;
                Z = r * sin(lon);
                X = r * cos(lon);
                Y = h * cos(lat) - h / 2;
                u = midTex + X / R / 2;
                v = midTex + Z / R / 2;
                //u = lon / (2 * PI);
                //v = lat / (PI / 2);
                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);

                r = (lat + PI / 2 / total) / (PI / 2) * R;
                Z = r * sin(lon);
                X = r * cos(lon);
                Y = h * cos(lat + PI / 2 / total) - h / 2;
                u = midTex + X / R / 2;
                v = midTex + Z / R / 2;
                //u = lon / (2 * PI);
                //v = (lat + PI / 2 / total) / (PI / 2);
                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);
            }
            glEnd();
        }

        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(midTex * cos(i) + midTex, midTex * sin(i) + midTex);
            glVertex3f(r * cos(i), -h / 2, r * sin(i));
        }
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
    }
};


class Hyperboloid : public Shape {
private:
    GLfloat angle, R, bR, h, ROT, depth;
    int texID, total;

public:
    Hyperboloid() {
        angle = 0;
        ROT = getRandomFloat();
        depth = getRandomDepth();
        R = _random(13, 20, 0.2f);
        bR = _random(2, 11, 0.2f);

        h = _random(5, 20, 0.2f);
        total = 50;
        texID = _random(0, NUM_TEXTURE - 1, 1);
        glBindTexture(GL_TEXTURE_2D, texture[texID]);

        std::cout << "     Hyperboloid:" << std::endl;
        std::cout << "          Rotation per time unit: " << ROT << "rad" << std::endl;
        std::cout << "          Depth: " << depth << std::endl;
        std::cout << "          Big Radius: " << R << std::endl;
        std::cout << "          Small Radius: " << bR << std::endl;
        std::cout << "          Height: " << h << std::endl;
        std::cout << "          Number of segments: " << total << std::endl;
        std::cout << "          Texture: tex" << texID << ".jpg" << std::endl << std::endl;
    }

    void render(GLfloat * x, GLfloat * y) {
        glLoadIdentity();
        glTranslatef(*x, *y, depth);
        if (!resetPaused) glRotatef(angle, 1.0f, 1.0f, 1.0f);

        GLfloat r, X, Y, Z, u, v, midTex = 0.5f;

        glPushMatrix();
        for (GLfloat lat = 0; lat <= PI; lat += PI / 2 / total) {
            glBegin(GL_QUAD_STRIP);
            for (GLfloat lon = 0; lon <= 2 * PI + 2 * PI / total;
                lon += 2 * PI / total) {
                r = R - (R - bR) * sin(lat);
                Z = r * sin(lon);
                X = r * cos(lon);
                Y = h / 2 * cos(lat);
                u = lon / (2 * PI);
                v = lat / PI;
                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);

                r = R - (R - bR) * sin(lat + PI / 2 / total);
                Z = r * sin(lon);
                X = r * cos(lon);
                Y = h / 2 * cos(lat + PI / 2 / total);
                u = lon / (2 * PI);
                v = (lat + PI / 2 / total) / PI;
                glTexCoord2f(u, v);
                glVertex3f(X, Y, Z);
            }
            glEnd();
        }

        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(midTex * cos(i) + midTex, midTex * sin(i) + midTex);
            glVertex3f(r * cos(i), -h/2, r * sin(i));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        for (float i = 0; i <= 2 * PI; i += PI / total) {
            glTexCoord2f(midTex * cos(i) + midTex, midTex * sin(i) + midTex);
            glVertex3f(r * cos(i), h/2, r * sin(i));
        }
        glEnd();
        glPopMatrix();

        angle -= paused ? 0 : ROT;
    }
};

#endif // !SHAPE