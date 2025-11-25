#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	CompileAllShaderPrograms();

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particles
	CreateParticles(1000);

	//Create Grid Mesh
	CreateGridMesh(1000, 1000);

	//Rain Drop Points
	int index = 0;
	for (int i = 0; i < MAX_POINTS; i++)
	{
		float x = 2 * (float)rand() / (float)RAND_MAX - 1;
		float y = 2 * (float)rand() / (float)RAND_MAX - 1;
		float sTime = (float)rand() / (float)RAND_MAX * 6;
		float lTime = (float)rand() / (float)RAND_MAX;
		m_Points[index] = x; index++;
		m_Points[index] = y; index++;
		m_Points[index] = sTime; index++;
		m_Points[index] = lTime; index++;
	}

	//Create FBOs
	CreateFBOs();

	//Load Textures
	m_RGBTexture = CreatePngTexture("./Textures/rgb.png", GL_NEAREST);
	m_IVETexture = CreatePngTexture("./Textures/rgb.png", GL_NEAREST);
	m_NumTexture[0] = CreatePngTexture("./Textures/0.png", GL_NEAREST);
	m_NumTexture[1] = CreatePngTexture("./Textures/1.png", GL_NEAREST);
	m_NumTexture[2] = CreatePngTexture("./Textures/2.png", GL_NEAREST);
	m_NumTexture[3] = CreatePngTexture("./Textures/3.png", GL_NEAREST);
	m_NumTexture[4] = CreatePngTexture("./Textures/4.png", GL_NEAREST);
	m_NumTexture[5] = CreatePngTexture("./Textures/5.png", GL_NEAREST);
	m_NumTexture[6] = CreatePngTexture("./Textures/6.png", GL_NEAREST);
	m_NumTexture[7] = CreatePngTexture("./Textures/7.png", GL_NEAREST);
	m_NumTexture[8] = CreatePngTexture("./Textures/8.png", GL_NEAREST);
	m_NumTexture[9] = CreatePngTexture("./Textures/9.png", GL_NEAREST);
	m_TotalNumTexture = CreatePngTexture("./Textures/numbers.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteShader(m_SolidRectShader);
	glDeleteShader(m_TestShader);
	glDeleteShader(m_ParticleShader);
	glDeleteShader(m_GridMeshShader);
	glDeleteShader(m_FullScreenShader);
}

