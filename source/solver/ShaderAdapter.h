#pragma once

#include <QByteArray>

class ShaderAdapter
{
public:
	struct Settings
	{
		int size;
	};

	ShaderAdapter(const QString & filename, Settings settings);
	QByteArray Adapt();

private:
	QByteArray shaderCode;
	Settings   settings;
};
