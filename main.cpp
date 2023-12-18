#include <glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int window_width = 800;
int window_height = 600;

const int num_textures = 70;

struct Texture
{
	float x;
	float y;
	float speed;
	GLuint id;
	int width;
	int height;
};

Texture textures[num_textures];
Texture background_texture;

void init();
void update();
void draw_texture(float x, float y, float width, float height, GLuint texture_id);
void display();
void reshape(int width, int height);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Snowflake animation | OpenGL");

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	init();

	// Enter the main loop
	glutMainLoop();

	return 0;
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	srand(static_cast<unsigned int>(time(nullptr)));

	for (size_t i = 0; i < num_textures; ++i)
	{
		textures[i].x = static_cast<float>(rand() % window_width);
		textures[i].y = static_cast<float>(rand() % window_height + window_height);
		textures[i].speed = static_cast<float>(rand() % 5 + 1) / 10.0f;
		glGenTextures(1, &textures[i].id);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		stbi_set_flip_vertically_on_load(true);
		int channels;
		unsigned char* image = stbi_load("./resource/texture.png", &textures[i].width, &textures[i].height,
			&channels, STBI_rgb_alpha);

		if (image)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i].width, textures[i].height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i].width, textures[i].height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
		}
		else
		{
			std::cerr << "failed to load texture: snowflake.png" << std::endl;
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenTextures(1, &background_texture.id);
	glBindTexture(GL_TEXTURE_2D, background_texture.id);
	stbi_set_flip_vertically_on_load(true);
	int channels;
	unsigned char* background_image = stbi_load("./resource/background.png", &background_texture.width,
		&background_texture.height, &channels, STBI_rgb_alpha);

	if (background_image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, background_texture.width, background_texture.height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, background_image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, background_texture.width, background_texture.height, 0, 
			GL_RGBA,
			GL_UNSIGNED_BYTE, background_image);
		stbi_image_free(background_image);
	}
	else
	{
		std::cout << "failed to load background image: background.jpg" << std::endl;
	}
}

void update()
{
	for (size_t i = 0; i < num_textures; ++i)
	{
		textures[i].y -= textures[i].speed;
		if (textures[i].y < -textures[i].height)
		{
			textures[i].y = static_cast<float>(rand() % window_height + window_height);
		}
	}

	glutPostRedisplay();
}

void draw_texture(float x, float y, float width, float height, GLuint texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + width, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + width, y + height);
	glTexCoord2f(0, 1);
	glVertex2f(x, y + height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	draw_texture(0.0f, 0.0f, window_width, window_height, background_texture.id);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < num_textures; ++i)
	{
		draw_texture(textures[i].x, textures[i].y, textures[i].width, textures[i].height, textures[i].id);
	}

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	window_width = width;
	window_height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, -1.0, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}