void Renderer::CompileAllShaderPrograms()
{
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TestShader = CompileShaders("./Shaders/Test.vs", "./Shaders/Test.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_FullScreenShader = CompileShaders("./Shaders/FullScreenColor.vs", "./Shaders/FullScreenColor.fs");
	m_FSShader = CompileShaders("./Shaders/FS.vs", "./Shaders/FS.fs");
	m_TexShader = CompileShaders("./Shaders/Texture.vs", "./Shaders/Texture.fs");
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::ReloadAllShaderPrograms()
{
	DeleteAllShaderPrograms();
	CompileAllShaderPrograms();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float center = 0.5;
	float size = 0.1;
	float testPos[]
		=
	{
		// x, y, z, value
		(0.f - center) * size, (0.f - center) * size, 0.f, 1,
		(1.f - center) * size, (0.f - center) * size, 0.f, 1,
		(1.f - center) * size, (1.f - center) * size, 0.f, 1,
		(0.f - center) * size, (0.f - center) * size, 0.f, 1,
		(1.f - center) * size, (1.f - center) * size, 0.f, 1,
		(0.f - center) * size, (1.f - center) * size, 0.f, 1, // Quad1

		(0.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (1.f - center) * size, 0.f, 0.5,
		(0.f - center) * size, (0.f - center) * size, 0.f, 0.5,
		(1.f - center) * size, (1.f - center) * size, 0.f, 0.5,
		(0.f - center) * size, (1.f - center) * size, 0.f, 0.5, // Quad2
	};

	glGenBuffers(1, &m_VBOTestPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW);

	float testColor[]
		=
	{
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f, // Quad1

		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f // Quad2
	};

	glGenBuffers(1, &m_VBOTestColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testColor), testColor, GL_STATIC_DRAW);

	float fullRect[]
		=
	{
		-1.f, -1.f, 0.f, 1.f, 1.f, 0.f, -1.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 1.f, -1.f, 0.f, 1.f, 1.f, 0.f,
	};
	glGenBuffers(1, &m_FullScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRect), fullRect, GL_STATIC_DRAW);

	float fullRectFS[]
		=
	{
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f,
	};
	glGenBuffers(1, &m_FSVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRectFS), fullRectFS, GL_STATIC_DRAW);

	float texRect[]
		=
	{
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f,
	};
	glGenBuffers(1, &m_TexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -0.5;
	float basePosY = -0.5;
	float targetPosX = 0.5;
	float targetPosY = 0.5;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		assert(0);
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}

void Renderer::DrawTexture(float x, float y, float sx, float sy, GLuint texID)
{
	//Program select
	GLuint shader = m_TexShader;
	glUseProgram(shader);

	int uTex = glGetUniformLocation(shader, "u_TexID");
	glUniform1i(uTex, 0);
	int uTrans = glGetUniformLocation(shader, "u_Trans");
	glUniform2f(uTrans, x, y);
	int uScale = glGetUniformLocation(shader, "u_Scale");
	glUniform2f(uScale, sx, sy);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	int aPos = glGetAttribLocation(shader, "a_Pos");
	int aTex = glGetAttribLocation(shader, "a_Tex");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CreateFBOs()
{
	for (int idx = 0; idx < 5; idx++)
	{
		//Color Gen : texture(render target)
		glGenTextures(1, &m_RTs[idx]);
		glBindTexture(GL_TEXTURE_2D, m_RTs[idx]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		//Depth Gen
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//FBO Gen
		glGenFramebuffers(1, &m_FBOs[idx]);

		//Attach
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[idx]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RTs[idx], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		//Check
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			assert(0);
		}
	}

	//Restore Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];

	size_t slen = strlen(pShaderText);
	if (slen > INT_MAX) {
		// Handle error
	}
	GLint len = (GLint)slen;

	Lengths[0] = len;
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTest()
{
	m_Time += 0.00016;

	GLuint shader = m_TestShader;

	//Program select
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	// Pos Attribute 설정
	int aPosLoc = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);
	glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// Value Attribute 설정
	int aValueLoc = glGetAttribLocation(shader, "a_Value");
	glEnableVertexAttribArray(aValueLoc);
	glVertexAttribPointer(aValueLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float) * 3));

	// Color Attribute 설정
	int aColorLoc = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestColor);
	glVertexAttribPointer(aColorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Time += 0.00016;

	GLuint shader = m_ParticleShader;

	//Program select
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	int uForceLoc = glGetUniformLocation(shader, "u_Force");
	glUniform3f(uForceLoc, 10.f, 0.f, 0.f);

	int stride = 15;

	int aPosLoc = glGetAttribLocation(shader, "a_Position");
	int aValueLoc = glGetAttribLocation(shader, "a_Value");
	int aColorLoc = glGetAttribLocation(shader, "a_Color");
	int aSTimeLoc = glGetAttribLocation(shader, "a_STime");
	int aVelLoc = glGetAttribLocation(shader, "a_Vel");
	int aLifeTimeLoc = glGetAttribLocation(shader, "a_LifeTime");
	int aMassLoc = glGetAttribLocation(shader, "a_Mass");
	int aPeriodLoc = glGetAttribLocation(shader, "a_Period");

	glEnableVertexAttribArray(aPosLoc);
	glEnableVertexAttribArray(aValueLoc);
	glEnableVertexAttribArray(aColorLoc);
	glEnableVertexAttribArray(aSTimeLoc);
	glEnableVertexAttribArray(aVelLoc);
	glEnableVertexAttribArray(aLifeTimeLoc);
	glEnableVertexAttribArray(aMassLoc);
	glEnableVertexAttribArray(aPeriodLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);

	glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, 0);
	glVertexAttribPointer(aValueLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aColorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(aSTimeLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(aVelLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 9));
	glVertexAttribPointer(aLifeTimeLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 12));
	glVertexAttribPointer(aMassLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 13));
	glVertexAttribPointer(aPeriodLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 14));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticlesVertexCount);

	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::DrawGridMesh()
{
	m_Time += 0.00016;

	//Program select
	GLuint shader = m_GridMeshShader;
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	int uSamplerRGB = glGetUniformLocation(shader, "u_RGBTexture");
	glUniform1i(uSamplerRGB, 0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	int uPointsLoc = glGetUniformLocation(shader, "u_Points");
	glUniform4fv(uPointsLoc, MAX_POINTS, m_Points);

	int uDCLoc = glGetUniformLocation(shader, "u_DropCount");
	glUniform1i(uDCLoc, 100);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawFullScreenColor(float r, float g, float b, float a)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	GLuint shader = m_FullScreenShader;
	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::DrawFS()
{
	m_Time += 0.00066;
	int number = (int)floor(m_Time) % 10 + 2;

	//Program select
	GLuint shader = m_FSShader;
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_Time);

	int uSamplerRGB = glGetUniformLocation(shader, "u_RGBTexture");
	glUniform1i(uSamplerRGB, 1);

	int uSamplerNum = glGetUniformLocation(shader, "u_NumTexture");
	glUniform1i(uSamplerNum, number);

	int uSamplerTotalNum = glGetUniformLocation(shader, "u_TotalNumTexture");
	glUniform1i(uSamplerTotalNum, 12);

	int uSamplerNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1i(uSamplerNumber, int(m_Time));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_IVETexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[1]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[2]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[3]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[4]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[5]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[6]);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[7]);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[8]);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[9]);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, m_TotalNumTexture);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int attribTexPos = glGetAttribLocation(shader, "a_TexPos");
	
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexPos);

	glBindBuffer(GL_ARRAY_BUFFER, m_FSVBO);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawDebugTextures()
{
	DrawTexture(-0.5, -0.5, 0.5, 0.5, m_RTs[0]);
	DrawTexture(0.5, -0.5, 0.5, 0.5, m_RTs[1]);
}

