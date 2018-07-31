#pragma once

#include "Font.h"
#include "Camera.h"

class Label
{
private:
	POINT _Position;
	POINT _Size;
	LPCWSTR _Text;
	std::wstring stemp;
	Font *_Font;

public:
	Label();
	~Label();

	Label(const Label& label);
	Label& operator=(const Label &label);

	void SetText(std::string &text);
	Label(const std::string &text, Font *font, float x, float y, float width, float height);

	void Draw(Camera *cam);

	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetFont(Font *font);

};