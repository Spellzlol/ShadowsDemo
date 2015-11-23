#include "Texture.h"
#include "utils.h"

extern jobject m_bitmapLoader;

Texture* Texture::create(const std::string& filename)
{
	const auto env = JniHelpers::get()->getEnv();

	auto load = JniHelpers::get()->getMethodID("com/gsom/simpleengine/BitmapLoader", "load", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	auto jname = env->NewStringUTF(filename.c_str());
	auto jbitmap = env->CallObjectMethod(m_bitmapLoader, load, jname);

	auto getWidth = JniHelpers::get()->getMethodID("com/gsom/simpleengine/BitmapLoader", "getWidth", "(Landroid/graphics/Bitmap;)I");
	int width = env->CallIntMethod(m_bitmapLoader, getWidth, jbitmap);

	auto getHeight = JniHelpers::get()->getMethodID("com/gsom/simpleengine/BitmapLoader", "getHeight", "(Landroid/graphics/Bitmap;)I");
	int height = env->CallIntMethod(m_bitmapLoader, getHeight, jbitmap);
    
	auto getPixels = JniHelpers::get()->getMethodID("com/gsom/simpleengine/BitmapLoader", "getPixels", "(Landroid/graphics/Bitmap;[I)V");
	jintArray jpixels = env->NewIntArray(width * height);
	env->CallVoidMethod(m_bitmapLoader, getPixels, jbitmap, jpixels);

	int *pixels = env->GetIntArrayElements(jpixels, 0);

    auto texture = new (std::nothrow) Texture(pixels, width, height);

    env->DeleteLocalRef(jname);
    env->DeleteLocalRef(jbitmap);
	env->ReleaseIntArrayElements(jpixels, pixels, 0);
	env->DeleteLocalRef(jpixels);

    return texture;
}

Texture* Texture::create(int width, int height, const GLubyte* pixels /* = nullptr */)
{
	auto texture = new (std::nothrow) Texture(pixels, width, height);
	return texture;
}

Texture::Texture(const int* pixels, int width, int height)
{
	GLubyte *data = nullptr;

	if (pixels != nullptr)
	{
		data = new (std::nothrow) GLubyte[width * height * 4];

		//android bitmap is argb 32bit int per pixel
		for (int i = 0; i < width * height; ++i)
		{
			int j = i * 4;

			data[j  ] = (pixels[i] >> 16) & 0xff; //r
			data[j+1] = (pixels[i] >> 8)  & 0xff; //g
			data[j+2] =  pixels[i]		  & 0xff; //b
			data[j+3] = (pixels[i] >> 24) & 0xff; //a
		}
	}

	init(data,width,height);
}

Texture::Texture(const GLubyte* pixels, int width, int height)
{
	init(pixels,width,height);
}

void Texture::init(const GLubyte* pixels, int width, int height)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	m_width = width;
	m_height = height;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR;		
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureId);
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::enableAliasTexture(bool enable)
{
	if (m_aliasEnabled == enable)
	{
		return;
	}

	m_aliasEnabled = enable;

	bind();

	if (m_aliasEnabled)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	unbind();
}