void Renderer::DrawFBOs()
{
	//FBO0 Render
	int idx = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[idx]);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawParticle();

	//FBO1 Render
	idx = 1;
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[idx]);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawGridMesh();

	//Restore
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticles(int count)
{
	int particleCount = count;
	int verticesCounts = particleCount * 6;
	// x, y, z, value, r, g, b, a, sTime, vx, vy, vz, lifeTime, mass
	int floatCountsPerVertex = 3 + 1 + 4 + 1 + 3 + 1 + 1 + 1;
	int totalFloatCounts = floatCountsPerVertex * verticesCounts;
	int floatCountsPerParticle = floatCountsPerVertex * 6;

	float* temp = nullptr;
	temp = new float[totalFloatCounts];

	for (int i = 0; i < particleCount; i++)
	{
		float size = 0.01; // 0.02 * (float(rand()) / float(RAND_MAX));
		float centerX = 0; // (float(rand()) / float(RAND_MAX)) * 2.f - 1.f;	// [-1, 1]
		float centerY = 0; // (float(rand()) / float(RAND_MAX)) * 2.f - 1.f;	// [-1, 1]
		float value = (float(rand()) / float(RAND_MAX));							// [0, 1]
		float r = (float(rand()) / float(RAND_MAX));
		float g = (float(rand()) / float(RAND_MAX));
		float b = (float(rand()) / float(RAND_MAX));
		float a = 1.f;
		float sTime = (float(rand()) / float(RAND_MAX)) * 2.f;					// [0, 2]
		float vx = ((float(rand()) / float(RAND_MAX)) * 2.f - 1.f) * 3.f;		// [-3, 3]
		float vy = (float(rand()) / float(RAND_MAX)) * 5.f + 0.5f;				// [5.5, 5.5]
		float vz = 0;
		float lifeTime = (float(rand()) / float(RAND_MAX)) + 0.3f;
		float mass = (float(rand()) / float(RAND_MAX)) + 1.f;					// [1, 2]
		float period = (float(rand()) / float(RAND_MAX));

		int index = i * floatCountsPerParticle;

		temp[index] = centerX - size; index++;	// x
		temp[index] = centerY - size; index++;	// y
		temp[index] = 0; index++;				// z
		temp[index] = value; index++;			// value
		temp[index] = r; index++;				// r
		temp[index] = g; index++;				// g
		temp[index] = b; index++;				// b
		temp[index] = a; index++;				// a
		temp[index] = sTime; index++;			// sTime
		temp[index] = vx; index++;				// vx
		temp[index] = vy; index++;				// vy
		temp[index] = vz; index++;				// vz
		temp[index] = lifeTime; index++;		// lifeTime
		temp[index] = mass; index++;			// mass
		temp[index] = period; index++;			// period

		temp[index] = centerX + size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX - size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX - size; index++;
		temp[index] = centerY - size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX + size; index++;
		temp[index] = centerY - size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;

		temp[index] = centerX + size; index++;
		temp[index] = centerY + size; index++;
		temp[index] = 0; index++;
		temp[index] = value; index++;
		temp[index] = r; index++;
		temp[index] = g; index++;
		temp[index] = b; index++;
		temp[index] = a; index++;
		temp[index] = sTime; index++;
		temp[index] = vx; index++;
		temp[index] = vy; index++;
		temp[index] = vz; index++;
		temp[index] = lifeTime; index++;
		temp[index] = mass; index++;
		temp[index] = period; index++;
	}

	glGenBuffers(1, &m_VBOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCounts, temp, GL_STATIC_DRAW);

	delete[] temp;
	m_VBOParticlesVertexCount = verticesCounts;
}
