#include "RujinPCH.h"
#include "MainRenderProvider.h"

#include "imgui.h"
#include "Rujin.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

#include "SceneProvider.h"
#include "ServiceLocator.h"

#include "Texture.h"
#include "TransformComponent.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

rujin::MainRenderProvider::MainRenderProvider()
	: m_WindowInfo(Rujin::Get()->GetWindowContext())
{
	LOG_DEBUG("Initializing OpenGL & SDL MainRenderProvider...");

	const settings::InitParams& initParams = Rujin::Get()->GetInitParams();

	m_pContext = SDL_GL_CreateContext(m_WindowInfo.pWindow);

	if(!m_pContext)
		LOG_ERROR_("SDL_GL_CreateContext Error: {}", SDL_GetError());

	//Set VSync
	SDL_GL_SetSwapInterval(static_cast<int>(initParams.vsyncMode));
	m_WindowInfo.vSyncMode = initParams.vsyncMode;

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	glOrtho(0, m_WindowInfo.windowSize.x, 0, m_WindowInfo.windowSize.y, -1, 1); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, m_WindowInfo.windowSize.x, m_WindowInfo.windowSize.y);

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set clear color
	glClearColor(initParams.glClearColor.r, initParams.glClearColor.b, initParams.glClearColor.g, initParams.glClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
	m_WindowInfo.glClearColor = initParams.glClearColor;

	//initialize ImGui
	LOG_DEBUG("Hooking ImGUI...");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_WindowInfo.pWindow, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();

	LOG_DEBUG("MainRenderProvider Initialized!");
}

rujin::MainRenderProvider::~MainRenderProvider()
{
	if (SDL_GL_GetCurrentContext()) // if we have a context, it is initialized
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(m_pContext);
	}
}

void rujin::MainRenderProvider::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	auto& sceneService = ServiceLocator::GetService<SceneService>();

	sceneService.Draw();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_WindowInfo.pWindow);
	ImGui::NewFrame();

	sceneService.OnGui(m_WindowInfo.pWindow);

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();

	// Update screen: swap back and front buffer
	SDL_GL_SwapWindow(m_WindowInfo.pWindow);
}

void rujin::MainRenderProvider::RenderTexture(const Texture& texture, const Transform& transform, const glm::vec2& pivot, const Rectf* srcRect) const
{
	const glm::ivec2& texSize = texture.GetSize();

	Margins texCoords;
	glm::ivec2 srcSize;

	//determine texture coords.
	if (!srcRect) //no source rect specified.
	{
		// Use complete texture
		texCoords.left = 0.0f;
		texCoords.right = 1.0f;
		texCoords.top = 0.0f;
		texCoords.bottom = 1.0f;

		srcSize = texSize;
	}
	else
	{
		// Convert to the range [0.0, 1.0]
		texCoords.left = srcRect->left / texSize.x;
		texCoords.right = (srcRect->left + srcRect->width) / texSize.x;
		texCoords.top = (srcRect->bottom - srcRect->height) / texSize.y;
		texCoords.bottom = srcRect->bottom  / texSize.y;

		srcSize = glm::ivec2{ srcRect->width, srcRect->height};
	}

	//create dest rect
	Rectf destRect;
	destRect.width = srcSize.x * transform.scale.x;
	destRect.height = srcSize.y * transform.scale.y;

	const glm::vec2 pivotOffset{ destRect.width * pivot.x, destRect.height * pivot.y };

	destRect.left = transform.pos.x - pivotOffset.x;
	destRect.bottom = transform.pos.y - pivotOffset.y;

	//perform tranform tranformations
	glPushMatrix();
	glTranslatef(transform.pos.x, transform.pos.y, 0);
	glRotatef(glm::degrees(transform.rot), 0.f, 0.f, 1.f);
	glScalef(transform.scale.x, transform.scale.y, 1.f);
	glTranslatef(-transform.pos.x, -transform.pos.y, 0);

	//Create vertex info
	const Margins vertexInfo{ destRect.left, destRect.left + destRect.width, destRect.bottom + destRect.height, destRect.bottom };

	glBindTexture(GL_TEXTURE_2D, texture.GetId());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Draw
	glEnable(GL_TEXTURE_2D);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(texCoords.left, texCoords.bottom);
			glVertex2f(vertexInfo.left, vertexInfo.bottom);

			glTexCoord2f(texCoords.left, texCoords.top);
			glVertex2f(vertexInfo.left, vertexInfo.top);

			glTexCoord2f(texCoords.right, texCoords.top);
			glVertex2f(vertexInfo.right, vertexInfo.top);

			glTexCoord2f(texCoords.right, texCoords.bottom);
			glVertex2f(vertexInfo.right, vertexInfo.bottom);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix(); //remove transformations
}

#pragma region Primitive Drawing
void rujin::MainRenderProvider::SetColor(const glm::vec4& color) const
{
	glColor4f(color.r, color.g, color.b, color.a);
}

void rujin::MainRenderProvider::DrawPoint(const glm::vec2& p, float pointSize) const
{
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	{
		glVertex2f(p.x, p.y);
	}
	glEnd();
}

void rujin::MainRenderProvider::DrawLine(const glm::vec2& p1, const glm::vec2& p2, float lineWidth) const
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
}

void rujin::MainRenderProvider::DrawRect(const Rectf& rect, float lineWidth) const
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		const auto vertices = rect.GetVertices();
		glVertex2f(vertices.bottomLeft.x, vertices.bottomLeft.y);
		glVertex2f(vertices.bottomRight.x, vertices.bottomRight.y);
		glVertex2f(vertices.topRight.x, vertices.topRight.y);
		glVertex2f(vertices.topLeft.x, vertices.topLeft.y);
	}
	glEnd();
}

void rujin::MainRenderProvider::FillRect(const Rectf& rect) const
{
	glBegin(GL_POLYGON);
	{
		const auto vertices = rect.GetVertices();
		glVertex2f(vertices.bottomLeft.x, vertices.bottomLeft.y);
		glVertex2f(vertices.bottomRight.x, vertices.bottomRight.y);
		glVertex2f(vertices.topRight.x, vertices.topRight.y);
		glVertex2f(vertices.topLeft.x, vertices.topLeft.y);
	}
	glEnd();
}

#pragma endregion